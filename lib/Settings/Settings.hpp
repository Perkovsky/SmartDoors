#pragma once

#include <String.h>

struct LogLevelettings {
    String serial;
    String sd;
};

struct WiFiSettings {
    String ssid;
    String password;
};

struct EthernetSettings {
    byte mac[6];
};

struct TelegramSettings {
    String botId;
    String chatId;
};

struct Settings {
    LogLevelettings logLevel;
    WiFiSettings wifi;
    EthernetSettings ethernet;
    TelegramSettings telegram;
    uint16_t tcpServerPort;
};