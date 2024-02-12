//experiment at receiving gps data from two modules via alternation of active software serial ports
//technically works, but sometimes grabs nonsense data and frequently has incomplete sentences
//this may be phase effects
//  transmissions from module to board seem to happen on a regular schedule
//  the alternation here is happening on a regular schedule
//  those schedules may align/misalign in weird ways
//  might be opening/closing ports in the middle of a module->board transfer, fouling the data
//  attempting to align these schedules via timing delays alone is not technically sound.
//a more advanced system which actually parses the sentences the module sends could work
//don't just alternate blindly between modules -- stick with one until a location is actually parsed
//the basic approach of using multiple software serial ports seems flawed
//  data will always be lost since there is a module dumping sentences into an inactive port
//  however, most of that data is repetitive, and once a fix is made, it continually broadcasts location
//  so despite being a flawed approach, this is probably workable

#include <Arduino.h>
#include <SoftwareSerial.h>

//(rx, tx)
SoftwareSerial SoftwareOne(9, 8);
SoftwareSerial SoftwareTwo(5, 4);

void setup() {
  Serial.begin(9600);
}


int phase = 1;
char charRead;
void loop() {
  if(phase) {
    Serial.println("First Receiver:");
    SoftwareOne.begin(9600);
    //the port was just opened -- the only time it receives data is in this delay
    //if it is too short, there won't be any sentences sent during the window
    delay(2500);
    while(SoftwareOne.available()) {
      charRead = SoftwareOne.read();
      Serial.print(charRead);
    }
    //need to end the port to stop blocking resources
    //otherwise the other port won't be able to receive data
    SoftwareOne.end();
  } else {
    Serial.println("Second Receiver:");
    SoftwareTwo.begin(9600);
    delay(2500);
    while(SoftwareTwo.available()) {
      charRead = SoftwareTwo.read();
      Serial.print(charRead);
    }
    SoftwareTwo.end();
  }
  Serial.println();
  phase = (phase + 1) % 2;
}
