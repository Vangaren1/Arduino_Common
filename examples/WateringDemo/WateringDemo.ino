#include <SOILSENSOR.h>
#include <PumpController.h>

SoilMoistureSensor sensor(A0);
PumpController pump(5);

void setup()
{
    Serial.begin(9600);
    sensor.begin();
    pump.begin();
}

void loop()
{
    int moisture = sensor.readPercent();
    if (moisture < 30)
    {
        pump.dispenseML(100);
    }
    delay(2000);
}
