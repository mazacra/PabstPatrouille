#include <LibRobus.h>
#include <Adafruit_TCS34725.h>

bool isStart;
const float DISTANCE_ENTRE_ROUE = 18.7;  //Valeur en cm
const float CIRCONFERENCE_ROUE = 23.939;
short couleur = 1;														//3=Vert, 2=Jaune
short section = 1;														//1=TournantTapis, 2=ligneTaperVerre, 3=TournantBalle, 4=LigneSaut
int tour = 0;
Adafruit_TCS34725 tcs = Adafruit_TCS34725();

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
	MOTOR_SetSpeed(RIGHT, vitesseD);											//Changement de vitesse
	
}

//Potentiellement à modifier pour qu'il avance toujours (on a pas de raison de s'arrêter pour l'instant)
//Donc pourrait ce simplifier à allumer les moteurs, et on appelera notre PID ailleurs
void avancer(){
  	int idelay = 300;
  	float vitesse0 = 0.25;
	float vitesse1 = 0.25;
	float ponderation = 0.0001;
	
	ENCODER_Reset(LEFT);																//Reset encoder
	ENCODER_Reset(RIGHT);																//Reset encoder

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

void avancer1(float distance) //distance à parcourir en cm
{
	Serial.println("avancer1");
	float vitesseMax = 0.6;
	int idelay = 100;
  	float vitesse0 = 0.2;
	float vitesse1 = 0.2;
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
				//MOTOR_SetSpeed(RIGHT, vitesseRoueDroite(60.96 + 5.89, 91.44 - 5.89, vitesseRoueGauche));
				MOTOR_SetSpeed(RIGHT, 0.80 * vitesseRoueGauche);
			}
			Serial.println("fin");
			break;
		default:
			break;
	}
}

void changementVoie(float distanceDevant, float distanceCote)		//permet de changer de voie dans la section 9 ou 0
{																	
	float angle = atan(distanceCote / distanceDevant);
	float angleRad = angle * (PI/180);
	float distanceParcourutTournantDevant = DISTANCE_ENTRE_ROUE * sin(angleRad);			//Robot ne tourne pas sur place donc calcul des valeur qui nous fait decaler
	float distanceParcourutTournantCote = DISTANCE_ENTRE_ROUE - (DISTANCE_ENTRE_ROUE * cos(angleRad));

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

int LectureCouleur()
{
	int couleurLue = 0;
	uint16_t r, g, b, c;
	tcs.getRawData(&r, &g, &b, &c);
	tcs.getRawData(&r, &g, &b, &c);

	Serial.println(r);
	Serial.println(g);
	Serial.println(b);

	if (r == 1 && g == 1 && b == 0)
	{
		couleurLue = 1; // rouge
		return couleurLue;
	}
	else if (r == 1 && g == 1 && b == 1)
	{
		couleurLue = 2; // jaune
		return couleurLue;
	}
	else if (r == 0 && g == 1 && b == 0)
	{
		couleurLue = 3; // vert
		return couleurLue;
	}
	else if (r == 0 && g == 1 && b == 1)
	{
		couleurLue = 4; // bleu
		return couleurLue;
	}
}

//Tournant + Tapis
void section1Loop(){

	SERVO_SetAngle(1,50);
	SERVO_SetAngle(0,112);

	setMoteurSection1();
	if(couleur == 3)
	{
		avancer1(61);
	}

	else
	{
		avancer1(70);
	}
	
	setMoteurSection1();
	arret();
	section = 2;
}

//Taper balle
void section2()
{
	demarrer(0.2, 0.2);
	Serial.println("bonjour voici la section 2");
    switch (couleur)
    {
    case 2: //jaune
 
        while (true){
            if (ROBUS_ReadIR(1) < 100)
                break;
        }
 
        SERVO_SetAngle(1, 0);
        
        break;
 
    case 3: //vert
 
        while (true){
            if (ROBUS_ReadIR(0) < 100)
                break;
        }
        SERVO_SetAngle(1, 180);
        
        break;
    }
	SERVO_SetAngle(1,49);
	section = 3;
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
	section = 4;
}

void section4Loop()
{
	avancer1(10);
	SERVO_SetAngle(0,112);


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
			Serial.println("case = 2");
    	  	break;
    	/*case 3://deuxième tournant
			section3Loop();
      		break;
    	case 4://ligne droite
			section4Loop();
      		break;
    	default:
      		break;
    	}*/
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