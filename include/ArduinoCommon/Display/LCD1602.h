#ifndef ARDUINOCOMMON_DISPLAY_LCD1602_H
#define ARDUINOCOMMON_DISPLAY_LCD1602_H

#include <Arduino.h>
#include <stdint.h>

// Forward declaration to avoid pulling implementation details into the header
class LiquidCrystal_I2C;

namespace ArduinoCommon {
namespace Display {

/**
 * @brief Driver wrapper for a 16x2 character LCD (LCD1602) with I2C backpack.
 *
 * This class provides a safe, high-level interface for controlling a 16x2
 * character LCD over I2C. It integrates with PinManager to prevent pin
 * conflicts and supports runtime validation of the I2C device.
 *
 * Key features:
 *  - Pin reservation via PinManager
 *  - Configurable I2C address
 *  - I2C presence validation during begin()
 *  - Safe row-based printing with automatic line clearing
 *
 * Typical usage:
 * @code
 * LCD1602 lcd(A4, A5);   // Uno-style I2C pins
 * if (lcd.begin()) {
 *   lcd.printLine(0, F("Hello"));
 *   lcd.printLine(1, F("World"));
 * }
 * @endcode
 */
class LCD1602 {
 private:
  LiquidCrystal_I2C* lcd;  ///< Pointer to underlying I2C LCD driver
  uint8_t sdaPin;          ///< SDA pin used for I2C (tracked via PinManager)
  uint8_t sclPin;          ///< SCL pin used for I2C (tracked via PinManager)
  uint8_t i2cAddress;      ///< I2C address of the LCD backpack
  bool validConfig;        ///< True if pins reserved and object initialized

  /**
   * @brief Probe the I2C bus to verify the LCD is present.
   *
   * @return true if the device responds at the configured I2C address
   * @return false otherwise
   */
  bool probeI2C_() const;

 public:
  /**
   * @brief Construct a new LCD1602 object.
   *
   * This constructor reserves the specified SDA and SCL pins using
   * PinManager and prepares the LCD object. No I2C communication
   * occurs until begin() is called.
   *
   * @param sdaP SDA pin number
   * @param sclP SCL pin number
   * @param address I2C address of the LCD (default 0x27)
   */
  LCD1602(uint8_t sdaP, uint8_t sclP, uint8_t address = 0x27) noexcept;

  /**
   * @brief Destroy the LCD1602 object.
   *
   * Frees allocated resources and releases reserved pins.
   */
  ~LCD1602();

  /**
   * @brief Initialize the LCD and I2C bus.
   *
   * This function:
   *  - Initializes the I2C bus
   *  - Verifies the LCD responds at the configured address
   *  - Clears the display and enables the backlight
   *
   * @return true if initialization succeeds
   * @return false if configuration or I2C validation fails
   */
  bool begin();

  /**
   * @brief Check whether the LCD configuration is valid.
   *
   * This reflects successful pin reservation and object construction.
   * It does not indicate whether begin() has succeeded.
   *
   * @return true if configuration is valid
   * @return false otherwise
   */
  bool validConfiguration() const noexcept;

  /**
   * @brief Clear the entire LCD display.
   */
  void clear();

  /**
   * @brief Print text to a specific row of the display.
   *
   * The specified row is cleared before printing.
   * Only rows 0 and 1 are valid.
   *
   * @param row Row index (0 or 1)
   * @param text Null-terminated C string to display
   */
  void printLine(uint8_t row, const char* text);

  /**
   * @brief Print a flash-stored string to a specific row of the display.
   *
   * Supports Arduino F("...") strings stored in program memory.
   *
   * @param row Row index (0 or 1)
   * @param text Flash-resident string
   */
  void printLine(uint8_t row, const __FlashStringHelper* text);
};

}  // namespace Display
}  // namespace ArduinoCommon

#endif
