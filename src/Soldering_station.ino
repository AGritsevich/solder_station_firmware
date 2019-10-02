SolderingHairdryer *dryer = nullptr;
SolderStick *stick = nullptr;
Screen *screen = nullptr;

void setup() {
    screen = new Screen();
    if (!screen) {
        // TODO alarm
    }
    
    dryer = new SolderingHairdryer(screen);
    if (!dryer) {
        //TODO alarm
    }

    stick = new SolderStick(screen);
    if (!stick) {
        // TODO alarm
    }

    screen.hello();
}

void loop() {
    dryer.update();
    stick.upadte();
}