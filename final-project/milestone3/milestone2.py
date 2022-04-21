import serial

ser1 = serial.Serial('/dev/ttyAMA1', 115200)

while(1):

    print("Angle: ")
    angle = input()
    ser1.write(bytes(angle))

    print("Speed: ")
    speed = input()
    ser1.write(bytes(speed))


ser1.close()

