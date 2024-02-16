#include "xArmServoController.h"

xArmServoController arm(xArm, Serial1);

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
}

void loop() {
  //xArmServo wrapper[1] = {{axis, position}};
  //arm->setPosition(wrapper, 1, 1000, true);
  //#define X_ID 2
  //increasing r goes counterclockwise
  //450 seems to be center
  //#define R_ID 4
  //#define Y_ID 5
  //(servo, position, duration, wait)

  //y:
  //  higher numbers move up out of the table

  return;

  arm.setPosition(5, 0, 1500, true);
  delay(2500);
  arm.setPosition(5, 500, 1500, true);
  delay(2500);
  arm.setPosition(5, 1000, 1500, true);
  delay(2500);
  

  //x
  //  higher numbers move away from 12'o'clock

  arm.setPosition(2, 0, 1500, true);
  delay(2500);
  arm.setPosition(2, 500, 1500, true);
  delay(2500);
  arm.setPosition(2, 1000, 1500, true);
  delay(2500);



  //r:
  //  higher values counterclockwise
  //  450 is 12'o'clock
  //  825 is 9'o'clock
  arm.setPosition(4, 450, 1500, true);
  delay(2000);
  arm.setPosition(4, 950, 1350, true);
  delay(2000);

  return;
  arm.setPosition(2, 500, 1500, true);
  arm.setPosition(4, 500, 1500, true);
  arm.setPosition(5, 500, 1500, true);
  delay(2500);
  arm.setPosition(2, 750, 1500, true);
  arm.setPosition(4, 750, 1500, true);
  arm.setPosition(5, 750, 1500, true);
  delay(2500);
  arm.setPosition(2, 100, 5000, true);
  arm.setPosition(4, 100, 5000, true);
  arm.setPosition(5, 100, 5000, true);
}
