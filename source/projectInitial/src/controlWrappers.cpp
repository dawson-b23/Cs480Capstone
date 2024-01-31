#include "controlWrappers.h"
#include "xArmServoController.h"

ControlWrapper::ControlWrapper(enum xArmMode mode, UartClass port) {
//xArmServoController *arm = new xArmServoController(xArm, Serial1);

arm = new xArmServoController(mode, Serial1);

xArmServo home[] = {{1, 500},
                    {2, 500},
                    {3, 500},
                    {4, 500},
                    {5, 500},
                    {6, 500}};
arm->setPosition(home, 6, 3000, true);

 xArmServo left[] = {{2,300}};
 xArmServo right[] = {{2,700}};
 xArmServo up1[] = {{5,700}};
 xArmServo up2[] = {{5,650}};
    
arm->setPosition (up1, 1, 1000, true),
delay (1000);
arm->setPosition (left, 1, 3000, true),
delay (1000);
arm->setPosition (up2, 1, 500, true),
delay (1000);
arm->setPosition (right, 1, 3000, true),
delay (1000); 
}

void ControlWrapper::moveX(unsigned int position) {
    moveAxis(X_ID, position);
}

void ControlWrapper::moveY(unsigned int position) {
    moveAxis(Y_ID, position);
}

void ControlWrapper::moveR(unsigned int position) {
    moveAxis(R_ID, position);
}

void ControlWrapper::moveAxis(unsigned int axis, unsigned int position) {
    //servo_id, position, duration, wait
    //arm->setPosition(axis, position, 1000, true);

    //(xArmServo servos[], int count, unsigned duration = 1000, bool wait = false);
    xArmServo wrapper[1] = {{axis, position}};
    arm->setPosition(wrapper, 1, 1000, true);
}

ControlWrapper::~ControlWrapper() {
    free(arm);
}