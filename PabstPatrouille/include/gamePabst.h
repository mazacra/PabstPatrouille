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
    int etape = 0;
    int cptrMode3 = 0;
    

    class GamePabst
    {
        public:
            //signatures des méthode pour les moteur de déplacement
            void startGame(int diff);
            void currentGame(int &pointsVert, int &pointsOrange);
            void endGame(int cpt);
            void nettoyage(int cpt);
            void retourHome();
            void mode1();
            void mode2();
            void mode3();
            void mode4();
            void joueurGagnant(int pointsVert, int pointsOrange, int scoreGagnat, char couleurGagnante[]);

            int pointsVert;
            int pointsOrange;
            int scoreGagnant;
            char couleurGagnante[10];

        private:
            
    };

    void GamePabst::startGame(int diff)
    {
        //Déplacer le robot dans la "zone" jeu
        moteur.tDroite(180);
        moteur.vitesseConstante(150, 0.5, 0.5); //Changer la distance où on veux dans le 2x2 mètre
        Serial.println("123");
        
        switch (diff)
        {
            case 1:
                moteur.tDroite(90);
                break;
            
            case 2:
                break;

            case 3:
                break;

             case 4:
                moteur.tGauche(90);
                moteur.vitesseConstante(30, 0.3, 0.3);
                moteur.tDroite(90);
                break;
            
            default:
                Serial.println("non");
                moteur.tDroite(180);
                break;
        }
        moteur.arret();
        delay(3000);

        currentGame(pointsVert, pointsOrange); //pour savoir cb de points
        //return currentGame();
    }

    void GamePabst::mode1() //pas tester encore
    {
        bool condition = true;
        float vitesse = 0.2;
        if(etape == 0)
        {
            if(((50 / CIRCONFERENCE_ROUE) * 3200) > ENCODER_Read(LEFT))
                moteur.demarrer(vitesse, vitesse);
            else
            {
                etape = 1;
                ENCODER_Reset(LEFT);
            }
        }
        if(etape == 1)
        {
            if(((100 / CIRCONFERENCE_ROUE) * -3200) < ENCODER_Read(LEFT))
                moteur.demarrer(-vitesse, -vitesse);
            else
            {
                etape = 2;
                ENCODER_Reset(LEFT);
            }
        }
        if(etape == 2)
        {
            if(((100 / CIRCONFERENCE_ROUE) * 3200) > ENCODER_Read(LEFT))
                moteur.demarrer(vitesse, vitesse);
            else
            {
                etape = 1;
                ENCODER_Reset(LEFT);
            }
        }
    }

    void GamePabst::mode2()//pas tester encore
    {
        static int coterAleatoire;
        static int angleAleatoire;
        int valeurEncoder;
        static float vitesseAleatoire;
 
        if(etape == 0)
        {
            if(ROBUS_ReadIR(2) < 150)//avancer jusqua une certaine distance du mur
                moteur.demarrer(0.2, 0.2);
            else
            {
                coterAleatoire = random(2);
                angleAleatoire = random(100,180);
                valeurEncoder = ((2 * PI * DISTANCE_ENTRE_ROUE * angleAleatoire / 360) * 3200 / CIRCONFERENCE_ROUE)/2;
                etape = 1;
                moteur.arret();
                ENCODER_Reset(LEFT);
                ENCODER_Reset(RIGHT);
            }
        }
        if(etape == 1)
        {
            Serial.println(coterAleatoire);
            if(valeurEncoder > ENCODER_Read(RIGHT) && coterAleatoire == 0)
                moteur.demarrer(-0.2, 0.2);
            else if(valeurEncoder > ENCODER_Read(LEFT) && coterAleatoire == 1)
                moteur.demarrer(0.2, -0.2);
            else
            {
                int nbsAleatoire = random(20,31);
                vitesseAleatoire = nbsAleatoire * 0.01;
                etape = 2;
                moteur.arret();
                ENCODER_Reset(LEFT);
                ENCODER_Reset(RIGHT);
            }
        }
        if(etape == 2)
        {
            if(ROBUS_ReadIR(2) < 150)//avancer jusqua une certaine distance du mur
                moteur.demarrer(vitesseAleatoire, vitesseAleatoire);
            else
            {
                coterAleatoire = random(2);
                angleAleatoire = random(100,180);
                valeurEncoder = ((2 * PI * DISTANCE_ENTRE_ROUE * angleAleatoire / 360) * 3200 / CIRCONFERENCE_ROUE)/2;
                etape = 1;
                moteur.arret();
                ENCODER_Reset(LEFT);
                ENCODER_Reset(RIGHT);
            }
        }
    }

    void GamePabst::mode3()//sa marche mais regarder pour les valeur d'encodeur
    {
        float vitesseRoueExterieur = 0.25;
        
        float roueInterieurDistance = (2 * PI * (10)) / 4;
        float roueExterieurDistance = (2 * PI * (10 + DISTANCE_ENTRE_ROUE)) / 4;
        //Calcul la vitesse à laquelle la roue droite doit s'ajuster selon la vitesse de la roue gauche
        float rapport = roueInterieurDistance / roueExterieurDistance;
        float vitesseRoueInterieur = vitesseRoueExterieur * rapport;
        
        if(etape == 0)
        {
            if(((roueInterieurDistance / CIRCONFERENCE_ROUE) * 3200) + 1200 > ENCODER_Read(LEFT))
                moteur.demarrer(vitesseRoueInterieur, vitesseRoueExterieur);
            else
            {
                moteur.arret();
                etape = 1;
                ENCODER_Reset(RIGHT);
                cptrMode3++;
            }
        }
        if(etape == 1)
        {
            if(((roueInterieurDistance / CIRCONFERENCE_ROUE) * 3200) + 1200 > ENCODER_Read(RIGHT))
                moteur.demarrer(vitesseRoueExterieur, vitesseRoueInterieur);
            else
            {
                moteur.arret();
                etape = 0;
                ENCODER_Reset(LEFT);
                ENCODER_Reset(RIGHT);
                cptrMode3++;
            }
        }
        Serial.println("compteur");
        Serial.println(cptrMode3);
        Serial.println("etape");
        Serial.println(etape);
        if(((cptrMode3 - 2) % 5) == 0 || etape == 3)
        {
            etape = 3;
            if(((roueInterieurDistance / CIRCONFERENCE_ROUE) * 3200) + 1200 > ENCODER_Read(RIGHT))
                moteur.demarrer(vitesseRoueExterieur, vitesseRoueInterieur);
            else
            {
                etape = 0;
                ENCODER_Reset(LEFT);
                cptrMode3++;
            }
        }
    }

    void GamePabst::mode4()//sa marche
    {
        float vitesseRoueExterieur = 0.25;
        
        float roueInterieurDistance = (2 * PI * (36.37)) / 4;
        float roueExterieurDistance = (2 * PI * (55.07)) / 4;
        //Calcul la vitesse à laquelle la roue droite doit s'ajuster selon la vitesse de la roue gauche
        float rapport = roueInterieurDistance / roueExterieurDistance;
        float vitesseRoueInterieur = vitesseRoueExterieur * rapport;
        moteur.demarrer(vitesseRoueExterieur, vitesseRoueInterieur);
    }

    void GamePabst::currentGame(int &pointsVert, int &pointsOrange)
    {
        pointsVert = 0;
        pointsOrange = 0;

        while (millis() < (tempsStart + (60000)))
        {
            if(module.detectionBallePanierTemp() == 1) //si détecte balle verte
            {
                pointsVert++;
            }

            if(module.detectionBallePanierTemp() == 2) //si détecte balle orange
            {
                pointsOrange++;
            }
        }
        Serial.println("GAME DONE");

        joueurGagnant(pointsVert, pointsOrange, scoreGagnant, couleurGagnante);//joueur gagnant
    }

    void GamePabst::endGame(int cpt)
    {
        //nettoyage(cpt);
    }

    void prepNettoyage(){
        while (ROBUS_ReadIR(2) < 400)
        {
            MOTOR_SetSpeed(LEFT, 0.16);
            MOTOR_SetSpeed(RIGHT, 0.16);
        }
        while (ROBUS_ReadIR(3) < 450)
        {
            MOTOR_SetSpeed(LEFT, -0.16);
            MOTOR_SetSpeed(RIGHT, 0.16);
        }
        MOTOR_SetSpeed(LEFT, 0);
        MOTOR_SetSpeed(RIGHT, 0);
        
        delay(2000);

        float vg = 0.16;

        while (ROBUS_ReadIR(2) < 400)
        {
            MOTOR_SetSpeed(LEFT, vg);
            MOTOR_SetSpeed(RIGHT, 0.16);

            if (ROBUS_ReadIR(1) > 335)
            {
                Serial.println("trop proche");
                Serial.println(ROBUS_ReadIR(1));
                MOTOR_SetSpeed(LEFT, 0.20);
                MOTOR_SetSpeed(RIGHT, 0.40);	
            }
            if (ROBUS_ReadIR(1) < 325 && ROBUS_ReadIR(1) > 240)
            {
                Serial.println("trop loin");
                Serial.println(ROBUS_ReadIR(1));
                MOTOR_SetSpeed(LEFT, 0.40);
                MOTOR_SetSpeed(RIGHT, 0.20);
            }
            //if(ROBUS_ReadIR(1) < ROBUS_ReadIR(3)){
            //    Serial.println("Gauche");
            //    if(vg < 0.20)
            //        vg += 0.01;
            //}
            //if(ROBUS_ReadIR(1) > ROBUS_ReadIR(3)){
            //    Serial.println("Droite");
            //    if(vg > 0.1)
            //        vg -= 0.01;
            //}
        }
        while (ROBUS_ReadIR(1) + 50 > ROBUS_ReadIR(3) && ROBUS_ReadIR(1) - 50 < ROBUS_ReadIR(3))
        {
            MOTOR_SetSpeed(0, 0.15);
            MOTOR_SetSpeed(1, -0.15);
        }
        
        MOTOR_SetSpeed(0, 0);
        MOTOR_SetSpeed(1, 0);
        ENCODER_Reset(0);
        ENCODER_Reset(1);
        moteur.tourneDir(RIGHT);
        moteur.tourneDir(RIGHT);
    }

    void nettoyage(){
        bool dirTournant = 1;

        prepNettoyage();
        while (true)
        {
            moteur.avanceDistance(1.7);
            moteur.tourneDir(dirTournant);
            if(ROBUS_ReadIR(2) > 100 && ROBUS_ReadIR(1) > 100){
                break;
            }
            moteur.avanceDistance(0.3);
            moteur.tourneDir(dirTournant);
            dirTournant = dirTournant == 0 ? 1 : 0;
        }
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

    void GamePabst::joueurGagnant(int pointsVert, int pointsOrange, int scoreGagnat, char couleurGagnate[])
    {
        int scoreGagnant = 0;
        char couleurGagnante[10] = "";

        if (pointsVert > pointsOrange)
        {
            scoreGagnant = pointsVert;
            strcpy(couleurGagnante, "Vert");
        }

        if (pointsVert < pointsOrange)
        {
            scoreGagnant = pointsOrange;
            strcpy(couleurGagnante, "Orange");
        }

        if (pointsVert == pointsOrange)
        {
            scoreGagnant = pointsVert;
            strcpy(couleurGagnante, "Egalite");
        }
    }
}
