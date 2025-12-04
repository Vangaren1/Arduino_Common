#include <Arduino.h>
#include <unity.h>
#include <ArduinoCommon/Utils/PinManager.h>

using ArduinoCommon::Utils::PinManager;

void test_reserve_and_release_pin(void) {
  TEST_ASSERT_TRUE(PinManager::reservePin(2));
  TEST_ASSERT_TRUE(PinManager::isPinUsed(2));
  PinManager::releasePin(2);
  TEST_ASSERT_FALSE(PinManager::isPinUsed(2));
}

void setup() {
  UNITY_BEGIN();
  RUN_TEST(test_reserve_and_release_pin);
  UNITY_END();
}

void loop() {}
