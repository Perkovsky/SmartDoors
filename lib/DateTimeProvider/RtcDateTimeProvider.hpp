#pragma once

#include <uRTCLib.h>
#include "AbstractDateTimeProvider.hpp"

class RtcDateTimeProvider final : public AbstractDateTimeProvider {
private:
    uRTCLib& _rtc;

public:
    RtcDateTimeProvider(uRTCLib& rtc): _rtc(rtc) {}

    String getTimestamp() override {
        char timestamp[20];
        sprintf(timestamp, "20%02d-%02d-%02d %02d:%02d:%02d", _rtc.year(), _rtc.month(), _rtc.day(), _rtc.hour(), _rtc.minute(), _rtc.second());
        return String(timestamp);
    }

    String getDateTime() override {
        char timestamp[17];
        sprintf(timestamp, "20%02d-%02d-%02d %02d:%02d", _rtc.year(), _rtc.month(), _rtc.day(), _rtc.hour(), _rtc.minute());
        return String(timestamp);
    }
};