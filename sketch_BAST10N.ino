#include <AFMotor.h> // Adafruit V1 motor shield library

#define forwardSpeed 255
#define turnSpeed    255
#define attackSpeed  255
#define attackDistance 20  // distance in cm to trigger attack

// Motor setup
AF_DCMotor motor1(4, MOTOR34_1KHZ);
AF_DCMotor motor2(1, MOTOR12_1KHZ);

// Ultrasonic sensor pins
#define trigPin 9
#define echoPin 8

// Function to measure distance
long getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  long duration = pulseIn(echoPin, HIGH);
  long distance = duration * 0.034 / 2; // Convert to cm
  return distance;
}

void setup() {
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  int AINL = digitalRead(A0);
  int AINC = digitalRead(A1);
  int AINR = digitalRead(A2);

  long distance = getDistance();

  // ATTACK MODE: if object is close
  if (distance > 0 && distance <= attackDistance) {
    motor1.setSpeed(attackSpeed);
    motor2.setSpeed(attackSpeed);
    motor1.run(FORWARD);
    motor2.run(FORWARD);
    delay(100);
    return; // skip rest of logic to prioritize attack
  }

  // LINE SENSOR LOGIC
  if (!AINL && AINC && !AINR) { // 010
    motor1.setSpeed(forwardSpeed);
    motor2.setSpeed(forwardSpeed);
    motor1.run(FORWARD);
    motor2.run(FORWARD);
  }
  else if (AINL && AINC && !AINR) { // 110
    motor1.setSpeed(turnSpeed);
    motor2.setSpeed(turnSpeed);
    motor1.run(FORWARD);
    motor2.run(RELEASE);
  }
  else if (!AINL && AINC && AINR) { // 011
    motor1.setSpeed(turnSpeed);
    motor2.setSpeed(turnSpeed);
    motor1.run(RELEASE);
    motor2.run(FORWARD);
  }
  else if (AINL && !AINC && !AINR) { // 100
    motor1.setSpeed(turnSpeed);
    motor2.setSpeed(turnSpeed);
    motor1.run(FORWARD);
    motor2.run(BACKWARD);
  }
  else if (!AINL && !AINC && AINR) { // 001
    motor1.setSpeed(turnSpeed);
    motor2.setSpeed(turnSpeed);
    motor1.run(BACKWARD);
    motor2.run(FORWARD);
  }
}
