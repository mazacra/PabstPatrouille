#include <LibRobus.h>
#include <Adafruit_TCS34725.h>

namespace Module
{
    class ModulePabst
    {
        public:
            //signatures des méthode pour les moteur de déplacement
            void doSmtg();
            bool detectionBallePanier();
            bool detectionBalleSol(float &dist_balle);
        //private:
            Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_2_4MS, TCS34725_GAIN_16X);
            uint16_t r, g, b, c;
            float dist_balle = 1000; //JSP si on est mieux de l'initialiser ici ou ailleurs

            int SONAR_BAS = 0;//plug dans J5
            int SONAR_HAUT = 1;//plug dans J4

    };
    
    void ModulePabst::doSmtg()
    {

    }


    bool ModulePabst::detectionBallePanier()
    {
	    tcs.getRawData(&r, &g, &b, &c);
	    tcs.getRawData(&r, &g, &b, &c);

        if (r == 0 && g == 0 && b == 0)
        {
        //Détection de la couleur noir (donc si aucune balle ne passe devant le sensor) 
        //--> j'ai mis des valeurs aléatoires, va falloir vérifier les valeurs de r g b une fois le sensor bien placé
            return false;
        }

        else
        {
            //Si il détecte n'importe quoi autre que noir, ca veut dire qu'il a détecté l'entrée d'une balle
            return true;
        }
    }

    bool ModulePabst::detectionBalleSol(float &dist_balle)
    {
            if (SONAR_GetRange(SONAR_BAS) < SONAR_GetRange(SONAR_HAUT))
            {
                dist_balle = SONAR_GetRange(SONAR_BAS); //"Retourne" la distance de la balle détectée en cm
                return true;
            }

            return false;
    }
}
