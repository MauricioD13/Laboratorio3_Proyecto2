import random



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
    
    aux_addr = 0
    message.append(chr(aux_addr))
    message1.append(aux_addr)
    
    aux_addr = 0
    message.append(chr(aux_addr))
    message1.append(aux_addr)
    
    message.append(',')
    message1.append(',')
    for _ in range(40):
        aux = random.randint(47,90)
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