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
#define SMALLSTEP (5)
//search whole axis
#define MEDRADIUS (1000)
//search only smaller band after
#define SMALLRADIUS (250)

#define TIME_RATIO (5)

//midpoints:
//500
//450
//500

xArmServoController arm(xArm, Serial1);

int positions[6] = {0, 0, 0, 0, 0, 0};

void initialPosition();

void optimizeAxis(int axis, int stepSize, int radius);

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
  //optimizeAxis(X_ID, SMALLSTEP);
  //optimizeAxis(Y_ID, SMALLSTEP);

  optimizeAxis(R_ID, MEDSTEP, MEDRADIUS);
  //optimizeAxis(X_ID, MEDSTEP, MEDRADIUS);
  //optimizeAxis(Y_ID, MEDSTEP, MEDRADIUS);

  Serial.println("--------------------");
  Serial.println("Swapping to low gain");
  Serial.println("--------------------");

  lightSensor.setGain(TSL2591_GAIN_LOW);
  int radius = SMALLRADIUS;
  while(lightSensor.getLuminosity(TSL2591_VISIBLE) < 2000) {
    optimizeAxis(X_ID, SMALLSTEP, radius);
    optimizeAxis(Y_ID, SMALLSTEP, radius);
    optimizeAxis(R_ID, SMALLSTEP, radius);
    radius -= SMALLSTEP;
  }
  Serial.println("-----------------------");
  Serial.println("-----------------------");
  Serial.println("-----------------------");
  Serial.println("Final position reached:");
  Serial.print("   R: ");
  Serial.println(positions[R_ID]);
  Serial.print("   X: ");
  Serial.println(positions[X_ID]);
  Serial.print("   Y: ");
  Serial.println(positions[Y_ID]);
  Serial.print("Luminosity:");
  Serial.println(lightSensor.getLuminosity(TSL2591_VISIBLE));
  while(1) { delay(5); }
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

void optimizeAxis(int axis, int stepSize, int radius) {
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
  } while(positions[axis] > (initialPosition - radius) && positions[axis] > stepSize);

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
  } while(positions[axis] < (initialPosition + radius) && positions[axis] < (1000 - stepSize));
  Serial.print("Finished increasing axis to ");
  Serial.print(positions[axis]);
  Serial.print(", returning to brightest position of ");
  Serial.println(brightestPosition);

  positions[axis] = brightestPosition;
  arm.setPosition(axis, positions[axis], 1500, true);
}