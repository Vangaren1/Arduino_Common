#include <Arduino.h>
#include <ArduinoCommon.h>

using ArduinoCommon::Display::LCD1602;

LCD1602 lcd(A4, A5);

void formatUptime(char* buf, size_t len);

bool lcdReady = false;

void setup() {
  Serial.begin(9600);
  delay(200);
  lcdReady = lcd.begin();

  if (!lcdReady) {
    Serial.println("LCD failed to initialize.");
  } else {
    lcd.printLine(0, F("Uptime"));
  }
}

void loop() {
  if (!lcdReady) {
    delay(1000);
    return;
  }

  char timeBuf[17];
  formatUptime(timeBuf, sizeof(timeBuf));
  lcd.printLine(1, timeBuf);

  delay(1000);
}

void formatUptime(char* buf, size_t len) {
  unsigned long seconds = millis() / 1000;
  unsigned long mins = (seconds / 60) % 60;
  unsigned long hrs = (seconds / 3600);

  snprintf(buf, len, "%02lu:%02lu:%02lu", hrs, mins, seconds % 60);
}
