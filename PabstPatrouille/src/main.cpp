#include <Arduino.h>
#include <LibRobus.h>


void setup() {
  BoardInit(); //Init libraire LibRobus

  //Autre setup (Variable, serial, etc)
}

void loop() {
  
}

//fonction pour calibrer sensor de couleur
#include <ADJDS311.h>
#include <Wire.h>


uint8_t ledPin = 39;
ADJDS311 color(ledpin);

void setup(){
    Serial.begin(9600);
    color.init();
    color.ledOn();
    // ::init() preset values in the registers.
    // The S311 and S371 have different gains.
    // Here you can use ::calibrate() to adjust the sensor's gains.
    // You can also use ::setintegrationtime() and ::setcapacitor() to adjust the sensor manually

    color.calibrate();  // first make sure the sensor faces a white surface at the focal point
}

//mettre sensor proche papier blanc et run le programme. prendre en note valeur high et low
void loop() {
  Serial.print("R: "); Serial.print(color.readRed());Serial.print(", ");
  Serial.print("G: "); Serial.print(color.readGreen());Serial.print(", ");
  Serial.print("B: "); Serial.print(color.readBlue());Serial.print(", ");
  Serial.print("C: "); Serial.print(color.readClear());
  Serial.println();
  
  delay(500);
}