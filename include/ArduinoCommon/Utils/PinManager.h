#ifndef ARDUINOCOMMON_UTILS_PINMANAGER_H
#define ARDUINOCOMMON_UTILS_PINMANAGER_H

#include <Arduino.h>

namespace ArduinoCommon {
namespace Utils {

#if defined(NUM_DIGITAL_PINS)
static constexpr uint8_t MaxPins = NUM_DIGITAL_PINS;
#else
static constexpr uint8_t MaxPins = 64;
#endif

static_assert(MaxPins <= 64,
              "PinManager: MaxPins exceeds 64, adjust bitmask type if needed.");

enum class PinModeType : uint8_t {
  Free = 0,
  Input,
  InputPullup,
  Output,
// only for ESP32
#ifdef INPUT_PULLDOWN
  InputPulldown,
#endif
#ifdef OUTPUT_OPENDRAIN
  OutputOpenDrain
#endif

};

class PinManager {
 private:
  static uint64_t pinMask;
  static PinModeType pinModes[MaxPins];

 public:
  static bool reservePin(uint8_t pin);
  static void releasePin(uint8_t pin);
  static bool isPinUsed(uint8_t pin);

  static bool configureOutput(uint8_t pin, bool openDrain = false);
  static bool configureInput(uint8_t pin, bool pullup = false,
                             bool pulldown = false);

  static PinModeType getPinMode(uint8_t pin);

  static void debugDump(Stream& out = Serial);
};

}  // namespace Utils
}  // namespace ArduinoCommon

#endif
