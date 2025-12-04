#ifndef ARDUINOCOMMON_CONFIG_EEPROMSTORAGE_H
#define ARDUINOCOMMON_CONFIG_EEPROMSTORAGE_H

#include <Arduino.h>
#include <EEPROM.h>
#include "IConfigStorage.h"

namespace ArduinoCommon {
namespace Config {
class EepromStorage : public IConfigStorage {
 private:
  size_t _size = 0;

 public:
  explicit EepromStorage(size_t size) {
#if defined(ESP32) || defined(ARDUINO_ARCH_ESP32) || defined(ARDUINO_ARCH_SAMD)
    EEPROM.begin(size);
#endif
    _size = size;
  }

  bool read(uint16_t key, void* data, size_t len) override {
    if (key + len > _size) return false;
    uint8_t* bytes = static_cast<uint8_t*>(data);
    for (size_t i = 0; i < len; ++i) {
      bytes[i] = EEPROM.read(key + i);
    }
    return true;
  }

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

  virtual bool isUsed(uint16_t key, size_t len) {
    for (size_t i = 0; i < len; i++) {
      if (EEPROM.read(key + i) != 0xFF) {
        return true;
      }
    }
    return false;
  }

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