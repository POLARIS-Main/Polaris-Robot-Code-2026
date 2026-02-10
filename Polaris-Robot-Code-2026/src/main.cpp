#include <Arduino.h>
#include <JumpingMotors.h>
#include <FlyWheelMotors.h>

unsigned long lastJumpTime = 0;

void setup() {
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
}

void loop() {
  if (millis() - lastJumpTime >= JUMP_INTERVAL) {
    jump();
    lastJumpTime = millis();
  }
  flywheelRollCorrection(255, 0); // 0 will be replaced with actual roll value from IMU
  flywheelPitchCorrection(255, 0);  // 0 will be replaced with actual pitch value from IMU
}



