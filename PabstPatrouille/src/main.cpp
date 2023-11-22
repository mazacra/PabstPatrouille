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
    
	Serial.begin(9600);
}

void loop() 
{
	Serial.print(1);
	//LCD AFFICHAGE MENU
	menu.MenuStart();
}
