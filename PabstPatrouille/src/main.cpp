#include <LibRobus.h>
bool isStart;

void CommencerTerminer(){
    if(ROBUS_IsBumper(3) || analogRead(PIN_A4) > 600){					//bumper ou sifflet pour démarrer
        isStart = !isStart;
    }

	while(analogRead(PIN_A4) > 600){}									//ne démarre pas tant qu'il entends le sifflet
}

//Potentiellement inutile
void arret(){
  	MOTOR_SetSpeed(LEFT, 0);
	MOTOR_SetSpeed(RIGHT, 0);
  	ENCODER_Reset(LEFT);
  	ENCODER_Reset(RIGHT);
	delay(100);
}

float diffClic (){
	int clic0 = ENCODER_Read(LEFT);
	int clic1 = ENCODER_Read(RIGHT);

	return (clic0 - clic1)/2;
}

//Potentiellement à modifier pour qu'il avance toujours (on a pas de raison de s'arrêter pour l'instant)
//Donc pourrait ce simplifier à allumer les moteurs, et on appelera notre PID ailleurs
void avancer(){
  	int idelay = 300;
  	float vitesse0 = 0.25;
	float vitesse1 = 0.25;
	float ponderation = 0.0001;
	float cycle = 1;
	
	ENCODER_Reset(LEFT);																//Reset encoder
	ENCODER_Reset(RIGHT);																//Reset encoder

	while (ENCODER_Read(RIGHT) < 6750 && ENCODER_Read(LEFT) < 6750){
		CommencerTerminer();

        if(isStart){
			MOTOR_SetSpeed(LEFT, vitesse0);											//Changement de vitesse
			MOTOR_SetSpeed(RIGHT, vitesse1);											//Changement de vitesse
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

			cycle = cycle * 1.3;
		 }else
            break;	
	}

	arret();																		//Arret
}

//Pas vraiment nécessaire dans Nascar, on tourne que à droite
//A garder pour trouver la balle
//POurrait surement être simplifier
void tGauche(){
    ENCODER_Reset(LEFT);							//Reset encoder
    ENCODER_Reset(RIGHT);							//Reset encoder
	
	while (ENCODER_Read(RIGHT) < 1970)				//À changer potentiellement, car plus des carrées
	{
		CommencerTerminer();
    	if(isStart){
			MOTOR_SetSpeed(LEFT, -0.15);			//Changement de vitesses
			MOTOR_SetSpeed(RIGHT, 0.15);			//Changement de vitesses
		}else
        	break;
	}	

    arret();									//Stop
}

//à changer pour pas que ça tourne sur place, ou créer une autre fonction pour faire les gros tournants
void tDroite(){
	ENCODER_Reset(LEFT);							//Reset encoder
  	ENCODER_Reset(RIGHT);							//Reset encoder

	while (ENCODER_Read(LEFT) < 2010)				//À changer potentiellement, car plus des carrées
	{			
		CommencerTerminer();			
        if(isStart){			
			MOTOR_SetSpeed(LEFT, 0.15);			//Changement de vitesse
			MOTOR_SetSpeed(RIGHT, -0.15);			//Changement de vitesse
		}else
        	break;
	}
	
	arret();									//Stop
}

//Créer logique pour Section1 (départ,jump)
//Créer logique pour 1er tournant (tapis)
//Créer logique pour Section2 (taper le verre)
//Créer logique pour 2ème tournant (trouver la balle)
float ajustementVitesseTournerDroite(float rayonDroit, float rayonGauche, float vitesseRoueGauche)
{
	//Calcul la distance que les 2 roues parcour
	float roueDroiteDistance = (2 * PI * (rayonDroit)) / 4;
	float roueGaucheDistance = (2 * PI * (rayonGauche)) / 4;

	//Calcul la vitesse à laquelle la roue droite doit s'ajuster selon la vitesse de la roue gauche
	float difference = roueDroiteDistance / roueGaucheDistance;
	float vitesseRoueDroite = vitesseRoueGauche * difference;

	return vitesseRoueDroite;
}
void section1Et3()
{
	ENCODER_Reset(LEFT);							//Reset encoder
  	ENCODER_Reset(RIGHT);
	float vitesseRoueGauche = 0.3;
	
	//Si couleurDétecter = vert(programmer la condition)
	while(ENCODER_Read(LEFT) < 11833)//ajuster la valeur au besoin
	{
		if(isStart)
		{
			MOTOR_SetSpeed(LEFT, vitesseRoueGauche);			//Changement de vitesse
			MOTOR_SetSpeed(RIGHT, ajustementVitesseTournerDroite(30.48 + 5.89, 60.96 - 5.89, vitesseRoueGauche));
		}else
			break;
	}

	//Si couleurDétecter = jaune (programmer la condition)
	while(ENCODER_Read(LEFT) < 18382)//ajuster la valeur au besoin
	{
		if(isStart)
		{
			MOTOR_SetSpeed(LEFT, vitesseRoueGauche);			//Changement de vitesse
			MOTOR_SetSpeed(RIGHT, ajustementVitesseTournerDroite(60.96 + 5.89, 91.44 - 5.89, vitesseRoueGauche));
		}else
			break;
		
	}
}

void avancer1(float distance) //distance à parcourir en cm
{
	float vitesseMax = 0.4;
	int idelay = 300;
  	float vitesse0 = 0.2;
	float vitesse1 = 0.2;
	float ponderation = 0.0001;
	float distanceEncoder = (distance / 23.939) * 3200;

	while (ENCODER_Read(RIGHT) < distanceEncoder && ENCODER_Read(LEFT) < distanceEncoder)
	{
        if(isStart)
		{
			MOTOR_SetSpeed(LEFT, vitesse0);											//Changement de vitesse
			MOTOR_SetSpeed(RIGHT, vitesse1);										//Changement de vitesse
			delay(idelay);

			vitesse0 = vitesseMax * sin(2 * PI * ENCODER_Read(LEFT) / distanceEncoder) + 0.2;	//À tester voir si l'accélération et décélération marche
			vitesse1 = vitesseMax * sin(2 * PI * ENCODER_Read(RIGHT) / distanceEncoder) + 0.2;	//vrai vitesse max = 0.6
			
			vitesse0 = (vitesse0 - diffClic() * ponderation);
			vitesse1 = (vitesse1 + diffClic() * ponderation);
		}
	}
}


void setup(){
	BoardInit();
	Serial.begin(9600);

	//initialiser les senseurs ajoutés

	pinMode(PIN_A4, INPUT);
    pinMode(PIN_A5, INPUT);
}

void loop() {
	//Appel CommencerTerminer

	//Condition if start
	//{
	//	Logique
	//}
}