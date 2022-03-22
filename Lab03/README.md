# Sensor Reading: TFMini Lidar 

In this lab, we will attach a small Lidar sensor, which measures distance, to the HiFive1 board via UART and develop software to read the sensor data.  

# Part 1: TFmini Lidar

The TFmini sensor can be connected to the HiFive1 board via a UART connection.  

![The TFmini Sensor](doc/img1.png "The TFmini Sensor")

You need to connect the TFMini’s Tx (green) and Rx (white) to the HiFive1 board’s UART 0 RX (pin 0 = gpio 16) and TX (pin 1 = gpio 17), respectively. The 5V (red) and GND (black) should be connected to HiFive’s 5V and GND lines, respectively.  

![HiFive1 REb V Pinout](doc/img2.png "HiFive1 REb V Pinout")

The figure below shows the final connections. 

![Final Connections](doc/img3.jpg "Final Connections")

Now, let’s open the datasheet of the TFmini lidar (<em>docs/benewake-tfmini-datasheet.pdf</em>) in order to find out how to retrieve data from the sensor. The data exchange format of the sensor is described in Section 6.1 “Standard Data Format of Serial Port” of the datasheet. The simplified version of the Table 6 of Section 6.1 is shown below. 

|Byte 1 |2      |3      |4      |5      |6      |7      |8      |9      |
|-------|-------|-------|-------|-------|-------|-------|-------|-------|
|0x59   |0x59   |Dist_L |Dist_H |...    |...    |...    |...    |Chksum |

In this sensor, each measured data is encoded in a 9 byte data frame. The first two bytes are frame headers (0x59 = ‘Y’ in ASCII). The next two bytes encode the actual distance data (in cm). Note that the byte 3 (Dist_L) is the low 8 bits and byte 4 (Dist_H) is the high 8 bits of the measured distance data (16 bit). The rest of the bytes of a data frame are not used in this lab. 

Note that the sensor operates at 100 Hz, which means that it generates one data frame (distance measurement) at every 10 ms.

# Part 2: TFmini sensor reader programming

You will re-use the `ser_read()` function you implemented in lab02 here. You only need to modify eecs388_tfmini.c to complete this lab. 

## Task 2.1. Implement TFmini data frame reader

The first task is to interact with the TFmini sensor via UART0 to parse the sensor’s distance data. Note that the distance data is stored in bytes 3 and 4 of the sensor’s data frame. So, you need to do a bit of data manipulation to obtain the distance value. Note that the variable `dist` should contain the obtained distance value. You will then print the value to the console, by using either `printf` or `sprintf & ser_printline` combination. 

## Task 2.2. Control LED based on distance. 

The next task is to control the red and green leds based on the observed distance. If the distance between the sensor and the object is less than 50 cm, turn on the red LED. Otherwise, turn on the green LED. 

Once you are finished, demo your work to the TA and push to your GitLab repository. 