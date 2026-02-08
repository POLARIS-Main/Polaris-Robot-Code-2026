#include <Arduino.h>


void jump();
void motorForward(int speed);
void motorReverseRamp(int speed, int stepDelay);
void stopMotor();
void brakeMotor();


#define MOTOR_A_PWM 9
#define MOTOR_A_IN1 7
#define MOTOR_A_IN2 8

#define MOTOR_B_PWM 10
#define MOTOR_B_IN1 11
#define MOTOR_B_IN2 12

#define COMPRESS_SPEED 50 
#define RELEASE_SPEED 255  


#define COMPRESS_TIME 2000
#define RELEASE_TIME 150  
#define BRAKE_TIME 100   
#define JUMP_INTERVAL 20000 

unsigned long lastJumpTime = 0;

void setup() {
  analogWriteResolution(8); // Set PWM resolution to 8 bits (0-255) so it is like a standard Arduino Uno

  pinMode(MOTOR_A_PWM, OUTPUT);
  pinMode(MOTOR_A_IN1, OUTPUT);
  pinMode(MOTOR_A_IN2, OUTPUT);

  pinMode(MOTOR_B_PWM, OUTPUT);
  pinMode(MOTOR_B_IN1, OUTPUT);
  pinMode(MOTOR_B_IN2, OUTPUT);
  
  stopMotor();
  delay(1000);
}

void loop() {

  if (millis() - lastJumpTime >= JUMP_INTERVAL) {
    jump();
    lastJumpTime = millis();
  }
}

void jump() {
  motorForward(COMPRESS_SPEED);
  delay(COMPRESS_TIME);
  stopMotor();
  delay(100);
  
  motorReverseRamp(RELEASE_SPEED, 10);
  delay(RELEASE_TIME);
  
  brakeMotor();
  delay(BRAKE_TIME);
  stopMotor();
}


void motorForward(int speed) {
  digitalWrite(MOTOR_A_IN1, HIGH);
  digitalWrite(MOTOR_A_IN2, LOW);
  digitalWrite(MOTOR_B_IN1, HIGH);
  digitalWrite(MOTOR_B_IN2, LOW);

  analogWrite(MOTOR_A_PWM, speed);
  analogWrite(MOTOR_B_PWM, speed);
}

void motorReverseRamp(int targetSpeed, int stepDelay) {
  digitalWrite(MOTOR_A_IN1, LOW);
  digitalWrite(MOTOR_A_IN2, HIGH);
  digitalWrite(MOTOR_B_IN1, LOW);
  digitalWrite(MOTOR_B_IN2, HIGH);

  for (int s = 0; s <= targetSpeed; s += 5) {
    analogWrite(MOTOR_A_PWM, s);
    analogWrite(MOTOR_B_PWM, s);
    delay(stepDelay);
  }
}

void stopMotor() {
  digitalWrite(MOTOR_A_IN1, LOW);
  digitalWrite(MOTOR_A_IN2, LOW);
  analogWrite(MOTOR_A_PWM, 0);

  digitalWrite(MOTOR_B_IN1, LOW);
  digitalWrite(MOTOR_B_IN2, LOW);
  analogWrite(MOTOR_B_PWM, 0);
}

void brakeMotor() {
  digitalWrite(MOTOR_A_IN1, HIGH);
  digitalWrite(MOTOR_A_IN2, HIGH);
  analogWrite(MOTOR_A_PWM, 255);

  digitalWrite(MOTOR_B_IN1, HIGH);
  digitalWrite(MOTOR_B_IN2, HIGH);
  analogWrite(MOTOR_B_PWM, 255);
}

