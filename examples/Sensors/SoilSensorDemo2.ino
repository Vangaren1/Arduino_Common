#include <Arduino.h>
#include <ArduinoCommon.h>

using ArduinoCommon::Sensors::IAnalogSensor;
using ArduinoCommon::Sensors::SoilSensor;

SoilSensor soil1(A0);
SoilSensor soil2(A1);

// Polymorphic array — all sensors seen as IAnalogSensor
IAnalogSensor* sensors[] = {&soil1, &soil2};

const size_t SENSOR_COUNT = sizeof(sensors) / sizeof(sensors[0]);

void setup() {
  Serial.begin(9600);
  delay(200);

  soil1.begin(500, 200);
  soil2.begin(550, 250);

  Serial.println(F("Starting polymorphic sensor demo..."));
}

void loop() {
  for (size_t i = 0; i < SENSOR_COUNT; i++) {
    Serial.print("Sensor ");
    Serial.print(i);
    Serial.print(": ");
    sensors[i]->logPercent(Serial);  // Polymorphic call!
  }

  Serial.println();
  delay(1500);
}
