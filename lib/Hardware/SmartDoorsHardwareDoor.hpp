#pragma once

#include <map>
#include <String.h>
#include "LedLighter.hpp"
#include "LcdDisplay.hpp"

struct SmartDoorsHardwareDoor {
    LedLighter* ledLighter;
    LcdDisplay* lcdDisplay;
};