#include <stdint.h>

#include "eecs388_lib.h"

/*
   Task 2.1: Review eecs388_lib.h and eecs388_lib.c 

   Task 2.2: Implement RGB blinky. 

   - set red, green, and blue LEDs for output mode, using the gpio_mode() function
   - in the main loop, turn on/off a different LED at each iteration. 
      for example, 
      at 1st iteration
         turn on RED led
	      delay 500 ms 
	      turn off RED led
	      delay 300 ms.
	   at 2nd iteration
	      do the same but for GREEN led
	   at 3rd iteration
	      do the same but for BLUE led
	   at 4th iteration
	      back to RED led. 
	   ...
   - You must use array and modulo (%) operator to change the color. 

   Task 2.3. Implement RGBA (bonus: +1 point)
   - rotate red, green, blue, and white (all LEDs are enabled) colors.
*/

int main()
{
   int gpioR = RED_LED;
   int gpioG = GREEN_LED;
   int gpioB = BLUE_LED;

   int ledArray[3] = {gpioR, gpioG, gpioB};

   gpio_mode(gpioR, OUTPUT);
   gpio_mode(gpioG, OUTPUT);
   gpio_mode(gpioB, OUTPUT);

   while(1)
   {
      for(int i = 0; i < 4; i++)
      {
         if(i%4 ==0)
         {
            gpio_write(ledArray[i], ON);
            delay(500);
            gpio_write(ledArray[i], OFF);
            delay(300);
         }
         else if(i%4 == 1)
         {
            gpio_write(ledArray[i], ON);
            delay(500);
            gpio_write(ledArray[i], OFF);
            delay(300);
         }
         else if(i%4 == 2)
         {
            gpio_write(ledArray[i], ON);
            delay(500);
            gpio_write(ledArray[i], OFF);
            delay(300);
         }
         else if(1%4 == 3)
         {
            for(int j = 0; j < 3; j++)
            {
               gpio_write(ledArray[0], ON);
               gpio_write(ledArray[1], ON);
               gpio_write(ledArray[2], ON);
            }
            delay(500);

            for(int j = 0; j < 3; j++)
            {
               gpio_write(ledArray[0], OFF);
               gpio_write(ledArray[1], OFF);
               gpio_write(ledArray[2], OFF);
            }
            delay(300);
         }
      }
   }
}
