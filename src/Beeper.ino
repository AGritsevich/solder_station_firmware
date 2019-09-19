#pragma once

typedef enum {
    B0  = 31,
    C1  = 33,
    CS1 = 35,
    D1  = 37,
    DS1 = 39,
    E1  = 41,
    F1  = 44,
    FS1 = 46,
    G1  = 49,
    GS1 = 52,
    A1  = 55,
    AS1 = 58,
    B1  = 62,
    C2  = 65,
    CS2 = 69,
    D2  = 73,
    DS2 = 78,
    E2  = 82,
    F2  = 87,
    FS2 = 93,
    G2  = 98,
    GS2 = 104,
    A2  = 110,
    AS2 = 117,
    B2  = 123,
    C3  = 131,
    CS3 = 139,
    D3  = 147,
    DS3 = 156,
    E3  = 165,
    F3  = 175,
    FS3 = 185,
    G3  = 196,
    GS3 = 208,
    A3  = 220,
    AS3 = 233,
    B3  = 247,
    C4  = 262,
    CS4 = 277,
    D4  = 294,
    DS4 = 311,
    E4  = 330,
    F4  = 349,
    FS4 = 370,
    G4  = 392,
    GS4 = 415,
    A4  = 440,
    AS4 = 466,
    B4  = 494,
    C5  = 523,
    CS5 = 554,
    D5  = 587,
    DS5 = 622,
    E5  = 659,
    F5  = 698,
    FS5 = 740,
    G5  = 784,
    GS5 = 831,
    A5  = 880,
    AS5 = 932,
    B5  = 988,
    C6  = 1047,
    CS6 = 1109,
    D6  = 1175,
    DS6 = 1245,
    E6  = 1319,
    F6  = 1397,
    FS6 = 1480,
    G6  = 1568,
    GS6 = 1661,
    A6  = 1760,
    AS6 = 1865,
    B6  = 1976,
    C7  = 2093,
    CS7 = 2217,
    D7  = 2349,
    DS7 = 2489,
    E7  = 2637,
    F7  = 2794,
    FS7 = 2960,
    G7  = 3136,
    GS7 = 3322,
    A7  = 3520,
    AS7 = 3729,
    B7  = 3951,
    C8  = 4186,
    CS8 = 4435,
    D8  = 4699,
    DS8 = 4978
}Note;

typedef struct {
    uint8_t note;
    uint8_t quart;
}Quant;

Quant melody1 [] = {
    {Note::C4, 4}, 
    {Note::G3, 8}, 
    {Note::G3, 8}, 
    {Note::GS3, 4}, 
    {Note::G3, 4}, 
    {Note::B3, 4}, 
    {Note::C4, 4}, 
    {Note::C4, 4}
};

class Beeper {
  public:
    void play_quant(const Quant **per, uint8_t size) {
        // iterate over the notes of the melody:
        for (auto this = 0; this < size; this++) {
          // to calculate the note duration, take one second
          // divided by the note type.
          //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
          int duration = sec/per[this].quart;
          tone(8, per[this],duration);
          //pause for the note's duration plus 30 ms:
          delay(duration + 30);
        }
    }

  private:
    static const uint8_t Beeper_PIN = Configure::PIN::beeper;
    static const uint8_t sec = 1000u; // ms
};