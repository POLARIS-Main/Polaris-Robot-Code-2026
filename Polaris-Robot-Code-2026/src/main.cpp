#include <Arduino.h>
#include "JumpingMotors.h"
#include "FlyWheelMotors.h"
#include "BalancingSensors.h"

unsigned long lastJumpTime = 0;
unsigned long lastLedToggleTime = 0;
constexpr unsigned long LED_BLINK_INTERVAL = 500;

void setup() {
  Serial.begin(115200);  // Enable serial for debugging
  
  analogWriteResolution(8); // Set PWM resolution to 8 bits (0-255) so it is like a standard Arduino Uno

  pinMode(JUMP_MTR_A_PWM, OUTPUT);
  pinMode(JUMP_MTR_A_IN1, OUTPUT);
  pinMode(JUMP_MTR_A_IN2, OUTPUT);

  pinMode(JUMP_MTR_B_PWM, OUTPUT);
  pinMode(JUMP_MTR_B_IN1, OUTPUT);
  pinMode(JUMP_MTR_B_IN2, OUTPUT);

  pinMode(FLYWHEEL_MTR_FRONT_PWM, OUTPUT);
  pinMode(FLYWHEEL_MTR_FRONT_IN1, OUTPUT);
  pinMode(FLYWHEEL_MTR_FRONT_IN2, OUTPUT);

  pinMode(FLYWHEEL_MTR_SIDE_PWM, OUTPUT);
  pinMode(FLYWHEEL_MTR_SIDE_IN1, OUTPUT);
  pinMode(FLYWHEEL_MTR_SIDE_IN2, OUTPUT);
  
  stopMotor();
  delay(1000);

  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  if (millis() - lastJumpTime >= JUMP_INTERVAL) {
    jump();
    lastJumpTime = millis();
  }
  if (millis() - lastLedToggleTime >= LED_BLINK_INTERVAL) {
    lastLedToggleTime = millis();
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  }
  flywheelPitchCorrection(100, getCurrentPitch());
  flywheelRollCorrection(100, getCurrentRoll());
} 


