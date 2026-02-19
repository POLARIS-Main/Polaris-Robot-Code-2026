#include "DemoMotor.h"

BLDCMotor motor = BLDCMotor(POLE_PAIRS);
BLDCDriver3PWM driver = BLDCDriver3PWM(MTR_IN1, MTR_IN2, MTR_IN3, MTR_EN);

// Live tunable values - initialized from defaults
float tuneVoltageLimit = VOLTAGE_LIMIT;
float tuneSwingSpeed = SWING_SPEED;
unsigned long tuneSwingTime = SWING_TIME_MS;
unsigned long tuneHoldTime = HOLD_TIME_MS;
unsigned long tuneRampTime = RAMP_TIME_MS;
int tuneDirection = MOTOR_DIR;
int tuneSeqMode = SEQ_MODE;

static bool ready = false;
static bool robotEnabled = false;
static String inputBuffer = "";

void initDemoMotor() {
    Serial.println("Init driver...");
    driver.voltage_power_supply = VOLTAGE_SUPPLY;
    driver.init();
    
    Serial.println("Init motor...");
    motor.linkDriver(&driver);
    motor.voltage_limit = tuneVoltageLimit;
    motor.controller = MotionControlType::velocity_openloop;
    motor.init();
    
    // Start disabled
    driver.disable();
    
    ready = true;
    Serial.println("Ready! Waiting for ENABLE command...");
    
    // Print current tuning values
    Serial.println("=== Current Tuning ===");
    Serial.print("VLIMIT:"); Serial.println(tuneVoltageLimit);
    Serial.print("SPEED:"); Serial.println(tuneSwingSpeed);
    Serial.print("SWING_MS:"); Serial.println(tuneSwingTime);
    Serial.print("HOLD_MS:"); Serial.println(tuneHoldTime);
    Serial.print("RAMP_MS:"); Serial.println(tuneRampTime);
    Serial.print("DIR:"); Serial.println(tuneDirection);
    Serial.print("SEQ:"); Serial.println(tuneSeqMode);
}

// Parse SET:param:value commands
void handleSetCommand(String cmd) {
    // Format: SET:PARAM:VALUE
    int first = cmd.indexOf(':');
    int second = cmd.indexOf(':', first + 1);
    if (first < 0 || second < 0) {
        Serial.println("Bad SET format. Use SET:PARAM:VALUE");
        return;
    }
    
    String param = cmd.substring(first + 1, second);
    String valStr = cmd.substring(second + 1);
    float val = valStr.toFloat();
    
    if (param == "VLIMIT") {
        tuneVoltageLimit = constrain(val, 0.0f, 12.0f);
        motor.voltage_limit = tuneVoltageLimit;
        Serial.print("VLIMIT="); Serial.println(tuneVoltageLimit);
    } else if (param == "SPEED") {
        tuneSwingSpeed = constrain(val, 0.0f, 50.0f);
        Serial.print("SPEED="); Serial.println(tuneSwingSpeed);
    } else if (param == "SWING_MS") {
        tuneSwingTime = (unsigned long)constrain(val, 10.0f, 30000.0f);
        Serial.print("SWING_MS="); Serial.println(tuneSwingTime);
    } else if (param == "HOLD_MS") {
        tuneHoldTime = (unsigned long)constrain(val, 0.0f, 30000.0f);
        Serial.print("HOLD_MS="); Serial.println(tuneHoldTime);
    } else if (param == "RAMP_MS") {
        tuneRampTime = (unsigned long)constrain(val, 0.0f, 10000.0f);
        Serial.print("RAMP_MS="); Serial.println(tuneRampTime);
    } else if (param == "DIR") {
        tuneDirection = (val >= 0) ? 1 : -1;
        Serial.print("DIR="); Serial.println(tuneDirection);
    } else if (param == "SEQ") {
        tuneSeqMode = (int)constrain(val, 0.0f, 2.0f);
        Serial.print("SEQ="); Serial.println(tuneSeqMode);
    } else {
        Serial.print("Unknown param: "); Serial.println(param);
    }
}

// Check for serial commands
void checkCommands() {
    while (Serial.available()) {
        char c = Serial.read();
        if (c == '\n' || c == '\r') {
            inputBuffer.trim();
            if (inputBuffer == "ENABLE") {
                robotEnabled = true;
                driver.enable();
                Serial.println("ROBOT ENABLED");
            } else if (inputBuffer == "DISABLE") {
                robotEnabled = false;
                driver.disable();
                Serial.println("ROBOT DISABLED");
            } else if (inputBuffer.startsWith("SET:")) {
                handleSetCommand(inputBuffer);
            } else if (inputBuffer == "GET") {
                // Report all current values
                Serial.print("VLIMIT:"); Serial.println(tuneVoltageLimit);
                Serial.print("SPEED:"); Serial.println(tuneSwingSpeed);
                Serial.print("SWING_MS:"); Serial.println(tuneSwingTime);
                Serial.print("HOLD_MS:"); Serial.println(tuneHoldTime);
                Serial.print("RAMP_MS:"); Serial.println(tuneRampTime);
                Serial.print("DIR:"); Serial.println(tuneDirection);
                Serial.print("SEQ:"); Serial.println(tuneSeqMode);
            } else if (inputBuffer.length() > 0) {
                Serial.print("Unknown command: ");
                Serial.println(inputBuffer);
            }
            inputBuffer = "";
        } else {
            inputBuffer += c;
        }
    }
}

// Helper: run motor in one direction with ramp, for tuneSwingTime ms
static void runOneMove(float direction) {
    unsigned long start = millis();
    while (millis() - start < tuneSwingTime) {
        checkCommands();
        if (!robotEnabled) return;
        unsigned long elapsed = millis() - start;
        float rampFactor = (tuneRampTime > 0 && elapsed < tuneRampTime)
                           ? (float)elapsed / (float)tuneRampTime
                           : 1.0f;
        motor.move(tuneSwingSpeed * direction * rampFactor);
        delayMicroseconds(100);
    }
}

// Helper: rest period with driver off
static void restPeriod() {
    driver.disable();
    Serial.println("-- Rest (driver off)");
    delay(tuneHoldTime);
    checkCommands();
    if (robotEnabled) driver.enable();
}

void runSwingDemo() {
    if (!ready) return;
    
    // Always check for commands
    checkCommands();
    
    // Don't run if disabled
    if (!robotEnabled) {
        delay(10);
        return;
    }
    
    float dir = (tuneDirection >= 0) ? 1.0f : -1.0f;
    
    if (tuneSeqMode == 0) {
        // Mode 0: SWING - forward then backward
        Serial.println(">> Forward");
        runOneMove(dir);
        if (!robotEnabled) return;
        restPeriod();
        if (!robotEnabled) return;
        
        Serial.println("<< Back");
        runOneMove(-dir);
        if (!robotEnabled) return;
        restPeriod();
        
    } else if (tuneSeqMode == 1) {
        // Mode 1: REPEAT - same direction every time
        Serial.print(">> Go (dir="); Serial.print((int)dir); Serial.println(")");
        runOneMove(dir);
        if (!robotEnabled) return;
        restPeriod();
        
    } else if (tuneSeqMode == 2) {
        // Mode 2: SINGLE SHOT - one move then auto-disable
        Serial.print(">> Single shot (dir="); Serial.print((int)dir); Serial.println(")");
        runOneMove(dir);
        driver.disable();
        robotEnabled = false;
        Serial.println("ROBOT DISABLED (single shot done)");
    }
}
