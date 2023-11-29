#include <LibRobus.h>
#include <LiquidCrystal_I2C.h>
#include "gamePabst.h"

using namespace Game;

#define BUMPARRIERE 3
#define BUMPAVANT 2
#define BUMPDROITE 1
#define BUMPGAUCHE 0

#define BOUTONROUGE 31
#define BOUTONBLEU 32
#define BOUTONJAUNE 33
#define BOUTONVERT 34
GamePabst game;

namespace Menu
{

    LiquidCrystal_I2C lcd(0x27, 20, 4);  

    class MenuPabst
    {
        public:
            void initializeTopScores();
            pointage MenuStart();
            pointage MenuSelectDiff();
            pointage MenuSelectMulti(int diff);
            void MenuDebutJeu();
            void AffichagePointInGame(int pointsVert, int pointsOrange);
            void MenuFin(int pointV, int pointO = -1);
            void MenuTopFive(int top[]);

            static int top[5];

        private:

    };
    //bumper brancher dans un numerique
    //digitalRead(leNumeroDeLaPine) pour avoir 


    // Initialisation des scores au démarrage du programme
    void MenuPabst::initializeTopScores()//initialiser avec setup()
    {
        for (int i = 0; i < 5; i++)
        {
            top[i] = 0;
        }
    }

    pointage MenuPabst::MenuStart()
    {
        lcd.setCursor(0,0);
        lcd.print("--------Menu--------");
        lcd.setCursor(0,1);
        lcd.print("    Jaune: score   ");
        lcd.setCursor(0,2);
        lcd.print("Vert:  commencer"); ///////////////////------
        lcd.setCursor(0,3);
        lcd.print("--------------------");

        while (true)
        {
            if(digitalRead(BOUTONJAUNE))//////////////////////
            {
                //MenuPabst::MenuTopFive();
            }
            Serial.println(digitalRead(BOUTONVERT));

            if(digitalRead(BOUTONVERT))///////////////////--------
            {
                while(digitalRead(BOUTONVERT));
                return MenuPabst::MenuSelectDiff();
            }
        } 
    }

    pointage MenuPabst::MenuSelectDiff()
    {
        int diff = 0;

        lcd.setCursor(0,0);
        lcd.print("Selection difficulte");
        lcd.setCursor(0,1);
        lcd.print("<--      0       -->");
        lcd.setCursor(0,2);
        lcd.print("Vert:  commencer"); //////////////////----
        lcd.setCursor(0,3);
        lcd.print("--------------------");
        
        delay(500);
        while (true)
        {
            if(digitalRead(BOUTONVERT)){/////////////////----
                while(digitalRead(BOUTONVERT));
                lcd.clear();
                return MenuPabst::MenuSelectMulti(diff);
                //return cpt;
            }

            if(digitalRead(BOUTONBLEU)){///////////////////////
                if(diff < 4)
                {
                    diff++;
                    lcd.setCursor(9,1);
                    lcd.print(diff);
                    while (digitalRead(BOUTONBLEU)){}//////////////
                    
                }
            }
            if(digitalRead(BOUTONJAUNE)){
                if(diff > 0)
                {
                    Serial.println("1");
                    diff--;
                    lcd.setCursor(9,1);   
                    lcd.print(diff);
                    while (digitalRead(BOUTONJAUNE)){}/////////////////

                }
            }
        }
    }

    pointage MenuPabst::MenuSelectMulti(int diff)
    {
        bool mutlijoueur = false;

        lcd.setCursor(0,0);
        lcd.print(" Selection  Joueurs ");
        lcd.setCursor(0,1);
        lcd.print("<--     SOLO     -->");
        lcd.setCursor(0,2);
        lcd.print("Vert:  commencer");
        lcd.setCursor(0,3);
        lcd.print("--------------------");
        
        delay(500);
        while (true)
        {
            if(digitalRead(BOUTONVERT)){/////////////////////
                lcd.clear();
                return game.startGame(diff, mutlijoueur);
                //return cpt;
            }

            if(digitalRead(BOUTONBLEU)){////////////////////// MULTIJOUEUR
                mutlijoueur = true;
                lcd.setCursor(7,1);
                lcd.print("MULTI");
                while (digitalRead(BOUTONBLEU)){}//////////////
            }
            if(digitalRead(BOUTONJAUNE)){
                mutlijoueur = false;
                lcd.setCursor(7,1);
                lcd.print(" SOLO");
                while (digitalRead(BOUTONJAUNE)){}////////////
            }
        }
    }

    void MenuDebutJeu()
    {
        lcd.clear();
        lcd.setCursor(0,2);
        lcd.print("---------GO---------");

    }

    void AffichagePointInGame(int pointsVert, int pointsOrange)
    {
        if (multiJ)
        {       
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("V");
            lcd.setCursor(0,1);
            lcd.print(pointsVert);
            lcd.setCursor(10,0);
            lcd.print("O");
            lcd.setCursor(10,1);
            lcd.print(pointsOrange);
        }

        if (!multiJ)
        {
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("POINTS");
            lcd.setCursor(0,1);
            lcd.print(pointsVert);
        }
    }

    void MenuPabst::MenuFin(int pointV, int pointO = -1)
    {
        lcd.setCursor(0,0);
        lcd.print("-----Bien joue!-----");
        lcd.setCursor(5,1);
        if (pointO == -1)
        {
            lcd.print(game.pointsVert);
            lcd.print(" points ");
        }
        else
        {        
            lcd.print("Vert : ");
            lcd.print(game.pointsVert);
            lcd.setCursor(3,2);
            lcd.print("Orange : ");
            lcd.print(game.pointsOrange);
        }

        lcd.setCursor(1,3);
        lcd.print("Derriere:  quitter");

        game.nettoyage();

        while (true)
        {
            if(digitalRead(BOUTONROUGE))/////////////
                return;
        }
        
    }

    void MenuPabst::MenuTopFive(int top[])
    {
        for (int i = 0; i < 5; i++)
        {
            if (top[i] < game.scoreGagnant)
            {
                top[i] = game.scoreGagnant;
                break;
            }
        }

        //Afficher le top5
        lcd.setCursor(0,0);
        lcd.print("--------Top 5--------");
        lcd.setCursor(0,1);
        lcd.print("    1.   ");
        lcd.print(top[0]);
        lcd.setCursor(0,2);
        lcd.print("    2.   ");
        lcd.print(top[1]);
        lcd.setCursor(0,3);
        lcd.print("    3.   ");
        lcd.print(top[2]);
        lcd.setCursor(0,4);
        lcd.print("    4.   ");
        lcd.print(top[3]);
        lcd.setCursor(0,5);
        lcd.print("    5.   ");
        lcd.print(top[5]);



    }
}
