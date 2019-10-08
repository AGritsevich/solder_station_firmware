class SolderingHairdryer {
  public:
    SolderingHairdryer(Beeper *beep, Screen *screen;) {
        pinMode(Configure::PINs::hairdryer_ctrl, OUTPUT);
        pinMode(Configure::PINs::hairdryer_btn, INPUT_PULLUP);
        pinMode(Configure::PINs::hairdryer_motor, INPUT);
        pinMode(Configure::PINs::hot_hair_dryer_led, OUTPUT);

        attachInterrupt(digitalPinToInterrupt(Configure::PINs::hairdryer_btn), SolderingHairdryer::change_state, CHANGE);

        beeper_ = beeper;
        screen_ = screen;
        if (!screen_ && beeper_) {
            beeper_.critical_fail();
        }

        if (!beeper_ && screen_) {
            screen_.error_message("Hardryer: beeper fail")
        }

        if (!beeper_ && !screen_) {
            reset_mk();
        }

        if (temp() > Configure::Upper_temperature_limit ) {
            screen_.error_message("Hardryer: screen fail")
            beeper_.critical_fail(); // loop
        }

        hot_led_state_ = false;
        laying_mode_ = false;
        heating_state_ = false;
        setup_mode_ = false;
        setup_time_ = 0;
        first_heat_ = true;
        last_target_temp_ = get_target_temp(); // load_eeprom()
    };

  private:
    SolderingHairdryer();

    void enable_heater() {
        digitalWrite(Configure::PINs::hairdryer_ctrl, HIGH);
        heating_state_ = true;
    };
    void disable_heater() {
        digitalWrite(Configure::PINs::hairdryer_ctrl, LOW);
        heating_state_ = false;
    };
    uint16_t get_target_fan_speed() {
        return map(analogRead(Configure::PINs::hairdryer_target_motor_speed),  0, 1023, 130, 255);
    };
    void set_fan_speed(uint8_t power) { // 0-255
        analogWrite(Configure::PINs::hairdryer_motor, power);
    };
    uint8_t get_fan_speed() {
        return map(analogRead(Configure::PINs::hairdryer_motor_ctrl), 0, 1023, 0, 255);
    };
    uint16_t temp() {
        return map(analogRead(Configure::PINs::hairdryer_therm), 0, 1023, 0, 480);
    };
    uint16_t get_target_temp() {
        return map(analogRead(Configure::PINs::hairdryer_target_heater_temp), 0, 1023, 0, 480);
    };
    bool is_lay() { // not lay - high level
        return analogRead(Configure::PINs::hairdryer_gerkon);
    };
    bool hot_led_on() {
        digitalWrite(Configure::PINs::hot_hair_dryer_led, HIGH);
        return true;
    };
    bool hot_led_off() {
        digitalWrite(Configure::PINs::hot_hair_dryer_led, LOW);
        return false;
    };

  public:
    void update() {
        if (state_ == true) { // enabled
            uint16_t target_temp = laying_mode_ ? Configure:HAIRDRYER_SAVE_TEMP : get_target_temp();
            uint16_t show_temp = temp();
            int32_t delta_temp = target_temp - show_temp;
            uint16_t speed = get_target_fan_speed();

            if (!setup_mode_) {
                if (last_target_temp_ != target_temp) {
                    setup_mode_ = true;
                    setup_time_ = millis();
                    last_target_temp_ = target_temp;
                } 
            } else {
                setup_mode_ = (millis() - setup_time_) >= Configure::TIME_To_REACT_ms;
            }

            if (!laying_mode_) {
                if (is_lay()) { // put on - keep power
                    set_fan_speed(Configure::FUN_5_percent);
                    laying_mode_ = true;
                }
            } else { // not lay
                if (!is_lay()) {
                    laying_mode_ = false;
                    set_fan_speed(target_speed);
                }
            }

            if (heating_state_) { // heating ON
                if (delta_temp > 0) { // taget temp not reached: heating
                    beeper_.increase_beep(first_heat_);
                } else { // target temp reached: stop heating
                    heating_state_ = disable_heater();
                    beeper_.temprature_reached(first_heat_);
                    first_heat_ = false;
                }
            } else { // taget temp loose: cooling down
                if (delta_temp >= Configure::TEMP_DELTA)  { 
                    heating_state_ = enable_heater();
                }
            }

            screen.update_hairdryer(setup_mode_, show_temp, speed);
        }

        if ( !hot_led_state_ && temp() >= Configure::HOT_TEMP) { 
            hot_led_state_ = hot_led_off();
        }

        if (hot_led_state_ && temp() < Configure::HOT_TEMP)) { 
            hot_led_state_ = hot_led_off();
            set_fan_speed(Configure::FUN_0_percent);
        }
    };

    static void change_state() {
        state_ != state_;
        if (state_ == true ) { // enable
           enable_heater(); // maybe there need speed turn on\turn off
           set_fan_speed(Configure::FUN_5_percent);
           first_heat_ = true;
        } else if (state == false ) { // disable
            disable_heater();
            set_fan_speed(Configure::FUN_100_percent);
        }
    };

  private:
    volatile bool state_ = false;
    bool hot_led_state_;
    bool laying_mode_;
    bool heating_state_;
    bool setup_mode_;
    bool first_heat_;
    uint64_t setup_time_;
    uint32_t last_target_temp_;

    Beeper *beeper_;
    Screen *screen_;
};