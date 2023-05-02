#pragma once

#include <SPI.h>
#include <MFRC522.h>
//#include <Servo.h>
#include "BaseHardwareDevice.hpp"

class RfidReader final : public BaseHardwareDevice {
private:
    //const u_int8_t _buzzerPin;
    MFRC522* _mfrc522;
    //Servo _servo;

public:
    RfidReader(/*const u_int8_t buzzerPin,*/ const byte ssPin, const byte rstPin)
        //: _buzzerPin(buzzerPin)
    {
       _mfrc522 = new MFRC522(ssPin, rstPin);
    }

    void init() override {
        //pinMode(_buzzerPin, OUTPUT);
        SPI.begin();
        _mfrc522->PCD_Init();
        //_servo.attach(6);
        //_servo.write(0);
    }

    unsigned long getCardUid() {
        if (!_mfrc522->PICC_IsNewCardPresent()) {
            return 0;
        }
        
        if (!_mfrc522->PICC_ReadCardSerial()) {
            return 0;
        }

        unsigned long uid = 0;
        for (byte i = 0; i < _mfrc522->uid.size; i++) {
            uid = uid * 256 + _mfrc522->uid.uidByte[i];
        }

        return uid;
    }

    void unlock() {
        //_servo.write(90);
        //tone(5, 200, 500);
    }

    void lock() {
        //_servo.write(0);
        //tone(5, 500, 500);
    }
};