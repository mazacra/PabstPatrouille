#include <LibRobus.h>
namespace Moteur
{
    #define DISTANCE_ENTRE_ROUE 18.7
    #define CIRCONFERENCE_ROUE 23.939

    int g_diffAvancerG = 0;
    int g_diffAvancerD = 0;
    int g_diffTournerG = 0;
    int g_diffTournerD = 0;

    class MoteurPabst
    {
        public:
            //signatures des méthode pour les moteur de déplacement
            float diffClic();
            void arret();
            void demarrer(float vitesseG, float vitesseD);
            void vitesseConstante(float distance, float vitesseG, float vitesseD);
            void tGauche(int angle);
            void tDroite(int angle);
            void rotationGauche(float vitesse);
            void rotationDroite(float vitesse);
            bool avancerMode1(float distance, float tempsStart);
            bool reculerMode1(float distance, float tempsStart);
            void tourneDir(int dir);
            void avanceDistance(float distance);

        private:
    };

    float MoteurPabst::diffClic(){
	    int clic0 = ENCODER_Read(LEFT);
	    int clic1 = ENCODER_Read(RIGHT);

	    return (clic0 - clic1)/2;
    }

    void MoteurPabst::arret(){
        MOTOR_SetSpeed(LEFT, 0);
        MOTOR_SetSpeed(RIGHT, 0);
        ENCODER_Reset(LEFT);
        ENCODER_Reset(RIGHT);
    }

    void MoteurPabst::demarrer(float vitesseG, float vitesseD){
	    //ENCODER_Reset(LEFT);	//Reset encoder                 enlever
	    //ENCODER_Reset(RIGHT);	//Reset encoder

	    MOTOR_SetSpeed(LEFT, vitesseG);		//Changement de vitesse
	    MOTOR_SetSpeed(RIGHT, vitesseD);	//Changement de vitesse
    }

    void MoteurPabst::tGauche(int angle)				//angle en degré
    {
        ENCODER_Reset(LEFT);							//Reset encoder
        ENCODER_Reset(RIGHT);							//Reset encoder
    	float vitesseRoueDroite = 0.15;

    	int valeurEncoder = (2 * PI * DISTANCE_ENTRE_ROUE * angle / 360) * 3200 / CIRCONFERENCE_ROUE;
    	while (ENCODER_Read(RIGHT) < valeurEncoder)				
    	{
    			MOTOR_SetSpeed(LEFT, 0);							//Changement de vitesses
    			MOTOR_SetSpeed(RIGHT, vitesseRoueDroite);			//Changement de vitesses
    	}	
        arret();
    }

    void MoteurPabst::tDroite(int angle)				//angle en degré
    {
        ENCODER_Reset(LEFT);							//Reset encoder
        ENCODER_Reset(RIGHT);							//Reset encoder
        float vitesseRoueGauche = 0.15;

        int valeurEncoder = (2 * PI * DISTANCE_ENTRE_ROUE * angle / 360) * 3200 / CIRCONFERENCE_ROUE;
        while (ENCODER_Read(LEFT) < valeurEncoder)				
        {					
                MOTOR_SetSpeed(LEFT, vitesseRoueGauche);			//Changement de vitesse
                MOTOR_SetSpeed(RIGHT, 0);							//Changement de vitesse

        }
        arret();
    }

    void MoteurPabst::vitesseConstante(float distance, float vitesseg,float vitessed) //distance à parcourir en cm
    {
	    int idelay = 100;
  	    float vitesseG = vitesseg;
	    float vitesseD = vitessed;
	    float distanceEncoder = (distance / CIRCONFERENCE_ROUE) * 3200;

	    ENCODER_Reset(LEFT);
	    ENCODER_Reset(RIGHT);

	    while (!(ENCODER_Read(RIGHT) > distanceEncoder && ENCODER_Read(LEFT) > distanceEncoder))
	    {
	    	MOTOR_SetSpeed(LEFT, vitesseG);											//Changement de vitesse
	    	MOTOR_SetSpeed(RIGHT, vitesseD);										//Changement de vitesse
	    	delay(idelay);
	    }
    }

    void MoteurPabst::rotationGauche(float vitesse)
    {
        //Reset encoder
        ENCODER_Reset(LEFT);
        ENCODER_Reset(RIGHT);

        MOTOR_SetSpeed(LEFT, vitesse);
	    MOTOR_SetSpeed(RIGHT, -(vitesse));
    }

    void MoteurPabst::rotationDroite(float vitesse)
    {
        //Reset encoder
        ENCODER_Reset(LEFT);
        ENCODER_Reset(RIGHT);

        MOTOR_SetSpeed(LEFT, -(vitesse));
	    MOTOR_SetSpeed(RIGHT, vitesse);
    }

    bool MoteurPabst::avancerMode1(float distance, float tempsStart)
    {
	    int idelay = 100;
        float vitesseG = 0.25;
        float vitesseD = 0.25;
	    float distanceEncoder = (distance / CIRCONFERENCE_ROUE) * 3200;

	    ENCODER_Reset(LEFT);
	    ENCODER_Reset(RIGHT);

        Serial.println("entrer");
	    while (!(ENCODER_Read(RIGHT) > distanceEncoder && ENCODER_Read(LEFT) > distanceEncoder))
	    {
            if((millis() - tempsStart) < 60000)
		    {	
                MOTOR_SetSpeed(LEFT, vitesseG);											//Changement de vitesse
	    	    MOTOR_SetSpeed(RIGHT, vitesseD);										//Changement de vitesse
	    	    delay(idelay);
			
		        //vitesseG = (vitesseG - diffClic()*ponderation);
		        //vitesseD = (vitesseD + diffClic()*ponderation);
		    }
            else
                return false;
	    }
        return true;
    }

