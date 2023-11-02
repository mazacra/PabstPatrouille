#include <LibRobus.h>
#include <Adafruit_TCS34725.h>

#define IRGauche 1
#define IRDroit 3
#define DISTANCE_ENTRE_ROUE 18.7
#define CIRCONFERENCE_ROUE 23.939
#define VITESSE_INITIALE 0.2

bool isStart;
short couleur;	//3=Vert, 2=Jaune
short section = 1;	//1=TournantTapis, 2=ligneTaperVerre, 3=TournantBalle, 4=LigneSaut
int tour = 0;
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_2_4MS, TCS34725_GAIN_16X);

void CommencerTerminer(){
	Serial.println(analogRead(PIN_A4));
    if(ROBUS_IsBumper(3)){					//bumper ou sifflet pour démarrer
        isStart = !isStart;
	}else if(analogRead(PIN_A4) > 590){
		isStart = true;
	}      

	section = 1;
}

//Potentiellement inutile
void arret(){
  	MOTOR_SetSpeed(LEFT, 0);
	MOTOR_SetSpeed(RIGHT, 0);
  	ENCODER_Reset(LEFT);
  	ENCODER_Reset(RIGHT);
	delay(500);
}

float diffClic (){
	int clic0 = ENCODER_Read(LEFT);
	int clic1 = ENCODER_Read(RIGHT);

	return (clic0 - clic1)/2;
}

void demarrer(float vitesseG, float vitesseD){
	ENCODER_Reset(LEFT);	//Reset encoder
	ENCODER_Reset(RIGHT);	//Reset encoder

	MOTOR_SetSpeed(LEFT, vitesseG);		//Changement de vitesse
	MOTOR_SetSpeed(RIGHT, vitesseD);	//Changement de vitesse
	
}

