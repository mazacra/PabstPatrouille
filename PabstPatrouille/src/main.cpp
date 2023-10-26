#include <LibRobus.h>
bool isStart;
const float DISTANCE_ENTRE_ROUE = 18.7;  //Valeur en cm
const float CIRCONFERENCE_ROUE = 23.939;
short couleur = 1;														//1=Vert, 2=Jaune
short section = 1;														//1=TournantTapis, 2=ligneTaperVerre, 3=TournantBalle, 4=LigneSaut

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
void tGauche(int angle)				//angle en degré
{
    ENCODER_Reset(LEFT);							//Reset encoder
    ENCODER_Reset(RIGHT);							//Reset encoder
	float vitesseRoueDroite = 0.15;

	int valeurEncoder = (2 * PI * DISTANCE_ENTRE_ROUE * angle / 360) * 3200 / CIRCONFERENCE_ROUE;
	
	while (ENCODER_Read(RIGHT) < valeurEncoder)				
	{
		CommencerTerminer();
    	if(isStart){
			MOTOR_SetSpeed(LEFT, 0);							//Changement de vitesses
			MOTOR_SetSpeed(RIGHT, vitesseRoueDroite);			//Changement de vitesses
		}else
        	break;
	}	
}

//à changer pour pas que ça tourne sur place, ou créer une autre fonction pour faire les gros tournants
void tDroite(int angle)				//angle en degré
{
	ENCODER_Reset(LEFT);							//Reset encoder
  	ENCODER_Reset(RIGHT);							//Reset encoder
	float vitesseRoueGauche = 0.15;

	int valeurEncoder = (2 * PI * DISTANCE_ENTRE_ROUE * angle / 360) * 3200 / CIRCONFERENCE_ROUE;

	while (ENCODER_Read(LEFT) < valeurEncoder)				
	{			
		CommencerTerminer();			
        if(isStart){			
			MOTOR_SetSpeed(LEFT, vitesseRoueGauche);			//Changement de vitesse
			MOTOR_SetSpeed(RIGHT, 0);							//Changement de vitesse
		}else
        	break;
	}
}

//Créer logique pour Section1 (départ,jump)
void avancer1(float distance) //distance à parcourir en cm
{
	float vitesseMax = 0.4;
	int idelay = 300;
  	float vitesse0 = 0.2;
	float vitesse1 = 0.2;
	float ponderation = 0.0001;
	float distanceEncoder = (distance / CIRCONFERENCE_ROUE) * 3200;

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

//Créer logique pour 2ème tournant (trouver la balle)
//Créer logique pour Section2 (taper le verre)


//Créer logique pour 1er tournant (tapis)
float vitesseRoueDroite(float rayonDroit, float rayonGauche, float vitesseRoueGauche)
{
	//Calcul la distance que les 2 roues parcour
	float roueDroiteDistance = (2 * PI * (rayonDroit)) / 4;
	float roueGaucheDistance = (2 * PI * (rayonGauche)) / 4;

	//Calcul la vitesse à laquelle la roue droite doit s'ajuster selon la vitesse de la roue gauche
	float difference = roueDroiteDistance / roueGaucheDistance;
	float vitesseRoueDroite = vitesseRoueGauche * difference;

	return vitesseRoueDroite;
}

void setMoteurSection1()
{
	ENCODER_Reset(LEFT);
  	ENCODER_Reset(RIGHT);
	float vitesseRoueGauche = 0.3;
	
	switch (couleur)
	{
		case 1://Vert
			MOTOR_SetSpeed(LEFT, vitesseRoueGauche);
			MOTOR_SetSpeed(RIGHT, vitesseRoueDroite(30.48 + 5.89, 60.96 - 5.89, vitesseRoueGauche));
			break;
		case 2://Jaune
			MOTOR_SetSpeed(LEFT, vitesseRoueGauche);
			MOTOR_SetSpeed(RIGHT, vitesseRoueDroite(60.96 + 5.89, 91.44 - 5.89, vitesseRoueGauche));
			break;
		default:
			break;
	}
}

void section1Loop(){
	int limiteEncoder = couleur == 1 ? 11833 : 18382;

	setMoteurSection1();//appel demarage des moteurs

	while (ENCODER_Read(LEFT) < limiteEncoder)
	{
        //Appel PID et ajustement
	}

	section = 2;
}

void changementVoie(float distanceDevant, float distanceCote)		//permet de changer de voie dans la section 9 ou 0
{																	
	float angle = atan(distanceCote / distanceDevant);

	float distanceParcourutTournantDevant = DISTANCE_ENTRE_ROUE * sin(angle);			//Robot ne tourne pas sur place donc calcul des valeur qui nous fait decaler
	float distanceParcourutTournantCote = DISTANCE_ENTRE_ROUE - (DISTANCE_ENTRE_ROUE * cos(angle));

	float distance = sqrt(pow(distanceDevant - distanceParcourutTournantDevant, 2) + pow(distanceCote - distanceParcourutTournantCote, 2));	//pytagore pour trouver l'hypotenuse

	//Si vert au jaune (programmer pour savoir sur quelle couleur on se trouve)
	tGauche(angle);
	avancer1(distance);
	tDroite(angle);

	//Si jaune au vert (programmer pour savoir sur quelle couleur on se trouve)
	tDroite(angle);
	avancer1(distance);
	tGauche(angle);
}


void setup(){
	BoardInit();
	Serial.begin(9600);

	//initialiser les senseurs ajoutés

	pinMode(PIN_A4, INPUT);
  pinMode(PIN_A5, INPUT);
}

void loop() {
  CommencerTerminer();  
  if(isStart)
  {
    switch (section)
    {
    case 1://Premier tournant
      	section1Loop();
      	break;
    case 2://Ligne droite
      
      break;
    case 3://deuxième tournant
      
      break;
    case 4://ligne droite

      break;
    default:
      break;
    }
	}
}