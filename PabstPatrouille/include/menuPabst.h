#include <LibRobus.h>
#include <LiquidCrystal_I2C.h>
#include "gamePabst.h"

using namespace Game;

/*#define BUMPARRIERE 3
#define BUMPAVANT 2
#define BUMPDROITE 1
#define BUMPGAUCHE 0*/

#define BOUTONBLEU 37
#define BOUTONROUGE 41
#define BOUTONVERT 38
#define BOUTONJAUNE 39
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
            void MenuFin(int pointV, int pointO);
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
        lcd.print("  Vert:  commencer"); 
        lcd.setCursor(0,3);
        lcd.print("--------------------");

        while (true)
        {
            if(digitalRead(BOUTONJAUNE))
            {
                //MenuPabst::MenuTopFive();
            }

            if(digitalRead(BOUTONVERT))
            {
                while(digitalRead(BOUTONVERT)){}
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
        lcd.print("Vert:  commencer");
        lcd.setCursor(0,3);
        lcd.print("--------------------");
        
        delay(500);
        while (true)
        {
            if(digitalRead(BOUTONVERT)){
                while(digitalRead(BOUTONVERT)){}
                lcd.clear();
                Serial.println(diff);
                return MenuPabst::MenuSelectMulti(diff);
            }

            if(digitalRead(BOUTONBLEU)){
                if(diff < 4)
                {
                    diff++;
                    lcd.setCursor(9,1);
                    lcd.print(diff);
                    while (digitalRead(BOUTONBLEU)){}
                    delay(100);
                }
            }
            if(digitalRead(BOUTONJAUNE)){
                if(diff > 0)
                {
                    diff--;
                    lcd.setCursor(9,1);   
                    lcd.print(diff);
                    while (digitalRead(BOUTONJAUNE)){}
                    delay(100);
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
            if(digitalRead(BOUTONVERT)){
                lcd.clear();
                Serial.println(mutlijoueur);
                return game.startGame(diff, mutlijoueur);
                //return cpt;
            }

            if(digitalRead(BOUTONBLEU)){
                mutlijoueur = true;
                lcd.setCursor(7,1);
                lcd.print("MULTI");
                while (digitalRead(BOUTONBLEU)){}
            }
            if(digitalRead(BOUTONJAUNE)){
                mutlijoueur = false;
                lcd.setCursor(7,1);
                lcd.print(" SOLO");
                while (digitalRead(BOUTONJAUNE)){}
            }
        }
    }

    void MenuPabst::MenuFin(int pointV, int pointO = -1)
    {
        lcd.setCursor(0,0);
        lcd.print("-----Bien joue!-----");
        lcd.setCursor(0,1);
        if (pointO == -1)
        {
            lcd.print(" POINTS :  ");
            lcd.print(game.pointsVert);
        }
        else
        {        
            lcd.print("VERT : ");
            lcd.print(game.pointsVert);
            lcd.setCursor(0,2);
            lcd.print("ORANGE : ");
            lcd.print(game.pointsOrange);
        }

        lcd.setCursor(1,3);
        lcd.print("Rouge :  Quitter");

        game.nettoyage();

        while (true)
        {
            if(digitalRead(BOUTONROUGE))
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
