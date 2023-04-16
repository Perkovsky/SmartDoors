#pragma once

#include <SdFat.h>
#include "BaseLogger.hpp"

class SdCardLogger final : public BaseLogger {
private:
    const String FIlE_NAME = F("log001.txt");
    SdFat& _sd;

public:
    SdCardLogger(const LogLevel logLevel, SdFat& sd, AbstractDateTimeProvider& dateTimeProvider, TelegramNotifier* notifier)
        : BaseLogger(logLevel, dateTimeProvider, notifier), _sd(sd) {}

protected:
    bool log(const String& message) override {
        File file = _sd.open(FIlE_NAME, FILE_WRITE);
        
        if (!file)
            return false;

        file.println(message);
        file.close();
        
        return true;
    }
};