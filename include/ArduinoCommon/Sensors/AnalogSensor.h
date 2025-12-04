#ifndef ARDUINOCOMMON_SENSORS_ANALOGSENSOR_H
#define ARDUINOCOMMON_SENSORS_ANALOGSENSOR_H

#include <Arduino.h>

namespace ArduinoCommon {
namespace Sensors {
class IAnalogSensor {
 public:
  virtual ~IAnalogSensor() = default;
  virtual bool validConfiguration() const = 0;
  virtual int readRaw() const = 0;
  virtual int readPercent() const { return -1; }

  virtual void logRaw(Stream& out, const char* label = nullptr) const {
    if (!validConfiguration()) return;

    int value = readRaw();
    if (value < 0) return;

    if (label) {
      out.print(label);
      out.print(F(": "));
    } else {
      out.print(F("Raw: "));
    }

    out.println(value);
  }

  virtual void logPercent(Stream& out, const char* label = nullptr) const {
    if (!validConfiguration()) return;

    int value = readPercent();
    if (value < 0) return;

    if (label) {
      out.print(label);
      out.print(F(": "));
    } else {
      out.print(F("Percent: "));
    }

    out.print(value);
    out.println(F("%"));
  }
};
}  // namespace Sensors
}  // namespace ArduinoCommon

#endif