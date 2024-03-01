#include <Arduino.h>
#include "Wire.h"

#include "Adafruit_TSL2591.h"



//#define SENSOR (analogRead(A4))

#define SENSOR_ADDRESS (0x29)

Adafruit_TSL2591 lightSensor(2);

void setup() {
  Serial.begin(9600);
  Serial.println("Starting program");

  //sensor initialization?

  lightSensor.begin();

  lightSensor.setGain(TSL2591_GAIN_MED);

  //Serial.print("Gain: ");
  //Serial.println(lightSensor.getGain());
  //Serial.print("Integration time: ");
  //Serial.println(lightSensor.getTiming());

  lightSensor.enable();

  //Wire.begin(SENSOR_ADDRESS);
  //Wire.write(0xA0);
  //Wire.write(0x01 | 0x02);
  //Wire.end();

  //Wire.begin(SENSOR_ADDRESS);
  //Wire.write("heh");
  //Wire.end();
}

void loop() {
  static char charRead;
  Serial.print("Full spectrum: ");
  Serial.println(lightSensor.getLuminosity(TSL2591_FULLSPECTRUM));
  Serial.print("Visible: ");
  Serial.println(lightSensor.getLuminosity(TSL2591_VISIBLE));
  Serial.print("Infrared: ");
  Serial.println(lightSensor.getLuminosity(TSL2591_INFRARED));
  Serial.print("Full luminosity: ");
  Serial.println(lightSensor.getFullLuminosity());
  //Serial.prntln("end loop");
  Serial.println();
  //Serial.println(SENSOR);
  delay(2500);
}
