#pragma once

class BaseHardwareDevice {
private:
    unsigned long _timer = 0;

protected:
    virtual ~BaseHardwareDevice() = default;

public:
    virtual void init() = 0;

    void setTimer(unsigned long value) {
        _timer = value;
    }

    unsigned long getTimer() {
        return _timer;
    }
};