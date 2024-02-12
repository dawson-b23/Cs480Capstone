//basic hub which uses i2c communication
//periodically polls device 2 for data then prints that
#include <Arduino.h>
#include <Wire.h>


void setup() {
  Serial.begin(9600);
  Wire.begin();
  Serial.println("GPSHub Beginning");
}

void loop() {
  static char charRead;
  delay(1250);
  Wire.requestFrom(2, 128);
  while(Wire.available()) {
    charRead = Wire.read();
    //if it tries to read a byte when none is being sent, ((int) charRead) has value -1
    //checking for positive values stops the spam of invalid character printouts
    if(charRead > 0) {
      Serial.print(charRead);
    }
  }
  //Serial.println();
}