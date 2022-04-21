#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "eecs388_lib.h"
#include "metal/i2c.h"

struct metal_i2c *i2c;
uint8_t bufWrite[5];
uint8_t bufRead[1];
volatile int g_angle;

//The entire setup sequence
void set_up_I2C()
{
    uint8_t oldMode;
    uint8_t newMode;
    _Bool success;

    bufWrite[0] = PCA9685_MODE1;
    bufWrite[1] = MODE1_RESTART;
    printf("%d\n",bufWrite[0]);
    
    i2c = metal_i2c_get_device(0);

    if(i2c == NULL){
        printf("Connection Unsuccessful\n");
    }
    else{
        printf("Connection Successful\n");
    }
    
    //Setup Sequence
    metal_i2c_init(i2c,I2C_BAUDRATE,METAL_I2C_MASTER);
    success = metal_i2c_write(i2c,PCA9685_I2C_ADDRESS,2,bufWrite,METAL_I2C_STOP_DISABLE);//reset
    delay(100);
    printf("resetting PCA9685 control 1\n");

    //Initial Read of control 1
    bufWrite[0] = PCA9685_MODE1;//Address
    success = metal_i2c_transfer(i2c,PCA9685_I2C_ADDRESS,bufWrite,1,bufRead,1);//initial read
    printf("Read success: %d and control value is: %d\n", success, bufWrite[0]);
    
    //Configuring Control 1
    oldMode = bufRead[0];
    newMode = (oldMode & ~MODE1_RESTART) | MODE1_SLEEP;
    printf("sleep setting is %d\n", newMode);
    bufWrite[0] = PCA9685_MODE1;//address
    bufWrite[1] = newMode;//writing to register
    success = metal_i2c_write(i2c,PCA9685_I2C_ADDRESS,2,bufWrite,METAL_I2C_STOP_DISABLE);//sleep
    bufWrite[0] = PCA9685_PRESCALE;//Setting PWM prescale
    bufWrite[1] = 0x79;
    success = metal_i2c_write(i2c,PCA9685_I2C_ADDRESS,2,bufWrite,METAL_I2C_STOP_DISABLE);//sets prescale
    bufWrite[0] = PCA9685_MODE1;
    bufWrite[1] = 0x01 | MODE1_AI | MODE1_RESTART;
    printf("on setting is %d\n", bufWrite[1]);
    success = metal_i2c_write(i2c,PCA9685_I2C_ADDRESS,2,bufWrite,METAL_I2C_STOP_DISABLE);//awake
    delay(100);
    printf("Setting the control register\n");
    bufWrite[0] = PCA9685_MODE1;
    success = metal_i2c_transfer(i2c,PCA9685_I2C_ADDRESS,bufWrite,1,bufRead,1);//initial read
    printf("Set register is %d\n",bufRead[0]);
} 
void breakup(int bigNum, uint8_t* low, uint8_t* high){
    *low = bigNum & 0xff;
    *high = (bigNum >> 8) & 0xff;
}

void steering(int angle){
    uint8_t LED1_OFF_L, LED1_OFF_H, LED1_ON_L, LED1_ON_H;

    int  off_cycle = getServoCycle(angle);
    breakup(off_cycle, &LED1_OFF_L, &LED1_OFF_H);
    bufWrite[0]= PCA9685_LED0_ON_L+4;
    bufWrite[1]=0;
    bufWrite[2]=0;
    bufWrite[3]=LED1_OFF_L;
    bufWrite[4]=LED1_OFF_H;
    metal_i2c_transfer(i2c,PCA9685_I2C_ADDRESS,bufWrite,5,bufRead,1);
}

void stopMotor(){
    uint8_t stop_l, stop_h;
    breakup(280 ,&stop_l, &stop_h);
    bufWrite[0]= PCA9685_LED0_ON_L;
    bufWrite[1]=0;
    bufWrite[2]=0;
    bufWrite[3]=stop_l;
    bufWrite[4]=stop_h;
    metal_i2c_transfer(i2c,PCA9685_I2C_ADDRESS,bufWrite,5,bufRead,1);
}

void driveForward(uint8_t speedFlag){
  uint8_t forward_l, forward_h;
  breakup(267, &forward_l, &forward_h);
    bufWrite[0]= PCA9685_LED0_ON_L;
    bufWrite[1]=0;
    bufWrite[2]=0;
    bufWrite[3]=forward_l;
    bufWrite[4]=forward_h;
    metal_i2c_transfer(i2c,PCA9685_I2C_ADDRESS,bufWrite,5,bufRead,1);
}

void driveReverse(uint8_t speedFlag){
      uint8_t reverse_l, reverse_h;
  breakup(296, &reverse_l, &reverse_h);
    bufWrite[0]= PCA9685_LED0_ON_L;
    bufWrite[1]=0;
    bufWrite[2]=0;
    bufWrite[3]=reverse_l;
    bufWrite[4]=reverse_h;
    metal_i2c_transfer(i2c,PCA9685_I2C_ADDRESS,bufWrite,5,bufRead,1);
}

void raspberrypi_int_handler(int devid)
{
    char * str = malloc(20 * sizeof(char));
    //string command[20];
    int angle;
    int speed;

    ser_readline(devid, 20,str);

    // "Direction:1"
    // "Angle:3"
    // "Time:2"

    // "Direction"
    // "Direction:1"

    char * turn = "Angle";
    char * direction = "Direction";
    //char * reverse = "reverse";
    //char * forward = "forward";

    if(strncmp(str, direction, 4)==0){
        
        sscanf(str+10, "%d", &speed);
        printf("%s %d\n", str, speed);
        if(speed==0){
            stopMotor();
        }
        else if(speed==1){
            driveForward(1);
        }
        else if(speed==2){
            driveReverse(1);
        }
    }
    else if(strncmp(str, turn, 4)==0){

        // "Angle:45"
        sscanf(str+6, "%d", &angle);
        printf("%s %d\n", str, angle);
        steering(angle);
    }

    /*else if(strncmp(str, reverse, 4)==0){
        steering(angle);
        driveReverse(1);
    }

    else if(strncmp(str, forward, 4)==0){
        steering(angle);
        driveForward(1);
    }*/



    //g_angle => readline from UART0

    free(str);
}


int main()
{
    // Initialize I2C
    set_up_I2C();
    delay(2000);

    // Calibrate Motor
    printf("Calibrate Motor.\n");
    stopMotor();
    delay(2000);

    // initialize UART channels
    ser_setup(1); // uart0 (receive from raspberry pi)
    
    printf("Setup completed.\n");
    printf("Begin the main loop.\n");
    
    // Initialize global angle
    g_angle = 0;

    // Drive loop
    while (1) {
        if(ser_isready(1)){
            raspberrypi_int_handler(1);
        }
    }
    return 0;
}
