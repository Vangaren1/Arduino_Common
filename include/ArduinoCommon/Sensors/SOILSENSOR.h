#ifndef ARDUINOCOMMON_SENSORS_SOILSENSOR_H
#define ARDUINOCOMMON_SENSORS_SOILSENSOR_H

#include <Arduino.h>
#include <ArduinoCommon/Utils/PinManager.h>
#include <ArduinoCommon/Sensors/AnalogSensor.h>
#include <ArduinoCommon/Config/IConfigStorage.h>

namespace ArduinoCommon {
namespace Sensors {

/**
 * @brief Calibration data for a soil moisture sensor.
 *
 * This struct stores the raw ADC values measured when the sensor is
 * completely dry and fully wet, along with a simple version field
 * for future extension.
 */
struct SoilCalibration {
  /// Raw reading when the sensor is in air / fully dry.
  int16_t dryRaw = -1;
  /// Raw reading when the sensor is fully wet (e.g., submerged in water).
  int16_t wetRaw = -1;
  /// Calibration data version (for potential future format changes).
  uint8_t version = 1;

  /**
   * @brief Check whether the calibration values appear valid.
   *
   * @return true  If both dryRaw and wetRaw are non-negative and not equal.
   * @return false If calibration is unset, invalid, or degenerate.
   */
  bool isValid() const {
    return dryRaw >= 0 && wetRaw >= 0 && dryRaw != wetRaw;
  }
};

/**
 * @brief Class for a capacitive soil moisture sensor.
 *
 * Wraps a single analog input pin used to measure soil moisture using
 * a capacitive probe. The class supports:
 *
 * - Raw ADC readings via readRaw()
 * - Percentage readings via readPercent(), using calibration
 * - Optional persistence of calibration via an IConfigStorage backend
 */
class SoilSensor : public IAnalogSensor {
 private:
  uint8_t _inputPin;
  bool _validConfig;

  SoilCalibration _calibration;

  // Optional non-owning pointer to a storage backend for calibration.
  Config::IConfigStorage* _storage;
  uint16_t _storageKey;

  /**
   * @brief Load calibration from the attached storage backend.
   *
   * Attempts to read SoilCalibration data from the configured storage
   * at the configured key. If successful and the data appears valid,
   * it is applied to this sensor.
   *
   * @return true  If calibration data was loaded and is valid.
   * @return false If no storage is attached, the read fails, or the
   *               loaded calibration is invalid.
   */
  bool loadCalibrationFromStorage();

  /**
   * @brief Save the current calibration to the attached storage backend.
   *
   * If a storage backend has been attached and the calibration is valid,
   * this writes the SoilCalibration struct to storage at the configured
   * key.
   *
   * @return true  If the write succeeds and storage is attached.
   * @return false If no storage is attached, calibration is invalid,
   *               or the write fails.
   */
  bool saveCalibrationToStorage() const;

 public:
  /**
   * @brief Construct a new SoilSensor object.
   *
   * The sensor is associated with a single analog input pin. The pin
   * is not reserved or configured until begin() is called.
   *
   * @param pin Analog input pin connected to the soil moisture sensor.
   */
  explicit SoilSensor(uint8_t pin);

  /**
   * @brief Attach a storage backend for calibration persistence.
   *
   * Uses an IConfigStorage implementation (e.g. EEPROM, flash, SD card)
   * to persist calibration data across resets. This does not perform
   * any I/O by itself; it only stores the pointer and key.
   *
   * @param storage Non-owning pointer to a storage implementation.
   *                The caller must ensure the lifetime exceeds that
   *                of this SoilSensor.
   * @param key     Byte offset or identifier within the storage backend
   *                where the SoilCalibration struct will be stored.
   *
   * @note If attachStorage() is never called, calibration will only
   *       exist in RAM for the lifetime of the SoilSensor instance.
   */
  void attachStorage(Config::IConfigStorage* storage, uint16_t key);

  /**
   * @brief Initialize the soil sensor.
   *
   * This performs initialization steps including:
   * - Reserving and configuring the analog pin
   * - Applying an initial calibration if dryCalibration/wetCalibration
   *   are provided
   * - If no explicit calibration is provided, attempting to load a
   *   stored calibration from the attached IConfigStorage (if any)
   *
   * @param dryCalibration Optional raw value when the sensor is dry.
   * @param wetCalibration Optional raw value when the sensor is fully wet.
   *
   * @return true  If initialization succeeds and a valid configuration
   *               is established.
   * @return false If pin reservation or configuration fails.
   */
  bool begin(int16_t dryCalibration = -1, int16_t wetCalibration = -1);

  /**
   * @brief Set the calibration values for the sensor.
   *
   * Updates the in-memory calibration using the supplied dry and wet
   * raw readings. If persist is true and a storage backend has been
   * attached, the calibration will also be written to storage so it
   * can be restored on the next boot.
   *
   * @param dryRaw  Raw reading when the sensor is in air / fully dry.
   * @param wetRaw  Raw reading when the sensor is fully wet
   *                (e.g., probe submerged in water).
   * @param persist If true, attempt to persist the calibration using
   *                the attached IConfigStorage backend.
   */
  void setCalibration(int16_t dryRaw, int16_t wetRaw, bool persist = true);

  /**
   * @brief Check whether a valid calibration has been set.
   *
   * @return true  If calibration values are valid.
   * @return false If no calibration is set or the values are invalid.
   */
  bool hasCalibration() const;

  /**
   * @brief Clear the current calibration.
   *
   * Resets the in-memory calibration to default values and, if a
   * storage backend is attached, clears the stored calibration there
   * as well (implementation-dependent).
   */
  void clearCalibration();

  /**
   * @brief Check whether the sensor configuration is valid.
   *
   * This typically checks that the analog pin has been successfully
   * reserved and that any required calibration has been applied.
   *
   * @return true  If the sensor is ready to provide readings.
   * @return false If the sensor has not been initialized or is in an
   *               invalid state.
   */
  bool validConfiguration() const override;

  /**
   * @brief Get the current calibration.
   *
   * @return SoilCalibration The current calibration struct.
   */
  SoilCalibration getCalibration() const;

  /**
   * @brief Read a raw value from the soil moisture sensor.
   *
   * The exact range depends on the underlying hardware and ADC
   * (e.g., 0–1023 on AVR-based Arduinos, 0–4095 on some 12-bit ADCs).
   *
   * @return int A non-negative raw reading on success; a negative value
   *             if the sensor is not configured or cannot be read.
   */
  int readRaw() const override;

  /**
   * @brief Read the soil moisture as a percentage.
   *
   * Uses the current calibration (dryRaw and wetRaw) to map the raw
   * sensor reading into a percentage in the range [0,100]. If
   * wetRaw < dryRaw (common for some capacitive sensors), the mapping
   * accounts for this inverted range. The result is clamped to [0,100].
   *
   * @return int A percentage in the range [0,100] on success, or a
   *             negative value if the sensor is not configured or
   *             no valid reading is available.
   */
  int readPercent() const override;

  /**
   * @brief Read multiple raw samples and return their average.
   *
   * This function performs @p samples individual analog readings, sums
   * them, and returns the integer average. It is useful for reducing
   * ADC noise and obtaining a more stable reading.
   *
   * @param samples Number of samples to read, defaults to 10.
   * @return int A non-negative averaged reading on success; a negative
   *             value if configuration is invalid or reads fail.
   */
  int readAveragedRaw(uint8_t samples = 10) const;
};

}  // namespace Sensors
}  // namespace ArduinoCommon

#endif
