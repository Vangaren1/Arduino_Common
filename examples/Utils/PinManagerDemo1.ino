#include <Arduino.h>
#include <ArduinoCommon.h>

using ArduinoCommon::Utils::PinManager;
using ArduinoCommon::Utils::PinModeType;

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ;  // wait for Serial on some boards
  }
  Serial.println(F("PinManager demo starting..."));

  // Reserve two pins
  bool okA0 = PinManager::reservePin(A0);
  bool okA1 = PinManager::reservePin(A1);

  Serial.print(F("Reserve A0: "));
  Serial.println(okA0 ? F("OK") : F("FAILED"));

  Serial.print(F("Reserve A1: "));
  Serial.println(okA1 ? F("OK") : F("FAILED"));

  // Release one of them
  PinManager::releasePin(A1);
  Serial.println(F("Released A1"));

  // Configure an input pin (no pullups)
  bool inOk = PinManager::configureInput(A2);
  Serial.print(F("Configure A2 as INPUT: "));
  Serial.println(inOk ? F("OK") : F("FAILED"));

  // Configure an output pin
  bool outOk = PinManager::configureOutput(A4);
  Serial.print(F("Configure A4 as OUTPUT: "));
  Serial.println(outOk ? F("OK") : F("FAILED"));

  // Show status for a few pins
  for (uint8_t pin : {A0, A1, A2, A4}) {
    Serial.print(F("Pin "));
    Serial.print(pin);
    Serial.print(F(" -> used="));
    Serial.print(PinManager::isPinUsed(pin) ? F("yes") : F("no"));
    Serial.print(F(", mode="));

    PinModeType mode = PinManager::getPinMode(pin);
    switch (mode) {
      case PinModeType::Free:
        Serial.println(F("Free"));
        break;
      case PinModeType::Input:
        Serial.println(F("Input"));
        break;
      case PinModeType::InputPullup:
        Serial.println(F("InputPullup"));
        break;
      case PinModeType::Output:
        Serial.println(F("Output"));
        break;
#ifdef INPUT_PULLDOWN
      case PinModeType::InputPulldown:
        Serial.println(F("InputPulldown"));
        break;
#endif
#ifdef OUTPUT_OPENDRAIN
      case PinModeType::OutputOpenDrain:
        Serial.println(F("OutputOpenDrain"));
        break;
#endif
      default:
        Serial.println(F("Unknown"));
        break;
    }
  }

  Serial.println();
  Serial.println(F("Full PinManager state:"));
  PinManager::debugDump(Serial);

  Serial.println(F("PinManager demo done."));
}

void loop() {
  // Nothing to do; this is a one-shot demo.
}
