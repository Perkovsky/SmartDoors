#pragma once

#include <String.h>
#include <Arduino.h>
#include <SdFat.h>
#include <ArduinoJson.h>
#include "Settings.hpp"

class SettingsAccessor {
private:
    const String FIlE_NAME = "settings.json";
    SdFat& _sd;

public:
    SettingsAccessor(SdFat& sd) : _sd(sd) {}

    Settings getSettings() {
        Settings settings;

        if (!_sd.exists(FIlE_NAME)) {
            Serial.print(FIlE_NAME);
            Serial.println(" does not exist");
            return settings;
        }

        File file = _sd.open(FIlE_NAME, FILE_READ);
        if (!file) {
            file.close();
            Serial.print("Failed to open ");
            Serial.println(FIlE_NAME);
            return settings;
        }

        StaticJsonDocument<512> doc;
        DeserializationError error = deserializeJson(doc, file);
        if (error) {
            Serial.print("Failed to parse ");
            Serial.print(FIlE_NAME);
            Serial.print(". Error: ");
            Serial.println(error.c_str());
            file.close();
            doc.clear();
            return settings;
        }

        const char* macAddress = doc["ethernet"]["mac"];
        if (strlen(macAddress) > 0) {
            byte mac[6];
            sscanf(macAddress, "%x:%x:%x:%x:%x:%x", &mac[0], &mac[1], &mac[2], &mac[3], &mac[4], &mac[5]);
            memcpy(settings.ethernet.mac, mac, sizeof(mac));
        }

        settings.logLevel.serial = doc["logLevel"]["serial"].as<String>();
        settings.logLevel.sd = doc["logLevel"]["sd"].as<String>();
        settings.wifi.ssid = doc["wifi"]["ssid"].as<String>();
        settings.wifi.password = doc["wifi"]["password"].as<String>();
        settings.telegram.botId = doc["telegram"]["botId"].as<String>();
        settings.telegram.chatId = doc["telegram"]["chatId"].as<String>();
        settings.tcpServerPort = doc["tcpServerPort"].as<uint16_t>();

        file.close();
        doc.clear();

        return settings;
    }
};