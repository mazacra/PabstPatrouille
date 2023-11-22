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
                break;
            
            default:
                Serial.println("non");
                moteur.tDroite(180);
                break;
        }

        return currentGame();
    }

    void GamePabst::mode1()
    {
        Serial.println("debut");
        moteur.avancerMode1(50, tempsStart);
        Serial.println(tempsStart);

        while(true)
        {
            Serial.println("boucle");
            if(!moteur.reculerMode1(100,tempsStart))
                break;

            moteur.arret();
            
            if(!moteur.avancerMode1(100,tempsStart))
                break;

            moteur.arret();
        }
        Serial.println(millis() - tempsStart);
        Serial.println("fin");
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
                moteur.demarrer(0, 0);
                moteur.vitesseConstante(distance, 0.2, 0.2);
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
                    while (SONAR_GetRange(0) > 100)
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
