#ifndef controlWrappers_h
#define controlWrappers_h

#define X_ID 2
#define R_ID 4
#define Y_ID 5

#include "xArmServoController.h"

class ControlWrapper {
    public:
        ControlWrapper(enum xArmMode mode, UartClass port);
        ~ControlWrapper();
        void moveX(unsigned int position);
        void moveY(unsigned int position);
        void moveR(unsigned int position);
        xArmServoController *arm;
    private:
        void moveAxis(unsigned int axis, unsigned int position);
};

#endif