#include <Arduino.h>
#include "DemoMotor.h"

void setup() {
    Serial.begin(115200);
    delay(2000);
    Serial.println("=== 90 DEGREE SWING DEMO ===");
    initDemoMotor();
    pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
    digitalWrite(LED_BUILTIN, HIGH);
    runSwingDemo();
    digitalWrite(LED_BUILTIN, LOW);
}
