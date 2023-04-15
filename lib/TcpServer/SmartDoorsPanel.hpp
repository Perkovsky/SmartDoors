#pragma once

#include <map>
#include <String.h>

struct SmartDoorsDoor {
    int8_t ledPin;
    // add other needed properties
};

struct SmartDoorsPanel {
    String id;
    std::map<int8_t, SmartDoorsDoor> doors;
};