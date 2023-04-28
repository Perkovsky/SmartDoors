#pragma once

#include "SmartDoorsPanelSetup.hpp"
#include "AbstractDateTimeProvider.hpp"
#include "SmartDoorsHardwareDoor.hpp"

class HardwareManager {
private:
    const u_int32_t DEFAULT_DELAY = 5000; // 5 seconds
    String _currentDateTime;
    const SmartDoorsPanelSetup& _panel;
    AbstractDateTimeProvider& _dateTimeProvider;
    SmartDoorsHardwareDoor doors[DOORS_COUNT];
    
    void setup() {
        for (u_int8_t i = 0; i < DOORS_COUNT; i++) {
            SmartDoorsHardwareDoor door;
            const SmartDoorsDoorSetup& doorSetup = _panel.doors.at(i + 1);
            const u_int8_t lcdAddress = doorSetup.lcdAddress;
            door.lcdDisplayObject.lcdDisplay = new LcdDisplay(lcdAddress);
            door.lcdDisplayObject.lcdDisplayTimer = 0;
            doors[i] = door;
        }
    }

    void printCurrentDateTime(SmartDoorsLcdDisplay& lcdDisplayObject) {
        if (millis() - lcdDisplayObject.lcdDisplayTimer < DEFAULT_DELAY) {
            return;
        }
        
        const auto dt = _dateTimeProvider.getDateTime();
        if (_currentDateTime == dt) {
            return;
        }
        
        _currentDateTime = dt;
        lcdDisplayObject.lcdDisplayTimer = 0;
        lcdDisplayObject.lcdDisplay->print(dt);
    }

public:
    HardwareManager(const SmartDoorsPanelSetup& panel, AbstractDateTimeProvider& dateTimeProvider)
        : _panel(panel), _dateTimeProvider(dateTimeProvider)
    {
        setup();
    }

    void init() {
        for (u_int8_t i = 0; i < DOORS_COUNT; i++) {
            doors[i].lcdDisplayObject.lcdDisplay->init();
        }
    }

    void refresh() {
        for (u_int8_t i = 0; i < DOORS_COUNT; i++) {
            printCurrentDateTime(doors[i].lcdDisplayObject);
        }
    }

    String getPanelId() {
        return _panel.id;
    }

    bool isDoorExisting(u_int8_t door) {
        return _panel.doors.find(door) != _panel.doors.end();
    }

    void print(u_int8_t door, const String& text, u_int8_t line = 0, u_int8_t pos = 0) {
        doors[door - 1].lcdDisplayObject.lcdDisplayTimer = millis();
        doors[door - 1].lcdDisplayObject.lcdDisplay->print(text, line, pos);
        _currentDateTime = text;
    }
};