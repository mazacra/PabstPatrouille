#include <LibRobus.h>

int x,y,angle;
bool isStart;

void CommencerTerminer(){
	if(ROBUS_IsBumper(3) || digitalRead(PIN_A4) == 1 || y == 10){
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

	while (ENCODER_Read(1) < 6600 && ENCODER_Read(0) < 6600){
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
    ENCODER_Reset(1);
    ENCODER_Reset(0);

	while (ENCODER_Read(1) < iteration * 1970)
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

	switch (angle)
	{
		case 0:
			angle -= 90;	
			break;
		case 90:
			angle -= 90;
			break;
		case -90:
			angle = 180;
			break;
		case 180:
			angle=90;
			break;
		default:
			break;
	}	
}

void tDroite(int iteration = 1){
	//Serial.println("Tourne a droite");
	ENCODER_Reset(0);
  	ENCODER_Reset(1);
	while (ENCODER_Read(0) < iteration * 2010)
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

	switch (angle)
	{
		case 0:
			angle += 90;
			break;
		case 90:
			angle += 90;
			break;
		case -90:
			angle += 90;
			break;
		case 180:
			angle = -90;
			break;
		default:
			break;
	}	
}

void Verification(){
	bool gauche = false;
	bool droite = false;
	int anglePresent = angle;

    if((anglePresent == 0 && x != 1) || (anglePresent == 180 && x != 3) || (anglePresent == -90 && y != 1) || anglePresent   == 90)
	{
		tGauche();
		if(digitalRead(33) == 1 && digitalRead(39) == 1){
			Serial.println("Gauche OK!");
			gauche = true;
		}

		if(anglePresent != 90 && !gauche){
			tDroite();
		}
	}

		if(anglePresent != 90 && !gauche){
    		if((anglePresent == 0 && x != 3) || (anglePresent == 180 && x != 1) || (anglePresent == 90 && y != 1) || anglePresent == -90)
	  		{
			  	tDroite();
			  	if(digitalRead(33) == 1 && digitalRead(39) == 1){
					Serial.println("Droite OK!");
			  		droite = true;
				}
	
			if(anglePresent != -90 && !droite){
		  		tGauche();
			}
		}
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
}

void setup(){
	BoardInit();
	Serial.begin(9600);

	pinMode(PIN_A4, INPUT);
    pinMode(PIN_A5, INPUT);
}

void loop() {

	CommencerTerminer();

	if(isStart && y < 10){
		Serial.print("Current POS:");
		Serial.print(x);
		Serial.println(y);
		Serial.println(angle);


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

			if(angle == 90 || angle == -90){
				if(angle == 90){
					tGauche();
				}else{
					tDroite();
				}

				if(digitalRead(33) == 0 && digitalRead(39) == 0){
					if(angle == 90){
						tDroite();
					}else{
						tGauche();
					}
				}
			}
  		}
  		else
  		{
    		if(y % 2 == 1)
    		{ 	
				Verification();
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