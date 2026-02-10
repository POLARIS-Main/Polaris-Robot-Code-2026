#include "FlyWheelMotors.h"

#define CORRECTION_INTERVAL 100

void flywheelRollCorrection(int speed, int roll) {
  if (roll > 10 && roll < 350) { 
    if (roll <= 180) {
      // Make front flywheel spin forward
      Serial.println("Front flywheel forward");
    }
    else if (roll > 180) {
      // Make front flywheel spin backward 
      Serial.println("Front flywheel backward");
    }
    else {
      // Stop front flywheels
      Serial.println("Front flywheel stopped");
    }
  }  
}

void flywheelPitchCorrection(int speed, int pitch) {
  if (pitch > 10 && pitch < 350) { 
    if (pitch <= 180) {
      // Make side flywheel spin forward
      Serial.println("Side flywheel forward");
    }
    else if (pitch > 180) {
      // Make side flywheel spin backward 
      Serial.println("Side flywheel backward");
    }
    else {
      // Stop side flywheels
      Serial.println("Side flywheel stopped");
    }
  }
}