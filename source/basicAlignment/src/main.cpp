#include <Arduino.h>
#include <SoftwareSerial.h>
#include "TinyGPS++.h"
#include "xArmServoController.h"

TinyGPSPlus laserGPS;
//(rx, tx)
SoftwareSerial laserSerial(13, 12);

TinyGPSPlus robotGPS;
SoftwareSerial robotSerial(8, 9);

xArmServoController arm(xArm, Serial1);

int phase;
void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  laserSerial.begin(9600);
  phase = 0;
}

void loop() {
  static char charRead;
  if(phase == 0) {
    Serial.println("Phase 0");
    while(laserSerial.available()) {
      charRead = laserSerial.read();
      laserGPS.encode(charRead);
    }
    if(laserGPS.location.isValid()) {
      phase++;
      laserSerial.end();
      robotSerial.begin(9600);
      Serial.println("Laser location obtained: ");
      Serial.print("Lat: ");
      Serial.println(laserGPS.location.lat());
      Serial.print("Lng: ");
      Serial.println(laserGPS.location.lng());
    }
    delay(1500);
  }

  if(phase == 1) {
    Serial.println("Phase 1");
    while(robotSerial.available()) {
      charRead = robotSerial.read();
      robotGPS.encode(charRead);
    }
    if(robotGPS.location.isValid()) {
      phase++;
      robotSerial.end();
      Serial.println("Robot location obtained: ");
      Serial.print("Lat: ");
      Serial.println(robotGPS.location.lat());
      Serial.print("Lng: ");
      Serial.println(robotGPS.location.lng());
    }
    delay(1500);
  }

  if(phase == 2) {
    Serial.println("Phase 2");
    arm.setPosition(5, 0, 1500, true);
    delay(2500);
    arm.setPosition(5, 100, 1500, true);
    arm.setPosition(5, 200, 1500, true);
    arm.setPosition(5, 300, 1500, true);
    arm.setPosition(5, 400, 1500, true);
    arm.setPosition(5, 500, 1500, true);
    delay(2500);
    arm.setPosition(5, 600, 1500, true);
    arm.setPosition(5, 700, 1500, true);
    arm.setPosition(5, 800, 1500, true);
    arm.setPosition(5, 900, 1500, true);
    arm.setPosition(5, 1000, 1500, true);
    delay(2500);
  }
}
