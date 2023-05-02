#pragma once

#include <map>
#include <String.h>
#include "LedLighter.hpp"
#include "LcdDisplay.hpp"
#include "DoorOpenModule.hpp"

struct SmartDoorsHardwareDoor {
    LedLighter* ledLighter;
    LcdDisplay* lcdDisplay;
    DoorOpenModule* doorOpenModule;
};