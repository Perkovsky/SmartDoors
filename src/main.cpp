#include <Arduino.h>
#include <SdFat.h>
#include <uRTCLib.h>
#include "WiFiManager.hpp"
#include "TelegramNotifier.hpp"
#include "RtcDateTimeProvider.hpp"
#include "LoggerFactory.hpp"
#include "SettingsAccessor.hpp"
#include "HardwareManager.hpp"
#include "SmartDoorsTcpCommandProcessor.hpp"
#include "TcpServer.hpp"

uRTCLib rtc(0x68);
RtcDateTimeProvider dateTimeProvider(rtc);
SdFat sd;
TelegramNotifier* notifier;
LoggerFactory* logger;
WiFiManager* wifiManager;

HardwareManager* hardwareManager;
AbstractTcpCommandProcessor* tcpCommandProcessor;
TcpServer* tcpServer; 

void setup() {
    Serial.begin(9600);
    sd.begin(4, SPI_HALF_SPEED);
    URTCLIB_WIRE.begin();
    rtc.refresh();

    // settings
    SettingsAccessor settingsAccessor(sd);
    Settings settings = settingsAccessor.getSettings();
    
    // notifier
    notifier = new TelegramNotifier(settings.telegram.botId, settings.telegram.chatId);
    
    // logger
    logger = new LoggerFactory(dateTimeProvider, notifier);
    logger->writeToSerial(settings.logLevel.serial, Serial);
    logger->writeToSdCard(settings.logLevel.sd, sd);

    // WiFi
    wifiManager = new WiFiManager(settings.wifi.ssid, settings.wifi.password, *logger);
    wifiManager->connect();
    delay(1000);
    wifiManager->checkConnection();

    // hardware
    hardwareManager = new HardwareManager(dateTimeProvider);
    hardwareManager->init();

    // TCP Server
    tcpCommandProcessor = new SmartDoorsTcpCommandProcessor(*hardwareManager, *logger);
    tcpServer = new TcpServer(*tcpCommandProcessor, *logger);
    tcpServer->bebin(settings.tcpServerPort);
}

void loop() {
    rtc.refresh();
    hardwareManager->refresh();
}