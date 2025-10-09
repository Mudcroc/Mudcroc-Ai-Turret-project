#include "HUSKYLENS.h"
#include "SoftwareSerial.h"
#include "Servo.h"


HUSKYLENS huskylens;
SoftwareSerial mySerial(7, 9); // RX, TX (Arduino side)

int ledFiring = 8;

Servo trackerServo;

#define SERVO_PIN 10
#define SCREEN_CENTER 160  // HuskyLens X center (320 / 2)
#define SCREEN_WIDTH 320

int servoPos = 90;          // start facing center (0–180° range)
int deadZone = 10;

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
  trackerServo.attach(SERVO_PIN);
  trackerServo.write(servoPos);
  pinMode(ledFiring, OUTPUT);
  if (!huskylens.begin(mySerial)) {
    Serial.println("HuskyLens not connected! Check wiring & protocol.");
    while (1);
  }
  Serial.println("HuskyLens ready.");
}

void loop() {

  if (!huskylens.request()) {
    Serial.println("No data from HuskyLens");
    return;
  }

  for (int i = 0; i < huskylens.count(); i++) {
    HUSKYLENSResult result = huskylens.get(i);

    int distanceFromCenter = result.xCenter - SCREEN_CENTER;
    
    int brightness = map(abs(distanceFromCenter), 0, SCREEN_CENTER, 0, 255);
    brightness = constrain(brightness, 0, 255);

    Serial.print("Detected ID: ");
    Serial.println(result.ID);

    if (result.ID == 1) {
      huskylens.customText("Jet", result.xCenter, result.yCenter - 20);
    }
    else if (result.ID == 2) {
      huskylens.customText("Mars <3", result.xCenter, result.yCenter - 20);
    }
    else if (result.ID == 3) {
      huskylens.customText("Pippa", result.xCenter, result.yCenter - 20);
    }
    else if (result.ID == 4) {
      huskylens.customText("Paige", result.xCenter, result.yCenter - 20);
    }
     else if (result.ID == 5) {
      huskylens.customText("Alex", result.xCenter, result.yCenter - 20);
    }
     else {
      huskylens.customText("Unknown", result.xCenter, result.yCenter - 20);
    }
    int targetPos = map(result.xCenter, 0, SCREEN_WIDTH, 170, 10);
    servoPos = servoPos + (targetPos - servoPos) / 5;  // move 1/5 of the difference each loop
    trackerServo.write(servoPos);

  Serial.print("Offset: ");
  Serial.print(distanceFromCenter);
  Serial.print(" | Servo angle: ");
  Serial.println(servoPos);
   if (result.ID != 1){
      digitalWrite(8, HIGH);
      Serial.println("spining firing motors");
    }
    else {
    Serial.println("not firing");
    digitalWrite(8, LOW);
    }
}

    delay(50); // small delay for stability
   
  }
  
