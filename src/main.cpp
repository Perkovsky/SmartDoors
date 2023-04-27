#include <Arduino.h>
#include <SdFat.h>
#include <uRTCLib.h>
#include "WiFiManager.hpp"
#include "TelegramNotifier.hpp"
#include "RtcDateTimeProvider.hpp"
#include "LoggerFactory.hpp"
#include "SettingsAccessor.hpp"
#include "TcpServer.hpp"
#include "SmartDoorsPanel.hpp"
#include "SmartDoorsTcpCommandProcessor.hpp"
#include "HardwareComponentsManager.hpp"


uRTCLib rtc(0x68);
RtcDateTimeProvider dateTimeProvider(rtc);
SdFat sd;
TelegramNotifier* notifier;
LoggerFactory* logger;
WiFiManager* wifiManager;

SmartDoorsPanel panel;
HardwareComponentsManager* hardwareComponentsManager;
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

    // smart doors panel
    SmartDoorsDoor door1;
    door1.ledPin = 6;
    panel.id = "0050c23d8000";
    panel.doors = std::map<int8_t, SmartDoorsDoor>();
    panel.doors[1] = door1;

    // Hardware
    hardwareComponentsManager = new HardwareComponentsManager(dateTimeProvider);
    hardwareComponentsManager->init();

    // TCP Server
    tcpCommandProcessor = new SmartDoorsTcpCommandProcessor(panel, *logger);
    tcpServer = new TcpServer(*tcpCommandProcessor, *logger);
    tcpServer->bebin(settings.tcpServerPort);
}

void loop() {
    rtc.refresh();
    hardwareComponentsManager->refresh();
    logger->logInfo("ping");
    delay(1000);
}