#include <LibRobus.h>

int x,y,angle;
bool isStart;

void CommencerTerminer(){
    if(ROBUS_IsBumper(3) || analogRead(PIN_A4) > 600 || y == 10){
        if(!(analogRead(PIN_A4) > 600 && isStart)){ 
            isStart = !isStart;
            delay(1000);
            x = 2;
            y = 1;
			angle = 0;
        }
    }
}

void arret(){
	MOTOR_SetSpeed(1, 0);
  	MOTOR_SetSpeed(0, 0);
  	ENCODER_Reset(1);
  	ENCODER_Reset(0);
	delay(100);
}

float diffClic (){
	int clic0 = ENCODER_Read(0);
	int clic1 = ENCODER_Read(1);
	float diff;

		diff = (clic0 - clic1)/2;
		return diff;
}

void avancer(){
  	int idelay = 300;
  	float vitesse0 = 0.25;
	float vitesse1 = 0.25;
	float ponderation = 0.0001;
	float cycle = 1;
	ENCODER_Reset(1);
	ENCODER_Reset(0);

	while (ENCODER_Read(1) < 6750 && ENCODER_Read(0) < 6750){
		CommencerTerminer();
        if(isStart){
			MOTOR_SetSpeed(1, vitesse1);
			MOTOR_SetSpeed(0, vitesse0);
			delay(idelay);
			if(cycle < 1.8)
			{
				vitesse0 = (vitesse0 - diffClic()*ponderation) * cycle * 1.00452;
				vitesse1 = (vitesse1 + diffClic()*ponderation) * cycle;
			}
			else
			{
				vitesse0 = (vitesse0 - diffClic()*ponderation) * 0.65;
				vitesse1 = (vitesse1 + diffClic()*ponderation) * 0.65;
			}

			Serial.println("gauche");
			Serial.println(vitesse0);
			Serial.println(diffClic());
			Serial.println("droite");
			Serial.println(vitesse1);
			Serial.println(diffClic());
			Serial.println(cycle);
			cycle = cycle * 1.3;
		 }else
            break;	
	}
	arret();
}

void tGauche(int iteration = 1){
	for(int i = 0; i < iteration; i++){
    	ENCODER_Reset(1);
    	ENCODER_Reset(0);
		Serial.println("tourne gauche");
		while (ENCODER_Read(1) < 1970)
		{
			CommencerTerminer();
        	if(isStart){
				MOTOR_SetSpeed(0, -0.15);
				MOTOR_SetSpeed(1, 0.15);
				delay(1);
			}else
            	break;
		}	
    	arret();	
	}
}

void tDroite(int iteration = 1){
	//Serial.println("Tourne a droite");
	for (int i = 0; i < iteration; i++){
		ENCODER_Reset(0);
  		ENCODER_Reset(1);
		Serial.println("tourne droite");
		while (ENCODER_Read(0) < 2010)
		{
			CommencerTerminer();
    	    if(isStart){
				MOTOR_SetSpeed(0, 0.15);
				MOTOR_SetSpeed(1, -0.15);
				delay(1);
			}else
    	    	break;
		}
    	arret();
	}
}


void setup(){
	BoardInit();
	Serial.begin(9600);

	pinMode(PIN_A4, INPUT);
    pinMode(PIN_A5, INPUT);
}

void loop() {

}