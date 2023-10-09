#include <LibRobus.h>

int x,y,angle;
bool isStart;


void CommencerTerminer(){
	if(ROBUS_IsBumper(3) || y == 10){
		isStart = !isStart;
		delay(500);
		x = 2;
		y = 1;
	}
}

void arret(){
	MOTOR_SetSpeed(1, 0);
  	MOTOR_SetSpeed(0, 0);
  	ENCODER_Reset(1);
  	ENCODER_Reset(0);
	delay(500);
}

float calculeclic (float vitesse, int id,int cycle)
{
    int clicSeconde = ENCODER_Read(id);
    Serial.println(clicSeconde);
    float vitessed = (1290*cycle);
    float difference = vitessed - clicSeconde;
    float ponderation = 0.0001;
    if (difference * ponderation < 0.05 && difference * ponderation > -0.05)
    {
      return vitesse;
    }
    
    else
    {
      vitesse = difference * ponderation + vitesse;
      return vitesse;
    }
}

void avancer(){
  	int idelay = 500;
  	float vitesse1 = 0.25;
  	float vitesse0 = 0.25;
  	int cycle = 1;
	ENCODER_Reset(1);
	ENCODER_Reset(0);
	while (ENCODER_Read(1) < 6683)
	{
		CommencerTerminer();
        if(isStart){
			MOTOR_SetSpeed(1, vitesse1);
			MOTOR_SetSpeed(0, vitesse0);
			delay(idelay);
			Serial.println("gauche");
			Serial.println(calculeclic(vitesse0,0,cycle));
			Serial.println("droite");
			Serial.println(calculeclic(vitesse1,1,cycle));
			cycle++;
		 }else
            break;	
	}
	
	arret();	
}

void tGauche(int iteration = 1){
	//Serial.println("Tourne a gauche");
    ENCODER_Reset(1);
    ENCODER_Reset(0);

	while (ENCODER_Read(1) < iteration * 1944)
	{
		CommencerTerminer();
        if(isStart){
			MOTOR_SetSpeed(0, -0.25);
			MOTOR_SetSpeed(1, 0.25);
			delay(1);
		}else
            break;
	}
	
    arret();
}

void tDroite(int iteration = 1){
	//Serial.println("Tourne a droite");
	ENCODER_Reset(0);
  	ENCODER_Reset(1);
	while (ENCODER_Read(0) < iteration * 1994)
	{
		CommencerTerminer();
        if(isStart){
			MOTOR_SetSpeed(0, 0.25);
			MOTOR_SetSpeed(1, -0.25);
			delay(1);
		}else
        	break;
	}
	
    arret();
}

void setup(){
	BoardInit();
	Serial.begin(9600);

}

void loop() {

	CommencerTerminer();

	if(isStart && y < 10){
		Serial.print("Current POS:");
		Serial.print(x);
		Serial.println(y);
		Serial.println(angle);

		bool gauche = false;
		bool droite = false;

		if((digitalRead(33) == 1 && digitalRead(39) == 1) && !(angle == 180 && y == 1) && !(angle == 90 && x == 3) && !(angle == -90 && x == 1))
		{
			Serial.println("rien devant");
    		avancer();

			if(angle == 0)
				y++;
		  	if(angle == 180)
				y--;
		  	if(angle == 90)
			  	x++;
		  	if(angle == -90)
			 	x--;
  		}
  		else
  		{
    		if(y % 2 == 0)
			{
				Serial.println("Cul de sac 1");
    			tDroite(2);
		  		if(angle == 0)
		  			angle = 180;
		  		else
		  			angle = 0;
			}
    		else
    		{
    	  		if((angle == 0 && x != 3) || (angle == 180 && x != 1) || (angle == 90 && y != 1) || angle == -90)
			  	{
				  	tDroite();
					Serial.println("droite");
					Serial.println(digitalRead(33),digitalRead(39));
				  	if(digitalRead(33) == 1 && digitalRead(39) == 1){
						Serial.println("Droite OK!");
				  		droite = true;
					}
	
				  	tGauche();
			  	}

    	  		if((angle == 0 && x != 1) || (angle == 180 && x != 3) || (angle == -90 && y != 1) || angle == 90)
			  	{
			  		tGauche();
					Serial.println("gauche");
			  		if(digitalRead(33) == 1 && digitalRead(39) == 1){
						Serial.println("Gauche OK!");
			  			gauche = true;
					}

			  		tDroite();
			  	}

    	  		if(!droite && !gauche)
    	  		{
					Serial.println("Cul de sac 2");
				  	tDroite(2);

				  	if(angle == 0)
				  		angle = 180;
				  	if(angle == 180)
				  		angle = 0;
				  	if(angle == 90)
				  		angle = -90;
				  	if(angle == -90)
				  		angle = 90;
			  	}

    	  		if(droite && gauche)
		  		{
					Serial.println("Les deux sont OK!");
					if(angle == 0||angle == 180||angle == 90)
						droite=false;
					else 
						gauche=false;
		  		}

				switch (angle)
				{
					case 0:
						if (gauche)
						{
							tGauche();
							angle -= 90;
						}

						if(droite)
						{
							tDroite();
							angle += 90;
						}		
						break;
					case 90:
						if(gauche)
						{
							tGauche();
							angle -= 90;
						}
						if(droite)
						{
							tDroite();
							angle += 90;
						}
						break;
					case -90:
						if(gauche)
						{
							tGauche();
							angle = 180;
						}
						if (droite)
						{
							tDroite();
							angle += 90;
						}
						break;
					case 180:
						if(gauche)
						{
							tGauche();
							angle=90;
						}
						if (droite)
						{
							tDroite();
							angle = -90;
						}
						break;
					default:
						break;
				}	
		  	}

    	}   
	}

}

