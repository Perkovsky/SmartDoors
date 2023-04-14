#pragma once

#include <Arduino.h>
#include "BaseLogger.hpp"

class SerialLogger final : public BaseLogger {
private:
    Stream& _stream;

public:
    SerialLogger(const LogLevel logLevel, Stream& stream, AbstractDateTimeProvider& dateTimeProvider, TelegramNotifier* notifier)
        : BaseLogger(logLevel, dateTimeProvider, notifier), _stream(stream) {}

protected:
    bool log(const String& message) override {
        _stream.println(message);
        return true;
    }
};