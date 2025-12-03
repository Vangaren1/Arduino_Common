#ifndef ARDUINOCOMMON_DISPLAY_LCD1602_H
#define ARDUINOCOMMON_DISPLAY_LCD1602_H

#include <stdint.h>  // <- guarantees uint8_t, independent of Arduino
#include <Arduino.h>

namespace ArduinoCommon {
namespace Display {

using ::uint8_t;

class LCD1602 {
 private:
  uint8_t sdaPin;
  uint8_t sclPin;
  bool validConfig;

 public:
  LCD1602(uint8_t sdaP, uint8_t sclP);
  bool begin();
  bool validConfiguration();
  void clear();
  void printLine(int row, const String& text);
};

}  // namespace Display
}  // namespace ArduinoCommon

#endif
