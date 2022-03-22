#include <stdio.h>
#include <stdint.h>

#include "eecs388_lib.h"

#define SERVO_PULSE_MAX 2400    /* 2400 us */
#define SERVO_PULSE_MIN 544     /* 544 us */
#define SERVO_PERIOD    20000   /* 20000 us (20ms) */

/**
 * Generate a pwm signal
 * 
 * Input:
 *  @gpio   gpio number
 *  @pos    degree [0,180]
 */
void servo(int gpio, int pos)
{
    int duty_cycle = ((pos * (SERVO_PULSE_MAX - SERVO_PULSE_MIN)) / 180) + SERVO_PULSE_MIN;
    gpio_write (gpio, ON);
    delay_usec(duty_cycle);

    gpio_write (gpio, OFF);
    delay_usec(SERVO_PERIOD - duty_cycle);

}

int main()
{
    int gpio = PIN_19;
    gpio_mode(gpio, OUTPUT);

    while (1) {
        /**
         * Sweeps through angles 0 -> 180 to test the 'servo' function 
         * 
         * The inner loop calls the function 50 times, causing each angle
         * to be held on the servo for ~1 second. 
         * 
         * Do you understand why 1 second?
         * 
         */
        for (int pos = 0; pos <= 180; pos += 30) {
            printf("pos: %d (degree)\n", pos);
            /* control the servor for 1 sec duration */
            for (int i = 0; i < 50; i++)
                servo(gpio, pos);
        }
    }
}