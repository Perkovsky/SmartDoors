#pragma once

#include <String.h>
#include <WiFi.h>
#include "LoggerFactory.hpp"

class WiFiManager {
private:
    const char* _ssid;
    const char* _password;
    LoggerFactory& _logger;

public:
    WiFiManager(const String& ssid, const String& password, LoggerFactory& logger)
        : _ssid(ssid.c_str()), _password(password.c_str()), _logger(logger) {}

    void connect() {
        pinMode(LED_BUILTIN, OUTPUT);
        digitalWrite(LED_BUILTIN, LOW);

        WiFi.begin(_ssid, _password);

        while (WiFi.status() == WL_CONNECTED) {
            delay(100);
            digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
        }

        digitalWrite(LED_BUILTIN, HIGH);
        _logger.logInfo("WiFi has been connected");
        
        // log IP
        String info("Local IP Address: ");
        info += WiFi.localIP().toString();
        _logger.logInfo(info);

        // log host name
        info = "Local Host Name: ";
        info += WiFi.getHostname();
        _logger.logInfo(info);
    }

    void checkConnection() {
        IPAddress ip;
        if (WiFi.hostByName("google.com", ip)) {
            String result("Ping google.com successful. IP address: ");
            result += ip.toString();
            _logger.logInfo(result);
        } else {
            _logger.logWarning("Ping google.com failed");
        }
    }

    void disconnect() {
        WiFi.disconnect();
        _logger.logInfo("WiFi has been disconnected");
    }
};