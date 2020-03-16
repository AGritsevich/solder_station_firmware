#pragma once

#include "Configure.h"
#include "Screen.h"
#include "Beeper.h"

class SolderStick;
static void change_stick();

static SolderStick *g_stick = nullptr;

class SolderStick {
  public:
    SolderStick(Beeper *beep, Screen *screen) {
        pinMode(Configure::PINs::solder_ctrl, OUTPUT); // switcher of heater
        pinMode(Configure::PINs::solder_btn, INPUT_PULLUP);
        //pinMode(Configure::PINs::hairdryer_motor, INPUT);
        pinMode(Configure::PINs::hot_solder_stick_led, OUTPUT);

        attachInterrupt(digitalPinToInterrupt(Configure::PINs::solder_btn), change_stick, RISING);

        g_stick = this;

        beep_ = beep;
        screen_ = screen;
        if (!screen_ && beep_) {
            beep_->alarm();
        }

        if (!beep_ && screen_) {
            screen_->exception("Stick: beeper fail");
        }

        if (!beep_ && !screen_) {
            reset_mk();
        }

        if (temp() > Configure::Upper_temperature_limit || temp() < 10) {
            //screen_->exception("Stick: temperature sensor fail");
            //beep_->alarm();

            //TODO
        }

        stick_state_ = false;
        hot_led_state_ = false;
        heating_state_ = false;
        first_heat_ = true;
        last_target_temp_ = get_target_temp(); // load_eeprom()

        //enable();
 
    };

    void update() {
        uint16_t temperature = temp();

        if (stick_state_ == true) { // enabled
            uint16_t target_temp = get_target_temp();
            int32_t delta_temp = target_temp - temperature;

            if (last_target_temp_ != target_temp) {
                last_target_temp_ = target_temp;
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
            
        //screen_->update_solder_stick(setup_mode, temperature); // TODO
        screen_->update_solder_stick(false, temp());

        if ( !hot_led_state_ && (temperature >= Configure::HOT_TEMP)) { 
            hot_led_state_ = true;
            hot_led_on();
        }

        if (hot_led_state_ && (temperature < Configure::HOT_TEMP)) { 
            hot_led_off();
            hot_led_state_ = false;
        }
    };

    void switch_solder() {
        stick_state_ = inv(stick_state_);

        if (stick_state_) enable();
        else disable();
    }

private:
    SolderStick();

    void enable() {
            first_heat_ = true;
            enable_heater();
            screen_->solder_stick_switch(true);
    }

    void disable() {
        first_heat_ = false;
        disable_heater();
        screen_->solder_stick_switch(false);
        hot_led_off();
    }

    void enable_heater() {
        digitalWrite(Configure::PINs::solder_ctrl, HIGH);
    };
    void disable_heater() {
        digitalWrite(Configure::PINs::solder_ctrl, LOW);
    };
    uint16_t temp() {
        return map(analogRead(Configure::PINs::solder_therm), 0, 1023, 0, 480);
        //return analogRead(Configure::PINs::solder_therm); // TODO
    };
    uint16_t get_target_temp() {
        return map(analogRead(Configure::PINs::solder_target_temp), 0, 1023, 0, 480);
        //return analogRead(Configure::PINs::solder_target_temp);  // TODO
    };
    bool is_lay() { 
        return false; // future release
    };
    void hot_led_on() {
        digitalWrite(Configure::PINs::hot_solder_stick_led, HIGH); 
    };
    void hot_led_off() {
        digitalWrite(Configure::PINs::hot_solder_stick_led, LOW);
    };
   
  private:
    Beeper *beep_;
    Screen *screen_;

    bool stick_state_;
    bool hot_led_state_;
    bool heating_state_;
    bool last_target_temp_;
    bool first_heat_;
};

// static functions

static void change_stick() {
    if (g_stick) g_stick->switch_solder(); 
};