#include <Arduino.h>
#include <LibRobus.h>
#include <Adafruit_TCS34725.h>
#include "gamePabst.h"
#include "modulePabst.h"

using namespace Game;
using namespace Module;

//Global var
ModulePabst module;
GamePabst game;
int diff = 0;

void setup() {
	BoardInit(); //Init libraire LibRobus
    
	Serial.begin(9600);
}

void loop() {
	if(true)//if bouton pressed
	{	
		game.startGame(diff);
	}
}
