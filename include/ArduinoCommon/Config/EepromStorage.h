#ifndef ARDUINOCOMMON_CONFIG_EEPROMSTORAGE_H
#define ARDUINOCOMMON_CONFIG_EEPROMSTORAGE_H

#include <Arduino.h>
#include <EEPROM.h>
#include "IConfigStorage.h"

namespace ArduinoCommon {
namespace Config {

/**
 * @brief Simple EEPROM-backed implementation of IConfigStorage.
 *
 * The storage is treated as a linear byte array of a fixed size, where
 * the @p key parameter in read/write refers to a byte offset from the
 * start of the region.
 *
 * This class does not perform wear-leveling; callers are responsible
 * for minimizing write cycles to a given region.
 */
class EepromStorage : public IConfigStorage {
 private:
  size_t _size = 0;

 public:
  /**
   * @brief Construct a new EepromStorage object for a given region size.
   *
   * On ESP32 / SAMD platforms this will call EEPROM.begin(size) to
   * initialize the emulated EEPROM. On AVR boards (e.g. Uno) this
   * call is not required and is skipped.
   *
   * @param size Number of bytes in the EEPROM region reserved for
   *             configuration data. All read/write operations must
   *             stay within [0, size).
   */

  explicit EepromStorage(size_t size) {
#if defined(ESP32) || defined(ARDUINO_ARCH_ESP32) || defined(ARDUINO_ARCH_SAMD)
    EEPROM.begin(size);
#endif
    _size = size;
  }

  /**
   * @brief Read a block of bytes from EEPROM.
   *
   * @param key  Byte offset into the reserved EEPROM region where
   *             the block begins (0-based).
   * @param data Pointer to a buffer where the data will be stored.
   * @param len  Number of bytes to read.
   * @return true  If key + len is within the configured region and
   *               all bytes were read.
   * @return false If the requested range exceeds the configured size.
   *
   * @note This function does not perform any checksum or validation;
   *       it returns raw bytes as stored in EEPROM.
   */
  bool read(uint16_t key, void* data, size_t len) override {
    if (key + len > _size) return false;
    uint8_t* bytes = static_cast<uint8_t*>(data);
    for (size_t i = 0; i < len; ++i) {
      bytes[i] = EEPROM.read(key + i);
    }
    return true;
  }

  /**
   * @brief Write a block of bytes to EEPROM.
   *
   * @param key  Byte offset into the reserved EEPROM region where
   *             the block will be written (0-based).
   * @param data Pointer to the buffer containing the bytes to write.
   * @param len  Number of bytes to write.
   * @return true  If key + len is within the configured region and
   *               all bytes were written.
   * @return false If the requested range exceeds the configured size.
   *
   * @note On ESP32 / SAMD platforms this function calls EEPROM.commit()
   *       after the write to persist changes to flash.
   */
  bool write(uint16_t key, const void* data, size_t len) override {
    if (key + len > _size) return false;
    const uint8_t* bytes = static_cast<const uint8_t*>(data);
    for (size_t i = 0; i < len; ++i) {
      EEPROM.update(key + i, bytes[i]);
    }
#if defined(ESP32) || defined(ARDUINO_ARCH_ESP32) || defined(ARDUINO_ARCH_SAMD)
    EEPROM.commit();
#endif
    return true;
  }

  /**
   * @brief Check whether a region of EEPROM appears to contain data.
   *
   * This function scans len bytes starting at the given key offset.
   * If any byte is not equal to 0xFF, the region is considered "used".
   * This relies on the convention that erased EEPROM bytes are 0xFF.
   *
   * @param key Byte offset into the reserved EEPROM region.
   * @param len Number of bytes to inspect.
   * @return true  If at least one byte in the range is not 0xFF.
   * @return false If all bytes in the range are 0xFF.
   *
   * @warning This is a heuristic: it cannot distinguish between a
   *          truly unused region and a region that was intentionally
   *          written with all 0xFF bytes.
   */
  virtual bool isUsed(uint16_t key, size_t len) {
    for (size_t i = 0; i < len; i++) {
      if (EEPROM.read(key + i) != 0xFF) {
        return true;
      }
    }
    return false;
  }

  /**
   * @brief Clear a region of EEPROM by filling it with 0xFF.
   *
   * @param key Byte offset into the reserved EEPROM region where
   *            the clear range begins.
   * @param len Number of bytes to clear.
   * @return true Always returns true if the requested range is within
   *              the configured region.
   *
   * @note On ESP32 / SAMD platforms this function calls EEPROM.commit()
   *       after writing.
   */

  virtual bool clear(uint16_t key, size_t len) {
    for (size_t i = 0; i < len; ++i) {
      EEPROM.update(key + i, 0xFF);
    }
#if defined(ESP32) || defined(ARDUINO_ARCH_ESP32) || defined(ARDUINO_ARCH_SAMD)
    EEPROM.commit();
#endif
    return true;
  }
};
}  // namespace Config
}  // namespace ArduinoCommon

#endif