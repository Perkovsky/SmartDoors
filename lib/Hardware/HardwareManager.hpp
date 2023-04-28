#pragma once

#include "SmartDoorsPanelSetup.hpp"
#include "AbstractDateTimeProvider.hpp"
#include "SmartDoorsHardwareDoor.hpp"

class HardwareManager {
private:
    const u_int32_t DEFAULT_DELAY = 5000; // 5 seconds
    const SmartDoorsPanelSetup _panel;
    String _currentDateTime;
    AbstractDateTimeProvider& _dateTimeProvider;
    SmartDoorsHardwareDoor doors[DOORS_COUNT];
    
    void setup() {
        for (u_int8_t i = 0; i < DOORS_COUNT; i++) {
            SmartDoorsHardwareDoor door;
            const SmartDoorsDoorSetup& doorSetup = _panel.doors.at(i + 1);
            const u_int8_t lcdAddress = doorSetup.lcdAddress;
            door.ledLighter = new LedLighter(doorSetup.ledPin);
            door.lcdDisplay = new LcdDisplay(lcdAddress);
            doors[i] = door;
        }
    }

    void lightOff(LedLighter* ledLighter) {
        if (millis() - ledLighter->getTimer() < DEFAULT_DELAY) {
            return;
        }
        
        ledLighter->setTimer(0);
        ledLighter->lightOff();
    }

    void printCurrentDateTime(LcdDisplay* lcdDisplay) {
        if (millis() - lcdDisplay->getTimer() < DEFAULT_DELAY) {
            return;
        }
        
        const auto dt = _dateTimeProvider.getDateTime();
        if (_currentDateTime == dt) {
            return;
        }
        
        _currentDateTime = dt;
        lcdDisplay->setTimer(0);
        lcdDisplay->print(dt);
    }

public:
    HardwareManager(AbstractDateTimeProvider& dateTimeProvider)
        : _dateTimeProvider(dateTimeProvider)
    {
        setup();
    }

    void init() {
        for (u_int8_t i = 0; i < DOORS_COUNT; i++) {
            doors[i].ledLighter->init();
            doors[i].lcdDisplay->init();
        }
    }

    void refresh() {
        for (u_int8_t i = 0; i < DOORS_COUNT; i++) {
            lightOff(doors[i].ledLighter);
            printCurrentDateTime(doors[i].lcdDisplay);
        }
    }

    String getPanelId() {
        return _panel.id;
    }

    bool isDoorExisting(u_int8_t door) {
        return _panel.doors.find(door) != _panel.doors.end();
    }

    void light(u_int8_t door) {
        doors[door - 1].ledLighter->setTimer(millis());
        doors[door - 1].ledLighter->lightOn();
    }

    void print(u_int8_t door, const String& text, u_int8_t line = 0, u_int8_t pos = 0) {
        doors[door - 1].lcdDisplay->setTimer(millis());
        doors[door - 1].lcdDisplay->print(text, line, pos);
        _currentDateTime = text;
    }
};