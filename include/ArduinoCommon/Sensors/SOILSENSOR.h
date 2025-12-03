#ifndef ARDUINO_SENSORS_SOILSENSOR_H
#define ARDUINO_SENSORS_SOILSENSOR_H

#include <Arduino.h>
#include <ArduinoCommon/Utils/PinManager.h>

namespace ArduinoCommon {
namespace Sensors {

class SoilSensor {
 private:
  uint8_t inputPin;
  bool validConfig;

  int dryVal;
  int wetVal;

 public:
  SoilSensor(uint8_t pin);
  bool begin(int dryCalibration = -1, int wetCalibration = -1);
  void setCalibration(int dryCalibration, int wetCalibration);
  bool validConfiguration() const;
  int readRaw() const;
  int readPercent() const;
  int readAveragedRaw(uint8_t samples = 10) const;
};

}  // namespace Sensors
}  // namespace ArduinoCommon

#endif