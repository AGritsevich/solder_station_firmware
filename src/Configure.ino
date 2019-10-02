class Configure { // or enam
  public:
    typedef enum {
      hairdryer_gerkon = A5,
      solder_target_temp = A4,  // change temp of solder stick
      solder_therm = A3,  // thermometr of solder stick
      hairdryer_target_heater_temp = A2,  // change temp of hot air
      hairdryer_therm = A1,  // thermometr of hairdryer
      hairdryer_target_motor_speed = A0,  // change speed of cooler
      lcd_D7 = 13, // data D7
      lcd_D6 = 12, // data D6
      lcd_D5 = 11, // data D5
      lcd_D4 = 10, // data D4
      lcd_E = 9, // E - erase pin
      lcd_RS = 8, // RS pin
      beeper = 7, // beeper + leg
      hairdryer_heater_ctrl = 6,  // switcher of heater
      solder_ctrl = 5,  // switcher of heater
      hairdryer_btn = 4,  // switch of\off of hairdryer TODO HW move to 3 Digital Pin
      hairdryer_motor = 3,  // switcher of motor cooler TODO HW move to 4 Digital Pin
      solder_btn = 2,  // switch on\off of solder stick
      hot_solder_stick_led = 1,
      hot_hair_dryer_led = 0
    } PINs;

    const static uint8_t HOT_TEMP = 61; // in Celsium
    const static uint8_t TEMP_DELTA = 10;
    const static uint8_t Upper_temperature_limit = 480;

    const static uint8_t FUN_0_percent = 0;
    const static uint8_t FUN_5_percent = 13;
    const static uint8_t FUN_10_percent = 26;
    const static uint8_t FUN_20_percent = 51;
    const static uint8_t FUN_30_percent = 77;
    const static uint8_t FUN_40_percent = 102;
    const static uint8_t FUN_50_percent = 128;
    const static uint8_t FUN_60_percent = 153;
    const static uint8_t FUN_70_percent = 179;
    const static uint8_t FUN_80_percent = 204;
    const static uint8_t FUN_90_percent = 230;
    const static uint8_t FUN_100_percent = 255;

    const static uint64_t TIME_To_REACT_ms = 10000u;
};