#ifndef SoilSensor_H
#define SoilSensor_H

#include <Arduino.h>
#include "PinManager.h"

class SoilSensor
{
private:
    int inputPin;
    bool validConfig;

    int dryVal;
    int wetVal;

public:
    SoilSensor(int pin);
    bool begin(int dryCalibration = -1, int wetCalibration = -1);
    void setCalibration(int dryCalibration, int wetCalibration);
    bool validConfiguration() const;
    int readRaw() const;
    int readPercent() const;
    int readAveragedRaw(uint8_t samples = 10) const;
};

#endif