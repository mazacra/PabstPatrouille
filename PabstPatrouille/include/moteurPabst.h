#include <LibRobus.h>
namespace Moteur
{
    #define DISTANCE_ENTRE_ROUE 18.7
    #define CIRCONFERENCE_ROUE 23.939

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
	    ENCODER_Reset(LEFT);	//Reset encoder
	    ENCODER_Reset(RIGHT);	//Reset encoder

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
    }

    void MoteurPabst::vitesseConstante(float distance, float vitesseg,float vitessed) //distance à parcourir en cm
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
	    	MOTOR_SetSpeed(LEFT, vitesseG);											//Changement de vitesse
	    	MOTOR_SetSpeed(RIGHT, vitesseD);										//Changement de vitesse
	    	delay(idelay);
			
		    vitesseG = (vitesseG - diffClic()*ponderation);
		    vitesseD = (vitesseD + diffClic()*ponderation);
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
        float vitesseG = 0.15;
        float vitesseD = 0.15;
	    float ponderation = 0.0001;
	    float distanceEncoder = (distance / CIRCONFERENCE_ROUE) * 3200;

	    ENCODER_Reset(LEFT);
	    ENCODER_Reset(RIGHT);

	    while (!(ENCODER_Read(RIGHT) > distanceEncoder && ENCODER_Read(LEFT) > distanceEncoder))
	    {
            Serial.println("entrer");
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
}
