#include <ArduinoCommon/Display/LCD1602.h>
#include <ArduinoCommon/Utils/PinManager.h>

#include <Wire.h>

#ifdef ARDUINOCOMMON_TESTING
#include "FakeLiquidCrystal_I2C.h"
#else
#include <LiquidCrystal_I2C.h>
#endif

namespace ArduinoCommon {
namespace Display {

LCD1602::LCD1602(uint8_t sdaP, uint8_t sclP, uint8_t address) noexcept
    : lcd(nullptr),
      sdaPin(sdaP),
      sclPin(sclP),
      i2cAddress(address),
      validConfig(false) {
  bool sdaOk = Utils::PinManager::reservePin(sdaPin);
  bool sclOk = Utils::PinManager::reservePin(sclPin);

  if (!sdaOk || !sclOk) {
    if (sdaOk) Utils::PinManager::releasePin(sdaPin);
    if (sclOk) Utils::PinManager::releasePin(sclPin);
    return;
  }

  lcd = new LiquidCrystal_I2C(i2cAddress, 16, 2);
  validConfig = true;
}

LCD1602::~LCD1602() {
  if (lcd) {
    delete lcd;
    lcd = nullptr;
  }
  if (validConfig) {
    Utils::PinManager::releasePin(sdaPin);
    Utils::PinManager::releasePin(sclPin);
  }
}

bool LCD1602::begin() {
  if (!validConfig || lcd == nullptr) return false;

#ifdef ARDUINOCOMMON_TESTING
  // Test build: do not touch real I2C hardware
  lcd->init();
  lcd->clear();
  lcd->backlight();
  return true;
#else

#if defined(ESP32)
  Wire.begin(sdaPin, sclPin);
#else
  Wire.begin();
#endif

  if (!probeI2C_()) return false;

  lcd->init();
  lcd->clear();
  lcd->backlight();
  return true;
#endif
}

bool LCD1602::validConfiguration() const noexcept { return validConfig; }

void LCD1602::clear() {
  if (!validConfig || lcd == nullptr) return;

  lcd->clear();
}

bool LCD1602::probeI2C_() const {
#ifdef ARDUINOCOMMON_TESTING
  return true;  // Test build: assume device present
#else
  Wire.beginTransmission(i2cAddress);
  return (Wire.endTransmission() == 0);
#endif
}

void LCD1602::printLine(uint8_t row, const char* text) {
  if (!validConfig || lcd == nullptr || row > 1) return;

  lcd->setCursor(0, row);
  lcd->print(F("                "));
  lcd->setCursor(0, row);
  lcd->print(text ? text : "");
}

void LCD1602::printLine(uint8_t row, const __FlashStringHelper* text) {
  if (!validConfig || lcd == nullptr || row > 1) return;

  lcd->setCursor(0, row);
  lcd->print(F("                "));
  lcd->setCursor(0, row);
  lcd->print(text);
}

}  // namespace Display
}  // namespace ArduinoCommon
