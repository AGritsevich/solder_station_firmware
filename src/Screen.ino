#include <LiquidCrystal.h>

class Screen {
  public:
    Screen():
      lcd(Configure::PINs:led_D7, Configure::PINs:led_D6, Configure::PINs:led_D5, 
          Configure::PINs:led_D4, Configure::PINs:led_E, Configure::PINs:led_RS) {
        
        lcd.begin(16, 2);
        lcd.createChar(1, cels);
    };

    void update() {
        lcd.clear();
    };

    void update_solder_stick(setup_mode_, temp);
    screen.update_hairdryer(setup_mode_, temp, speed);

    void show_hello() {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("SOLDER STATION");
        lcd.setCursor(0, 1);
        lcd.print("ver. 0.1");
        delay (3000);
    };

  private:
    LiquidCrystal lcd;
    static const cels[8] = {B00111, B00101, B00111, B00000, B00000, B00000, B00000};

};