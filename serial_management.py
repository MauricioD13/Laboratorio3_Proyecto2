import serial      
import os
import time
result = []

flag_init = 0;

i =0
option = int(input("[1]Enviar trama\n[2]Salir\n"))
message = ['R','B'];
dir = 100;
dir = chr(100)
message.append(0)
message.append(dir)
message.append('\n')
print(message)
if(option==1):
        for idx, value in enumerate(message):
            try:
                os.system("send_serial.exe "+ str(ord(value)))
            except:
                os.system("send_serial.exe "+ str(value))
