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
            door.ledLighter = new LedLighter(doorSetup.ledPin);
            door.lcdDisplay = new LcdDisplay(doorSetup.lcdAddress);
            door.rfidReader = new RfidReader(/*doorSetup.buzzerPin,*/ doorSetup.ssPin, doorSetup.rstPin);
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

    void lockDoor(RfidReader* rfidReader) {
        if (millis() - rfidReader->getTimer() < DEFAULT_DELAY) {
            return;
        }
        
        rfidReader->setTimer(0);
        rfidReader->lock();
    }

    bool isValidCard(u_int8_t door, unsigned long searchValue) {
        const SmartDoorsDoorSetup& doorSetup = _panel.doors.at(door);
        unsigned int size = sizeof(doorSetup.cardUids) / sizeof(unsigned long);

        for (u_int8_t i = 0; i < size; i++) {
            if (doorSetup.cardUids[i] == searchValue) {
                return true;
            }
        }

        return false;
    }

    void listeningDoorEvents(u_int8_t door, RfidReader* rfidReader) {
        unsigned long uid = rfidReader->getCardUid();
        if (uid == 0) {
            return;
        }
        
        if (isValidCard(door, uid)) {
            unlock(door);
        } else {
            Serial.println("not valid");
        }
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
            doors[i].rfidReader->init();
        }
    }

    void refresh() {
        for (u_int8_t i = 0; i < DOORS_COUNT; i++) {
            lightOff(doors[i].ledLighter);
            printCurrentDateTime(doors[i].lcdDisplay);
            lockDoor(doors[i].rfidReader);
            listeningDoorEvents(i + 1, doors[i].rfidReader);
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

    void unlock(u_int8_t door) {
        light(door);
        print(door, "Door is opened");
        doors[door - 1].rfidReader->setTimer(millis());
        doors[door - 1].rfidReader->unlock();
    }
};