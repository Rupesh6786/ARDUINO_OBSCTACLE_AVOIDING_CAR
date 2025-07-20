#include <AFMotor.h>  
#include <NewPing.h>
#include <Servo.h> 

#define TRIG_PIN A0 
#define ECHO_PIN A1 
#define MAX_DISTANCE 200 
#define MAX_SPEED 120
#define MAX_SPEED_OFFSET 20

NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE); 

AF_DCMotor motor1(1, MOTOR12_1KHZ); 
AF_DCMotor motor2(2, MOTOR12_1KHZ);
AF_DCMotor motor3(3, MOTOR34_1KHZ);
AF_DCMotor motor4(4, MOTOR34_1KHZ);
Servo myservo;   

boolean goesForward = false;
int distance = 100;
int speedSet = 0;

void setup() {
  Serial.begin(9600);
  myservo.attach(10);  
  myservo.write(115); 
  Serial.println("Servo centered to 115°");
  
  delay(2000);
  distance = readPing();
  delay(100);
  distance = readPing();
  delay(100);
  distance = readPing();
  delay(100);
  distance = readPing();
  delay(100);

  Serial.print("Initial distance: ");
  Serial.println(distance);
}

void loop() {
  int distanceR = 0;
  int distanceL = 0;
  delay(40);
  
  if (distance <= 15) {
    Serial.println("Obstacle detected! Executing avoidance sequence...");

    moveStop();
    delay(100);

    moveBackward();
    delay(300);

    moveStop();
    delay(200);

    distanceR = lookRight();
    Serial.print("Distance Right: ");
    Serial.println(distanceR);
    delay(200);

    distanceL = lookLeft();
    Serial.print("Distance Left: ");
    Serial.println(distanceL);
    delay(200);

    if (distanceR >= distanceL) {
      Serial.println("Turning Right...");
      turnRight();
    } else {
      Serial.println("Turning Left...");
      turnLeft();
    }

    moveStop();
  } else {
    Serial.println("Path is clear. Moving Forward...");
    moveForward();
  }

  distance = readPing();
  Serial.print("Measured Distance: ");
  Serial.println(distance);
}

int lookRight() {
  Serial.println("Looking Right...");
  myservo.write(50); 
  delay(500);
  int distance = readPing();
  delay(100);
  myservo.write(115); 
  Serial.println("Servo centered after looking right");
  return distance;
}

int lookLeft() {
  Serial.println("Looking Left...");
  myservo.write(170); 
  delay(500);
  int distance = readPing();
  delay(100);
  myservo.write(115); 
  Serial.println("Servo centered after looking left");
  delay(100);
  return distance;
}

int readPing() { 
  delay(70);
  int cm = sonar.ping_cm();
  if (cm == 0) {
    cm = 250;
    Serial.println("Ping timeout. Returning 250 cm");
  }
  return cm;
}

void moveStop() {
  Serial.println("Stopping all motors");
  motor1.run(RELEASE); 
  motor2.run(RELEASE);
  motor3.run(RELEASE);
  motor4.run(RELEASE);
} 
  
void moveForward() {
  if (!goesForward) {
    goesForward = true;
    Serial.println("Moving Forward...");
    motor1.run(FORWARD);      
    motor2.run(FORWARD);
    motor3.run(FORWARD); 
    motor4.run(FORWARD);     
    for (speedSet = 0; speedSet < MAX_SPEED; speedSet += 2) {
      motor1.setSpeed(speedSet);
      motor2.setSpeed(speedSet);
      motor3.setSpeed(speedSet);
      motor4.setSpeed(speedSet);
      delay(5);
    }
  }
}

void moveBackward() {
  goesForward = false;
  Serial.println("Moving Backward...");
  motor1.run(BACKWARD);      
  motor2.run(BACKWARD);
  motor3.run(BACKWARD);
  motor4.run(BACKWARD);  
  for (speedSet = 0; speedSet < MAX_SPEED; speedSet += 2) {
    motor1.setSpeed(speedSet);
    motor2.setSpeed(speedSet);
    motor3.setSpeed(speedSet);
    motor4.setSpeed(speedSet);
    delay(5);
  }
}  

void turnRight() {
  Serial.println("Turning Right...");
  motor1.run(FORWARD);
  motor2.run(FORWARD);
  motor3.run(BACKWARD);
  motor4.run(BACKWARD);     
  delay(500);
  motor1.run(FORWARD);      
  motor2.run(FORWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);      
} 
 
void turnLeft() {
  Serial.println("Turning Left...");
  motor1.run(BACKWARD);     
  motor2.run(BACKWARD);  
  motor3.run(FORWARD);
  motor4.run(FORWARD);   
  delay(500);
  motor1.run(FORWARD);     
  motor2.run(FORWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);
}  
