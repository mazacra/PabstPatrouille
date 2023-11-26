#include "moteurPabst.h"
#include "modulePabst.h"
#include <Arduino.h>
#include <LibRobus.h>

using namespace Moteur;
using namespace Module;

namespace Game
{
    MoteurPabst moteur;
    ModulePabst module;
    float tempsStart;
    int angle = 0;

    class GamePabst
    {
        public:
            //signatures des méthode pour les moteur de déplacement
            int startGame(int diff);
            int currentGame();
            void endGame(int cpt);
            void nettoyage(int cpt);
            void retourHome();
            void mode1();
            void mode2();
            void mode3();
            void mode4();

        private:
            
    };

    int GamePabst::startGame(int diff)
    {
        //Déplacer le robot dans la "zone" jeu
        moteur.tDroite(180);
        moteur.vitesseConstante(150, 0.5, 0.5); //Changer la distance où on veux dans le 2x2 mètre
        Serial.println("123");
        
        switch (diff)
        {
            case 1:
                moteur.tDroite(90);
                tempsStart = millis();
                mode1();
                moteur.arret();
                delay(1000);
                break;
            
            case 2:
                tempsStart = millis();
                mode2();
                moteur.arret();
                delay(1000);
                break;

            case 3:
                tempsStart = millis();
                mode3();
                moteur.arret();
                delay(1000);
                break;

             case 4:
                tempsStart = millis();
                mode4();
                moteur.arret();
                delay(1000);
                break;
            
            default:
                Serial.println("non");
                moteur.tDroite(180);
                break;
        }
        moteur.arret();
        delay(3000);

        return currentGame();
    }

    void GamePabst::mode1() //pas tester encore
    {
        bool condition = true;
        float vitesse = 0.2;
        ENCODER_Reset(LEFT);
        while(((50 / CIRCONFERENCE_ROUE) * 3200) > ENCODER_Read(LEFT))
        {
            MOTOR_SetSpeed(LEFT, vitesse);											//Changement de vitesse
	    	MOTOR_SetSpeed(RIGHT, vitesse);	
        }
        while(condition)
        {
            ENCODER_Reset(LEFT);
            while(((100 / CIRCONFERENCE_ROUE) * -3200) > ENCODER_Read(LEFT))
            {
                MOTOR_SetSpeed(LEFT, -vitesse);											//Changement de vitesse
	    	    MOTOR_SetSpeed(RIGHT, -vitesse);
                if((millis() - tempsStart) >= 60000)
                {
                    condition = false;
                    break;
                }	
            }

            ENCODER_Reset(LEFT);
            while(((100 / CIRCONFERENCE_ROUE) * 3200) > ENCODER_Read(LEFT))
            {
                MOTOR_SetSpeed(LEFT, vitesse);											//Changement de vitesse
	    	    MOTOR_SetSpeed(RIGHT, vitesse);
                if((millis() - tempsStart) >= 60000)
                {
                    condition = false;
                    break;
                }	
            }
        }
    }

    void GamePabst::mode2()//pas tester encore
    {
        bool condition = true;
        while(condition)
        {
            int coterAleatoire = random(1);
            int angleAleatoire = random(100,180);
            int valeurEncoder = ((2 * PI * DISTANCE_ENTRE_ROUE * angleAleatoire / 360) * 3200 / CIRCONFERENCE_ROUE)/2;
            float vitesseTourner = 0.2;
            int vitesseAleatoire = random(20,30);
            vitesseAleatoire *= 0.01;

            ENCODER_Reset(LEFT);
            if(coterAleatoire == 0)
            {
                while(valeurEncoder > ENCODER_Read(LEFT))
                {
                    moteur.demarrer(-vitesseTourner, vitesseTourner);
                    if((millis() - tempsStart) >= 60000)
                    {
                        condition = false;
                        break;
                    }
                }
            }
            else
            {
                while(valeurEncoder > ENCODER_Read(LEFT))
                {
                    moteur.demarrer(-vitesseTourner, vitesseTourner);
                    if((millis() - tempsStart) >= 60000)
                    {
                        condition = false;
                        break;
                    }
                }
            }
            
            while(true)             //tant que le capteur sonore n'est pas a une certaine distance du mur
            {
                if((millis() - tempsStart) >= 60000)
                {
                    condition = false;
                    break;
                }
                moteur.demarrer(vitesseAleatoire, vitesseAleatoire);
            }
        }
    }

    void GamePabst::mode3()//sa marche mais regarder pour les distances une fois dans larene
    {
        Serial.println("dedans");
        bool condition = true;

        float vitesseRoueExterieur = 0.25;
        
        float roueInterieurDistance = (2 * PI * (36.37)) / 4;
	    float roueExterieurDistance = (2 * PI * (55.07)) / 4;
	    //Calcul la vitesse à laquelle la roue droite doit s'ajuster selon la vitesse de la roue gauche
	    float rapport = roueInterieurDistance / roueExterieurDistance;
	    float vitesseRoueInterieur = vitesseRoueExterieur * rapport;
        int cptr = 0;
            
        while(condition)             //tant que le capteur sonore n'est pas a une certaine distance du mur
        {
            ENCODER_Reset(LEFT);
            while(11800 > ENCODER_Read(LEFT))
            {
                Serial.println("tournant 1");
                moteur.demarrer(vitesseRoueInterieur, vitesseRoueExterieur);
                if((millis() - tempsStart) >= 60000)
                {
                    condition = false;
                    break;
                }
            }

            ENCODER_Reset(RIGHT);
            while(11800 > ENCODER_Read(RIGHT))
            {
                moteur.demarrer(vitesseRoueExterieur, vitesseRoueInterieur);
                if((millis() - tempsStart) >= 60000)
                {
                    condition = false;
                    break;
                }
            }

            if((cptr % 2) == 0)
            {
                ENCODER_Reset(RIGHT);
                Serial.println("tourner complet");
                while(11800 > ENCODER_Read(RIGHT))
                {
                    moteur.demarrer(vitesseRoueExterieur, vitesseRoueInterieur);
                    if((millis() - tempsStart) >= 60000)
                    {
                        condition = false;
                        break;
                    }
                }
            }
            cptr++;
        }
    }

