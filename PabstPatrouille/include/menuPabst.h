#include <LibRobus.h>
#include <LiquidCrystal_I2C.h>
#include "gamePabst.h"

using namespace Game;

#define BUMPARRIERE 3
#define BUMPAVANT 2
#define BUMPDROITE 1
#define BUMPGAUCHE 0

GamePabst game;

namespace Menu
{

    LiquidCrystal_I2C lcd(0x27, 20, 4);  

    class MenuPabst
    {
        public:
            void initializeTopScores();
            void MenuStart();
            void MenuSelectDiff();
            void MenuFin();
            void MenuTopFive(int top[]);

            static int top[5];

        private:

    };

    // Initialisation des scores au démarrage du programme
    void MenuPabst::initializeTopScores()//initialiser avec setup()
    {
        for (int i = 0; i < 5; i++)
        {
            top[i] = 0;
        }
    }

    void MenuPabst::MenuStart()
    {
        lcd.setCursor(0,0);
        lcd.print("--------Menu--------");
        lcd.setCursor(0,1);
        lcd.print("    Devant: score   ");
        lcd.setCursor(0,2);
        lcd.print("Derriere:  commencer");
        lcd.setCursor(0,3);
        lcd.print("--------------------");

        while (true)
        {
            if(ROBUS_IsBumper(BUMPAVANT))
            {
                return;
            }
            if(ROBUS_IsBumper(BUMPARRIERE))
            {
                return MenuSelectDiff();
            }
        } 
    }

    void MenuPabst::MenuSelectDiff()
    {
        int cpt;
        int diff = 0;

        lcd.setCursor(0,0);
        lcd.print("Selection difficulte");
        lcd.setCursor(0,1);
        lcd.print("<--      0       -->");
        lcd.setCursor(0,2);
        lcd.print("Derriere:  commencer");
        lcd.setCursor(0,3);
        lcd.print("--------------------");
        
        delay(500);
        while (true)
        {
            if(ROBUS_IsBumper(BUMPARRIERE)){
                lcd.clear();
                //cpt = game.startGame(diff);
                cpt = game.pointsVert + game.pointsOrange; //points total
	            game.endGame(cpt);
                //return cpt;
            }

            if(ROBUS_IsBumper(BUMPDROITE)){
                if(diff < 4)
                {
                    diff++;
                    lcd.setCursor(9,1);
                    lcd.print(diff);
                    while (ROBUS_IsBumper(BUMPDROITE)){}
                    
                }
            }
            if(ROBUS_IsBumper(BUMPGAUCHE)){
                if(diff > 0)
                {
                    diff--;
                    lcd.setCursor(9,1);
                    lcd.print(diff);
                    while (ROBUS_IsBumper(BUMPGAUCHE)){}
                }
            }
        }
    }

    void MenuPabst::MenuFin()
    {
        lcd.setCursor(0,0);
        lcd.print("-----Bien joué!-----");
        lcd.setCursor(6,1);
        lcd.print(game.pointsVert);
        lcd.print(" points pour Vert");
        lcd.setCursor(7,1);
        lcd.print(game.pointsOrange);
        lcd.print(" points pour Orange");
        lcd.setCursor(0,2);
        lcd.print("Derriere:  quitter");
        lcd.setCursor(0,3);
        lcd.print("--------------------");

        while (true)
        {
            if(ROBUS_IsBumper(BUMPARRIERE))
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
