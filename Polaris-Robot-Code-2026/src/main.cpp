#include <Arduino.h>
#include "JumpingMotorFOC.h"

unsigned long lastJumpTime = 0;
unsigned long lastLedToggleTime = 0;
constexpr unsigned long LED_BLINK_INTERVAL = 500;

void setup() {
  Serial.begin(115200);
  while (!Serial && millis() < 3000); // Wait for serial (with timeout)
  Serial.println("Polaris Robot - Jump Motor Demo");
  
  // Initialize SimpleFOC jumping motor
  initJumpingMotor();
  
  delay(1000);

  pinMode(LED_BUILTIN, OUTPUT);
  Serial.println("Setup complete! First jump in 20 seconds...");
}

void loop() {
  // IMPORTANT: Update FOC algorithm every loop iteration
  updateJumpingMotor();
  
  // Jump sequence on interval
  if (millis() - lastJumpTime >= JUMP_INTERVAL) {
    jump();
    lastJumpTime = millis();
  }
  
  // LED heartbeat
  if (millis() - lastLedToggleTime >= LED_BLINK_INTERVAL) {
    lastLedToggleTime = millis();
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  }
} 


