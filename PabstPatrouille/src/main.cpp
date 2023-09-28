#include <LibRobus.h>

int x,y,angle;

void avancer(){
	MOTOR_SetSpeed(0, 1);
	MOTOR_SetSpeed(1, 1);
	delay(1000);
	MOTOR_SetSpeed(0, 0);
	MOTOR_SetSpeed(1, 0);
}

void tGauche(int iteration = 1){
	MOTOR_SetSpeed(0, -1);
	MOTOR_SetSpeed(1, 1);
	delay(iteration * 1000);
	MOTOR_SetSpeed(0, 0);
	MOTOR_SetSpeed(1, 0);
}

void tDroite(int iteration = 1){
	MOTOR_SetSpeed(0, 1);
	MOTOR_SetSpeed(1, -1);
	delay(iteration * 1000);
	MOTOR_SetSpeed(0, 0);
	MOTOR_SetSpeed(1, 0);
}

void setup(){
	BoardInit();

  	x = 2;
  	y = 1;
}

void loop() {
	if(!(angle == 180 && y == 1) && !(angle == 90 && x == 3) && !(angle == -90 && x == 1))
	{
    	avancer();
	
		if(angle == 0)
			y++;
	  	if(angle == 180)
			y--;
	  	if(angle == 90)
		  	x++;
	  	if(angle == -90)
		 	x--;
  	}
  	else
  	{
		bool gauche, droite;
    	if(y % 2 == 0)
		{
    		tDroite(2);
	  		if(angle == 0)
	  			angle == 180;
	  		else
	  			angle == 0;
		}
    	else
    	{
      		if((angle == 0 && x != 3) || (angle == 180 && x != 1) || (angle == 90 && y != 1))
		  	{
			  	tDroite();
  
			  	if(true /*!LEDCapteurObstacle*/)
			  		droite = true;
  
			  	tGauche();
		  	}

      		if((angle == 0 && x != 1) && (angle == 180 && x != 3) || (angle == -90 && y != 1))
		  	{
		  		tGauche();

		  		if(true /*!LEDCapteurObstacle*/)
		  			gauche = true;

		  		tDroite();
		  	}

      		if(!droite && !gauche)
      		{
			  	tDroite(2);

			  	if(angle == 0)
			  		angle = 180;
			  	if(angle == 180)
			  		angle = 0;
			  	if(angle == 90)
			  		angle = -90;
			  	if(angle == -90)
			  		angle = 90;
		  	}

      		if(droite && gauche)
	  		{
				if(angle == 0||angle == 180||angle == 90)
					droite=false;
				else 
					gauche=false;
	  		}

			switch (angle)
			{
				case 0:
					if (gauche)
					{
						tGauche();
						angle -= 90;
					}

					if(droite)
					{
						tDroite();
						angle += 90;
					}		
					break;
				case 90:
					if(gauche)
					{
						tGauche();
						angle -= 90;
					}
					if(droite)
					{
						tDroite();
						angle += 90;
					}
					break;
				case -90:
					if(gauche)
					{
						tGauche();
						angle = 180;
					}
					if (droite)
					{
						tDroite();
						angle -= 90;
					}
					break;
				case 180:
					if(gauche)
					{
						tGauche();
						angle=90;
					}
					if (droite)
					{
						tDroite();
						angle = -90;
					}
					break;
				default:
					break;
			}	
	  	}

    }   

}

