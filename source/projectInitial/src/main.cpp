#include <xArmServoController.h>
#include <controlWrappers.h>

//ControlWrapper controls(xArm, Serial1);



void setup() {
    Serial1.begin(9600);
    pinMode(LED_BUILTIN, OUTPUT);
}

int cycle = 0;
int cycles = 10;
void loop() {
    //return;
    static ControlWrapper controls(xArm, Serial1);
    return;

    
    controls.moveR(50 + cycle * 100);
    delay(250);
    controls.moveX(50 + cycle * 100);
    delay(250);
    controls.moveY(50 + cycle * 100);
    delay(250);
    

    //move places
    cycle++;
    cycle = cycle % cycles;

}