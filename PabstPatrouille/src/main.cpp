#include <Arduino.h>
#include <LibRobus.h>
#include <Adafruit_TCS34725.h>
#include "gamePabst.h"

using namespace Game;

//Global var
GamePabst game;
int diff = 0;
int cpt;

void setup() 
{
	BoardInit(); //Init libraire LibRobus
    
	Serial.begin(9600);
}

void loop() 
{
	//LCD AFFICHAGE MENU

	if(true)//if bouton pressed
	{	
		cpt = game.startGame(diff);
		game.endGame(cpt);
	}
}
