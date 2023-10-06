#include <Arduino.h>
#include <xArmServoController.h>

xArmServoController arm = xArmServoController(xArm, Serial1);

#define NUM_SAMPLES 10

#define STEP_SIZE 10
#define SERVO_SPEED 1.0

#define XY_MIN 250
#define XY_MID 625
#define XY_MAX 1000
#define R_MIN 0
#define R_MID 150
#define R_MAX 300

#define VOLT_THRESH 0.7

#define SCAN_SPACE 3

xArmServo home[] = {{2, XY_MID}, {4, R_MID}, {5, XY_MID}};
xArmServo bot_right[] = {{2, XY_MIN}, {4, R_MID}, {5, XY_MIN}};
xArmServo bot_left[] = {{2, XY_MAX}, {4, R_MID}, {5, XY_MIN}};
xArmServo top_left[] = {{2, XY_MAX}, {4, R_MID}, {5, XY_MAX}};
xArmServo top_right[] = {{2, XY_MIN}, {4, R_MID}, {5, XY_MAX}};

enum Direction {UP, DOWN, LEFT, RIGHT};

xArmServo currPos[] = {{2, 0}, {4, 0}, {5, 0}};

xArmServo vMaxPos[] = {{2, 0}, {4, 0}, {5, 0}};
float vMax = 0;

bool setPosition(xArmServo * pos) {
  if ( pos[0].position < XY_MIN
    || pos[0].position > XY_MAX
    || pos[1].position < R_MIN
    || pos[1].position > R_MAX
    || pos[2].position < XY_MIN
    || pos[2].position > XY_MAX ) return false;

  // Serial.print("pos: x = ");
  // Serial.print(pos[0].position);
  // Serial.print(", y = ");
  // Serial.println(pos[2].position);

  int dist = max(abs(pos[0].position - currPos[0].position),
             max(abs(pos[1].position - currPos[1].position),
                 abs(pos[2].position - currPos[2].position)));

  Serial.println();
  arm.setPosition(pos, 3, abs((float)dist / SERVO_SPEED), true);
  return true;
}

bool checkThresh() {
  float currVolt = 0;
  for (int i = 0; i < NUM_SAMPLES; i++) {
    currVolt += analogRead(A4);
    delay(10);
  }
  currVolt = (currVolt / (float)NUM_SAMPLES * 5.015) / 1024.0;
  Serial.println(currVolt);

  if (currVolt > vMax) {
    vMax = currVolt;
    arm.getPosition(vMaxPos, 3);
    Serial.print("New vMax of ");
    Serial.print(vMax);
    Serial.print(" found at ");
    Serial.print(vMaxPos[0].position);
    Serial.print(", ");
    Serial.println(vMaxPos[1].position);
  }
  return currVolt > VOLT_THRESH;
}

bool spiralScan() {
  enum Direction dir = UP;
  int legSize = SCAN_SPACE * STEP_SIZE;
  int strideSize = SCAN_SPACE * STEP_SIZE;

  while (1) {
    for (int i = 0; i < legSize / strideSize; i++) {
      if (checkThresh()) return true;

      arm.getPosition(currPos, 3);
      switch (dir) {
        case UP:
          currPos[2].position += strideSize;
          break;
        case DOWN:
          currPos[2].position -= strideSize;
          break;
        case LEFT:
          currPos[0].position += strideSize;
          break;
        case RIGHT:
          currPos[0].position -= strideSize;
          break;
        default: Serial.println("Switch case defaulted. Strange!");
      }
      if (!setPosition(currPos)) {
        arm.getPosition(currPos, 3);
        return false;
      }
    }
    
    switch (dir) {
      case UP:
        dir = RIGHT;
        break;
      case RIGHT:
        dir = DOWN;
        legSize += SCAN_SPACE * STEP_SIZE;
        break;
      case DOWN:
        dir = LEFT;
        break;
      case LEFT:
        dir = UP;
        legSize += SCAN_SPACE * STEP_SIZE;
        break;
    }
  }
}

