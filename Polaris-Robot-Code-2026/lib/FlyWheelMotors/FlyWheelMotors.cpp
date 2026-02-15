#include "FlyWheelMotors.h"

constexpr float CORRECTION_DEADBAND = 10.0f;
constexpr float CORRECTION_UPPER_LIMIT = 350.0f;
constexpr int DIRECTION_FORWARD = 0;
constexpr int DIRECTION_REVERSE = 1;

static void defaultRollCorrection(int speed, float roll);
static void defaultPitchCorrection(int speed, float pitch);

static FlywheelCorrectionCallback rollCorrectionHandler = defaultRollCorrection;
static FlywheelCorrectionCallback pitchCorrectionHandler = defaultPitchCorrection;

static void applyFrontFlywheelDirection(int direction) {
  if (direction == DIRECTION_FORWARD) {
    digitalWrite(FLYWHEEL_MTR_FRONT_IN1, HIGH);
    digitalWrite(FLYWHEEL_MTR_FRONT_IN2, LOW);
  } else {
    digitalWrite(FLYWHEEL_MTR_FRONT_IN1, LOW);
    digitalWrite(FLYWHEEL_MTR_FRONT_IN2, HIGH);
  }
}

static void applySideFlywheelDirection(int direction) {
  if (direction == DIRECTION_FORWARD) {
    digitalWrite(FLYWHEEL_MTR_SIDE_IN1, HIGH);
    digitalWrite(FLYWHEEL_MTR_SIDE_IN2, LOW);
  } else {
    digitalWrite(FLYWHEEL_MTR_SIDE_IN1, LOW);
    digitalWrite(FLYWHEEL_MTR_SIDE_IN2, HIGH);
  }
}

static void defaultRollCorrection(int speed, float roll) {
  if (roll > CORRECTION_DEADBAND && roll < CORRECTION_UPPER_LIMIT) {
    if (roll <= 180.0f) {
      applyFrontFlywheelDirection(DIRECTION_FORWARD);
    } else {
      applyFrontFlywheelDirection(DIRECTION_REVERSE);
    }
    analogWrite(FLYWHEEL_MTR_FRONT_PWM, speed);
  } else {
    analogWrite(FLYWHEEL_MTR_FRONT_PWM, 0);
    digitalWrite(FLYWHEEL_MTR_FRONT_IN1, LOW);
    digitalWrite(FLYWHEEL_MTR_FRONT_IN2, LOW);
  }
}

static void defaultPitchCorrection(int speed, float pitch) {
  if (pitch > CORRECTION_DEADBAND && pitch < CORRECTION_UPPER_LIMIT) {
    if (pitch <= 180.0f) {
      applySideFlywheelDirection(DIRECTION_FORWARD);
    } else {
      applySideFlywheelDirection(DIRECTION_REVERSE);
    }
    analogWrite(FLYWHEEL_MTR_SIDE_PWM, speed);
  } else {
    analogWrite(FLYWHEEL_MTR_SIDE_PWM, 0);
    digitalWrite(FLYWHEEL_MTR_SIDE_IN1, LOW);
    digitalWrite(FLYWHEEL_MTR_SIDE_IN2, LOW);
  }
}

void flywheelRollCorrection(int speed, float roll) {
  rollCorrectionHandler(speed, roll);
}

void flywheelPitchCorrection(int speed, float pitch) {
  pitchCorrectionHandler(speed, pitch);
}

void registerRollCorrectionCallback(FlywheelCorrectionCallback callback) {
  rollCorrectionHandler = callback ? callback : defaultRollCorrection;
}

void registerPitchCorrectionCallback(FlywheelCorrectionCallback callback) {
  pitchCorrectionHandler = callback ? callback : defaultPitchCorrection;
}