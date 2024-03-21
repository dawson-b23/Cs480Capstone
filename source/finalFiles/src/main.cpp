#include <Arduino.h>
#include "xArmServoController.h"

#include "Adafruit_TSL2591.h"

#define SENSOR_VALUE (analogRead(A4))

Adafruit_TSL2591 lightSensor(2);

#define SENSOR_VALUE (lightSensor.getLuminosity(TSL2591_VISIBLE))

#define X_ID (2)
#define R_ID (4)
#define Y_ID (5)

#define MEDSTEP (10)
#define SMALLSTEP (2)

#define TIME_RATIO (5)

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

int positions[6] = {0, 0, 0, 0, 0, 0};

void initialPosition();

void optimizeAxis(int axis, int stepSize);

void setup() {
  Serial1.begin(9600);
  Serial.begin(9600);

  lightSensor.begin();
  lightSensor.setGain(TSL2591_GAIN_MED);
  lightSensor.enable();
  //delay(5000);

  Serial.println("Alignment program beginning.");
}

void loop() {
  initialPosition();
  //initialSearch();
  //delay(3500);
  //return;
  optimizeAxis(R_ID, MEDSTEP);
  optimizeAxis(X_ID, MEDSTEP);
  optimizeAxis(Y_ID, MEDSTEP);
  optimizeAxis(X_ID, SMALLSTEP);
  optimizeAxis(Y_ID, SMALLSTEP);
  delay(3500);
}

void initialPosition() {
  positions[X_ID] = 500;
  positions[R_ID] = 500;
  positions[Y_ID] = 500;

  Serial.println("Initializing positions.");

  arm.setPosition(X_ID, positions[X_ID], 1500, true);
  arm.setPosition(R_ID, positions[R_ID], 1500, true);
  arm.setPosition(Y_ID, positions[Y_ID], 1500, true);
}

void initialSearch() {
  uint16_t brightest = 0;
  int brightestPosition;
  for(int i = 975; i > MEDSTEP; i -= MEDSTEP) {
    positions[R_ID] = i;
    arm.setPosition(R_ID, positions[R_ID], (MEDSTEP * TIME_RATIO), true);
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
  positions[R_ID] = brightestPosition;
  arm.setPosition(R_ID, positions[R_ID], 1500, true);
}

void optimizeAxis(int axis, int stepSize) {
  //make positions[axis] small, until it's dark
  //make positions[axis] big, until it's dark
  //along the way, store the brightest position found
  //at the end, return to that position

  int initialPosition = positions[axis];
  int brightestPosition = initialPosition;
  uint16_t brightest, measured, initialLight;
  measured = lightSensor.getLuminosity(TSL2591_VISIBLE);
  //measured = SENSOR_VALUE;
  initialLight = measured;
  brightest = measured;
  Serial.print("Initial light: ");
  Serial.println(initialLight);

  Serial.print("Optimize axis called, axis: ");
  Serial.println(axis);

  do {
    positions[axis] = positions[axis] - stepSize;
    arm.setPosition(axis, positions[axis], (stepSize * TIME_RATIO), true);
    //measured = SENSOR_VALUE;
    measured = lightSensor.getLuminosity(TSL2591_VISIBLE);
    Serial.print("   Measured luminosity ");
    Serial.println(measured);
    if(measured > brightest) {
      brightestPosition = positions[axis];
      brightest = measured;
    }
  } while(measured > (initialLight / 4) && positions[axis] > stepSize);

  Serial.print("Finished decreasing axis to ");
  Serial.print(positions[axis]);
  Serial.print(", returning to initial position of ");
  Serial.println(initialPosition);
  arm.setPosition(axis, initialPosition, 1500, true);
  positions[axis] = initialPosition;
  do {
    positions[axis] = positions[axis] + stepSize;
    arm.setPosition(axis, positions[axis], (stepSize * TIME_RATIO), true);
    measured = lightSensor.getLuminosity(TSL2591_VISIBLE);
    //measured = SENSOR_VALUE;
    Serial.print("   Measured luminosity ");
    Serial.println(measured);
    if(measured > brightest) {
      brightestPosition = positions[axis];
      brightest = measured;
    }
  } while(measured > (initialLight / 4) && positions[axis] < (1000 - stepSize));
  Serial.print("Finished increasing axis to ");
  Serial.print(positions[axis]);
  Serial.print(", returning to brightest position of ");
  Serial.println(brightestPosition);

  positions[axis] = brightestPosition;
  arm.setPosition(axis, positions[axis], 1500, true);
}