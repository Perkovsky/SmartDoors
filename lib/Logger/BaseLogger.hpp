#pragma once

#include <String.h>
#include <uRTCLib.h>
#include "LogLevel.hpp"
#include "AbstractDateTimeProvider.hpp"
#include "TelegramNotifier.hpp"

class BaseLogger {
private:
    const LogLevel _logLevel;
    AbstractDateTimeProvider& _dateTimeProvider;
    TelegramNotifier* _notifier;

    static String toString(const LogLevel& logLevel) {
        switch (logLevel) {
            case LogLevel::Debug:
                return F("DBG");
            case LogLevel::Warning:
                return F("WRN");
            case LogLevel::Error:
                return F("ERR");
            default:
                return F("INF");
        }
    }

    String buildLogMessage(const LogLevel& logLevel, const String& message) {
        const auto timestamp = _dateTimeProvider.getTimestamp();
        const auto strLogLevel = toString(logLevel);
        
        String result("[");
        result += timestamp;
        result += " ";
        result += strLogLevel;
        result += "] ";
        result += message;

        return result;
    }

    void log(const LogLevel& logLevel, const String& message)
    {
        if (logLevel < _logLevel)
            return;

        const String logMessage = buildLogMessage(logLevel, message);
        
        if  (!log(logMessage))
        {
            String notifyMessage("Cannot log message: ");
            notifyMessage += message;
            _notifier->notify(notifyMessage);
        }
    }

protected:
    explicit BaseLogger(const LogLevel logLevel, AbstractDateTimeProvider& dateTimeProvider, TelegramNotifier* notifier)
        : _logLevel(logLevel), _dateTimeProvider(dateTimeProvider), _notifier(notifier) {}

    virtual ~BaseLogger() = default;

    virtual bool log(const String& message) = 0;

public:
    void logDebug(const String& message) {
        log(LogLevel::Debug, message);
    }

    void logInfo(const String& message) {
        log(LogLevel::Info, message);
    }

    void logWarning(const String& message) {
        log(LogLevel::Warning, message);
    }

    void logError(const String& message) {
        log(LogLevel::Error, message);
    }
};