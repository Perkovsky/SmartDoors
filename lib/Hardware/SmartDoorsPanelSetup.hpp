#pragma once

#include <map>
#include <String.h>

struct SmartDoorsDoorSetup {
    u_int8_t ledPin;
    u_int8_t lcdAddress;
};

struct SmartDoorsPanelSetup {
    String id;
    std::map<u_int8_t, SmartDoorsDoorSetup> doors;
};