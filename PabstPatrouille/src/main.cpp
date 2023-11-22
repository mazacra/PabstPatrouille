#include <Arduino.h>
#include <LibRobus.h>
#include <Adafruit_TCS34725.h>

void setup() {
	BoardInit(); //Init libraire LibRobus
    
	Serial.begin(9600);
}

void loop() {
	while (true)
	{
		MOTOR_SetSpeed(LEFT, 0);
		MOTOR_SetSpeed(RIGHT, 0);
		Serial.print("1:");
		Serial.println(SONAR_GetRange(0));
		delay(500);
		Serial.print("2:");
		Serial.println(SONAR_GetRange(1));
		Serial.println();
		delay(2000);
	}
	
}
