import os
import serial
import time

image = "./build/kernel.img"

serial = serial.Serial("/dev/ttyUSB1",115200)
time.sleep(1)

print("sending request...")
serial.write(b'boot?')

# get reply from target
resp = serial.readline()
if(resp == b'Ok\r\n'):
 print("Ok!")
 # send size of image
 size = os.path.getsize(image)
 print("sending size " + str(size))
 serial.write(size.to_bytes(4, byteorder='big'))

 #wait for confirmation of size
 size_conf = serial.read(4)
 if(int.from_bytes(size_conf, byteorder='big') == size):
    print("size confirmed")

    #send image
    print("sending image")
    img_file = open(image, mode='rb')
    binary = img_file.read()
    serial.write(binary)
    img_file.close()
    print("sending done")

while 1:
    print(serial.readline().decode('utf-8'))
serial.close()
 


