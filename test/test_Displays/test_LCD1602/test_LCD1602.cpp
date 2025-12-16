#include <Arduino.h>
#include <unity.h>

#include "fakes/FakeLiquidCrystal_I2C.h"
#include <ArduinoCommon/Display/LCD1602.h>

using ArduinoCommon::Display::LCD1602;

void test_printLine_valid_row() {
  Serial.println("before begin()");
  LCD1602 lcd(A4, A5);

  bool ok = lcd.begin();
  Serial.println("after begin()");
  TEST_ASSERT_TRUE(ok);

  lcd.printLine(0, "Hello");
  Serial.println("after printLine()");

  auto fake = lcd._getLcdForTests();
  TEST_ASSERT_NOT_NULL(fake);
  TEST_ASSERT_EQUAL_UINT8(0, fake->lastRow);
  TEST_ASSERT_EQUAL_STRING("Hello", fake->lastPrint.c_str());
}

void setup() {
  Serial.begin(115200);

  // Give PlatformIO time to open the serial port AFTER upload/reset.
  // Print repeatedly so we don't miss the window.
  for (int i = 0; i < 40; i++) {  // ~4 seconds
    Serial.println("TEST BOOT");
    delay(100);
  }

  Serial.println("about to start unity");

  UNITY_BEGIN();
  RUN_TEST(test_printLine_valid_row);
  UNITY_END();

  Serial.println("done");

  while (true) delay(1000);
}

void loop() {}
