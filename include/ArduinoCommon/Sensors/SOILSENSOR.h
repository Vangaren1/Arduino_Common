#ifndef ARDUINO_SENSORS_SOILSENSOR_H
#define ARDUINO_SENSORS_SOILSENSOR_H

#include <Arduino.h>
#include <ArduinoCommon/Utils/PinManager.h>
#include <ArduinoCommon/Sensors/AnalogSensor.h>
#include <ArduinoCommon/Config/IConfigStorage.h>

namespace ArduinoCommon {
namespace Sensors {

struct SoilCalibration {
  int16_t dryRaw = -1;
  int16_t wetRaw = -1;
  uint8_t version = 1;

  bool isValid() const {
    return dryRaw >= 0 && wetRaw >= 0 && dryRaw != wetRaw;
  }
};

class SoilSensor : public IAnalogSensor {
 private:
  uint8_t _inputPin;
  bool _validConfig;

  SoilCalibration _calibration;

  Config::IConfigStorage* _storage;
  uint16_t _storageKey;

  bool loadCalibrationFromStorage();
  bool saveCalibrationToStorage() const;

 public:
  explicit SoilSensor(uint8_t pin);

  // optional, attach storage
  void attachStorage(Config::IConfigStorage* storage, uint16_t key);

  /*
  Initializes sensor:
  - reserves pin
  - optionally uses provided calibration
  - if no calibration values passed, tries to load from storage
  */
  bool begin(int16_t dryCalibration = -1, int16_t wetCalibration = -1);

  // sets the calibration
  void setCalibration(int16_t dryRaw, int16_t wetRaw, bool persist = true);

  // checks to see if calibration is already set
  bool hasCalibration() const;

  // removes existing calibration
  void clearCalibration();

  // confirms valid configuration
  bool validConfiguration() const override;

  // gets existing calibration
  SoilCalibration getCalibration() const;

  // IAnalogSensor Interface
  int readRaw() const override;
  int readPercent() const override;

  int readAveragedRaw(uint8_t samples = 10) const;
};

}  // namespace Sensors
}  // namespace ArduinoCommon

#endif