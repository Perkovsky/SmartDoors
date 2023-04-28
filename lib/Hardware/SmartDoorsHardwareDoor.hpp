#pragma once

#include <map>
#include <String.h>
#include "LedLighter.hpp"
#include "LcdDisplay.hpp"

#define DOORS_COUNT 1

struct SmartDoorsHardwareDoor {
    LedLighter* ledLighter;
    LcdDisplay* lcdDisplay;
};