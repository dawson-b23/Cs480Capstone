#include <xArmServoController.h>
#include <controlWrappers.h>
#include "SoftwareSerial.h"

#include "TinyGPS++.h"

//ControlWrapper controls(xArm, Serial1);

TinyGPSPlus gpsOne;
//(rx, tx)
SoftwareSerial digSerialOne(8, 9);

TinyGPSPlus gpsTwo;
SoftwareSerial digSerialTwo(13, 12);



void setup() {
    digSerialOne.begin(9600);
    digSerialTwo.begin(9600);
    //Serial1.begin(9600);
    Serial.begin(9600);
    Serial.println("Starting: ");
    pinMode(LED_BUILTIN, OUTPUT);
    delay(3000);
}

int cycle = 0;
int cycles = 10;
char charRead;
int phase = 0;
void loop() {
    if(phase) {
        Serial.println("First one:");
        
        while(digSerialOne.available()) {
            charRead = digSerialOne.read();
            Serial.print(charRead);
            gpsOne.encode(charRead);
        }
       
    } else {
        
        Serial.println("Second one:");
        
        while(digSerialTwo.available()) {
            charRead = digSerialTwo.read();
            Serial.print(charRead);
            gpsOne.encode(charRead);
        }
        
    }
    Serial.println();
    phase++;
    phase = phase % 2;
    delay(5000);
    return;
    /*
    Serial.println();
    Serial.print("RAWLAT="); Serial.println(gps.location.rawLat().deg);
    if(gps.location.isValid()) {
        Serial.print("LAT="); Serial.println(gps.location.lat(), 6);
        Serial.print("LNG="); Serial.println(gps.location.lng(), 6);
    }
    */


    return;
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