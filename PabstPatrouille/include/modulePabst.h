#include <LibRobus.h>
#include <Adafruit_TCS34725.h>

namespace Module
{
    bool scored = false;

    class ModulePabst
    {
        public:
            bool detectionBallePanier();
            int detectionBallePanierTemp();
            bool detectionBalleSol(float &dist_balle);
        //private:
            Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_2_4MS, TCS34725_GAIN_16X);
            uint16_t r, g, b, c;
            float dist_balle = 1000; //JSP si on est mieux de l'initialiser ici ou ailleurs

            int SONAR_BAS = 0;//plug dans J5
            int SONAR_HAUT = 1;//plug dans J4

    };


    bool ModulePabst::detectionBallePanier()
    {
	    tcs.getRawData(&r, &g, &b, &c);
        tcs.getRawData(&r, &g, &b, &c);

        if((r > 9 && b > 9 && g > 9) && !scored){
            scored = true;
            Serial.println("GOAL");
        	return true;
        }
        if((r < 10 && b < 10 && g < 10) && scored){
            scored = false;
        	return false;
        }
        return false;
    }

    int ModulePabst::detectionBallePanierTemp()
    {
	    tcs.getRawData(&r, &g, &b, &c);
        tcs.getRawData(&r, &g, &b, &c);

		//noir : T == 7388
		//vert : T == 4691
		//orange : T == 2148

		int temp = tcs.calculateColorTemperature(r, g, b);

		if (temp > 7000)
		{
			//NOIR
			Serial.println("NO GOAL");
			return 0;
		}

		if (temp > 4000 && temp < 7000)
		{
			//VERT
			Serial.println("GOAL VERT");
			return 1;
		}

		if (temp > 2000 && temp < 4000)
		{
			//ORANGE
			Serial.println("GOAL ORANGE");
			return 2;
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
