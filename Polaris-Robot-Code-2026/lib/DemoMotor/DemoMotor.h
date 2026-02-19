#ifndef DEMO_MOTOR_H
#define DEMO_MOTOR_H

#include <Arduino.h>
#include <SimpleFOC.h>

#define MTR_IN1  2
#define MTR_IN2  3
#define MTR_IN3  4
#define MTR_EN   5
#define ENC_OUT  6

#define POLE_PAIRS      11
#define VOLTAGE_SUPPLY  12.0f
#define VOLTAGE_LIMIT   1.50f   // MAX TORQUE - full voltage

// ===========================================
// TUNABLE DEFAULTS - can be changed live via Driver Station
// ===========================================
#define SWING_SPEED     3.0f    // rad/s - SLOW
#define SWING_TIME_MS   5000     // how long to spin
#define HOLD_TIME_MS    500      // pause at each end
#define RAMP_TIME_MS    500      // ramp-up time in ms (0 = instant)
#define MOTOR_DIR       1        // 1 = forward, -1 = reverse
#define SEQ_MODE        0        // 0 = swing (fwd/back), 1 = repeat same dir, 2 = single shot

// Live tunable values (extern so they can be modified at runtime)
extern float tuneVoltageLimit;
extern float tuneSwingSpeed;
extern unsigned long tuneSwingTime;
extern unsigned long tuneHoldTime;
extern unsigned long tuneRampTime;
extern int tuneDirection;
extern int tuneSeqMode;

void initDemoMotor();
void runSwingDemo();
void checkCommands();

#endif
