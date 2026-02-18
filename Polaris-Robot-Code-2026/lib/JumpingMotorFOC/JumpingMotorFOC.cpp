#include "JumpingMotorFOC.h"

// ============================================
// SimpleFOC Objects
// ============================================

// BLDC motor instance (pole pairs defined in header)
BLDCMotor motor = BLDCMotor(MOTOR_POLE_PAIRS);

// SimpleFOC Mini driver (3 PWM pins + enable)
BLDCDriver3PWM driver = BLDCDriver3PWM(JUMP_MTR_IN1, JUMP_MTR_IN2, JUMP_MTR_IN3, JUMP_MTR_EN);

// Encoder instance
Encoder encoder = Encoder(ENCODER_A, ENCODER_B, ENCODER_PPR);

// Encoder interrupt callbacks
void doEncoderA() { encoder.handleA(); }
void doEncoderB() { encoder.handleB(); }

// Motor state
static bool motorInitialized = false;

// ============================================
// Initialization
// ============================================

void initJumpingMotor() {
    Serial.println("Initializing SimpleFOC Jumping Motor...");
    
    // Initialize encoder
    encoder.init();
    encoder.enableInterrupts(doEncoderA, doEncoderB);
    Serial.println("  Encoder initialized");
    
    // Link encoder to motor
    motor.linkSensor(&encoder);
    
    // Configure driver
    driver.voltage_power_supply = POWER_SUPPLY_VOLTAGE;
    driver.init();
    Serial.println("  Driver initialized");
    
    // Link driver to motor
    motor.linkDriver(&driver);
    
    // ============================================
    // Motor Configuration
    // ============================================
    
    // FOC modulation type
    motor.foc_modulation = FOCModulationType::SpaceVectorPWM;
    
    // Control mode - velocity for jumping
    motor.controller = MotionControlType::velocity;
    
    // Velocity PID controller
    motor.PID_velocity.P = 0.2f; // TODO: Tune this value
    motor.PID_velocity.I = 20.0f;
    motor.PID_velocity.D = 0.0f;
    motor.PID_velocity.output_ramp = 1000;  // Volts/second ramp
    
    // Velocity low-pass filter
    motor.LPF_velocity.Tf = 0.01f;  // 10ms time constant
    
    // Voltage limit
    motor.voltage_limit = MOTOR_VOLTAGE_LIMIT;
    
    // Velocity limit (rad/s)
    motor.velocity_limit = 100.0f;
    
    // ============================================
    // Initialize Motor
    // ============================================
    
    motor.init();
    Serial.println("  Motor initialized");
    
    // Calibrate encoder and start FOC
    Serial.println("  Calibrating encoder (motor will move)...");
    motor.initFOC();
    Serial.println("  FOC calibration complete!");
    
    motorInitialized = true;
    Serial.println("SimpleFOC Jumping Motor READY");
}

// ============================================
// Update Loop (call in main loop)
// ============================================

void updateJumpingMotor() {
    if (!motorInitialized) return;
    
    // Main FOC algorithm - MUST be called as fast as possible
    motor.loopFOC();
    
    // Motion control loop
    motor.move();
}

// ============================================
// Jump Sequence
// ============================================

// Helper: wait while keeping FOC running
static void waitWithFOC(unsigned long duration) {
    unsigned long start = millis();
    while (millis() - start < duration) {
        updateJumpingMotor();
        delayMicroseconds(100);
    }
}

void jump() {
    if (!motorInitialized) {
        Serial.println("ERROR: Motor not initialized!");
        return;
    }
    
    Serial.println("JUMP: Starting sequence...");
    
    // Phase 1: Compress
    Serial.println("  Phase 1: Compressing...");
    motor.controller = MotionControlType::velocity;
    motor.move(COMPRESS_VELOCITY);
    waitWithFOC(COMPRESS_TIME);
    
    // Brief stop
    motor.move(0);
    waitWithFOC(100);
    
    // Phase 2: Release (FAST!)
    Serial.println("  Phase 2: Releasing!");
    motor.move(RELEASE_VELOCITY);
    waitWithFOC(RELEASE_TIME);
    
    // Phase 3: Brake
    Serial.println("  Phase 3: Braking...");
    motor.move(0);
    waitWithFOC(BRAKE_TIME);
    
    Serial.println("JUMP: Sequence complete");
}

// ============================================
// Motor Control Functions
// ============================================

void setMotorVelocity(float velocity) {
    if (!motorInitialized) return;
    motor.controller = MotionControlType::velocity;
    motor.move(velocity);
}

void setMotorPosition(float angle) {
    if (!motorInitialized) return;
    motor.controller = MotionControlType::angle;
    motor.move(angle);
}

void stopJumpingMotor() {
    if (!motorInitialized) return;
    motor.move(0);
}

void enableJumpingMotor() {
    if (!motorInitialized) return;
    motor.enable();
}

void disableJumpingMotor() {
    if (!motorInitialized) return;
    motor.disable();
}

// ============================================
// Getters
// ============================================

float getMotorVelocity() {
    if (!motorInitialized) return 0.0f;
    return motor.shaft_velocity;
}

float getMotorPosition() {
    if (!motorInitialized) return 0.0f;
    return motor.shaft_angle;
}

bool isMotorReady() {
    return motorInitialized;
}

// ============================================
// Debug
// ============================================

void printMotorStatus() {
    if (!motorInitialized) {
        Serial.println("Motor not initialized");
        return;
    }
    
    Serial.print("Pos: ");
    Serial.print(motor.shaft_angle, 2);
    Serial.print(" rad | Vel: ");
    Serial.print(motor.shaft_velocity, 2);
    Serial.print(" rad/s | V: ");
    Serial.print(motor.voltage.q, 2);
    Serial.println(" V");
}
