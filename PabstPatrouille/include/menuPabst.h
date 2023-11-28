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
            int MenuStart();
            int MenuSelectDiff();
            void MenuFin(int cpt);
        private:

    };
    
    int MenuPabst::MenuStart()
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

    int MenuPabst::MenuSelectDiff()
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
                cpt = game.startGame(diff);
	            game.endGame(cpt);
                return cpt;
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

    void MenuPabst::MenuFin(int cpt)
    {
        lcd.setCursor(0,0);
        lcd.print("-----Bien joué!-----");
        lcd.setCursor(6,1);
        lcd.print(cpt);
        lcd.print(" points");
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

}
