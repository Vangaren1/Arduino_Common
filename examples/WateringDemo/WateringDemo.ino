#include <ArduinoCommon.h>

ArduinoCommon::Sensors::SoilSensor sensor(A0);

void setup() {
  Serial.begin(9600);
  sensor.begin();
}

void loop() {
  int moisture = sensor.readPercent();
  if (moisture < 30) {
    // ? pump.dispenseML(100);
  }
  delay(2000);
}
