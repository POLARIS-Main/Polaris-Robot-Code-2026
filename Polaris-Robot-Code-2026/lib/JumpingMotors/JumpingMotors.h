#ifndef JUMPINGMOTORS_H
#define JUMPINGMOTORS_H

#include <Arduino.h>

#define MOTOR_A_PWM 9
#define MOTOR_A_IN1 7
#define MOTOR_A_IN2 8

#define MOTOR_B_PWM 10
#define MOTOR_B_IN1 11
#define MOTOR_B_IN2 12

#define JUMP_INTERVAL 20000

void motorForward(int speed);
void motorReverseRamp(int targetSpeed, int stepDelay);
void stopMotor();
void brakeMotor();
void jump();

#endif