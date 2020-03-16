#pragma once

#include "Configure.h"
#include "Screen.h"
#include "Beeper.h"

class SolderingHairdryer;
static void change_fan();

static SolderingHairdryer *g_hairdryer = nullptr;

class SolderingHairdryer {
  public:
    SolderingHairdryer(Beeper *beeper, Screen *screen) {
        TCCR2B = TCCR2B & 0b11111000 | 0x01; // freq PWM 11 и 3
        pinMode(Configure::PINs::hairdryer_heater_ctrl, OUTPUT);
        pinMode(Configure::PINs::hairdryer_btn, INPUT_PULLUP);
        pinMode(Configure::PINs::hairdryer_motor, OUTPUT);
        pinMode(Configure::PINs::hot_hairdryer_led, OUTPUT);

        // пин зануляется
        attachInterrupt(digitalPinToInterrupt(Configure::PINs::hairdryer_btn), change_fan, RISING); // the best

        g_hairdryer = this; // костыль

        beep_ = beeper;
        screen_ = screen;
        if (!screen_ && beep_) {
            beep_->alarm();
        }

        if (!beep_ && screen_) {
            screen_->exception("Fan: beeper fail");
        }

        if (!beep_ && !screen_) {
            reset_mk();
        }

        if (temp() > Configure::Upper_temperature_limit || temp() < 10 ) {
            //screen_->exception("Fan: temperature sensor fail");
            //beep_->alarm(); // loop

            //TODO
        }

        hot_led_state_ = false;
        laying_mode_ = false;
        heating_state_ = false;
        first_heat_ = true;
        last_target_temp_ = get_target_temp(); // load_eeprom()
        last_target_speed_ = get_target_fan_speed();

        enable();
    };

    void update() {
        int32_t show_temp = temp();
        uint16_t speed = get_target_fan_speed();
        bool setup_temp = false;
        bool setup_speed = false;

        set_fan_speed(Configure::FUN_100_percent); // 20%

        if (hairdryer_state_ == true) { // enabled
            uint16_t target_temp = laying_mode_ ? Configure::HAIRDRYER_SAVE_TEMP : get_target_temp();
            int32_t delta_temp = target_temp - temp();

            if (last_target_temp_ != target_temp) {
                setup_temp = true;
                show_temp = get_target_temp();
                last_target_temp_ = target_temp;
            } 
        
            if (last_target_speed_ != speed) {
                setup_speed = true;
                last_target_speed_ = speed;
            } 

            if (!laying_mode_) {
                if (is_lay()) { // put on - keep power
                    set_fan_speed(Configure::FUN_5_percent);
                    laying_mode_ = true;
                }
            } else { // not lay
                if (!is_lay()) {
                    laying_mode_ = false;
                    set_fan_speed(speed);
                }
            }

            if (heating_state_) { // heating ON
                if (delta_temp > 0) { // taget temp not reached: heating
                    beep_->increase_beep(first_heat_);
                } else { // target temp reached: stop heating
                    disable_heater();
                    heating_state_ = false;
                    beep_->temprature_reached(first_heat_);
                    first_heat_ = false;
                }
            } else { // taget temp loose: cooling down
                if (delta_temp >= Configure::TEMP_DELTA)  { 
                    enable_heater();
                    heating_state_ = true;
                }
            }
        }

        screen_->update_hairdryer(setup_temp, setup_speed, show_temp, speed); // TODO
        //screen_->update_hairdryer(false, false, temp(), get_target_fan_speed());

        if ( !hot_led_state_ && (temp() >= Configure::HOT_TEMP)) { 
            hot_led_state_ = true; 
            hot_led_on();
        }

        if (hot_led_state_ && (temp() < Configure::HOT_TEMP)) { 
            hot_led_state_ = false;
            hot_led_off();
            set_fan_speed(Configure::FUN_0_percent);
        }
    };

    void switch_fan() {
        hairdryer_state_ = inv(hairdryer_state_);

        if (hairdryer_state_) enable();
        else disable();
    }

  private:
    SolderingHairdryer();

    void enable_heater() {
        digitalWrite(Configure::PINs::hairdryer_heater_ctrl, HIGH);
    };
    void disable_heater() {
        digitalWrite(Configure::PINs::hairdryer_heater_ctrl, LOW);
    };
    uint16_t get_target_fan_speed() {
        return map(analogRead(Configure::PINs::hairdryer_target_motor_speed),  0, 1023, 0, 99);
        //return analogRead(Configure::PINs::hairdryer_target_motor_speed);
    };
    void set_fan_speed(uint8_t power) { // 0-255
        //analogWrite(Configure::PINs::hairdryer_motor, map(power, 0, 99, 0, 255));
        digitalWrite(Configure::PINs::hairdryer_motor, HIGH);
    };
    uint16_t temp() {
        return map(analogRead(Configure::PINs::hairdryer_therm), 0, 1023, 0, 480);
        //return analogRead(Configure::PINs::hairdryer_therm);  //TODO
    };
    uint16_t get_target_temp() {
        return map(analogRead(Configure::PINs::hairdryer_target_heater_temp), 0, 1023, 0, 480);
        //return analogRead(Configure::PINs::hairdryer_target_heater_temp); // TODO
    };
    bool is_lay() { // not lay - high level
        return (analogRead(Configure::PINs::hairdryer_gerkon) < 4);
    };
    void hot_led_on() {
        //digitalWrite(Configure::PINs::hot_hairdryer_led, HIGH); TODO FIX unexpected switch on
    };
    void hot_led_off() {
        digitalWrite(Configure::PINs::hot_hairdryer_led, LOW);
    };

    void enable() {
           //enable_heater(); // maybe there need speed turn on\turn off // TODO on only of start works motor
           heating_state_ = true;
           set_fan_speed(Configure::FUN_100_percent); // 20%
           first_heat_ = true;
           screen_->fan_switch(true);
    }

    void disable() {
            disable_heater();
            heating_state_ = false;
            set_fan_speed(Configure::FUN_100_percent);
            screen_->fan_switch(false);
    }

  private:
    Beeper *beep_;
    Screen *screen_;

    bool hairdryer_state_;

    bool hot_led_state_;
    bool laying_mode_;
    bool heating_state_;
    bool first_heat_;
    uint16_t last_target_temp_;
    uint16_t last_target_speed_;
};

// static 
static void change_fan() {
    if(g_hairdryer) g_hairdryer->switch_fan();
};