    bool MoteurPabst::reculerMode1(float distance, float tempsStart)
    {
	    int idelay = 100;
        float vitesseG = -0.15;
        float vitesseD = -0.15;
	    float ponderation = 0.0001;
	    float distanceEncoder = -(distance / CIRCONFERENCE_ROUE) * 3200;

	    ENCODER_Reset(LEFT);
	    ENCODER_Reset(RIGHT);

	    while (!(ENCODER_Read(RIGHT) < distanceEncoder && ENCODER_Read(LEFT) < distanceEncoder))
	    {
            if((millis() - tempsStart) < 60000)
		    {										
			    MOTOR_SetSpeed(LEFT, vitesseG);											//Changement de vitesse
	    	    MOTOR_SetSpeed(RIGHT, vitesseD);										//Changement de vitesse
	    	    delay(idelay);
			
		        vitesseG = (vitesseG - diffClic()*ponderation);
		        vitesseD = (vitesseD + diffClic()*ponderation);
		    }
            else
                return false;
	    }
        return true;
    }

    float correction(int encodeurG, int encodeurD, int distG, int distD) 
    {
        if (encodeurG == 0 || encodeurD == 0)
            return 0;

        // Le montant net est calcule avec les valeurs absolues des encodeurs, donc distance parcourue net
        encodeurG = abs(encodeurG);
        encodeurD = abs(encodeurD);
        distG = abs(distG);
        distD = abs(distD);

        short ajustement = distG > distD ? 1 : -1;
        int difference = encodeurG - encodeurD;
        float newSpeed = (((-((ajustement + difference) / 2.0) + encodeurG) / encodeurG) - 1) / 2;
        return newSpeed;
    }

    float minmax(float minValue, float value, float maxValue) 
    {
        return max(minValue, min(value, maxValue));
    }

    void MoteurPabst::tourneDir(int dir) {
        float vitesseMenant = 0;
        float vitesseSuivant = 0;
        float correctionMenant = 1;
        float correctionSuivant = 1;
        int encodeurMenant = 0;
        int encodeurSuivant = 0;
        int distMenant = dir == LEFT ? g_diffAvancerD : g_diffAvancerG;
        int distSuivant = dir == LEFT ? g_diffAvancerG : g_diffAvancerD;
        short roueMenante = dir == LEFT ? RIGHT : LEFT;
    
        while(distMenant < 1990 || distSuivant > -1990)
        {
            int dist = (distMenant - distSuivant) / 2;
            float vitesse = (0.2 - 0.05) * 1.0 * sin((PI*dist)/1990) + 0.05;
            vitesseMenant = min(vitesse, 0.2);
            vitesseSuivant = -vitesseMenant;

            MOTOR_SetSpeed(roueMenante, vitesseMenant * correctionMenant);
            MOTOR_SetSpeed(!roueMenante, vitesseSuivant * correctionSuivant);
            delay(20);

            distMenant += encodeurMenant = ENCODER_ReadReset(roueMenante);
            distSuivant += encodeurSuivant = ENCODER_ReadReset(!roueMenante);

            if (encodeurMenant > 0 && encodeurSuivant < 0) {
                float ajustement = correction(encodeurMenant, encodeurSuivant, distMenant, distSuivant);

                correctionMenant += ajustement;
                correctionSuivant -= ajustement;
            }

            correctionMenant = minmax(0.8, correctionMenant, 1.2);
            correctionSuivant = minmax(0.8, correctionSuivant, 1.2);
        }
        demarrer(0,0);

        distMenant += ENCODER_ReadReset(roueMenante);
        distSuivant += ENCODER_ReadReset(!roueMenante);

        g_diffTournerG = roueMenante == LEFT ? distMenant - 1990 : distSuivant + 1990;
        g_diffTournerD = roueMenante == LEFT ? distSuivant + 1990 : distMenant - 1990;
    }

    void MoteurPabst::avanceDistance(float distance){
        //                          PULSE/TOUR         CIRCONFÉRENCE
        const int PULSES_A_PARCOURIR = 3200 * (distance / 0.239389);
        float vitesseG = 0;
        float vitesseD = 0;
        float encodeurG = 0;
        float encodeurD = 0;
        float distG = g_diffAvancerG;
        float distD = g_diffAvancerD;
        float correctionG = 1;
        float correctionD = 1;
        
        while(distG < PULSES_A_PARCOURIR || distD < PULSES_A_PARCOURIR)
        {
            const float dist = (distG + distD) / 2;
            const float VITESSE_BASE = (0.6 - 0.06) * 1.0 * sin((PI*dist)/PULSES_A_PARCOURIR) + 0.06;
            vitesseG = vitesseD = min(VITESSE_BASE, 0.6);
            MOTOR_SetSpeed(LEFT, vitesseG * correctionG);
            MOTOR_SetSpeed(RIGHT, vitesseD * correctionD);
            delay(20);
            distG += encodeurG = ENCODER_ReadReset(LEFT);
            distD += encodeurD = ENCODER_ReadReset(RIGHT);
            
            if (encodeurG > 0 && encodeurD > 0) {
                const float CORRECTION = correction(encodeurG, encodeurD, distG, distD);
                correctionG += CORRECTION;
                correctionD -= CORRECTION;
            }
            
            correctionG = minmax(0.8, correctionG, 1.2);
            correctionD = minmax(0.8, correctionD, 1.2);
        }
        
        MOTOR_SetSpeed(LEFT, 0);
        MOTOR_SetSpeed(RIGHT, 0);
        distG += ENCODER_ReadReset(LEFT);
        distD += ENCODER_ReadReset(RIGHT);
        g_diffAvancerG = distG - PULSES_A_PARCOURIR;
        g_diffAvancerD = distD - PULSES_A_PARCOURIR;
    }
}
