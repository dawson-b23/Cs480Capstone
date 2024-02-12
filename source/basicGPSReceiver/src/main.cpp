//reads sentences from a serial port
//sentences are then transmitted in response to i2c requests
#include <Arduino.h>
#include <SoftwareSerial.h>
#include <Wire.h>

//rx, tx
SoftwareSerial ourSerial(8, 9);
//at some point, could swap to using the physical serial port
//since this board's serial port isn't being used for anything

//event handler for i2c requests
void requestHandler();

void setup() {
  Wire.begin(2);
  Wire.onRequest(requestHandler);
  //Serial.begin(9600);
  ourSerial.begin(9600);
}

//two buffers -- we probably don't want to send raw sentences later on
//however, during the stage where we are sending raw sentences
//using two buffers lets complete sentences be read and sent
//rather than sending half-read fragments
char buffer[2][128];
int length[2];
int whichBuffer = 0;
void loop() {
  static char charRead;
  while(ourSerial.available()) {
    charRead = ourSerial.read();
    if(charRead == '$') {
      whichBuffer = (whichBuffer + 1) % 2;
      length[whichBuffer] = 0;
    }
    buffer[whichBuffer][length[whichBuffer]] = charRead;
    length[whichBuffer] = length[whichBuffer] + 1;
  }
  delay(250);
}

//event handler for i2c requests
void requestHandler() {
  //phase offset -- send the full sentence which was read, not the sentence currently being read
  int nextBuffer = (whichBuffer + 1) % 2;
  buffer[nextBuffer][length[nextBuffer] + 1] = '\0';
  Wire.write(buffer[nextBuffer], (length[nextBuffer] * sizeof(char)));
}
