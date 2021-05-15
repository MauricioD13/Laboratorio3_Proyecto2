
#include "uart.h"
#include <pic18f47k42.h>

void config_UART(){
     
    //UART config
    U1CON0 = 0x30;
    U1CON1 = 0x80;
    U1CON2 = 0x00;
    U1FIFO = 0x00;
    U1ERRIR = 0x00;
    U1ERRIE = 0x00;
    U1UIR = 0x00;
    
    //TX
    TRISCbits.TRISC2 = 0; // Output TX
    ANSELCbits.ANSELC2 = 0; //As digital
    RC6PPS = 0x13;// 0b00010011;
    U1FIFObits.STPMD = 0;
    U1FIFObits.TXBE = 1;
    
    //RX
    TRISCbits.TRISC7 = 1; // Input RX
    ANSELCbits.ANSELC7 = 0; // As digital
    PIE3bits.U1RXIE = 1; //Interrupt enable
    PIE3bits.U1IE = 1;
    U1RXPPS = 0x17; //0b00010111;
    
    
    //X = ((Fosc/Desired baud rate)/16) - 1
    
    U1BRGL = 0xA0;
    //207 19200
    //415 9600
    U1BRGH = 1;
    
    //Enable receiver interrupts
    //PIE3bits.U1RXIE = 1;
   
}

short int error_handler(int *cont_tx,short int error_handling){
   
    if(*cont_tx == 0){
        TO_TRANSMIT = 69;
        (*cont_tx)++;
    }else if((*cont_tx) == 1){
        TO_TRANSMIT = 82;
        (*cont_tx)++;
    }else{
        TO_TRANSMIT = 82;
        error_handling = 0;
        (*cont_tx) = 0;
        return error_handling;
    }
    return 1;
    //E 69
    //R 82
    // \n 10
}

int receive_UART(){
    return RECEIVED;
}