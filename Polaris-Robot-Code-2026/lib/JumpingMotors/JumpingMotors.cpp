#include "JumpingMotors.h"

#define COMPRESS_SPEED 50 
#define RELEASE_SPEED 255  

#define COMPRESS_TIME 2000
#define RELEASE_TIME 150  
#define BRAKE_TIME 100  // may have to change to higher value depending on how high it jumps

#define RAMP_SPEED_STEP 5

void motorForward(int speed) {
  digitalWrite(JUMP_MTR_A_IN1, HIGH);
  digitalWrite(JUMP_MTR_A_IN2, LOW);
  digitalWrite(JUMP_MTR_B_IN1, HIGH);
  digitalWrite(JUMP_MTR_B_IN2, LOW);

  analogWrite(JUMP_MTR_A_PWM, speed);
  analogWrite(JUMP_MTR_B_PWM, speed);
}

void motorReverseRamp(int targetSpeed, int stepDelay) {
  digitalWrite(JUMP_MTR_A_IN1, LOW);
  digitalWrite(JUMP_MTR_A_IN2, HIGH);
  digitalWrite(JUMP_MTR_B_IN1, LOW);
  digitalWrite(JUMP_MTR_B_IN2, HIGH);

  for (int s = 0; s <= targetSpeed; s += RAMP_SPEED_STEP) {
    analogWrite(JUMP_MTR_A_PWM, s);
    analogWrite(JUMP_MTR_B_PWM, s);
    delay(stepDelay);
  }
}

void stopMotor() {
  digitalWrite(JUMP_MTR_A_IN1, LOW);
  digitalWrite(JUMP_MTR_A_IN2, LOW);
  analogWrite(JUMP_MTR_A_PWM, 0);

  digitalWrite(JUMP_MTR_B_IN1, LOW);
  digitalWrite(JUMP_MTR_B_IN2, LOW);
  analogWrite(JUMP_MTR_B_PWM, 0);
}

void brakeMotor() {
  digitalWrite(JUMP_MTR_A_IN1, HIGH);
  digitalWrite(JUMP_MTR_A_IN2, HIGH);
  analogWrite(JUMP_MTR_A_PWM, 255);

  digitalWrite(JUMP_MTR_B_IN1, HIGH);
  digitalWrite(JUMP_MTR_B_IN2, HIGH);
  analogWrite(JUMP_MTR_B_PWM, 255);
}

void jump() {
  // This should compress the motors, although I don't exactly know the directions we may have to switch them
  motorForward(COMPRESS_SPEED);
  delay(COMPRESS_TIME);
  stopMotor();
  delay(100);
  
  motorReverseRamp(RELEASE_SPEED, 10);
  delay(RELEASE_TIME);
  
  brakeMotor();
  delay(BRAKE_TIME);
  stopMotor();
}