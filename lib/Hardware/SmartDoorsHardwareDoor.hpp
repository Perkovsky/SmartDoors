#pragma once

#include <map>
#include <String.h>
#include "LcdDisplay.hpp"

#define DOORS_COUNT 1

struct SmartDoorsLcdDisplay {
    LcdDisplay* lcdDisplay;
    unsigned long lcdDisplayTimer;
};

struct SmartDoorsHardwareDoor {
    SmartDoorsLcdDisplay lcdDisplayObject;
};