#include "Common.h"
#include "Screen.h"
#include "Solder_hairdryer.h"
#include "Soldering_stick.h"

Screen *screen = nullptr;
Beeper *beeper = nullptr;
SolderingHairdryer *dryer = nullptr;
SolderStick *stick = nullptr;

void setup() {
    screen = new Screen();
    beeper = new Beeper();

    if (!screen && beeper) {
        beeper->alarm();
    }

    if (screen && !beeper) {
        screen->alarm();
    }

    if (!screen && !beeper) {
        reset_mk();
    }
    
    dryer = new SolderingHairdryer(beeper, screen);
    if (!dryer) {
        beeper->alarm();
        screen->exception("Hairdryer module error!");
    }

    stick = new SolderStick(beeper, screen);
    if (!stick) {
        beeper->alarm();
        screen->exception("Soldering stick module error!");
    }

    screen->show_hello();
    screen->show_main_screen();
}

//uint64_t looper = 0;
void loop() {
    dryer->update();
    stick->update();
    screen->update();
    //looper++;

//     if (looper == 1000) screen->test_setup_solder_temp();
//     if (looper == 3000) screen->test_setup_fan_temp();
//     if (looper == 5000) screen->test_setup_fan_speed();
 }