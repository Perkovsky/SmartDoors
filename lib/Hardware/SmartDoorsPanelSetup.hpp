#pragma once

#include <map>
#include <String.h>

struct SmartDoorsDoorSetup {
    u_int8_t ledPin;
    u_int8_t lcdAddress;
};

struct SmartDoorsPanelSetup {
private:
    SmartDoorsDoorSetup door1 = {16, 0x27};

public:
    String id;
    std::map<u_int8_t, SmartDoorsDoorSetup> doors;

    SmartDoorsPanelSetup()
        : id("0050c23d8000"), doors({{1, door1}}) {}
};

// Declare the Panel variable as extern
extern SmartDoorsPanelSetup Panel;