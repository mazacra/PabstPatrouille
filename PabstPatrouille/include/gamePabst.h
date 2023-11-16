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
}
