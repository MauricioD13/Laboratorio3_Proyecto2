import serial      
import os
import time
result = []

flag_init = 0;

i =0
option = int(input("[1]Enviar trama\n[2]Salir\n"))
message = ['W','B','1','1'];
for i in range(1,10):
    message.append(str(i))
message.append('\n')

if(option==1):
        for i in message:
            os.system("send_serial.exe "+ str(ord(i)))
            
