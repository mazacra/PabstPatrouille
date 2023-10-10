#include <LibRobus.h>

int x,y,angle;
bool isStart;

void CommencerTerminer(){
	if(ROBUS_IsBumper(3) || y == 10){
		isStart = !isStart;
		delay(1000);
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

float diffClic (){
	int clic0 = ENCODER_Read(0);
	int clic1 = ENCODER_Read(1);
	float diff;

		diff = (clic0 - clic1)/2;
		return diff;
}

void avancer(){
  	int idelay = 300;
  	float vitesse0 = 0.30;
	float vitesse1 = 0.30;
	float ponderation = 0.0001;
	ENCODER_Reset(1);
	ENCODER_Reset(0);

	while (ENCODER_Read(1) < 6000 && ENCODER_Read(0) < 6000){
		CommencerTerminer();
        if(isStart){
			MOTOR_SetSpeed(1, vitesse1);
			MOTOR_SetSpeed(0, vitesse0);
			delay(idelay);
			
			vitesse0 = vitesse0 - diffClic()*ponderation;
			Serial.println("gauche");
			Serial.println(vitesse0);
			Serial.println(diffClic());
			vitesse1 = vitesse1 + diffClic()*ponderation;
			Serial.println("droite");
			Serial.println(vitesse1);
			Serial.println(diffClic());
		 }else
            break;	
	}
	arret();	
}

void tGauche(int iteration = 1){
    ENCODER_Reset(1);
    ENCODER_Reset(0);

	while (ENCODER_Read(1) < iteration * 1900)
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

void tDroite(int iteration = 1){
	//Serial.println("Tourne a droite");
	ENCODER_Reset(0);
  	ENCODER_Reset(1);
	while (ENCODER_Read(0) < iteration * 1990)
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
				  	if(digitalRead(33) == 1 && digitalRead(39) == 1){
						Serial.println("Droite OK!");
				  		droite = true;
					}
	
				  	tGauche();
			  	}

    	  		if((angle == 0 && x != 1) || (angle == 180 && x != 3) || (angle == -90 && y != 1) || angle == 90)
			  	{
			  		tGauche();
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

/*float calculeclic (float vitesse, int id,int cycle)
{
    int clicSeconde = ENCODER_Read(id);
    Serial.println(clicSeconde);
    float vitessed = (1000*cycle);
    float difference = vitessed - clicSeconde;
    float ponderation = 0.0002;

    if (difference * ponderation < 0.0004 && difference * ponderation > -0.0004)
    {
      return vitesse;
    }

    else
    {
      vitesse = difference * ponderation + vitesse;
      return vitesse;
    }
}*/