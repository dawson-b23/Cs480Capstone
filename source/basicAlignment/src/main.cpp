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

double rbtLat;
double rbtLng;
double lsrLat;
double lsrLng;

int included = 0;
double deltaLat = 0;
double deltaLng = 0;

void loop() {
  static char charRead;
  if(phase == 0) {
    //Serial.println("Phase 0");
    while(laserSerial.available()) {
      charRead = laserSerial.read();
      //Serial.print(charRead);
      laserGPS.encode(charRead);
    }
    if(laserGPS.location.isUpdated()) {
      phase++;
      laserSerial.end();
      robotSerial.begin(9600);
      Serial.print("Laser location obtained, HDOP ");
      Serial.print(laserGPS.hdop.hdop(), 20);
      Serial.println(": ");
      lsrLat = laserGPS.location.lat();
      lsrLng = laserGPS.location.lng();
      Serial.print("Lat: ");
      Serial.println(lsrLat, 20);
      Serial.print("Lng: ");
      Serial.println(lsrLng, 20);
    }
    delay(1);
  }

  if(phase == 1) {
    //Serial.println("Phase 1");
    while(robotSerial.available()) {
      charRead = robotSerial.read();
      //Serial.print(charRead);
      robotGPS.encode(charRead);
    }
    if(robotGPS.location.isUpdated()) {
      phase = 0;
      robotSerial.end();
      laserSerial.begin(9600);
      Serial.print("Robot location obtained, HDOP ");
      Serial.print(robotGPS.hdop.hdop(), 20);
      Serial.println(": ");
      rbtLat = robotGPS.location.lat();
      rbtLng = robotGPS.location.lng();
      Serial.print("Lat: ");
      Serial.println(rbtLat, 20);
      Serial.print("Lng: ");
      Serial.println(rbtLng, 20);

      Serial.print("Distance between modules: ");
      Serial.println(robotGPS.distanceBetween(lsrLat, lsrLng, rbtLat, rbtLng), 30);

      if(included) {
        included++;
        deltaLat = deltaLat * ((included - 1) / (double) included);
        deltaLat += (rbtLat - lsrLat) * (1.0 / included);
        deltaLng = deltaLng * ((included - 1) / (double) included);
        deltaLng += (rbtLng - lsrLng) * (1.0 / included);
      } else {
        included++;
        deltaLat = rbtLat - lsrLat;
        deltaLng = rbtLng - lsrLng;
      }
      Serial.print("From ");
      Serial.print(included);
      Serial.println(" samples, average displacement is: ");
      Serial.print("Lat: ");
      Serial.println(deltaLat, 20);
      Serial.print("Lng: ");
      Serial.println(deltaLng, 20);

      Serial.println();
    }
    delay(1);
  }

  if(phase == 2) {
    return;
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
