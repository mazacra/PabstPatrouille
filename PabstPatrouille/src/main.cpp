#include <LibRobus.h>
bool isStart;
const float DISTANCE_ENTRE_ROUE = 18.7;  //Valeur en cm
const float CIRCONFERENCE_ROUE = 23.939;
short couleur = 1;														//1=Vert, 2=Jaune
short section = 1;														//1=TournantTapis, 2=ligneTaperVerre, 3=TournantBalle, 4=LigneSaut
int tour = 0;

void CommencerTerminer(){
    if(ROBUS_IsBumper(3)){					//bumper ou sifflet pour démarrer
        isStart = !isStart;
		couleur = 1; //détecter couleur
	}else if(analogRead(PIN_A4) > 600){
		isStart = true;
		couleur = 1; //détecter couleur
	}

	section = 1;
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

void demarrer(float vitesseG, float vitesseD){
	ENCODER_Reset(LEFT);																//Reset encoder
	ENCODER_Reset(RIGHT);																//Reset encoder

	MOTOR_SetSpeed(LEFT, vitesseG);											//Changement de vitesse
	MOTOR_SetSpeed(RIGHT, vitesseD);											//Changement de vitesse
	
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

//Tournant Tapis
void section1Loop(){
	int limiteEncoder = couleur == 1 ? 11833 : 18382;
	tour++;

	setMoteurSection1();//appel demarage des moteurs

	while (ENCODER_Read(LEFT) < limiteEncoder)
	{
        //Appel PID et ajustement
	}

	section = 2;
}

//Taper balle
void section2Loop(){
	demarrer(0.2, 0.2);

	while (true)
	{
		if(tour % 2 == 1)
		{
			/*
			if(couleur == jaune){
				if(ROBUS_ReadIR(1 ou 2 ou 3) > que X)
					SERVO_SetAngle(1, 0);
			}else{
				if(ROBUS_ReadIR(0) > que X)
					SERVO_SetAngle(1, 180);
			}
			*/
		}
		else
		{
			//Raccourcis
			int ref = true /*Detecter couleur*/ ? 1 /*valeur bleu*/ : 2 /*valeur vert*/;
			
			if(true) //Distance IR > Distance IR < ref - 100
			{
				//Tourner a droite
			}
		}
	}
	
}

//Tournant blanc
void section3Loop(){
	SERVO_SetAngle(0, 25);

	while (true)
	{
		switch (analogRead(A0))
		{
		case 1/* gauche */:
			/* tourner a droite */
			break;
		case 2/* droite */:
			/*trouner a gauche*/
			break;
		default:
			/*avancer*/
			break;
		}		
	}
	
	SERVO_SetAngle(0, 22);
}

void section4Loop(){

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
			section2Loop();
    	  	break;
    	case 3://deuxième tournant
			section3Loop();
      		break;
    	case 4://ligne droite
			section4Loop();
      		break;
    	default:
      		break;
    	}
	}
}



/*
//fonction pour calibrer sensor de couleur
#include <ADJDS311.h>

uint8_t ledPin = 39;
ADJDS311 color(ledpin);

void setup(){
    Serial.begin(9600);
    color.init();
    color.ledOn();
    // ::init() preset values in the registers.
    // The S311 and S371 have different gains.
    // Here you can use ::calibrate() to adjust the sensor's gains.
    // You can also use ::setintegrationtime() and ::setcapacitor() to adjust the sensor manually

    color.calibrate();  // first make sure the sensor faces a white surface at the focal point
}


void loop() {
  Serial.print("R: "); Serial.print(color.readRed());Serial.print(", ");
  Serial.print("G: "); Serial.print(color.readGreen());Serial.print(", ");
  Serial.print("B: "); Serial.print(color.readBlue());Serial.print(", ");
  Serial.print("C: "); Serial.print(color.readClear());
  Serial.println();
  
  delay(500);
}

//valeurs de calibration lors de la compilation avec feuille de papier blanc
unsigned int RedLow = ;
unsigned int RedHigh = ;
unsigned int GreenLow = ;
unsigned int GreenHigh = ;
unsigned int BlueLow = ;
unsigned int BlueHigh = ;
unsigned int ClearLow = ;
unsigned int ClearHigh = ;

//Determiner sur quelle couleur le robot est
void loop(){
 if (RedHigh && GreenHigh > BlueHigh && ClearHigh)
 {
	Serial.println("couleur jaune");
 }
 if (GreenHigh > RedHigh && BlueHigh && ClearHigh)
 {
	Serial.println("couleur vert");
 }
 if (BlueHigh > GreenHigh && RedHigh && ClearHigh)
 {
	Serial.println("couleur bleue");
 }
 if (RedHigh > GreenHigh && BlueHigh && ClearHigh)
 {
	Serial.println("couleur rouge");
 }
}
*/