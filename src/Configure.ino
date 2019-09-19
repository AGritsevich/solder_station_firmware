class Configure { // or enam
  public:
    typedef enum {
      solder_ctrl = A4,  // change temp of solder stick
      solder_therm = A3,  // thermometr of solder stick
      hairdryer_ctrl = A2,  // change temp of hot air
      hairdryer_therm = A1,  // thermometr of hairdryer
      hairdryer_motor_ctrl = A0,  // change speed of cooler
      lcd_D7 = 13, // data D7
      lcd_D6 = 12, // data D6
      lcd_D5 = 11, // data D5
      lcd_D4 = 10, // data D4
      lcd_E = 9, // E - erase pin
      lcd_RS = 8, // RS pin
      beeper = 7, // beeper + leg
      hairdryer_heater = 6,  // switcher of heater
      solder_heater = 5,  // switcher of heater
      hairdryer_switch = 4,  // switcher of hairdryer
      hairdryer_motor = 3,  // switcher of motor cooler
      solder_switch = 2,  // switcher of solder stick
    } PINs;
};