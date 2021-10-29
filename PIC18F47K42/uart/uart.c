
#include "uart.h"
#include <pic18f47k42.h>

void config_UART(){
     
    //UART config
    U1CON0 = 0x30;
    U1CON0bits.TXEN = 0;
    U1CON1 = 0x80;
    U1CON2 = 0x00;
    U1FIFO = 0x00;
    U1ERRIR = 0x00;
    U1ERRIE = 0x00;
    U1UIR = 0x00;
    
    //TX
    TRISCbits.TRISC2 = 0; //Output TX
    ANSELCbits.ANSELC2 = 0; //As digital
    RC6PPS = 0x13;// 0b00010011;
    U1FIFObits.STPMD = 0;
    U1FIFObits.TXBE = 1;
    
    //RX
    TRISCbits.TRISC7 = 1; // Input RX
    ANSELCbits.ANSELC7 = 0; // As digital
    
    U1ERRIEbits.U1TXMTIE = 1;
    PIE3bits.U1RXIE = 1; //Interrupt enable
    PIE3bits.U1TXIE = 1;
    PIE3bits.U1IE = 1;
    U1RXPPS = 0x17; //0b00010111;
    
    
    //X = ((Fosc/Desired baud rate)/16) - 1
    
    U1BRGL = 0xA0;
    //415 o A0 9600
    
    U1BRGH = 1;
    // 9600 1
    //Enable receiver interrupts
    //PIE3bits.U1RXIE = 1;
   
}

short int error_handler(char *vector, short int *cont, int *error){
   
    U1FIFObits.TXBE = 1, PIE3bits.U1TXIE = 1;
    
    *(vector + 1) = 'R';
    *(vector + 2) = 'R';
    *(vector + 3) = 'E';
    *cont = 4;
    U1CON0bits.TXEN = 1;
    return 0;
    //E 69
    //R 82
    // \n 10
}

void int_transmit(char *vector, short int *cont){
    
    U1FIFObits.TXBE = 1, PIE3bits.U1TXIE = 1;
    *(vector + 1) = 'K';
    *(vector + 2) = 'O';
    *cont = 3;
    U1CON0bits.TXEN = 1;
    
}
int receive_UART(){
    return RECEIVED;
}