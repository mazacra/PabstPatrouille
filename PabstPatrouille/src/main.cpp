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
	pinMode(31, INPUT);
	pinMode(32, INPUT);
	pinMode(33, INPUT_PULLUP);
	pinMode(34, INPUT);

    lcd.init();
    lcd.backlight();

	Serial.begin(9600);
}

void loop() 
{
	//LCD AFFICHAGE MENU
	pointage ptg = menu.MenuStart();
	if(ptg.pointV != -1)
	{
		if(multiJ)
		menu.MenuFin(ptg.pointV);
		else
		menu.MenuFin(ptg.pointV, ptg.pointO);
	}
	/*if(true)//if bouton pressed
	{	
		cpt = game.startGame(diff);
		game.endGame(cpt);
	}*/
	//game.startGame(4);
	//game.currentGame();
}
