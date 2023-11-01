#include <LibRobus.h>
#include <Adafruit_TCS34725.h>

#define IRGauche 1
#define IRDroit 3

bool isStart;
const float DISTANCE_ENTRE_ROUE = 18.7;  //Valeur en cm
const float CIRCONFERENCE_ROUE = 23.939;
short couleur = 1;														//3=Vert, 2=Jaune
short section = 1;														//1=TournantTapis, 2=ligneTaperVerre, 3=TournantBalle, 4=LigneSaut
int tour = 0;
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_2_4MS, TCS34725_GAIN_16X);

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

	MOTOR_SetSpeed(LEFT, vitesseG);												//Changement de vitesse
	MOTOR_SetSpeed(RIGHT, vitesseD);										//Changement de vitesse
	
}

//Potentiellement à modifier pour qu'il avance toujours (on a pas de raison de s'arrêter pour l'instant)
//Donc pourrait ce simplifier à allumer les moteurs, et on appelera notre PID ailleurs
void avancer(){
  	int idelay = 300;
  	float vitesse0 = 0.20;
	float vitesse1 = 0.20;
	float ponderation = 0.0001;
	
    if(isStart)
	{
		MOTOR_SetSpeed(LEFT, vitesse0);												//Changement de vitesse
		MOTOR_SetSpeed(RIGHT, vitesse1);											//Changement de vitesse
		delay(idelay);

		vitesse0 = (vitesse0 - diffClic()*ponderation);
		vitesse1 = (vitesse1 + diffClic()*ponderation);
	}
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
	Serial.println(valeurEncoder);
	while (ENCODER_Read(RIGHT) < valeurEncoder)				
	{
			MOTOR_SetSpeed(LEFT, 0);							//Changement de vitesses
			MOTOR_SetSpeed(RIGHT, vitesseRoueDroite);			//Changement de vitesses
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
		Serial.println("tourner");				
			MOTOR_SetSpeed(LEFT, vitesseRoueGauche);			//Changement de vitesse
			MOTOR_SetSpeed(RIGHT, 0);							//Changement de vitesse

	}
}

void avancer1(float distance) //distance à parcourir en cm
{
	Serial.println("avancer1");
	float vitesseMax = 0.6;
	int idelay = 100;
  	float vitesse0 = 0.25;
	float vitesse1 = 0.25;
	float ponderation = 0.0001;
	float distanceEncoder = (distance / CIRCONFERENCE_ROUE) * 3200;

	ENCODER_Reset(LEFT);
	ENCODER_Reset(RIGHT);

	while (!(ENCODER_Read(RIGHT) > distanceEncoder && ENCODER_Read(LEFT) > distanceEncoder))
	{
        if(isStart)
		{
			MOTOR_SetSpeed(LEFT, vitesse0);											//Changement de vitesse
			MOTOR_SetSpeed(RIGHT, vitesse1);										//Changement de vitesse
			delay(idelay);

			vitesse0 = vitesseMax * sin((PI * ENCODER_Read(LEFT)) / distanceEncoder) + 0.2;	//À tester voir si l'accélération et décélération marche
			vitesse1 = vitesseMax * sin((PI * ENCODER_Read(RIGHT)) / distanceEncoder) + 0.2;	//vrai vitesse max = 0.6
			
			vitesse0 = (vitesse0 - diffClic()*ponderation);
			vitesse1 = (vitesse1 + diffClic()*ponderation);
		}
	}
}


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
	Serial.println("setmoteursection1");

	ENCODER_Reset(LEFT);
  	ENCODER_Reset(RIGHT);
	float vitesseRoueGauche = 0.5;
	
	Serial.println(couleur);

	switch (couleur)
	{
		case 3://Vert
			Serial.println("vert");
			while(ENCODER_Read(LEFT) < 11250){
				MOTOR_SetSpeed(LEFT, vitesseRoueGauche);
				MOTOR_SetSpeed(RIGHT, vitesseRoueDroite(30.48 + 5.89, 60.96 - 5.89, vitesseRoueGauche));
			}
			Serial.println("fin");
			break;
		case 2://Jaune
			Serial.println("jaune");
			while (ENCODER_Read(LEFT) < 17750){
				MOTOR_SetSpeed(LEFT, vitesseRoueGauche);
				MOTOR_SetSpeed(RIGHT, vitesseRoueDroite(60.96 + 5.89, 91.44 - 5.89, vitesseRoueGauche));
				//MOTOR_SetSpeed(RIGHT, 0.78 * vitesseRoueGauche);
			}
			Serial.println("fin");
			break;
		default:
			break;
	}
}

