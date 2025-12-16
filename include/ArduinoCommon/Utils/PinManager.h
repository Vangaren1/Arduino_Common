#ifndef ARDUINOCOMMON_UTILS_PINMANAGER_H
#define ARDUINOCOMMON_UTILS_PINMANAGER_H

#include <Arduino.h>

namespace ArduinoCommon {
namespace Utils {

#if defined(NUM_DIGITAL_PINS)
static constexpr uint8_t MaxPins = NUM_DIGITAL_PINS;
#else
static constexpr uint8_t MaxPins = 64;
#endif

static_assert(MaxPins <= 64,
              "PinManager: MaxPins exceeds 64, adjust bitmask type if needed.");

/**
 * @brief Logical pin configuration modes supported by PinManager.
 *
 * These map to the microcontroller's electrical pin modes:
 * - Free: Pin is unassigned.
 * - Input: Equivalent to pinMode(pin, INPUT).
 * - InputPullup: Equivalent to pinMode(pin, INPUT_PULLUP).
 * - Output: Equivalent to pinMode(pin, OUTPUT).
 * - InputPulldown: For ESP32 and some ARM boards (INPUT_PULLDOWN).
 * - OutputOpenDrain: For ESP32 open-drain mode (OUTPUT_OPENDRAIN).
 */
enum class PinModeType : uint8_t {
  Free = 0,
  Input,
  InputPullup,
  Output,
// Only for boards that define these modes (e.g., ESP32 / some ARM boards).
#ifdef INPUT_PULLDOWN
  InputPulldown,
#endif
#ifdef OUTPUT_OPENDRAIN
  OutputOpenDrain
#endif
};

/**
 * @brief Centralized manager for reserving and configuring microcontroller
 * pins.
 *
 * PinManager prevents accidental double-allocation of pins by sensors,
 * actuators, and other modules. It tracks whether each pin is reserved,
 * what mode it is configured in, and helps ensure consistent access to the
 * microcontroller's hardware I/O.
 *
 * All methods are static, so PinManager acts as a global registry of
 * pin usage across the entire ArduinoCommon library and the user's sketch.
 */
class PinManager {
 private:
  /// Bitmask tracking reserved pins (1 = reserved).
  static uint64_t pinMask;
  /// Tracks configured mode for each pin index.
  static PinModeType pinModes[MaxPins];

 public:
  /**
   * @brief Reserve a pin so no other module can use it.
   *
   * Implementations typically treat a pin as unusable by other modules
   * once it has been successfully reserved. This function does not set
   * the pinMode; use configureInput()/configureOutput() for that.
   *
   * @param pin The pin number to reserve.
   * @return true  If the pin was within range and previously free, and
   *               is now marked reserved.
   * @return false If the pin is out of range or already reserved.
   */
  static bool reservePin(uint8_t pin);

  /**
   * @brief Release a reserved pin so other modules can use it.
   *
   * If the pin is out of range or not currently reserved, this function
   * has no effect.
   *
   * @param pin The pin number to release.
   */
  static void releasePin(uint8_t pin);

  /**
   * @brief Configure a pin as an output.
   *
   * This sets a specific pin on the board as an output pin. If the board
   * supports open-drain outputs (e.g., some ESP32 and ARM boards), the
   * @p openDrain flag can be used to select that mode.
   *
   * If openDrain is true on a board that does not support OUTPUT_OPENDRAIN,
   * the configuration fails.
   *
   * Implementations may also reserve the pin if it is not already reserved.
   *
   * @param pin       The pin to be configured as an output.
   * @param openDrain Whether to configure the pin as open-drain (if supported).
   * @return true  If the pin is within range, not conflicting, and has been
   *               successfully configured as an output.
   * @return false If the pin is out of range, already in use in an incompatible
   *               way, or the requested mode is not supported.
   */
  static bool configureOutput(uint8_t pin, bool openDrain = false);

  /**
   * @brief Configure a pin as an input.
   *
   * This sets a specific pin on the board as an input pin. The pin can be
   * configured with an internal pull-up or, where supported, an internal
   * pull-down resistor.
   *
   * If both @p pullup and @p pulldown are true, or if @p pulldown is true on
   * a board that does not support INPUT_PULLDOWN, the configuration fails.
   *
   * Implementations may also reserve the pin if it is not already reserved.
   *
   * @param pin      The pin to be configured as an input.
   * @param pullup   Whether to enable the internal pull-up resistor.
   * @param pulldown Whether to enable the internal pull-down resistor (if
   * supported).
   * @return true  If the pin is within range and successfully configured.
   * @return false If the requested configuration is invalid or unsupported.
   */
  static bool configureInput(uint8_t pin, bool pullup = false,
                             bool pulldown = false);

  /**
   * @brief Get the currently tracked mode for a pin.
   *
   * @param pin The pin to inspect.
   * @return PinModeType The mode associated with the pin. If @p pin is out of
   *         range, implementations may return PinModeType::Free.
   */
  static PinModeType getPinMode(uint8_t pin);

  /**
   * @brief Check whether a pin is currently marked as reserved.
   *
   * @param pin The pin to check.
   * @return true  If the pin is in use (reserved).
   * @return false If the pin is not reserved or is out of range.
   */
  static bool isPinUsed(uint8_t pin);

  /**
   * @brief Check whether a pin is an analog-capable pin.
   *
   * On boards where analog-capable pins have special numbering or
   * naming (e.g., A0, A1, etc.), this function can be used to determine
   * if a given pin supports analogRead().
   *
   * @param pin The pin to check.
   * @return true  If the pin is an analog-capable pin.
   * @return false If the pin is not analog-capable or is out of range.
   */
  static bool isAnalogPin(uint8_t pin);

  /**
   * @brief Check whether a pin is a digital-capable pin.
   *
   * On typical Arduino boards, most pins are digital-capable. This function
   * is provided for symmetry with isAnalogPin().
   *
   * @param pin The pin to check.
   * @return true  If the pin is a digital-capable pin.
   * @return false If the pin is not digital-capable or is out of range.
   */
  static bool isDigitalPin(uint8_t pin);

  /**
   * @brief Check whether a pin supports PWM output.
   *
   * @param pin The pin to check.
   * @return true  If the pin supports PWM (e.g., usable with analogWrite()).
   * @return false If the pin does not support PWM or is out of range.
   */
  static bool isPWMPin(uint8_t pin);

  /**
   * @brief Dump internal PinManager state to a stream for debugging.
   *
   * This can include information such as reserved pins, configured modes,
   * and any other useful state for diagnostics.
   *
   * @param out The output stream or data log (default is Serial).
   */
  static void debugDump(Stream& out = Serial);
};

}  // namespace Utils
}  // namespace ArduinoCommon

#endif
