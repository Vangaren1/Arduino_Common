#ifndef PUMPCONTROLLER_H
#define PUMPCONTROLLER_H

#include <Arduino.h>
#include "PinManager.h"

class PumpController
{
private:
    uint8_t outputPin1;
    uint8_t outputPin2;

    uint32_t maxRunTime;

    bool active;
    bool validConfig;
    bool calibrated;

    float mlPerMs;

public:
    PumpController(uint8_t outPin1, uint8_t outPin2);
    bool begin();

    bool turnOn();
    bool turnOff();

    bool dispenseFor(uint32_t durationMs);
    bool dispenseML(uint32_t volumeMl);

    bool isActive() const;
    bool isValid() const;
    bool isCalibrated() const;

    bool recordCalibrationResult(uint32_t volumeMl, uint32_t durationMs);
    bool setMaxRunTime(uint32_t durationMs);

    bool startDispenseFor(uint32_t durationMs);
    void update();
};

#endif