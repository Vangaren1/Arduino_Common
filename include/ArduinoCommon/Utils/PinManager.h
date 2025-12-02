#ifndef PINMANAGER_H
#define PINMANAGER_H

#include <Arduino.h>
#include <map>

class PinManager
{
private:
    static std::map<int, bool> pinUsage;

public:
    static bool reservePin(int pin)
    {
        if (pinUsage[pin])
        {
            Serial.print("Error: pin ");
            Serial.print(pin);
            Serial.println(" is already in use");
            return false; // already in use
        }
        pinUsage[pin] = true;
        return true;
    }

    static void releasePin(int pin)
    {
        pinUsage[pin] = false;
    }

    static bool isPinUsed(int pin)
    {
        return pinUsage[pin];
    }
};

#endif
