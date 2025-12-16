#ifndef ARDUINOCOMMON_SENSORS_ANALOGSENSOR_H
#define ARDUINOCOMMON_SENSORS_ANALOGSENSOR_H

#include <Arduino.h>

namespace ArduinoCommon {
namespace Sensors {

/**
 * @brief Interface and common utilities for analog sensors.
 *
 * Implementations provide the core behavior for reading raw data from
 * an analog source, optionally mapping it to a percentage, and validating
 * that the sensor is correctly configured. This base class also provides
 * standardized logging helpers for raw and percentage readings.
 */
class IAnalogSensor {
 public:
  /**
   * @brief Virtual destructor for interface class.
   *
   * Allows proper cleanup through a pointer or reference to IAnalogSensor.
   */
  virtual ~IAnalogSensor() = default;

  /**
   * @brief Check whether the sensor is properly configured.
   *
   * Implementations should return true once the sensor has been
   * initialized (e.g., pins reserved, calibration applied, etc.).
   *
   * @return true  If the sensor configuration is valid.
   * @return false If the configuration is invalid or not yet set up.
   */
  virtual bool validConfiguration() const = 0;

  /**
   * @brief Read a raw value from the analog sensor.
   *
   * The exact range depends on the underlying hardware and sensor
   * (e.g., 0–1023 on AVR-based Arduinos, 0–4095 on some 12-bit ADCs).
   *
   * @return int A non-negative raw reading on success; a negative value
   *             may be used by implementations to signal an error or
   *             unavailable reading.
   */
  virtual int readRaw() const = 0;

  /**
   * @brief Read the sensor value as a percentage.
   *
   * The percentage is typically derived from a pre-configured minimum
   * and maximum raw value, mapped into the range [0,100].
   *
   * The default implementation returns -1 to indicate that percentage
   * readings are not implemented; subclasses may override this.
   *
   * @return int An integer percentage in the range [0,100] on success,
   *             or a negative value if the reading is not available.
   */
  virtual int readPercent() const { return -1; }

  /**
   * @brief Log a raw sensor reading to the given Stream.
   *
   * If the configuration is invalid or the underlying read returns a
   * negative value, this function does nothing.
   *
   * Example output: "SoilRaw: 512"
   *
   * @param out   The output stream (e.g., Serial) where the value
   *              will be written.
   * @param label Optional label to prefix the reading with. If nullptr,
   *              "Raw" is used as a default label.
   */
  virtual void logRaw(Stream& out, const char* label = nullptr) const {
    if (!validConfiguration()) return;

    const int value = readRaw();
    if (value < 0) return;

    if (label) {
      out.print(label);
      out.print(F(": "));
    } else {
      out.print(F("Raw: "));
    }

    out.println(value);
  }

  /**
   * @brief Log a percentage sensor reading to the given Stream.
   *
   * If the configuration is invalid or the underlying readPercent()
   * returns a negative value, this function does nothing.
   *
   * Example output: "Soil: 76%"
   *
   * @param out   The output stream (e.g., Serial) where the value
   *              will be written.
   * @param label Optional label to prefix the reading with. If nullptr,
   *              "Percent" is used as a default label.
   */
  virtual void logPercent(Stream& out, const char* label = nullptr) const {
    if (!validConfiguration()) return;

    const int value = readPercent();
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
