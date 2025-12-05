#include <Arduino.h>
#include <unity.h>

#include <ArduinoCommon/Sensors/SoilSensor.h>

using ArduinoCommon::Sensors::SoilSensor;

void setUp(void) {}
void tearDown(void) {}

void test_soilsensor_begin_and_calibration(void) {
  SoilSensor sensor1(A0);

  TEST_ASSERT_TRUE(sensor1.begin());
  TEST_ASSERT_TRUE(sensor1.validConfiguration());
  TEST_ASSERT_FALSE(sensor1.hasCalibration());

  sensor1.setCalibration(500, 200, false);
  TEST_ASSERT_TRUE(sensor1.hasCalibration());

  auto cal = sensor1.getCalibration();
  TEST_ASSERT_EQUAL(500, cal.dryRaw);
  TEST_ASSERT_EQUAL(200, cal.wetRaw);
}

void setup() {
  delay(2000);
  UNITY_BEGIN();
  RUN_TEST(test_soilsensor_begin_and_calibration);
  UNITY_END();
}

void loop() {}
