#pragma once

#include "Configure.h"
#include <LiquidCrystal.h>

/*
Given a version number MAJOR.MINOR.PATCH, increment the:

MAJOR version when you make incompatible API changes,
MINOR version when you add functionality in a backwards compatible manner
PATCH version when you make backwards compatible bug fixes.
*/

#define VER_MAJOR 0
#define VER_MINOR 2
#define VER_PATCH 0

#define FIRST_LINE 0
#define SECOND_LINE 1

#define BEGIN_LINE 0
#define SOLDER_TEMP_POS 7
#define SOLDER_DEG_POS 10
#define FAN_TEMP_POS 4
#define FAN_DEG_POS 7
#define FAN_SPEED_POS 10
#define FAN_PERCENT_POS 12
#define STATUS_POS 14

uint8_t degree[8] = {
  B00110, 
  B01001, 
  B01001, 
  B00110, 
  B00000, 
  B00000, 
  B00000,
  B00000
};

class Screen {
  public:
    Screen():
      lcd(Configure::PINs::lcd_D7, Configure::PINs::lcd_D6, Configure::PINs::lcd_D5, 
          Configure::PINs::lcd_D4, Configure::PINs::lcd_E, Configure::PINs::lcd_RS) {
        
        lcd.begin(16, 2);
        lcd.createChar(0, degree);

        solder_t_ = 0;
        fan_t_ = 0;
        fan_spd_ = 0;

        solder_setup_ = false;
        fan_t_setup_ = false;
        fan_spd_setup_ = false;

        solder_enabled_ = false;
        fan_enabled_ = false;

        time_phase_ = millis() + Configure::BLINK_TIME_ms;
        phase_ = true; // true- show, false - empty
    };

    void update() { 
        uint64_t time_now = millis();
        uint64_t time_pass = time_now - setup_time_;

        if (time_pass >= Configure::SETUP_TIME_ms) { // setup time up
          solder_setup_ = false; 
          fan_t_setup_ = false;
          fan_spd_setup_ = false;
        }

        if (time_phase_ <= time_now) {
          time_phase_ = time_now + Configure::BLINK_TIME_ms;
          phase_ = inv(phase_); // switch
        }
        
        // first line       
        lcd.setCursor(SOLDER_TEMP_POS, FIRST_LINE);
        if (solder_setup_ && !phase_) {
          lcd.print("   ");
        } else {
          lcd.print(solder_t_);
        }

        // second line
        lcd.setCursor(FAN_TEMP_POS, SECOND_LINE); 
        if(fan_t_setup_ && !phase_) {
          lcd.print("   ");
        } else {
           lcd.print(fan_t_);
        }

        lcd.setCursor(FAN_SPEED_POS, SECOND_LINE);
        if (fan_spd_setup_ && !phase_) {
          lcd.print("  ");
        } else {
           lcd.print(fan_spd_);
        }
 
        lcd.setCursor(STATUS_POS, FIRST_LINE);
        if (!solder_enabled_) {
          lcd.print("OF");
        } else {
          lcd.print("ON ");
        }

        lcd.setCursor(STATUS_POS, SECOND_LINE);
        if (!fan_enabled_) {
          lcd.print("OF");
        } else {
          lcd.print("ON ");
        }
    };

    void update_solder_stick(bool is_setup_mode, uint16_t temp) {
          setup_time_ = millis(); 
        
        if (is_setup_mode) {
          fan_t_setup_ = false; // only one parameter setup per time
          solder_setup_ = true; 
          fan_spd_setup_ = false; // only one parameter setup per time
        } 
        solder_t_ = temp;
    };

    void update_hairdryer(bool is_setup_mode_t, bool is_setup_mode_spd, uint16_t temp, uint16_t speed) {
          setup_time_ = millis();

        if (is_setup_mode_t) {
          fan_t_setup_ = true;
          solder_setup_ = false; // only one parameter setup per time
          fan_spd_setup_ = false;// only one parameter setup per time
        } 

        if (is_setup_mode_spd) {
          fan_t_setup_ = false; // only one parameter setup per time
          solder_setup_ = false; // only one parameter setup per time
          fan_spd_setup_ = true;
        }
        
        fan_t_ = temp;
        fan_spd_ = speed;
    };

    void solder_stick_switch(bool status) {
      solder_enabled_  = status;
    };

    void fan_switch(bool status) {
      fan_enabled_ = status;
    }

    void show_hello() {
        lcd.clear();
        lcd.setCursor(BEGIN_LINE, FIRST_LINE);
        lcd.print(" SOLDER STATION");
        lcd.setCursor(BEGIN_LINE, SECOND_LINE);
        lcd.print(" version ");
        lcd.print(VER_MAJOR);
        lcd.print(".");
        lcd.print(VER_MINOR);
        lcd.print(".");
        lcd.print(VER_PATCH);
        delay (Configure::HELLO_TIMEOUT_ms);
        lcd.clear();
    };

    void show_main_screen() {
        build_solder_line();
        build_fan_line();
    }

    void alarm() {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("  Shit here now");

      delay(3000); // del
    };

    void exception(const char* message) { // half work
      lcd.clear();
      uint8_t len = strlen(message);
      lcd.setCursor(16, 0);
      lcd.autoscroll();

      uint8_t i = 0;
      while(i != len) {
          lcd.print(message[i++]);
          delay(300);
      }

      lcd.noAutoscroll();
    };

    void test_setup_solder_temp() { // DEL
        solder_stick_switch(true);
        update_solder_stick(true, 666);
    };

    void test_setup_fan_temp() { // DEL
        fan_switch(true);
        update_hairdryer(true, false, 999, 8);
    };

    void test_setup_fan_speed() { // DEL
        fan_switch(true);
        update_hairdryer(false, true, 333, 33);
    };

  private:
    void build_solder_line() {
        lcd.setCursor(BEGIN_LINE, FIRST_LINE);
        lcd.print("Solder ");
        
        lcd.setCursor(SOLDER_DEG_POS, FIRST_LINE);
        lcd.write((byte) 0); // degree
        lcd.print("C");
    };

    void build_fan_line() {
        lcd.setCursor(BEGIN_LINE, SECOND_LINE);
        lcd.print("Fan ");
        
        lcd.setCursor(FAN_DEG_POS, SECOND_LINE);
        lcd.write(byte(0)); // degree
        lcd.print("C");
        lcd.print(" ");

        lcd.setCursor(FAN_PERCENT_POS, SECOND_LINE);
        lcd.print("%");
    };

  private:
    LiquidCrystal lcd;
    uint16_t solder_t_;
    uint16_t fan_t_;
    uint16_t fan_spd_;

    bool solder_setup_;
    bool fan_t_setup_;
    bool fan_spd_setup_;

    uint64_t setup_time_;
    uint64_t time_phase_;
    
    bool solder_enabled_;
    bool fan_enabled_;
    bool phase_;// = true; // true- show, false - empty
};