    void GamePabst::mode4()//sa marche
    {
        bool condition = true;
        float vitesseRoueExterieur = 0.25;
        
        float roueInterieurDistance = (2 * PI * (36.37)) / 4;
	    float roueExterieurDistance = (2 * PI * (55.07)) / 4;
	    //Calcul la vitesse à laquelle la roue droite doit s'ajuster selon la vitesse de la roue gauche
	    float rapport = roueInterieurDistance / roueExterieurDistance;
	    float vitesseRoueInterieur = vitesseRoueExterieur * rapport;

        while(condition)
        {
            moteur.demarrer(vitesseRoueExterieur, vitesseRoueInterieur);
            if((millis() - tempsStart) >= 60000)
            {
                condition = false;
                break;
            }
        }
    }

    int GamePabst::currentGame()
    {
        int pointCounter = 0;

        while (millis() < (tempsStart + (60000)))
        {
            if(module.detectionBallePanier()) //si détecte qqc
            {
                pointCounter++;
            }
        }
        Serial.println("GAME DONE");
        return pointCounter;
    }

    void GamePabst::endGame(int cpt)
    {
        nettoyage(cpt);
    }

    void GamePabst::nettoyage(int cpt)
    {
        float distance = 0;
        
        while(cpt<10){
            moteur.demarrer(0.2, -0.2);
            if(module.detectionBalleSol(distance))
            {
                Serial.println(distance);
                moteur.demarrer(0, 0);
                moteur.vitesseConstante(distance, 0.2, 0.2);
                moteur.arret();
                cpt++;
                delay(500);
            }
        }

        retourHome();
    }

    void GamePabst::retourHome()
    {
        if (angle != 0 || angle != 180)
        {
            //On ce remet à 0 deg
            if(angle == 180)
                moteur.tGauche(angle);
            else
                moteur.tDroite(360 - angle);

            //Avance jusqu'au mur "sud"
            while(SONAR_GetRange(0) > 10){
                moteur.demarrer(0.2, 0.2);
            }
            moteur.demarrer(0, 0);

            //Tourne vers le centre
            if(angle <= 180)
            {
                moteur.tGauche(90);
                if(SONAR_GetRange(0) < 100) //Si pas du bon coté (juste au cas où)
                    moteur.tGauche(180);
            }
            else
            {
                moteur.tDroite(90);
                if(SONAR_GetRange(0) < 100) //Si pas du bon coté (juste au cas où)
                    moteur.tDroite(180);
            }

            //avancer jusqu'au centre
            while(SONAR_GetRange(0) > 100){
                moteur.demarrer(0.2, 0.2);
            }
            moteur.demarrer(0, 0);

            //cherche le bord
            while(SONAR_GetRange(0) > 10){
                moteur.demarrer(0.2, -0.2);
            }
            moteur.demarrer(0,0);
        }
        else
        {
            moteur.tDroite(90);

            //angle 0 et 180
            if(angle == 180)
            {
                //regarde à droite
                if(SONAR_GetRange(0) > 100)
                {
                    //trop a gauche
                    while (SONAR_GetRange(0) > 100)
                    {
                        moteur.demarrer(0.2, 0.2);
                    }
                    moteur.demarrer(0, 0);
                    moteur.tDroite(90);
                }
                else if (SONAR_GetRange(0) < 100)
                {
                    //trop a droite
                    moteur.tDroite(180);
                    while(SONAR_GetRange(0) > 100)
                    {
                        moteur.demarrer(0.2, 0.2);
                    }
                    moteur.demarrer(0, 0);
                    moteur.tGauche(90);
                }
                else
                {
                    moteur.tDroite(180);
                }
            }
            else
            {
                //regarde à gauche
                if(SONAR_GetRange(0) > 100)
                {
                    //trop a droite
                    while (SONAR_GetRange(0) > 100)
                    {
                        moteur.demarrer(0.2, 0.2);
                    }
                    moteur.demarrer(0, 0);
                    moteur.tGauche(90);
                }
                else if (SONAR_GetRange(0) < 100)
                {
                    //trop a gauche
                    moteur.tDroite(180);
                    while (SONAR_GetRange(0) > 100)
                    {
                        moteur.demarrer(0.2, 0.2);
                    }
                    moteur.demarrer(0, 0);
                    moteur.tDroite(90);
                }
            }

            //avance jusqu'au homebase
            while (SONAR_GetRange(0) > 10)
            {
                moteur.demarrer(0.2, 0.2);
            }
            moteur.demarrer(0, 0);
        }
    }
}
