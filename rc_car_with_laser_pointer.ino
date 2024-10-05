#include "BluetoothSerial.h"
#include <Arduino.h>
#include <ESP32Servo.h>

BluetoothSerial serialBT;

char btSignal;
int Speed = 1000;  
int flag = 1;      // flag to  mode change flag car to pointer

#define R 0  
#define L 1  


int enA = 5;
int enB = 23;
int IN1 = 22;
int IN2 = 21;
int IN3 = 19;
int IN4 = 18;
int Laser = 14;


Servo servo1;
Servo servo2;
int Servo_1 = 13;
int Servo_2 = 4;

int angle1 = 0;      
int angle2 = 0;      
int angleStep = 3;  

void setup() {
  Serial.begin(115200);

  serialBT.begin("Harshwardhan's RC car");

  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);

  ledcSetup(R, 5000, 8);  // Setup PWM channels for servo
  ledcAttachPin(enA, R);
  ledcSetup(L, 5000, 8);
  ledcAttachPin(enB, L);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(Laser, OUTPUT);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);

  servo1.attach(Servo_1);
  servo2.attach(Servo_2);
  servo1.write(angle1);
  servo2.write(angle2);
}

void loop() {

  while (serialBT.available()) {
    btSignal = serialBT.read();  //read bluetooth signal
    Serial.println(btSignal);    //print signal on serial mointor

    // mode change
    if (btSignal == 'q') {
      modechange();
    }

    // on laser light
    if (btSignal == '0') {
      LaserLight();
    }

    // car control
    if (flag == 1) {
      if (btSignal == 'B') {
        backward();
      } else if (btSignal == 'F') {
        forward();
      } else if (btSignal == 'L') {
        left();
      } else if (btSignal == 'R') {
        right();
      } else if (btSignal == 'S') {
        stop();
      }
    }

    // servo control
    else {
      if (btSignal == 'B') {
        rightservo2();
      } else if (btSignal == 'F') {
        leftservo2();
      } else if (btSignal == 'L') {
        leftservo1();
      } else if (btSignal == 'R') {
        rightservo1();
      } else if (btSignal == 'S') {
        stop();
      }
    }
  }
}


//car backwards
void backward() {
  ledcWrite(R, Speed);
  ledcWrite(L, Speed);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

//car forward
void forward() {
  ledcWrite(R, Speed);
  ledcWrite(L, Speed);

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

//car left
void left() {
  ledcWrite(R, Speed);
  ledcWrite(L, Speed);

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

//car right
void right() {
  ledcWrite(R, Speed);
  ledcWrite(L, Speed);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

//car stop
void stop() {
  ledcWrite(R, Speed);
  ledcWrite(L, Speed);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

//servo1 left
void leftservo1() {
  angle1 = angle1 + angleStep;
  if (angle1 > 180) angle1 = 180;

  servo1.write(angle1);
}

//servo1 right
void rightservo1() {
  angle1 = angle1 - angleStep;
  if (angle1 < 0) angle1 = 0;

  servo1.write(angle1);
}

//servo2 left
void leftservo2() {
  angle2 = angle2 + angleStep;
  if (angle2 > 180) angle2 = 180;

  servo2.write(angle2);
}

//servo2 right
void rightservo2() {
  angle2 = angle2 - angleStep;
  if (angle2 < 0) angle2 = 0;
  servo2.write(angle2);
}

void modechange() {
  if (flag == 1)
    flag = 0;
  else
    flag = 1;
}

//Laser light
void LaserLight() {
  int val = digitalRead(Laser);
  if (HIGH == val) {
    val = LOW;
  } else {
    val = HIGH;
  }
  digitalWrite(Laser, val);
}
