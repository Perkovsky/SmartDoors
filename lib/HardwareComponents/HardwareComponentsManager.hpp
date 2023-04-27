#pragma once

#include "AbstractDateTimeProvider.hpp"
#include "LcdDisplay.hpp"

class HardwareComponentsManager {
private:
    AbstractDateTimeProvider& _dateTimeProvider;
    LcdDisplay _lcdDisplay;

    String _currentDateTime;

    void printCurrentDateTime() {
        const auto dt = _dateTimeProvider.getDateTime();
        if (_currentDateTime == dt)
            return;
        
        _currentDateTime = dt;
        _lcdDisplay.print(dt);
    }

public:
    HardwareComponentsManager(AbstractDateTimeProvider& dateTimeProvider)
        : _dateTimeProvider(dateTimeProvider) {}

    void init() {
        _lcdDisplay.init();
    }

    void refresh() {
        printCurrentDateTime();
    }
};