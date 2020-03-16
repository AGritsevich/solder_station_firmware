#pragma once

class Configure { // or enam
  public:
    typedef enum {
      hairdryer_gerkon = A5, //                                   --- OK
      solder_target_temp = A4, // change temp of solder stick     --- OK
      solder_therm = A3,  // thermometr of solder stick           ---  ?
      hairdryer_target_heater_temp = A2, // change temp of hotair --- OK
      hairdryer_therm = A1,  // thermometr of hairdryer           ---  ?
      hairdryer_target_motor_speed = A0, // change speed of fan   --- OK
      lcd_D7 = 13, // data D7                                     --- OK
      lcd_D6 = 12, // data D6                                     --- OK
      lcd_D5 = 11, // data D5                                     --- OK // PWM
      lcd_D4 = 10, // data D4                                     --- OK // PWM
      lcd_E = 9, // E - erase pin                                 --- OK // PWM
      lcd_RS = 8, // RS pin                                       --- OK
      beeper = 7, // beeper '+' leg                               --- OK
      hairdryer_motor = 6,  // switcher of motor fan              ---  ?  // PWM  
      solder_ctrl = 5,  // switcher of heater                     --- OK  // PWM   can be rulled
      hairdryer_heater_ctrl = 4,  // switcher of heater           --- OK
      solder_btn = 3,  // switch on\off scenario of solder stick  ---  ? // INTERRUPTS, PWM
      hairdryer_btn = 2,  // switch on\off scenario of hairdryer  ---  ? // INTERRUPTS
      hot_solder_stick_led = 1, //                                --- OK
      hot_hairdryer_led = 0 //                                    --- OK
    } PINs;

    // PWM: DP11, DP10, DP9, DP6, DP5, DP3
    // INTERRUPTS: DP2, DP3 

    const static uint8_t HOT_TEMP = 61; // in Celsium
    const static uint8_t TEMP_DELTA = 10;
    const static uint16_t Upper_temperature_limit = 480;

    const static uint8_t FUN_0_percent = 0;
    const static uint8_t FUN_5_percent = 5;
    const static uint8_t FUN_10_percent = 10;
    const static uint8_t FUN_20_percent = 20;
    const static uint8_t FUN_30_percent = 30;
    const static uint8_t FUN_40_percent = 40;
    const static uint8_t FUN_50_percent = 50;
    const static uint8_t FUN_60_percent = 60;
    const static uint8_t FUN_70_percent = 70;
    const static uint8_t FUN_80_percent = 80;
    const static uint8_t FUN_90_percent = 90;
    const static uint16_t FUN_100_percent = 99;
    const static uint8_t HAIRDRYER_SAVE_TEMP = 150;
    const static uint64_t TIME_To_REACT_ms = 10000u;
    const static uint64_t SETUP_TIME_ms = 10000u;
    const static uint64_t BLINK_TIME_ms = 300u;
    const static uint32_t HELLO_TIMEOUT_ms = 500u;
};