#include <Arduino.h>
#include <LibRobus.h>
#include <Adafruit_TCS34725.h>
#include "moteurPabst.h"
#include "gamePabst.h"
#include "modulePabst.h"

using namespace Moteur;
using namespace Game;
using namespace Module;

//Global var
ModulePabst module;
GamePabst game;
MoteurPabst moteur;

void setup() {
	BoardInit(); //Init libraire LibRobus
    
	Serial.begin(9600);
}

void loop() {
  
}