//Potentiellement à modifier pour qu'il avance toujours (on a pas de raison de s'arrêter pour l'instant)
//Donc pourrait ce simplifier à allumer les moteurs, et on appelera notre PID ailleurs
void avancer(){
  	int idelay = 100;
  	float vitesse0 = 0.20;
	float vitesse1 = 0.20;
	float ponderation = 0.0001;
	
    if(isStart)
	{
		MOTOR_SetSpeed(LEFT, vitesse0);		//Changement de vitesse
		MOTOR_SetSpeed(RIGHT, vitesse1);	//Changement de vitesse
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
	float vitesseRoueDroite = 0.3;

	int valeurEncoder = (2 * PI * DISTANCE_ENTRE_ROUE * angle / 360) * 3200 / CIRCONFERENCE_ROUE;
	Serial.println(valeurEncoder);
	while (ENCODER_Read(RIGHT) < valeurEncoder)				
	{
			MOTOR_SetSpeed(LEFT, 0);							//Changement de vitesses
			MOTOR_SetSpeed(RIGHT, vitesseRoueDroite);			//Changement de vitesses
	}	
}

void tGaucheRecule(int angle)				//angle en degré
{
    ENCODER_Reset(LEFT);							//Reset encoder
    ENCODER_Reset(RIGHT);							//Reset encoder
	float vitesseRoueGauche = -0.15;

	int valeurEncoder = (((-2 * PI * DISTANCE_ENTRE_ROUE * angle) / 360) * 3200) / CIRCONFERENCE_ROUE;
	Serial.println(valeurEncoder);
	while (ENCODER_Read(RIGHT) > valeurEncoder)				
	{
			MOTOR_SetSpeed(LEFT, vitesseRoueGauche);							//Changement de vitesses
			MOTOR_SetSpeed(RIGHT, 0);			//Changement de vitesses
	}	
}

//à changer pour pas que ça tourne sur place, ou créer une autre fonction pour faire les gros tournants
void tDroite(int angle)				//angle en degré
{
	ENCODER_Reset(LEFT);							//Reset encoder
  	ENCODER_Reset(RIGHT);							//Reset encoder
	float vitesseRoueGauche = 0.3;

	int valeurEncoder = (2 * PI * DISTANCE_ENTRE_ROUE * angle / 360) * 3200 / CIRCONFERENCE_ROUE;

	while (ENCODER_Read(LEFT) < valeurEncoder)				
	{					
			MOTOR_SetSpeed(LEFT, vitesseRoueGauche);			//Changement de vitesse
			MOTOR_SetSpeed(RIGHT, 0);							//Changement de vitesse

	}
}

void tDroiteRecule(int angle)				//angle en degré
{
	ENCODER_Reset(LEFT);							//Reset encoder
  	ENCODER_Reset(RIGHT);							//Reset encoder
	float vitesseRoueDroite = -0.15;

	int valeurEncoder = (-2 * PI * DISTANCE_ENTRE_ROUE * angle / 360) * 3200 / CIRCONFERENCE_ROUE;

	while (ENCODER_Read(RIGHT) > valeurEncoder)				
	{					
			MOTOR_SetSpeed(LEFT, 0);			//Changement de vitesse
			MOTOR_SetSpeed(RIGHT, vitesseRoueDroite);							//Changement de vitesse

	}
}

void avancer1(float distance) //distance à parcourir en cm
{
	Serial.println("avancer1");
	int idelay = 100;
	float vitesseMax = 0.6;
  	float vitesseG = 0.2;
	float vitesseD = 0.2;
	float ponderation = 0.0001;
	float distanceEncoder = (distance / CIRCONFERENCE_ROUE) * 3200;

	ENCODER_Reset(LEFT);
	ENCODER_Reset(RIGHT);

	while (!(ENCODER_Read(RIGHT) > distanceEncoder && ENCODER_Read(LEFT) > distanceEncoder))
	{
        if(isStart)
		{
			MOTOR_SetSpeed(LEFT, vitesseG);											//Changement de vitesse
			MOTOR_SetSpeed(RIGHT, vitesseD);										//Changement de vitesse
			delay(idelay);

			vitesseG = vitesseMax * sin((PI * ENCODER_Read(LEFT)) / distanceEncoder) + VITESSE_INITIALE;	//À tester voir si l'accélération et décélération marche
			vitesseD = vitesseMax * sin((PI * ENCODER_Read(RIGHT)) / distanceEncoder) + VITESSE_INITIALE;	//vrai vitesse max = 0.6
			
			vitesseG = (vitesseG - diffClic()*ponderation);
			vitesseD = (vitesseD + diffClic()*ponderation);
		}
	}
}

void vitesseConstante(float distance, float vitesseg,float vitessed) //distance à parcourir en cm
{
	int idelay = 100;
  	float vitesseG = vitesseg;
	float vitesseD = vitessed;
	float ponderation = 0.0001;
	float distanceEncoder = (distance / CIRCONFERENCE_ROUE) * 3200;

	ENCODER_Reset(LEFT);
	ENCODER_Reset(RIGHT);

	while (!(ENCODER_Read(RIGHT) > distanceEncoder && ENCODER_Read(LEFT) > distanceEncoder))
	{
        if(isStart)
		{
			MOTOR_SetSpeed(LEFT, vitesseG);											//Changement de vitesse
			MOTOR_SetSpeed(RIGHT, vitesseD);										//Changement de vitesse
			delay(idelay);
			
			vitesseG = (vitesseG - diffClic()*ponderation);
			vitesseD = (vitesseD + diffClic()*ponderation);
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
	float rapport = roueDroiteDistance / roueGaucheDistance;
	float vitesseRoueDroite = vitesseRoueGauche * rapport;

	return vitesseRoueDroite;
}

void setMoteurSection1()
{
	Serial.println("setmoteursection1");

	ENCODER_Reset(LEFT);
  	ENCODER_Reset(RIGHT);
	float vitesseRoueGauche = 0.4;
	
	Serial.println(couleur);

	switch (couleur)
	{
		case 3://Vert
			Serial.println("vert");
			while(ENCODER_Read(LEFT) < 11800){
				MOTOR_SetSpeed(LEFT, vitesseRoueGauche);
				MOTOR_SetSpeed(RIGHT, vitesseRoueDroite(30.48 + 5.89, 60.96 - 5.89, vitesseRoueGauche));
			}
			Serial.println("fin");
			break;
		case 2://Jaune
			Serial.println("jaune");
			while (ENCODER_Read(LEFT) < 18300){
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

	/*Serial.println(r);
	Serial.println(g);
	Serial.println(b);*/

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

    // demarrer(-0.15, -0.15);
    // delay(1000);
    // couleur = LectureCouleur();
    // demarrer(0.15, 0.15);
    // delay(1000);

    // arret();

	// SERVO_SetAngle(1,131);
	// SERVO_SetAngle(0,112);
	// delay(100);

	//avancer1(16.5);
	
	setMoteurSection1();

	if(couleur == 3)
	{
    Serial.println("avance pcq vert");
		avancer1(50);
	}

	else
		avancer1(64);
	
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
		c = LectureCouleur();
		Serial.println("couleur = ");
		Serial.println(c);

		float vGauche = c == 2 ? 0.22 : 0.2; 
		float vDroit = c == 3 ? 0.22 : 0.2; 

		demarrer(vGauche,vDroit);

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
				if (ROBUS_ReadIR(IRGauche) > 150)
				{
					SERVO_SetAngle(1, 180);
					verreTrouve = true;
					break;
				}
			}
		}
	}
	arret();
	SERVO_SetAngle(1,131);
	
	section = 3;
}

//Tournant blanc
void section3Loop(){

	if(couleur == 2)
	{
		avancer1(66);
		tDroite(91);
		SERVO_SetAngle(0, 27);
		//avancer1(121.92);
		vitesseConstante(121.92, 0.3, 0.3);
	}
		
	if(couleur == 3)
	{
		avancer1(63);
		tDroite(100);
		SERVO_SetAngle(0, 27);
		//avancer1(103.44);
		vitesseConstante(103.44, 0.3, 0.3);
	}
	tDroite(50);
	avancer1(86.21);
	tDroite(45);
	SERVO_SetAngle(0, 22);

	arret();
	delay(500);

	demarrer(0.2,0.2);

	int c = LectureCouleur();
	while(c == 0)
	{
		Serial.println(c);
		c = LectureCouleur();
	}

	section = 4;
}

void section4Loop()
{
  vitesseConstante(10, 0.2, 0.2);
  SERVO_SetAngle(0,130);

  avancer1(130);
  tDroite(90);
  ENCODER_Reset(LEFT);

  while(ENCODER_Read(LEFT) < 11000)
	avancer();

  MOTOR_SetSpeed(LEFT, -0.15);
  MOTOR_SetSpeed(RIGHT, -0.15);
  delay(1250);
  tGauche(85);
  section = 5;
  
}

void section5loop()
{

	while(true)
	{
		Serial.println(ROBUS_ReadIR(IRDroit));
		if (ROBUS_ReadIR(IRDroit) > 335)
		{
			Serial.println("trop proche");
			Serial.println(ROBUS_ReadIR(IRDroit));
			MOTOR_SetSpeed(LEFT, 0.5);
			MOTOR_SetSpeed(RIGHT, 0.75);	
		}
		else if (ROBUS_ReadIR(IRDroit) < 325 && ROBUS_ReadIR(IRDroit) > 240)
		{
			Serial.println("trop loin");
			Serial.println(ROBUS_ReadIR(IRDroit));
			MOTOR_SetSpeed(LEFT, 0.75);
			MOTOR_SetSpeed(RIGHT, 0.5);
		}
		else if (ROBUS_ReadIR(IRDroit) < 240)
		{
			Serial.println("plus de mur");
			Serial.println(ROBUS_ReadIR(IRDroit));
			demarrer(0.4, 0.4);
			delay(650);
			while(ROBUS_ReadIR(IRDroit) < 240)
			{
				MOTOR_SetSpeed(LEFT, 0.45);
				MOTOR_SetSpeed(RIGHT, 0.0);
			}
		}

	
	}
	
}


void setup()
{
	BoardInit();
	Serial.begin(9600);

	//initialiser les senseurs ajoutés
	SERVO_Enable(0);
	SERVO_Enable(1);
	pinMode(PIN_A0, INPUT);
	pinMode(PIN_A4, INPUT);
  	pinMode(PIN_A5, INPUT);
}
void loop() 
{
  CommencerTerminer(); 
  Serial.println("droite");
  Serial.println(ROBUS_ReadIR(IRDroit));
  Serial.println("gauche");
  Serial.println(ROBUS_ReadIR(IRGauche));
  couleur = 3;
  couleur = LectureCouleur();
  if(isStart)
  {
	tDroite(90);
	if(couleur == 3)
		vitesseConstante(38, 0.2, 0.2);

	if(couleur == 2)
		vitesseConstante(65, 0.2, 0.2);

	MOTOR_SetSpeed(LEFT, -0.15);
   	MOTOR_SetSpeed(RIGHT, -0.15);
  	delay(1250);
  	tGauche(90);

	section = 5;

		/*switch (section)
		{
			// case 1://Premier tournant
			// Serial.println("debut case 1");
			// SERVO_SetAngle(1,131);
			// SERVO_SetAngle(0,112);
			// delay(100);

			// demarrer(-0.15, -0.15);
			// delay(1000);
			// arret();
			// couleur = LectureCouleur();
			// Serial.println(couleur);
			// delay(1000);
			// demarrer(0.15, 0.15);
			// delay(1000);

			// arret();
			// section1Loop();
			// section = 2;
        	// Serial.println("fin case 1");

			// case 2://Ligne droite
        	// Serial.println("debut case 2");
			// section2();
        	// Serial.println("fin case 2");

			// case 3://deuxième tournant
			// Serial.println("debut case 3");
        	// section3Loop();
        	// Serial.println("fin case 3");

			// case 4://ligne droite
        	// Serial.println("debut case 4");
			// section4Loop();
        	// Serial.println("fin case 4");

			case 5: //suivre mur
        	Serial.println("debut case 5");
			section5loop();
        	Serial.println("fin case 5");
			break;

			default:
				break;
		}
	}*/
}
}

