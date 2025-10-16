#include "HUSKYLENS.h"
#include "SoftwareSerial.h"
#include "Servo.h"
//importing diffrent code libarys so diffrent parts of the program like serial com, motor movement, and husky communictation are all possible

HUSKYLENS huskylens;
SoftwareSerial mySerial(7, 9); // RX, TX (Arduino side)
//this line tells the arduino what pins the huskylens will comuncate information to 

int ledFiring = 8;
//tells the arduino what pin the led is plugged into, and gives that pin a name that it can be refered to as.

Servo trackerServo;
#define servoPinX 10

Servo tiltServo;
#define servoPinY 11

//
#define screenCenterWidth 160
//this line defines the 
#define SCREEN_WIDTH 320

#define screenHeight 240

#define screenCenterHeight 120

int servoPos1 = 90;
int servoPos2 = 20;
int deadZone = 10;
int targetPosX;
int targetPosY;

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
  trackerServo.attach(servoPinX);
  trackerServo.write(servoPos1);
  tiltServo.attach(servoPinY);
  tiltServo.write(servoPos2);
  pinMode(ledFiring, OUTPUT);
  if (!huskylens.begin(mySerial)) {
    Serial.println("HuskyLens not connected");
    while (1);
  }
  Serial.println("HuskyLens ready");
}

void loop() {

  if (!huskylens.request()) {
    Serial.println("No data from HuskyLens");
    return;
  }

  for (int i = 0; i < huskylens.count(); i++) {
    HUSKYLENSResult result = huskylens.get(i);

    int distanceFromCenterY = result.yCenter - screenCenterHeight;

    int distanceFromCenterX = result.xCenter - screenCenterWidth;
    
    int positonX = map(abs(distanceFromCenterX), 0, screenCenterWidth, 0, 255);
    positonX = constrain(positonX, 0, 255);

    int positonY = map(abs(distanceFromCenterY), 0, screenCenterHeight, 160, 0);
    positonY = constrain(positonY, 0, 160);

    targetPosX = constrain(targetPosX, 10, 170);
    targetPosY = constrain(targetPosY, 30, 150);


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
    targetPosX = map(result.xCenter, 0, SCREEN_WIDTH, 170, 10);
    servoPos1 = servoPos1 + (targetPosX - servoPos1) / 5;  // move 1/5 of the difference each loop
    trackerServo.write(servoPos1);
    targetPosY = map(result.yCenter, 0, screenHeight, 30, 150); // adjust angles for your setup
    servoPos2 = servoPos2 + (targetPosY - servoPos2) / 5;
    tiltServo.write(servoPos2);

  Serial.print("Offset: ");
  Serial.print(distanceFromCenterX);
  Serial.print(" | Servo angleX: ");
  Serial.println(servoPos1);
 
  Serial.print("Offset: ");
  Serial.print(distanceFromCenterY);
  Serial.print(" | Servo angleY: ");
  Serial.println(servoPos2);
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
  