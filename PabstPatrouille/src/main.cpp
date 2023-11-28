#include <Arduino.h>
#include <LibRobus.h>
#include <Adafruit_TCS34725.h>
#include "menuPabst.h"

using namespace Menu;

//Global var
MenuPabst menu;
int diff = 0;
int cpt;

void setup() 
{
	BoardInit(); //Init libraire LibRobus

    lcd.init();
    lcd.backlight();

	Serial.begin(9600);
}

void loop() 
{
	//LCD AFFICHAGE MENU
	//menu.MenuStart();

	/*if(true)//if bouton pressed
	{	
		cpt = game.startGame(diff);
		game.endGame(cpt);
	}*/
	//game.startGame(4);
	//game.currentGame();
}
