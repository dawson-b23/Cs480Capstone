#include <Arduino.h>
#include "TinyGPS++.h"
#include <SoftwareSerial.h>

SoftwareSerial SoftSerial(9, 8);
TinyGPSPlus parser;


void setup() {
  Serial.begin(9600);
  SoftSerial.begin(9600);
  Serial.println("Beginning:");
}

void loop() {
  static char charRead;
  while(SoftSerial.available()) {
    charRead = SoftSerial.read();
    Serial.print(charRead);
    parser.encode(charRead);
  }
  if(parser.location.isValid()) {
    Serial.println();
    Serial.print("Lat: ");
    Serial.println(parser.location.lat());
    Serial.print("Long: ");
    Serial.println(parser.location.lng());
    Serial.println();

  }
  //Serial.println();
  delay(1500);
}
