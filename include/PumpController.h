#ifndef PumpController_H
#define PumpController_H

#include <Arduino.h>
#include "PinManager.h"

class PumpController
{
private:
    int outputPin1;
    int outputPin2;
    bool validConfig;

public:
    PumpController(int outPin1, int outPin2);
};

#endif