void changementVoie(float distanceDevant, float distanceCote)		//permet de changer de voie dans la section 9 ou 0
{			
	//xcouleur = LectureCouleur();														
	float angleRad = atan(distanceCote / distanceDevant);
	float angle = (angleRad * 180) / PI;
	float distanceParcourutTournantDevant = DISTANCE_ENTRE_ROUE * sin(angleRad);			//Robot ne tourne pas sur place donc calcul des valeur qui nous fait decaler
	float distanceParcourutTournantCote = DISTANCE_ENTRE_ROUE - (DISTANCE_ENTRE_ROUE * cos(angleRad));
	Serial.println(angle);
	float distance = sqrt(pow(distanceDevant - distanceParcourutTournantDevant, 2) + pow(distanceCote - distanceParcourutTournantCote, 2));	//pytagore pour trouver l'hypotenuse

	if(couleur == 3 )
	{
		tGauche(angle);
		avancer1(distance);
		tDroite(angle);
	}

	//Si jaune au vert (programmer pour savoir sur quelle couleur on se trouve)
	if(couleur == 2)
	{
		Serial.println("1111");
		tDroite(angle);
		avancer1(distance);
		tGauche(angle);
	}
	
}

// rajouter une condition si bug pour capter
int LectureCouleur()
{
	int couleurLue = 20;
	uint16_t r, g, b, c;
	tcs.getRawData(&r, &g, &b, &c);
	tcs.getRawData(&r, &g, &b, &c);

	Serial.println(r);
	Serial.println(g);
	Serial.println(b);

	if ((15 < r && r < 24) && (17 < g  && g < 26) && (13 < b && b < 19))
	{
		couleurLue = 0; // blanc
		return couleurLue;
	}

	else if ((8 < r && r < 14) && (7 < g && g < 12) && (6 < b && b < 11))
	{
		couleurLue = 1; // rouge
		return couleurLue;
	}
	else if ((14 < r && r < 22) && (15 < g && g < 21) && (8 < b && b < 13))
	{
		couleurLue = 2; // jaune
		return couleurLue;
	}
	else if ((6 < r && r < 9) && (9 < g && g < 12) && (7 < b && b < 10))
	{
		couleurLue = 3; // vert
		return couleurLue;
	}
	else if ((5 < r && r < 8) && (8 < g && g < 12) && (8 < b && b < 11))
	{
		couleurLue = 4; // bleu
		return couleurLue;
	}
	/*else if ((8 < r && r > 15) && ( 8 < g && g > 17) && (7 < b && b > 14)) // aproximatif
	{
		couleurLue = 5; //noir
		return couleurLue;
	}*/
	return couleurLue;
}

// void AjustementVoie(){

// 	int couleurActuelle = 0;

// 	switch (couleurDepart)
// 	{
// 		//pt mettre un case 0 au cas ou il a rien capté, genre le faire avancer un peu et reessayer de lire la couleur
// 		case 2: //jaune
// 			switch (couleurActuelle)
// 			{
// 				case 0: 
// 					couleurActuelle = LectureCouleur();
// 					break;
// 				case 1: //rouge
// 				//tourne a droite
// 					arret();

// 					tDroite(90);

// 					demarrer(0.2, 0.2)
// ;
// 					delay(2000);

// 					arret();

// 					tGauche(90);

// 					couleurActuelle = 0;

// 					break;
			
// 				case 2: //jaune
// 				//il est dans la bonne voie, rien a faire
// 					break;

// 				case 3: //vert
// 				//tourne a gauche
// 					break;
			
// 				case 4: //bleu
// 				//tourne a gauche
// 					/* code */
// 					break;
// 			}
// 			break;
	
// 		case 3: //vert
// 			switch (couleurDepart)
// 			{
// 				case 1: //rouge
// 				//torune a droite
// 					break;
			
// 				case 2: //jaune
// 				//tourne a droite
// 					break;

