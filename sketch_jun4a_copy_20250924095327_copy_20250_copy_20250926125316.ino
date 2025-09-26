#include "HUSKYLENS.h"
#include "SoftwareSerial.h"

HUSKYLENS huskylens;
SoftwareSerial mySerial(7, 9); // RX, TX (Arduino side)

int led = 8;

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
  pinMode(led, OUTPUT);


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
     if(result.ID == 1) {
      digitalWrite(8, HIGH);
      Serial.println("led on");
      delay(1000);
      digitalWrite(8, LOW);
     }
    else{
      digitalWrite(8, LOW);
      Serial.println("led off");
    }
  }
}
