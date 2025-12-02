#ifndef LCD1602_H
#define LCD1602_H

#include <Arduino.h>
#include "PinManager.h"

class LCD1602
{
private:
    int sdaPin;
    int sclPin;
    bool validConfig;

public:
    LCD1602(int sdaPin, int sclPin);
    bool begin();
    bool validConfiguration();
    void clear();
    void printLine(int row, const String &text);
};

#endif
