#pragma once
#include <Arduino.h>

class LiquidCrystal_I2C {
 public:
  LiquidCrystal_I2C(uint8_t addr, uint8_t cols, uint8_t rows)
      : address(addr), cols(cols), rows(rows) {}

  void init() { initCalled = true; }
  void clear() { clearCalled = true; }
  void backlight() { backlightCalled = true; }
  void setCursor(uint8_t col, uint8_t row) {
    lastCol = col;
    lastRow = row;
  }
  size_t print(const char* text) {
    lastPrint = text ? text : "";
    return lastPrint.length();
  }
  size_t print(const __FlashStringHelper*) {
    lastPrint = "[FLASH]";
    return lastPrint.length();
  }

  // Inspectable state
  bool initCalled = false;
  bool clearCalled = false;
  bool backlightCalled = false;
  uint8_t lastRow = 255;
  uint8_t lastCol = 255;
  String lastPrint;

 private:
  uint8_t address;
  uint8_t cols;
  uint8_t rows;
};
