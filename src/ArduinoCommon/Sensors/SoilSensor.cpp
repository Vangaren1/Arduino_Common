#include "ArduinoCommon/Sensors/SoilSensor.h"

namespace ArduinoCommon {
namespace Sensors {

using ArduinoCommon::Utils::PinManager;

SoilSensor::SoilSensor(uint8_t pin)
    : inputPin(pin), validConfig(false), dryVal(-1), wetVal(-1) {
  bool pinCheck = PinManager::reservePin(pin);

  if (!pinCheck) {
    validConfig = false;
    return;
  }

  inputPin = pin;
  validConfig = true;

  // default values for wet and dry

  dryVal = -1;
  wetVal = -1;
}

bool SoilSensor::validConfiguration() const { return validConfig; }

bool SoilSensor::begin(int dry, int wet) {
  if (!validConfig) return false;

  // For most boards analog pins don’t strictly require pinMode,
  // but it's harmless and can be helpful on some MCUs.
  pinMode(inputPin, INPUT);

  // If the user provides calibration values, use them.
  if (dry >= 0 && wet >= 0 && wet != dry) {
    dryVal = dry;
    wetVal = wet;
  } else {
    // Provide generic defaults:
    // On Uno: 0–1023, on many others: 0–4095. Using wide defaults
    // and encouraging the user to call setCalibration() is safest.
#if defined(ESP32)
    dryVal = 3000;  // likely “pretty dry”
    wetVal = 1200;  // likely “quite wet”
#else
    dryVal = 800;  // mostly dry on 10-bit
    wetVal = 400;  // fairly wet on 10-bit
#endif
  }

  return true;
}

void SoilSensor::setCalibration(int dry, int wet) {
  if (!validConfig) return;

  if (wet == dry) return;  // avoid divide-by-zero later

  dryVal = dry;
  wetVal = wet;
}

int SoilSensor::readRaw() const {
  if (!validConfig) return -1;

  return analogRead(inputPin);
}

int SoilSensor::readPercent() const {
  if (!validConfig) return -1;

  int raw = readRaw();
  if (raw < 0) return -1;

  // If calibration is not set properly, avoid bad math.
  if (dryVal < 0 || wetVal < 0 || dryVal == wetVal) return -1;

  // Many sensors are "low value = wet, high value = dry"
  // so map raw between dryValue→0% and wetValue→100%.
  // We handle either direction (wetValue may be < or > dryValue).

  float percent;

  if (wetVal < dryVal) {
    // Example: dry=800, wet=400 on a 10-bit ADC
    percent = (float)(dryVal - raw) / (float)(dryVal - wetVal) * 100.0f;
  } else {
    // Example: dry=1200, wet=3000 on a 12-bit ADC (uncommon, but we support it)
    percent = (float)(raw - dryVal) / (float)(wetVal - dryVal) * 100.0f;
  }

  if (percent < 0.0f) percent = 0.0f;
  if (percent > 100.0f) percent = 100.0f;

  return static_cast<int>(percent + 0.5f);  // round to nearest int
}

int SoilSensor::readAveragedRaw(uint8_t samples) const {
  if (!validConfig || samples == 0) return -1;

  long total = 0;
  for (uint8_t i = 0; i < samples; i++) {
    total += analogRead(inputPin);
    delay(5);  // small delay between samples to reduce noise
  }
  return static_cast<int>(total / samples);
}

}  // namespace Sensors
}  // namespace ArduinoCommon
