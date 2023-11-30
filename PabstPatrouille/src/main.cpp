#include <Arduino.h>
#include <LibRobus.h>
#include <Adafruit_TCS34725.h>
#include "menuPabst.h"

#define PINMOTEUR 42
using namespace Menu;

//Global var
MenuPabst menu;
int diff = 0;
int cpt;

void setup() 
{
	BoardInit(); //Init libraire LibRobus

	pinMode(37, INPUT);
	pinMode(38, INPUT);
	pinMode(39, INPUT);
	pinMode(41, INPUT);
	pinMode(PINMOTEUR, OUTPUT);

	digitalWrite(PINMOTEUR, LOW);

    lcd.init();
    lcd.backlight();
	lcdGame.init();
	lcdGame.backlight();

	Serial.begin(9600);
}

void loop() 
{
	//LCD AFFICHAGE MENU
	//MOTOR_SetSpeed(0, 0.2);
	//MOTOR_SetSpeed(1, 0.2);
//
	//Serial.print(ENCODER_Read(LEFT));
	//Serial.print("\t");
	//Serial.println(ENCODER_Read(RIGHT));

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
