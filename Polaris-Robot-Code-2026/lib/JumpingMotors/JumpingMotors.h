#ifndef JUMPINGMOTORS_H
#define JUMPINGMOTORS_H

#include <Arduino.h>

#define JUMP_MTR_A_PWM 9
#define JUMP_MTR_A_IN1 7
#define JUMP_MTR_A_IN2 8

#define JUMP_MTR_B_PWM 10
#define JUMP_MTR_B_IN1 11
#define JUMP_MTR_B_IN2 12

#define JUMP_INTERVAL 20000 // 20 seconds may have to change for CDR

extern boolean isJumping;

void motorForward(int speed);
void motorReverseRamp(int targetSpeed, int stepDelay);
void stopMotor();
void brakeMotor();
void jump();

#endif