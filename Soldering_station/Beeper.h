#pragma once

static const uint16_t note_B0  = 31;
static const uint16_t note_C1  = 33;
static const uint16_t note_CS1 = 35;
static const uint16_t note_D1  = 37;
static const uint16_t note_DS1 = 39;
static const uint16_t note_E1  = 41;
static const uint16_t note_F1  = 44;
static const uint16_t note_FS1 = 46;
static const uint16_t note_G1  = 49;
static const uint16_t note_GS1 = 52;
static const uint16_t note_A1  = 55;
static const uint16_t note_AS1 = 58;
static const uint16_t note_B1  = 62;
static const uint16_t note_C2  = 65;
static const uint16_t note_CS2 = 69;
static const uint16_t note_D2  = 73;
static const uint16_t note_DS2 = 78;
static const uint16_t note_E2  = 82;
static const uint16_t note_F2  = 87;
static const uint16_t note_FS2 = 93;
static const uint16_t note_G2  = 98;
static const uint16_t note_GS2 = 104;
static const uint16_t note_A2  = 110;
static const uint16_t note_AS2 = 117;
static const uint16_t note_B2  = 123;
static const uint16_t note_C3  = 131;
static const uint16_t note_CS3 = 139;
static const uint16_t note_D3  = 147;
static const uint16_t note_DS3 = 156;
static const uint16_t note_E3  = 165;
static const uint16_t note_F3  = 175;
static const uint16_t note_FS3 = 185;
static const uint16_t note_G3  = 196;
static const uint16_t note_GS3 = 208;
static const uint16_t note_A3  = 220;
static const uint16_t note_AS3 = 233;
static const uint16_t note_B3  = 247;
static const uint16_t note_C4  = 262;
static const uint16_t note_CS4 = 277;
static const uint16_t note_D4  = 294;
static const uint16_t note_DS4 = 311;
static const uint16_t note_E4  = 330;
static const uint16_t note_F4  = 349;
static const uint16_t note_FS4 = 370;
static const uint16_t note_G4  = 392;
static const uint16_t note_GS4 = 415;
static const uint16_t note_A4  = 440;
static const uint16_t note_AS4 = 466;
static const uint16_t note_B4  = 494;
static const uint16_t note_C5  = 523;
static const uint16_t note_CS5 = 554;
static const uint16_t note_D5  = 587;
static const uint16_t note_DS5 = 622;
static const uint16_t note_E5  = 659;
static const uint16_t note_F5  = 698;
static const uint16_t note_FS5 = 740;
static const uint16_t note_G5  = 784;
static const uint16_t note_GS5 = 831;
static const uint16_t note_A5  = 880;
static const uint16_t note_AS5 = 932;
static const uint16_t note_B5  = 988;
static const uint16_t note_C6  = 1047;
static const uint16_t note_CS6 = 1109;
static const uint16_t note_D6  = 1175;
static const uint16_t note_DS6 = 1245;
static const uint16_t note_E6  = 1319;
static const uint16_t note_F6  = 1397;
static const uint16_t note_FS6 = 1480;
static const uint16_t note_G6  = 1568;
static const uint16_t note_GS6 = 1661;
static const uint16_t note_A6  = 1760;
static const uint16_t note_AS6 = 1865;
static const uint16_t note_B6  = 1976;
static const uint16_t note_C7  = 2093;
static const uint16_t note_CS7 = 2217;
static const uint16_t note_D7  = 2349;
static const uint16_t note_DS7 = 2489;
static const uint16_t note_E7  = 2637;
static const uint16_t note_note_F7  = 2794;
static const uint16_t note_FS7 = 2960;
static const uint16_t note_G7  = 3136;
static const uint16_t note_GS7 = 3322;
static const uint16_t note_A7  = 3520;
static const uint16_t note_AS7 = 3729;
static const uint16_t note_B7  = 3951;
static const uint16_t note_C8  = 4186;
static const uint16_t note_CS8 = 4435;
static const uint16_t note_D8  = 4699;
static const uint16_t note_DS8 = 4978;
//}Note;

typedef struct {
    uint16_t note;
    uint16_t quart;
}Quant;

Quant melody1 [] = {
    {note_C4, 4}, 
    {note_G3, 8}, 
    {note_G3, 8}, 
    {note_GS3, 4}, 
    {note_G3, 4}, 
    {note_B3, 4}, 
    {note_C4, 4}, 
    {note_C4, 4}
};

class Beeper {
  public:
    void temprature_reached(bool is_very_first_heat) {
      if (is_very_first_heat) {
        // TODO spec beep here
        //play_quant(melody1);
      }
    };

    void increase_beep(bool is_very_first_heat) {
      if (is_very_first_heat) {
        // TODO spec beep here
        //play_quant(melody1);
      }
    };

    void alarm() {
      tone(Configure::PINs::beeper, note_C6, 200);
    };

private:
    void play_quant(const Quant melody[8]) {
        // iterate over the notes of the melody:
        for (uint16_t it = 0; it < 8; it++) {
          // to calculate the note duration, take one second
          // divided by the note type.
          //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
          int duration = sec/melody[it].quart;
          tone(Configure::PINs::beeper, melody[it].note, duration);
          //pause for the note's duration plus 30 ms:
          delay(duration + 30);
        }
    }


  private:
    static const uint32_t sec = 1000u; // ms
};