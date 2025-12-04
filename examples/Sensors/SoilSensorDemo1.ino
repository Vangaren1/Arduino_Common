#include <Arduino.h>
#include <ArduinoCommon.h>

using ArduinoCommon::Sensors::SoilSensor;

SoilSensor soil(A0);

void setup() {
  Serial.begin(9600);
  delay(200);

  soil.begin(500, 200);  // Example calibration (dry=500, wet=200)
}

void loop() {
  if (!soil.validConfiguration()) {
    Serial.println("Soil sensor invalid.");
    delay(1000);
    return;
  }

  int raw = soil.readRaw();
  int percent = soil.readPercent();

  Serial.print("Raw: ");
  Serial.print(raw);
  Serial.print("    Moisture: ");
  Serial.print(percent);
  Serial.println("%");

  delay(1000);
}
