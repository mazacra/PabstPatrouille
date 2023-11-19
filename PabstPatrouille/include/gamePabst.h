#include "moteurPabst.h"
#include <Arduino.h>
#include <LibRobus.h>

using namespace Moteur;

namespace Game
{
    MoteurPabst moteur;
    float tempsStart;

    class GamePabst
    {
        public:
            //signatures des méthode pour les moteur de déplacement
            int startGame(int diff);
            int currentGame();
            int endGame();
            void nettoyage();
            void retourHome(int angle);
        private:
            
    };

    int GamePabst::startGame(int diff)
    {
        //Déplacer le robot dans la "zone" jeu
        moteur.tDroite(180);
        moteur.vitesseConstante(150, 0.5, 0.5); //Changer la distance où on veux dans le 2x2 mètre
        
        switch (diff)
        {
            case 1:
                /* code */
                break;
            
            default:
                moteur.tDroite(180);
                break;
        }

        tempsStart = millis();

        return currentGame();
    }

    int GamePabst::currentGame()
    {
        int pointCounter = 0;

        while (millis() < (tempsStart + (1000 * 60)))
        {
            if(true) //si détecte qqc
            {
                pointCounter++;
            }
        }

        return pointCounter;
    }

    void GamePabst::nettoyage()
    {
        int angle = 0;

        retourHome(angle);
    }

    void GamePabst::retourHome(int angle)
    {
        if (angle != 0 || angle != 180)
        {
            //On ce remet à 0 deg
            if(angle = 180)
                tGauche(angle);
            else
                tDroite(360 - angle);

            //Avance jusqu'au mur "sud"
            while(SONAR_GetRange(0) > 10){
                demarrer(0.2, 0.2);
            }
            demarrer(0, 0);

            //Tourne vers le centre
            if(angle <= 180)
            {
                tGauche(90);
                if(SONAR_GetRange(0) < 100) //Si pas du bon coté (juste au cas où)
                    tGauche(180);
            }
            else
            {
                tDroite(90);
                if(SONAR_GetRange(0) < 100) //Si pas du bon coté (juste au cas où)
                    tDroite(180);
            }

            //avancer jusqu'au centre
            while(SONAR_GetRange(0) > 100){
                demarrer(0.2, 0.2);
            }
            demarrer(0, 0);

            //cherche le bord
            while(SONAR_GetRange(0) > 10){
                demarrer(0.2, -0.2);
            }
            demarrer(0,0);
        }
        else
        {
            tDroite(90);
            short dir;

            //angle 0 et 180
            if(angle == 180)
            {
                //regarde à droite
                if(SONAR_GetRange(0) > 100)
                {
                    //trop a gauche
                    while (SONAR_GetRange(0) > 100)
                    {
                        demarrer(0.2, 0.2);
                    }
                    demarrer(0, 0);
                    tDroite(90);
                }
                else if (SONAR_GetRange(0) < 100)
                {
                    //trop a droite
                    tDroite(180);
                    while (SONAR_GetRange(0) > 100)
                    {
                        demarrer(0.2, 0.2);
                    }
                    demarrer(0, 0);
                    tGauche(90);
                }
                else
                {
                    tDroite(180);
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
                        demarrer(0.2, 0.2);
                    }
                    demarrer(0, 0);
                    tGauche(90);
                }
                else if (SONAR_GetRange(0) < 100)
                {
                    //trop a gauche
                    tDroite(180);
                    while (SONAR_GetRange(0) > 100)
                    {
                        demarrer(0.2, 0.2);
                    }
                    demarrer(0, 0);
                    tDroite(90);
                }
            }

            //avance jusqu'au homebase
            while (SONAR_GetRange(0) > 10)
            {
                demarrer(0.2, 0.2);
            }
            demarrer(0, 0);
        }
    }
}
