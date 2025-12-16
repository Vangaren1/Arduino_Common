#ifndef ARDUINOCOMMON_CONFIG_ICONFIGSTORAGE_H
#define ARDUINOCOMMON_CONFIG_ICONFIGSTORAGE_H

#include <Arduino.h>

namespace ArduinoCommon {
namespace Config {

/**
 * @brief Interface for configuration storage backends.
 *
 * Implementations provide a simple byte-oriented API for reading,
 * writing, checking, and clearing regions of non-volatile (or
 * pseudo non-volatile) storage.
 *
 * Examples include EEPROM, flash, FRAM, or even RAM-backed mock
 * implementations for testing.
 */

class IConfigStorage {
 public:
  virtual ~IConfigStorage() = default;

  /**
   * @brief Read a block of bytes from storage.
   *
   * @param key  Byte offset into the reserved storage region where
   *             the block begins (0-based).
   * @param data Pointer to a buffer where the data will be stored.
   * @param len  Number of bytes to read.
   *
   * @return true  If the operation succeeded.
   * @return false If the requested range is invalid or cannot be read.
   *
   */
  virtual bool read(uint16_t key, void* data, size_t len) = 0;

  /**
   * @brief Write a block of bytes.
   *
   * @param key  Byte offset into the reserved storage region where
   *             the block will be written (0-based).
   * @param data Pointer to the buffer containing the bytes to write.
   * @param len  Number of bytes to write.
   *
   * @return true  If the operation succeeded.
   * @return false If the requested range is invalid or cannot be read.
   *
   */
  virtual bool write(uint16_t key, const void* data, size_t len) = 0;

  /**
   * @brief Check whether a region of storage appears to contain data.
   *
   * This function scans len bytes starting at the given key offset.
   * If any byte is not equal to 0xFF, the region is considered "used".
   * This relies on the convention that erased bytes are 0xFF.
   *
   * @param key Byte offset into the reserved storage region.
   * @param len Number of bytes to inspect.
   * @return true  If at least one byte in the range is not 0xFF.
   * @return false If all bytes in the range are 0xFF.
   *
   */
  virtual bool isUsed(uint16_t key, size_t len) = 0;

  /**
   * @brief Clear a region of storage by filling it with 0xFF.
   *
   * @param key Byte offset into the reserved storage region where
   *            the clear range begins.
   * @param len Number of bytes to clear.
   * @return true Always returns true if the requested range is within
   *              the configured region.
   */
  virtual bool clear(uint16_t key, size_t len) = 0;
};
}  // namespace Config
}  // namespace ArduinoCommon

#endif
