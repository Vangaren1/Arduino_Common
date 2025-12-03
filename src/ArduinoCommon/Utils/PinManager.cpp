#include <ArduinoCommon/Utils/PinManager.h>

namespace ArduinoCommon {
namespace Utils {

uint64_t PinManager::pinMask = 0;
PinModeType PinManager::pinModes[MaxPins] = {PinModeType::Free};

bool PinManager::reservePin(uint8_t pin) {
  if (pin >= MaxPins) return false;

  uint64_t mask = (uint64_t)1 << pin;

  if (pinMask & mask) {
    Serial.print(F("Error: pin "));
    Serial.print(pin);
    Serial.println(F(" is already in use"));
    return false;  // already in use
  }

  pinMask |= mask;
  return true;
}

void PinManager::releasePin(uint8_t pin) {
  if (pin < MaxPins) {
    pinMask &= ~((uint64_t)1 << pin);
    pinModes[pin] = PinModeType::Free;
  }
}

bool PinManager::isPinUsed(uint8_t pin) {
  if (pin >= MaxPins) return false;

  return pinMask & ((uint64_t)1 << pin);
}

bool PinManager::configureOutput(uint8_t pin, bool openDrain) {
  if (pin >= MaxPins) return false;

  if (openDrain) {
    return false;
  }

#ifdef OUTPUT_OPENDRAIN
  PinModeType desired =
      openDrain ? PinModeType::OutputOpenDrain : PinModeType::Output;
#else
  PinModeType desired = PinModeType::Output;
#endif

  if (isPinUsed(pin)) {
    // If already configured differently, error out
    if (pinModes[pin] != desired) {
      Serial.print(F("Error: pin "));
      Serial.print(pin);
      Serial.println(F(" already configured with a different mode"));
      return false;
    }

    // Same mode: nothing to change
    return true;
  }

  // New reservation
  if (!reservePin(pin)) return false;

  pinModes[pin] = desired;

#ifdef OUTPUT_OPENDRAIN
  pinMode(pin, openDrain ? OUTPUT_OPENDRAIN : OUTPUT);
#else
  if (openDrain) {
    Serial.print(F("Error: open-drain not supported on this board for pin "));
    Serial.println(pin);
    releasePin(pin);
    return false;
  }
  pinMode(pin, OUTPUT);
#endif

  return true;
}

bool PinManager::configureInput(uint8_t pin, bool pullup, bool pulldown) {
  if (pullup && pulldown) {
    Serial.println(
        F("Error: cannot enable both pull-up and pull-down on the same pin"));
    return false;
  }

  if (pin >= MaxPins) return false;

  PinModeType desired = PinModeType::Input;

#ifdef INPUT_PULLDOWN
  if (pulldown) desired = PinModeType::InputPulldown;
#endif

  if (pullup) desired = PinModeType::InputPullup;

  if (isPinUsed(pin)) {
    if (pinModes[pin] != desired) {
      Serial.print(F("Error: pin "));
      Serial.print(pin);
      Serial.println(F(" already configured with a different mode"));
      return false;
    }

    // Same mode: nothing to change
    return true;
  }

  if (!reservePin(pin)) return false;

  pinModes[pin] = desired;

#ifdef INPUT_PULLDOWN
  if (pulldown) {
    pinMode(pin, INPUT_PULLDOWN);
    return true;
  }
#endif

  pinMode(pin, pullup ? INPUT_PULLUP : INPUT);
  return true;
}

PinModeType PinManager::getPinMode(uint8_t pin) {
  if (pin >= MaxPins) return PinModeType::Free;

  return pinModes[pin];
}

void PinManager::debugDump(Stream& out) {
  out.println(F("[PinManager] Reserved pins:"));

  bool any = false;

  for (uint8_t pin = 0; pin < MaxPins; ++pin) {
    if (isPinUsed(pin)) {
      out.print(F("  Pin "));
      out.print(pin);
      out.print(F(" - "));

      switch (pinModes[pin]) {
        case PinModeType::Output:
          out.println(F("OUTPUT"));
          break;
        case PinModeType::Input:
          out.println(F("INPUT"));
          break;
        case PinModeType::InputPullup:
          out.println(F("INPUT_PULLUP"));
          break;
#ifdef OUTPUT_OPENDRAIN
        case PinModeType::OutputOpenDrain:
          out.println(F("OUTPUT_OPEN_DRAIN"));
          break;
#endif

#ifdef INPUT_PULLDOWN
        case PinModeType::InputPulldown:
          out.println(F("INPUT_PULLDOWN"));
          break;
#endif

        default:
          out.println(F("UNKNOWN / RESERVED"));
          break;
      }

      any = true;
    }
  }

  if (!any) {
    out.println(F("  (none)"));
  }
}

}  // namespace Utils
}  // namespace ArduinoCommon
