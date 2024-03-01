#include <Arduino.h>
#include "xArmServoController.h"

#include "Adafruit_TSL2591.h"

//#define SENSOR_VALUE (analogRead(A4))

Adafruit_TSL2591 lightSensor(2);

#define SENSOR_VALUE (lightSensor.getLuminosity(TSL2591_VISIBLE))

//step sizes:
//100
//10-50
//5

//midpoints:
//500
//450
//500

//#define X_ID 2
//#define R_ID 4
//#define Y_ID 5

void initialSearch();

xArmServoController arm(xArm, Serial1);

void setup() {
  Serial1.begin(9600);
  Serial.begin(9600);

  lightSensor.begin();
  lightSensor.setGain(TSL2591_GAIN_MED);
  lightSensor.enable();
}

void loop() {
  initialSearch();
  delay(5000);
}

void initialSearch() {
  uint16_t brightest = 0;
  int brightestPosition;
  arm.setPosition(4, 975, 1500, true);
  for(int i = 975; i > 25; i -= 50) {
    arm.setPosition(4, i, 250, true);
    uint16_t measured = lightSensor.getLuminosity(TSL2591_VISIBLE);
    Serial.print("Measured: ");
    Serial.println(measured);
    if(measured > brightest) {
      brightest = measured;
      brightestPosition = i;
    }
  }
  Serial.print("Position: ");
  Serial.println(brightestPosition);
  Serial.print("Value: ");
  Serial.println(brightest);
  arm.setPosition(4, brightestPosition, 1500, true);
}