// 				case 3: //vert
// 				//il est dans la bonne voie, rien a faire
// 					break;
			
// 				case 4: //bleu
// 				//tourne a gauche
// 					/* code */
// 					break;
// 			}

// 			break;
// 			}
// 	}

//Tournant + Tapis
void section1Loop(){

	Serial.println("bonjour voici la section 1");

	SERVO_SetAngle(1,49);
	SERVO_SetAngle(0,112);
	delay(100);

	avancer1(16.5);
	
	setMoteurSection1();

	if(couleur == 3)
	{
		avancer1(61);
	}

	else
	{
		avancer1(63);
	}
	
	setMoteurSection1();
	arret();
	section = 2;
}

//Taper balle
void section2()
{
	bool verreTrouve = false;
	int c = 1000;
	demarrer(0.2, 0.2);
	Serial.println("bonjour voici la section 2");
	Serial.println(couleur);
	float distanceEncoder = (243.84 / CIRCONFERENCE_ROUE) * 3200;
	Serial.println(distanceEncoder);

	while( c != 0)
	{
		ENCODER_Reset(LEFT);
		ENCODER_Reset(RIGHT);

		c = LectureCouleur();
		Serial.println("couleur = ");
		Serial.println(c);

		float vGauche = c == 2 ? 0.21 : 0.2; 
		float vDroit = c == 3 ? 0.21 : 0.2; 

		demarrer(vGauche,vDroit);

		/*if (c = 2)
		{
			demarrer(0.21, 0.20);
		}
		else
		{
			demarrer(0.20, 0.21);
		}*/

		if(!verreTrouve)
		{
			switch (couleur)
			{
			case 2: //jaune
				if (ROBUS_ReadIR(IRDroit) > 300)
				{
					SERVO_SetAngle(1, 0);
					verreTrouve = true;
					break;
				}
		
			case 3: //vert
				if (ROBUS_ReadIR(IRGauche) > 300)
				{
					SERVO_SetAngle(1, 180);
					verreTrouve = true;
					break;
				}
			}
		}
	}
	arret();
	SERVO_SetAngle(1,49);
	
	section = 3;
}

//Tournant blanc
void section3Loop(){
	Serial.println("Section 3");
	SERVO_SetAngle(1,49);
	SERVO_SetAngle(0, 25);
	arret();

	if(couleur == 2)
		setMoteurSection1();
		
	if(couleur == 3)
	{
		avancer1(30.48);
		tDroite(45);
	}
	while (LectureCouleur() != 2)
	{
		Serial.println(analogRead(A0));

		if(analogRead(A0) <= 450)	
		{
			MOTOR_SetSpeed(LEFT, 0.5);
			MOTOR_SetSpeed(RIGHT, 0.20);
		}
		else if(analogRead(A0) > 700 && analogRead(A0) < 750)
		{
			MOTOR_SetSpeed(LEFT, 0.20);
			MOTOR_SetSpeed(RIGHT, 0.20);
		}
		else 
		{
			MOTOR_SetSpeed(LEFT, 0.20);
			MOTOR_SetSpeed(RIGHT, 0.5);
		}
	}
	arret();
	
	SERVO_SetAngle(0, 22);
	section = 4;
}

void section4Loop()
{
	avancer1(30);
	SERVO_SetAngle(0,112);
	avancer1(91.92);
	changementVoie(121.92, 60.96);
}

void setup(){
	BoardInit();
	Serial.begin(9600);

	//initialiser les senseurs ajoutés
	SERVO_Enable(0);
	SERVO_Enable(1);
	pinMode(PIN_A0, INPUT);
	pinMode(PIN_A4, INPUT);
  	pinMode(PIN_A5, INPUT);
}

void loop() {
  CommencerTerminer();  
  if(isStart)
  {
	couleur = LectureCouleur();

	

	switch (section)
    {
    	case 1://Premier tournant
      		section1Loop();
			section = 2;
			Serial.println("case = 1");
      		//break;
    	case 2://Ligne droite
			section2();
			Serial.println(section);
			Serial.println("case = 2");
    	  	//break;
    	case 3://deuxième tournant
			section3Loop();
			Serial.println("case = 3");
      		//break;
    	case 4://ligne droite
			section4Loop();
      		break;
    	default:
      		break;
    	}
	}
}