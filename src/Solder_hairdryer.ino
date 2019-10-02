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
        last_target_temp_ = 0;
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
    void update() { // TODO 2 mode: change speed or temp, and current temp
        if (state_ == true) { // enabled
            uint32_t target_temp = get_target_temp();
            uint32_t temp = temp();
            int32_t delta_temp = target_temp - temp;
            uint32_t speed = get_target_fan_speed();

            if (!setup_mode_ && last_target_temp_ && last_target_temp_ != target_temp) {
                setup_mode_ = true;
                setup_time_ = millis();
            } else {
                last_target_temp_ = target_temp;
            }
            
            if (setup_mode_) {
                temp = target_temp;
                if ((millis() - setup_time_) >= Configure::TIME_To_REACT_ms) {
                    setup_mode_ = LOW;
                    // setup_mode_ = (millis() - setup_time_) >= Configure::TIME_To_REACT_ms);
                }
                // blinking numbers mean: setup
            }

            // TODO correct work of laying mode on and off
            if (is_lay()) { // enter only once
                if (!laying_mode_) {
                    set_fan_speed(Configure::FUN_5_percent);
                    laying_mode_ = true;
                }
            } else {
                if (laying_mode_) {
                    set_fan_speed(speed);
                    laying_mode_ = false;
                }
            }

            if (heating_state_ && delta_temp > 0) { // taget temp not reached: heating
                // what to do?
            }

            if (heating_state_ && delta_temp <= 0) { // target temp reached: stop heating
                disable_heater();
                beeper_.temprature_reached(); // TODO beep only once per setup
            }

            if (!heating_state_ && delta_temp >= Configure::TEMP_DELTA)  { // taget temp loose: cooling down
                enable_heater();
            }

            screen.update_hairdryer(setup_mode_, temp, speed);
            // is setup mode, temp, speed
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
        if (state_ == false ) { // enable
           enable_heater(); // maybe there need speed turn on\turn off
           set_fan_speed(Configure::FUN_5_percent);
        } else if (state == true ) { // disable
            disable_heater();
            set_fan_speed(Configure::FUN_100_percent);
        } else {
            // TODO FAIL alarm
        }
    };

  private:
    volatile bool state_ = false;
    bool hot_led_state_;
    bool laying_mode_;
    bool heating_state_;
    bool setup_mode_;
    uint64_t setup_time_;
    uint32_t last_target_temp_;

    Beeper *beeper_;
    Screen *screen_;
};