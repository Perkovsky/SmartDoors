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
        _logger.logInfo(F("WiFi has been connected"));
        
        // log IP
        String strIP = WiFi.localIP().toString();
        String infoIP("Local IP Address: ");
        infoIP.reserve(strIP.length() + 19);
        infoIP += strIP;
        _logger.logInfo(infoIP);

        // log host name
        String hostname = WiFi.getHostname();
        String infoHostname = "Local Host Name: ";
        infoIP.reserve(infoHostname.length() + 18);
        infoHostname += hostname;
        _logger.logInfo(infoHostname);
    }

    void checkConnection() {
        IPAddress ip;
        if (WiFi.hostByName("google.com", ip)) {
            String strIP = ip.toString();
            String result("Ping google.com successful. IP address: ");
            result.reserve(strIP.length() + 41);
            result += strIP;
            _logger.logInfo(result);
        } else {
            _logger.logWarning(F("Ping google.com failed"));
        }
    }

    void disconnect() {
        WiFi.disconnect();
        _logger.logInfo(F("WiFi has been disconnected"));
    }
};