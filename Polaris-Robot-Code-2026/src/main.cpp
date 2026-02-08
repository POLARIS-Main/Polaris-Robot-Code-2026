#include <Arduino.h>


void jump();
void motorForward(int speed);
void motorReverse(int speed);
void stopMotor();
void brakeMotor();

#define MOTOR_PWM_PIN 9     
#define MOTOR_IN1_PIN 7      
#define MOTOR_IN2_PIN 8   

#define COMPRESS_SPEED 200   
#define RELEASE_SPEED 255    


#define COMPRESS_TIME 2000  
#define RELEASE_TIME 150     
#define BRAKE_TIME 100       
#define JUMP_INTERVAL 20000   

unsigned long lastJumpTime = 0;

void setup() {
  pinMode(MOTOR_PWM_PIN, OUTPUT);
  pinMode(MOTOR_IN1_PIN, OUTPUT);
  pinMode(MOTOR_IN2_PIN, OUTPUT);
  
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
  
  motorReverse(RELEASE_SPEED);
  delay(RELEASE_TIME);
  
  brakeMotor();
  delay(BRAKE_TIME);
  stopMotor();
}


void motorForward(int speed) {
  digitalWrite(MOTOR_IN1_PIN, HIGH);
  digitalWrite(MOTOR_IN2_PIN, LOW);
  analogWrite(MOTOR_PWM_PIN, speed);
}

void motorReverse(int speed) {
  digitalWrite(MOTOR_IN1_PIN, LOW);
  digitalWrite(MOTOR_IN2_PIN, HIGH);
  analogWrite(MOTOR_PWM_PIN, speed);
}

void stopMotor() {
  digitalWrite(MOTOR_IN1_PIN, LOW);
  digitalWrite(MOTOR_IN2_PIN, LOW);
  analogWrite(MOTOR_PWM_PIN, 0);
}

void brakeMotor() {
  digitalWrite(MOTOR_IN1_PIN, HIGH);
  digitalWrite(MOTOR_IN2_PIN, HIGH);
  analogWrite(MOTOR_PWM_PIN, 255);
}
