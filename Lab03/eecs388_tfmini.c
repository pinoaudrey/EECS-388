/**
 * This program read the distance data from TFmini lidar sensor and 
 * print the read data and controls LED depending on the measured distance.
 */
#include <stdint.h>
#include <stdio.h>

#include "eecs388_lib.h"

int main()
{
    gpio_mode(RED_LED, OUTPUT);
    gpio_mode(GREEN_LED, OUTPUT);
    ser_setup();

    ser_printline("Setup completed.\n");

    /* 
        - LIDAR distance data is 16 bits.
    */
    uint16_t dist = 0;

    while (1) {
        /* 
            Task 1: 
            - read a data frame from the TFmini sensor
            
            - from Table 6, we know each LIDAR measurement sends 9 bytes
            - the function `ser_read()` can process 1 byte

            - This if statement will read the first two bytes of a valid data frame
        */
        if ('Y' == ser_read() && 'Y' == ser_read()) {
            int dist, dl, dh;
            dl = ser_read();
            dh = ser_read();
            dist = dh<<8 | dl;
            
            printf("%d\n", dist);
            for(int i =0; i<5; i++)
            {
                ser_read();
            }

        /* 
            Task 2: 
            - turn on the red LED if the distance is less than 50cm. 
            - otherwise turn on the green LED 

            - print the measured value to the console 
            - You can use printf or ser_printline or ser_write
            
            - Helpful guide for printing with printf()
            - https://en.cppreference.com/w/cpp/io/c/fprintf
        */
      
       
       if(dist < 50)
       {
           gpio_write(GREEN_LED, OFF);
           gpio_write(RED_LED, ON);
       }
       else{
           gpio_write(GREEN_LED, ON);
           gpio_write(RED_LED, OFF);
       }
       }
        }
    }
