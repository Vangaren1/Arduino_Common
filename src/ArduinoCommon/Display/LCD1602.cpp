#include <ArduinoCommon/Display/LCD1602.h>
#include <ArduinoCommon/Utils/PinManager.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

using ArduinoCommon::Utils::PinManager;

// Use the scanned I2C address:
static LiquidCrystal_I2C lcd(0x27, 16, 2);

namespace ArduinoCommon {
namespace Display {

LCD1602::LCD1602(uint8_t sdaP, uint8_t sclP)
    : sdaPin(sdaP), sclPin(sclP), validConfig(false) {
  // Reserve the *pins passed in*
  bool SDACheck = PinManager::reservePin(sdaPin);
  bool SCLCheck = PinManager::reservePin(sclPin);

  if (!SDACheck || !SCLCheck) {
    validConfig = false;
    return;
  }

  // We just track pin usage here.
  validConfig = true;
}

bool LCD1602::begin() {
  if (!validConfig) return false;

  Wire.begin();

  lcd.init();  // or lcd.begin(16, 2) for some libs
  lcd.clear();
  lcd.backlight();
  return true;
}

bool LCD1602::validConfiguration() { return validConfig; }

void LCD1602::clear() {
  if (!validConfig) return;
  lcd.clear();
}

void LCD1602::printLine(int row, const String& text) {
  if (!validConfig) return;

  lcd.setCursor(0, row);
  lcd.print("                ");  // clear 16 chars on this row
  lcd.setCursor(0, row);
  lcd.print(text);
}

}  // namespace Display
}  // namespace ArduinoCommon
