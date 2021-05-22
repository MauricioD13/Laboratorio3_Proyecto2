import random
from prettytable import PrettyTable


option = int(input("[1] Write frame\n[2] Read frame \n[3] Error frame\n"))
if option == 1:
    file = open('write_frame.txt','w')
    file1 = open('true_write.txt','w')
    message = []
    message1 = []
    
    aux_var = 'W'
    message.append(aux_var)
    message1.append(aux_var)
    
    aux_var = 'S'
    message.append(aux_var)
    message1.append(aux_var)
    
    aux_addr = 0 #High address
    message.append(chr(aux_addr))
    message1.append(aux_addr)
    
    aux_addr = 0 #Low address
    message.append(chr(aux_addr))
    message1.append(aux_addr)
    
    message.append(',')
    message1.append(',')
    for _ in range(1000):
        aux = random.randint(45,125)
        if(aux == 60):
            aux = 68
        
        message.append(chr(aux))
        message1.append(chr(aux))
        
        message.append(',')
        message1.append(',')
    message.append(chr(10))
    for i in message:
        file.write(str(i))
    for j in message1:
        file1.write(str(j))
    file.close()
    file1.close()
    
    table = PrettyTable()
    
    table.field_names = ['Posicion', 'Dato enviado','Direccion alta','Direccion baja']
    i = 0
    addr_low = 0
    addr_high = 0
    for _ in range(4):
        message.pop(0)
        message1.pop(0)
    for idx, value in enumerate(message1):
        
        if(value!=','):
            addr_low = 0x00FF & (i + aux_addr)
            addr_high = 0xFF00 & (i + aux_addr)
            addr_high = addr_high >> 8   
            table.add_row([i,value, addr_high, addr_low])
            i += 1
    
    print(table)
    
elif option == 2:
    file = open('read_frame.txt','w')
    message = []
    message.append('R')
    message.append('B')
    message.append(chr(0))
    message.append(chr(5))
    message.append(chr(10))
    
    for i in message:
        file.write(i)
    file.close()
else:
    file = open('error_frame.txt','w')
    message = []
    message.append('G')
    message.append('B')
    message.append(chr(0))
    message.append(chr(5))
    message.append(chr(10))
    for i in message:
        file.write(i)
    file.close()