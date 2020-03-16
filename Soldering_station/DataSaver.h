#include "Arduino.h"

class DataSaver {
  public:
    uint32_t eeprom_read_page(int length) {
        int start = here * 2;
        for (int x = 0; x < length; x++) {
            int addr = start + x;
            uint8_t ee = spi_transaction(0xA0, (addr >> 8) & 0xFF, addr & 0xFF, 0xFF);
            //SERIAL.print((char) ee);
        }
        return STK_OK;
    }
  private:
};