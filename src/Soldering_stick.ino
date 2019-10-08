class SolderStick {
  public:
    SolderStick(Beeper *beep, Screen *screen;) {
        pinMode(Configure::PINs::solder_ctrl, OUTPUT); // switcher of heater
        pinMode(Configure::PINs::hairdryer_btn, INPUT_PULLUP);
        pinMode(Configure::PINs::hairdryer_motor, INPUT);
        pinMode(Configure::PINs::hot_solder_stick_led, OUTPUT);

        attachInterrupt(digitalPinToInterrupt(Configure::PINs::solder_btn), SolderStick::change_state, CHANGE);

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
        heating_state_ = false;
        setup_mode_ = false;
        setup_time_ = 0;
        first_heat_ = true;
        last_target_temp_ = get_target_temp(); // load_eeprom()
    };

    void change_state() {
        state_ != state_;
        if (state_) { // enable
            first_heat_ = true;
            heating_state_ = enable_heater(); // maybe there need speed turn on\turn off
        } else { // disable
            heating_state_ = disable_heater();
            screen_.solder_stick_off();
            setup_mode_ = false;
        } 
    };

  private:
    SolderStick();

    bool enable_heater() {
        digitalWrite(Configure::PINs::solder_ctrl, HIGH);
        return true;
    };
    bool disable_heater() {
        digitalWrite(Configure::PINs::solder_ctrl, LOW);
        return false;
    };
    uint16_t temp() {
        return map(analogRead(Configure::PINs::solder_therm), 0, 1023, 0, 480);
    };
    uint16_t get_target_temp() {
        return map(analogRead(Configure::PINs::solder_target_temp), 0, 1023, 0, 480);
    };
    bool is_lay() { 
        return false; // future release
    };
    bool hot_led_on() {
        digitalWrite(Configure::PINs::hot_solder_stick_led, HIGH);
        return true;
    };
    bool hot_led_off() {
        digitalWrite(Configure::PINs::hot_solder_stick_led, LOW);
        return false;
    };

    void update() {
        if (state_ == true) { // enabled
            uint16_t target_temp = get_target_temp();
            uint16_t show_temp = temp();
            int32_t delta_temp = target_temp - temp();

            if (!setup_mode_) {
                if (last_target_temp_ != target_temp) {
                    setup_mode_ = true;
                    setup_time_ = millis();
                    last_target_temp_ = target_temp;
                } 
            } else {
                setup_mode_ = (millis() - setup_time_) >= Configure::TIME_To_REACT_ms;
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

            screen_.update_solder_stick(setup_mode_, temp); // switch on and update
        }

        if ( !hot_led_state_ && temp() >= Configure::HOT_TEMP) { 
            hot_led_state_ = hot_led_off();
        }

        if (hot_led_state_ && temp() < Configure::HOT_TEMP)) { 
            hot_led_state_ = hot_led_off();
        }
    };
   
  private:
    Beeper *beep_;
    Screen *screen_;
    static bool state_ = false;
    bool hot_led_state_;
    bool heating_state_;
    bool setup_mode_;
    bool setup_time_;
    bool last_target_temp_;
    bool first_heat_;
};