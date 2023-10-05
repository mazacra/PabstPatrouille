//Fichier pour la détection du sifflet. A mettre dans le main plus tard

#include <LibRobus.h>
#define INTENSITE_5KHZ 511

void init()
{
    pinMode(PIN_A4, INPUT);
    pinMode(PIN_A5, INPUT);
    
}

bool Detect5KHZ()
{
    while (loop)
    {
        if (digitalRead(PIN_A4) == INTENSITE_5KHZ)

        return true;
        break;
    }
}