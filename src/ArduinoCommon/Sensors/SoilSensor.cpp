#include "ArduinoCommon/Sensors/SoilSensor.h"

namespace ArduinoCommon {
namespace Sensors {

using ArduinoCommon::Utils::PinManager;

SoilSensor::SoilSensor(uint8_t pin)
    : _inputPin(pin),
      _validConfig(false),
      _calibration(),
      _storage(nullptr),
      _storageKey(0) {}

void SoilSensor::attachStorage(Config::IConfigStorage* storage, uint16_t key) {
  _storage = storage;
  _storageKey = key;
}

bool SoilSensor::begin(int16_t dryCalibration, int16_t wetCalibration) {
  if (!PinManager::reservePin(_inputPin)) {
    _validConfig = false;
    return false;
  }
  pinMode(_inputPin, INPUT);
  _validConfig = true;

  // If the user provides calibration values, use them.
  if (dryCalibration >= 0 && wetCalibration >= 0 &&
      wetCalibration != dryCalibration) {
    _calibration.dryRaw = dryCalibration;
    _calibration.wetRaw = wetCalibration;
    _calibration.version = 1;

    saveCalibrationToStorage();
    return true;
  }

  if (loadCalibrationFromStorage()) {
    return true;
  }

  _calibration = SoilCalibration{};
  return true;
}

bool SoilSensor::validConfiguration() const { return _validConfig; }

bool SoilSensor::hasCalibration() const { return _calibration.isValid(); }

void SoilSensor::clearCalibration() {
  _calibration = SoilCalibration{};
  if (_storage) {
    _storage->clear(_storageKey, sizeof(SoilCalibration));
  }
}

void SoilSensor::setCalibration(int16_t dryRaw, int16_t wetRaw, bool persist) {
  _calibration.dryRaw = dryRaw;
  _calibration.wetRaw = wetRaw;
  _calibration.version = 1;

  if (persist) {
    saveCalibrationToStorage();
  }
}

SoilCalibration SoilSensor::getCalibration() const { return _calibration; }

int SoilSensor::readRaw() const {
  if (!_validConfig) return -1;

  return analogRead(_inputPin);
}

/*
 * Returns a percentage where 0% is completely dry and 100% is saturated
 */
int SoilSensor::readPercent() const {
  int16_t raw = readRaw();
  if (raw < 0) {
    return 0;
  }

  if (!_calibration.isValid()) {
    return map(raw, 0, 1023, 0, 100);
  }

  int16_t minVal = _calibration.wetRaw;
  int16_t maxVal = _calibration.dryRaw;

  if (minVal > maxVal) {
    int16_t tmp = minVal;
    minVal = maxVal;
    maxVal = tmp;
  }

  int16_t clamped = raw;
  if (clamped < minVal) clamped = minVal;
  if (clamped > maxVal) clamped = maxVal;

  int percent = map(clamped, minVal, maxVal, 100, 0);

  if (percent < 0) percent = 0;
  if (percent > 100) percent = 100;
  return percent;
}

int SoilSensor::readAveragedRaw(uint8_t samples) const {
  if (!_validConfig || samples == 0) return -1;

  long total = 0;
  for (uint8_t i = 0; i < samples; i++) {
    total += static_cast<int16_t>(analogRead(_inputPin));
    delay(10);  // small delay between samples to reduce noise
  }
  return static_cast<int>(total / samples);
}

bool SoilSensor::loadCalibrationFromStorage() {
  if (!_storage) {
    return false;
  }

  if (!_storage->isUsed(_storageKey, sizeof(SoilCalibration))) {
    return false;
  }

  SoilCalibration tmp;
  if (!_storage->read(_storageKey, &tmp, sizeof(SoilCalibration))) {
    return false;
  }

  if (!tmp.isValid()) {
    return false;
  }

  _calibration = tmp;
  return true;
}

bool SoilSensor::saveCalibrationToStorage() const {
  if (!_storage || !_calibration.isValid()) {
    return false;
  }
  return _storage->write(_storageKey, &_calibration, sizeof(SoilCalibration));
}

}  // namespace Sensors
}  // namespace ArduinoCommon
