#ifndef FLYWHEELMOTORS_H
#define FLYWHEELMOTORS_H

#include <Arduino.h>

#define FLYWHEEL_MTR_FRONT_PWM 3
#define FLYWHEEL_MTR_FRONT_IN1 2
#define FLYWHEEL_MTR_FRONT_IN2 1

#define FLYWHEEL_MTR_SIDE_PWM 6
#define FLYWHEEL_MTR_SIDE_IN1 5
#define FLYWHEEL_MTR_SIDE_IN2 4

void flywheelRollCorrection(int speed, int roll);
void flywheelPitchCorrection(int speed, int pitch);

#endif