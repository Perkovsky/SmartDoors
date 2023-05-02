#pragma once

#include <map>
#include <String.h>

#define DOORS_COUNT 1

struct SmartDoorsDoorSetup {
    u_int8_t ledPin;
    u_int8_t lcdAddress;
    u_int8_t buzzerPin;
    byte ssPin;
    byte rstPin;
    unsigned long cardUids[2];
};

struct SmartDoorsPanelSetup {
private:
    SmartDoorsDoorSetup door1 = {16, 0x27, 11, 5, 0, {4087547319, 2736997647}};

public:
    String id;
    std::map<u_int8_t, SmartDoorsDoorSetup> doors;

    SmartDoorsPanelSetup()
        : id("0050c23d8000"), doors({{1, door1}}) {}
};

// Declare the Panel variable as extern
extern SmartDoorsPanelSetup Panel;