void yScan() {

  // Store starting position
  arm.getPosition(currPos, 3);
  xArmServo startPos[3];
  for (int i = 0; i < 3; i++) {
    startPos[i].servo_id = currPos[i].servo_id;
    startPos[i].position = currPos[i].position;
  }

  // UP until lost
  do {
    currPos[2].position += STEP_SIZE;
    if (!setPosition(currPos)) break;
  } while (checkThresh());

  // Back to start
  for (int i = 0; i < 3; i++) {
    currPos[i].servo_id = startPos[i].servo_id;
    currPos[i].position = startPos[i].position;
  }
  setPosition(currPos);

  // DOWN until lost
  do {
    currPos[2].position -= STEP_SIZE;
    if (!setPosition(currPos)) break;
  } while (checkThresh());

  // Go to opimal Y position
  for (int i = 0; i < 3; i++) {
    currPos[i].servo_id = vMaxPos[i].servo_id;
    currPos[i].position = vMaxPos[i].position;
  }
  setPosition(currPos);
}

void xScan() {
  
  // Store starting position
  arm.getPosition(currPos, 3);
  xArmServo startPos[3];
  for (int i = 0; i < 3; i++) {
    startPos[i].servo_id = currPos[i].servo_id;
    startPos[i].position = currPos[i].position;
  }

  // LEFT until lost
  do {
    currPos[0].position += STEP_SIZE;
    if (!setPosition(currPos)) break;
  } while (checkThresh());

  // Move back to optimal Y position
  for (int i = 0; i < 3; i++) {
    currPos[i].servo_id = startPos[i].servo_id;
    currPos[i].position = startPos[i].position;
  }
  setPosition(currPos);
  
  // RIGHT
  do {
    currPos[0].position -= STEP_SIZE;
    if (!setPosition(currPos)) break;
  } while (checkThresh());

  // Move to final best pos
  for (int i = 0; i < 3; i++) {
    currPos[i].servo_id = vMaxPos[i].servo_id;
    currPos[i].position = vMaxPos[i].position;
  }
  setPosition(currPos);
}

void rScan() {

  // Store starting position
  arm.getPosition(currPos, 3);
  xArmServo startPos[3];
  for (int i = 0; i < 3; i++) {
    startPos[i].servo_id = currPos[i].servo_id;
    startPos[i].position = currPos[i].position;
  }

  // LEFT until lost
  do {
    currPos[1].position += STEP_SIZE;
    if (!setPosition(currPos)) break;
  } while (checkThresh());

  // Move back to start position
  for (int i = 0; i < 3; i++) {
    currPos[i].servo_id = startPos[i].servo_id;
    currPos[i].position = startPos[i].position;
  }
  setPosition(currPos);
  
  // RIGHT
  do {
    currPos[1].position -= STEP_SIZE;
    if (!setPosition(currPos)) break;
  } while (checkThresh());

  // Move to final best pos
  for (int i = 0; i < 3; i++) {
    currPos[i].servo_id = vMaxPos[i].servo_id;
    currPos[i].position = vMaxPos[i].position;
  }
  setPosition(currPos);

}

void setup() {
  // Initialization
  Serial1.begin(9600);  // Hiwonder xarm serial communication
  Serial.begin(9600);   // USB serial monitoring
  delay(500);

  setPosition(home);
  delay(1000);
  arm.beep();

  if (spiralScan()) {
    Serial.println("Spiral scan complete.");
    delay(1000);

    yScan();
    Serial.println("yScan complete.");
    delay(1000);

    xScan();
    Serial.println("xScan complete.");
    delay(1000);

    rScan();
    Serial.println("rScan complete.");
  } else {
    Serial.println("Spiral scan failed!");
  }
  arm.beep();
}

void loop() {} // The main loop of the algorithm is in spiralScan()
