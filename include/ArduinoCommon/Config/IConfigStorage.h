#ifndef ARDUINOCOMMON_CONFIG_ICONFIGSTORAGE_H
#define ARDUINOCOMMON_CONFIG_ICONFIGSTORAGE_H

#include <Arduino.h>

namespace ArduinoCommon {
namespace Config {
class IConfigStorage {
 public:
  virtual ~IConfigStorage() = default;

  virtual bool read(uint16_t key, void* data, size_t len) = 0;
  virtual bool write(uint16_t key, const void* data, size_t len) = 0;

  virtual bool isUsed(uint16_t key, size_t len) = 0;
  virtual bool clear(uint16_t key, size_t len) = 0;
};
}  // namespace Config
}  // namespace ArduinoCommon

#endif
