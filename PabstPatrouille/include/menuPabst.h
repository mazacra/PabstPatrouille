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
            MenuPabst() {
                lcd.init();
                lcd.backlight();
            }

            void MenuStart();
            void MenuSelectDiff();
        private:

    };
    
    void MenuPabst::MenuStart()
    {
        Serial.print("2");

        lcd.setCursor(0,0);
        lcd.println("--------Menu--------");
        lcd.println("    Devant: score   ");
        lcd.println("Derriere:  commencer");
        lcd.print("--------------------");

        while (true)
        {
            if(ROBUS_IsBumper(BUMPAVANT))
            {
                MenuSelectDiff();
                return;
            }
            if(ROBUS_IsBumper(BUMPARRIERE))
            {
                return;
            }
        } 
    }

    void MenuPabst::MenuSelectDiff()
    {
        int cpt, diff;

        lcd.setCursor(0,0);
        lcd.println("Selection difficulte");
        lcd.println("<--      0       -->");
        lcd.println("Derriere:  commencer");
        lcd.print("--------------------");

        while (true)
        {
            if(ROBUS_IsBumper(BUMPARRIERE)){
                cpt = game.startGame(diff);
		        game.endGame(cpt);
                return;
            }

            if(ROBUS_IsBumper(BUMPDROITE)){
                if(diff < 1)
                {
                    diff++;
                    lcd.setCursor(1,9);
                    lcd.print(diff);
                }
            }
            if(ROBUS_IsBumper(BUMPGAUCHE)){
                if(diff > 0)
                {
                    diff--;
                    lcd.setCursor(1,9);
                    lcd.print(diff);
                }
            }
        }
        
    }
}
