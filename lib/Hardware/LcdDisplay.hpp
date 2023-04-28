#pragma once

#include <LiquidCrystal_I2C.h>

class LcdDisplay {
private:
    LiquidCrystal_I2C* _lcd;

public:
    LcdDisplay(const u_int8_t lcdAddress) {
        _lcd = new LiquidCrystal_I2C(lcdAddress, 16, 2);
    }

    void init() {
        _lcd->init();
        _lcd->backlight();
        _lcd->clear();
        _lcd->setCursor(0, 0);
    }

    void print(const String& text, u_int8_t line = 0, u_int8_t pos = 0) {
        _lcd->clear();
        _lcd->setCursor(pos, line);
        _lcd->print(text);
    }
};