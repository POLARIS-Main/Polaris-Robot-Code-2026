#ifndef FLYWHEELMOTORS_H
#define FLYWHEELMOTORS_H

#include <Arduino.h>

// Flywheel motor pins (H-bridge driver)
// Updated to avoid conflict with SimpleFOC jumping motor (pins 2-7)
#define FLYWHEEL_MTR_FRONT_PWM 8
#define FLYWHEEL_MTR_FRONT_IN1 9
#define FLYWHEEL_MTR_FRONT_IN2 10

#define FLYWHEEL_MTR_SIDE_PWM 11
#define FLYWHEEL_MTR_SIDE_IN1 12
#define FLYWHEEL_MTR_SIDE_IN2 14  // Pin 13 is LED_BUILTIN, skip to 14

using FlywheelCorrectionCallback = void (*)(int speed, float angle);

void flywheelRollCorrection(int speed, float roll);
void flywheelPitchCorrection(int speed, float pitch);
void registerRollCorrectionCallback(FlywheelCorrectionCallback callback);
void registerPitchCorrectionCallback(FlywheelCorrectionCallback callback);

#endif