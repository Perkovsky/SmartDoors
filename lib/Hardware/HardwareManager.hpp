#pragma once

#include "SmartDoorsPanelSetup.hpp"
#include "AbstractDateTimeProvider.hpp"
#include "SmartDoorsHardwareDoor.hpp"

class HardwareManager {
private:
    const SmartDoorsPanelSetup& _panel;
    AbstractDateTimeProvider& _dateTimeProvider;
    SmartDoorsHardwareDoor doors[DOORS_COUNT];
    
    void setup() {
        for (u_int8_t i = 0; i < DOORS_COUNT; i++) {
            SmartDoorsHardwareDoor door;
            const SmartDoorsDoorSetup& doorSetup = _panel.doors.at(i + 1);
            const u_int8_t lcdAddress = doorSetup.lcdAddress;
            door.lcdDisplayObject.lcdDisplay = new LcdDisplay(lcdAddress);
            doors[i] = door;
        }
    }

    void printCurrentDateTime(SmartDoorsLcdDisplay& lcdDisplayObject) {
        const auto dt = _dateTimeProvider.getDateTime();
        if (lcdDisplayObject.currentDateTime == dt)
            return;
        
        lcdDisplayObject.currentDateTime = dt;
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
            doors[0].lcdDisplayObject.lcdDisplay->init();
        }
    }

    void refresh() {
        for (u_int8_t i = 0; i < DOORS_COUNT; i++) {
            printCurrentDateTime(doors[i].lcdDisplayObject);
        }
    }
};