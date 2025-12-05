#include <Arduino.h>
#include <unity.h>

#include <ArduinoCommon/Utils/PinManager.h>

using ArduinoCommon::Utils::PinManager;

void setUp(void) {
  // e.g., reset shared state if needed
}

// Optional: runs after each test
void tearDown(void) {}

void test_reserve_and_release_pin(void) {
  const uint8_t pin = 2;

  // Ensure it's free to begin with (you might add a ResetAll() later)
  // For now, just try to reserve and assert success.
  bool reserved = PinManager::reservePin(pin);
  TEST_ASSERT_TRUE_MESSAGE(reserved, "Failed to reserve pin");

  TEST_ASSERT_TRUE(PinManager::isPinUsed(pin));

  PinManager::releasePin(pin);
  TEST_ASSERT_FALSE(PinManager::isPinUsed(pin));
}

void test_double_reserve_same_pin_fails(void) {
  const uint8_t pin = 3;

  TEST_ASSERT_TRUE(PinManager::reservePin(pin));
  TEST_ASSERT_TRUE(PinManager::isPinUsed(pin));

  // Second reservation should fail
  TEST_ASSERT_FALSE(PinManager::reservePin(pin));

  PinManager::releasePin(pin);
  TEST_ASSERT_FALSE(PinManager::isPinUsed(pin));
}

// Arduino-style test runner

void setup() {
  // Wait a bit for serial to come up (optional but often helpful)
  delay(2000);

  UNITY_BEGIN();

  RUN_TEST(test_reserve_and_release_pin);
  RUN_TEST(test_double_reserve_same_pin_fails);

  UNITY_END();
}

void loop() {
  // not used, but required by Arduino framework
}
