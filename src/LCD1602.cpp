#include "LCD1602.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Use the scanned I2C address:
LiquidCrystal_I2C lcd(0x27, 16, 2);

LCD1602::LCD1602(int sdaP, int sclP)
{
    // Reserve the *pins passed in*
    bool SDACheck = PinManager::reservePin(sdaP);
    bool SCLCheck = PinManager::reservePin(sclP);

    if (!SDACheck || !SCLCheck)
    {
        validConfig = false;
        // Serial.println("LCD1602 not configured: pins already in use.");
        return;
    }

    // Now store them in the member variables
    sdaPin = sdaP;
    sclPin = sclP;
    validConfig = true;

    // On Uno-style boards, Wire.begin() uses the fixed SDA/SCL pins
}

bool LCD1602::begin()
{
    if (!validConfig)
        return false;

    Wire.begin();

    lcd.init(); // (or lcd.begin(16,2) depending on your lib)
    lcd.clear();
    lcd.backlight();
    return true;
}

bool LCD1602::validConfiguration()
{
    return validConfig;
}

void LCD1602::clear()
{
    if (!validConfig)
        return;
    lcd.clear();
}

void LCD1602::printLine(int row, const String &text)
{
    if (!validConfig)
        return;

    lcd.setCursor(0, row);
    lcd.print("                "); // clear 16 chars on this row
    lcd.setCursor(0, row);
    lcd.print(text);
}
