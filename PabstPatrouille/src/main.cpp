#include <LibRobus.h>

int x,y,angle;

void setup(){
  BoardInit();

  x = 2;
  y = 1;
}

void loop() {
  if(!(angle == 180 && y == 1) && !(angle == 90 && x == 3) && !(angle == -90 && x == 1))
  {
    MOTOR_SetSpeed(0, 1);
	  MOTOR_SetSpeed(1, 1);
	  delay(1000);
	  MOTOR_SetSpeed(0, 0);
	  MOTOR_SetSpeed(1, 0);
	
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
      MOTOR_SetSpeed(0, 1);
	    MOTOR_SetSpeed(1, -1);
	    delay(2000);
	    MOTOR_SetSpeed(0, 0);
	    MOTOR_SetSpeed(1, 0);

		  if(angle == 0)
		  	angle == 180;
		  else
		  	angle == 0;
	  }
    else
    {
      if((angle == 0 && x != 3) || (angle == 180 && x != 1) || (angle == 90 && y != 1))
		  {
			  MOTOR_SetSpeed(0, 1);
	      MOTOR_SetSpeed(1, -1);
	      delay(1000);
	      MOTOR_SetSpeed(0, 0);
	      MOTOR_SetSpeed(1, 0);
  
			  if(true /*!LEDCapteurObstacle*/)
			  	droite = true;
  
			  MOTOR_SetSpeed(0, -1);
	      MOTOR_SetSpeed(1, 1);
	      delay(1000);
	      MOTOR_SetSpeed(0, 0);
	      MOTOR_SetSpeed(1, 0);
		  }

      if((angle == 0 && x != 1) && (angle == 180 && x != 3) || (angle == -90 && y != 1))
		  {
		  	MOTOR_SetSpeed(0, -1);
	      MOTOR_SetSpeed(1, 1);
	      delay(1000);
	      MOTOR_SetSpeed(0, 0);
	      MOTOR_SetSpeed(1, 0);

		  	if(true /*!LEDCapteurObstacle*/)
		  		gauche = true;

		  	MOTOR_SetSpeed(0, 1);
	      MOTOR_SetSpeed(1, -1);
	      delay(1000);
	      MOTOR_SetSpeed(0, 0);
	      MOTOR_SetSpeed(1, 0);
		  }

      if(!droite && !gauche)
      {
			  MOTOR_SetSpeed(0, 1);
	      MOTOR_SetSpeed(1, -1);
	      delay(2000);
	      MOTOR_SetSpeed(0, 0);
	      MOTOR_SetSpeed(1, 0);

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
			droite=false
		else 
		gauche=false
	  }

	  if (angle == 0)
	  {
		if (gauche)
		{
			MOTOR_SetSpeed(0,-1);
			MOTOR_SetSpeed(-1,1);
			delay(1000);
			MOTOR_SetSpeed(0,0);
			MOTOR_SetSpeed(1,0);

			angle = -90;
		}
		if(droite)
		{
			MOTOR_SetSpeed(0,1);
			MOTOR_SetSpeed(1,-1);
			delay(1000);
			MOTOR_SetSpeed(0,0);
			MOTOR_SetSpeed(1,0);

			angle = 90;
		}		
	  }
	  if (angle==180)
	  {
		if(gauche)
		{
			MOTOR_SetSpeed(0,-1);
			MOTOR_SetSpeed(-1,1);
			delay(1000);
			MOTOR_SetSpeed(0,0);
			MOTOR_SetSpeed(1,0);

			angle=90;
		}
		if (droite)
		{
			MOTOR_SetSpeed(0,1);
			MOTOR_SetSpeed(1,-1);
			delay(1000);
			MOTOR_SetSpeed(0,0);
			MOTOR_SetSpeed(1,0);

			angle = -90;
		}
	  }
	  if(angle==90)
	  {
		if(gauche)
		{
			MOTOR_SetSpeed(0,-1);
			MOTOR_SetSpeed(-1,1);
			delay(1000);
			MOTOR_SetSpeed(0,0);
			MOTOR_SetSpeed(1,0);

			angle = -90;
		}
		if(droite)
		{
			MOTOR_SetSpeed(0,1);
			MOTOR_SetSpeed(1,-1);
			delay(1000);
			MOTOR_SetSpeed(0,0);
			MOTOR_SetSpeed(1,0);

			angle = 90;
		}
	  }
		if(angle==-90)
		{
			if(gauche)
			{
				MOTOR_SetSpeed(0,-1);
				MOTOR_SetSpeed(-1,1);
				delay(1000);
				MOTOR_SetSpeed(0,0);
				MOTOR_SetSpeed(1,0);
				
				angle = 180;
			}
			if (droite)
			{
				MOTOR_SetSpeed(0,1);
				MOTOR_SetSpeed(1,-1);
				delay(1000);
				MOTOR_SetSpeed(0,0);
				MOTOR_SetSpeed(1,0);

				angle = 0;
			}
		}
	  }

    }   

  }
}
