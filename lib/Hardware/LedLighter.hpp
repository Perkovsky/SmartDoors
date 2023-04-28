#pragma once

#include <Arduino.h>

class LedLighter {
private:
    const u_int8_t _ledPin;
    unsigned long _timer = 0;

public:
    LedLighter(const u_int8_t ledPin)
        : _ledPin(ledPin) {}

    void init() {
        pinMode(_ledPin, OUTPUT);
    }

    void setTimer(unsigned long value) {
        _timer = value;
    }

    unsigned long getTimer() {
        return _timer;
    }

    void lightOn() {
        digitalWrite(_ledPin, HIGH);
    }

     void lightOff() {
        digitalWrite(_ledPin, LOW);
    }
};