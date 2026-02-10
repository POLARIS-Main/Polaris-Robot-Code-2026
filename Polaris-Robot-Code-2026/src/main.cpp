#include <Arduino.h>
#include <JumpingMotors.h>

unsigned long lastJumpTime = 0;

void setup() {
  analogWriteResolution(8); // Set PWM resolution to 8 bits (0-255) so it is like a standard Arduino Uno

  pinMode(JUMP_MTR_A_PWM, OUTPUT);
  pinMode(JUMP_MTR_A_IN1, OUTPUT);
  pinMode(JUMP_MTR_A_IN2, OUTPUT);

  pinMode(JUMP_MTR_B_PWM, OUTPUT);
  pinMode(JUMP_MTR_B_IN1, OUTPUT);
  pinMode(JUMP_MTR_B_IN2, OUTPUT);
  
  stopMotor();
  delay(1000);
}

void loop() {
  if (millis() - lastJumpTime >= JUMP_INTERVAL) {
    jump();
    lastJumpTime = millis();
  }
}



