#ifndef JUMPING_MOTOR_FOC_H
#define JUMPING_MOTOR_FOC_H

#include <Arduino.h>
#include <SimpleFOC.h>

// ============================================
// SimpleFOC Mini v1 Pin Configuration
// ============================================

// Motor driver pins (SimpleFOC Mini -> Teensy 4.1)
#define JUMP_MTR_IN1  2   // PWM Phase A
#define JUMP_MTR_IN2  3   // PWM Phase B
#define JUMP_MTR_IN3  4   // PWM Phase C
#define JUMP_MTR_EN   5   // Enable pin

// Encoder pins (must be interrupt-capable on Teensy 4.1)
#define ENCODER_A     6
#define ENCODER_B     7

// ============================================
// Motor Configuration
// ============================================

// Motor parameters - ADJUST THESE FOR YOUR MOTOR
#define MOTOR_POLE_PAIRS    7       // Number of pole pairs (check your motor specs)
#define ENCODER_PPR         2048    // Pulses per revolution (check your encoder specs)

// Power supply voltage
#define POWER_SUPPLY_VOLTAGE 12.0f  // Volts

// Voltage limits (SimpleFOC Mini max is ~2.5A per phase)
#define MOTOR_VOLTAGE_LIMIT  6.0f   // Max voltage to motor (for safety)

// ============================================
// Jump Sequence Timing
// ============================================

#define JUMP_INTERVAL       20000   // Time between jumps (ms)
#define COMPRESS_TIME       2000    // Compression phase duration (ms)
#define RELEASE_TIME        150     // Release phase duration (ms)
#define BRAKE_TIME          100     // Braking phase duration (ms)

// Velocity targets for jump phases (rad/s)
#define COMPRESS_VELOCITY   -5.0f   // Negative = compress direction
#define RELEASE_VELOCITY    50.0f   // Positive = release direction (fast!)

// ============================================
// Function Declarations
// ============================================

// Initialize the SimpleFOC motor and encoder
void initJumpingMotor();

// Must be called in loop() - runs FOC algorithm
void updateJumpingMotor();

// Execute jump sequence
void jump();

// Motor control functions
void setMotorVelocity(float velocity);
void setMotorPosition(float angle);
void stopJumpingMotor();
void enableJumpingMotor();
void disableJumpingMotor();

// Getters
float getMotorVelocity();
float getMotorPosition();
bool isMotorReady();

// Debug
void printMotorStatus();

#endif
