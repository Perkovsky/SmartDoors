#pragma once

#include <Arduino.h>
#include "BaseHardwareDevice.hpp"

class LedLighter final : public BaseHardwareDevice {
private:
    const u_int8_t _ledPin;

public:
    LedLighter(const u_int8_t ledPin)
        : _ledPin(ledPin) {}

    void init() override {
        pinMode(_ledPin, OUTPUT);
    }

    void lightOn() {
        digitalWrite(_ledPin, HIGH);
    }

     void lightOff() {
        digitalWrite(_ledPin, LOW);
    }
};