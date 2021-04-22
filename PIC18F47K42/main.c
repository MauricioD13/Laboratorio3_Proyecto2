/* 
 * File:   newmain.c
 * Author: guasonito
 *
 * Created on April 21, 2021, 10:10 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <pic18f47k42.h>

#include "PIC18F47K42/uart/uart.h"
/*
 * 
 */
void oscillator_module (void){
    /*
     OSCCON1
        New oscillator source 
        New divider*/
    OSCCON1 = 0x60;
    
    /*
     OSCCON2
        Oscillator source
        divider*/
    OSCCON2 = 0x60;
    
    /*
     OSCCON3
        Clock switch hold
        Secondary oscillator power
        */
    OSCCON3 = 0x80;
    
    
    OSCTUNE = 0x00;
    
    /*
     OSCFRQ
        Frequency selection*/
    OSCFRQ = 0x08;
    
    /*
      OSCEN
        Oscillator manual enable*/
    OSCENbits.HFOEN = 1;
    
}
int aux;
int rx;
int cont_rx;
PARAMETERS parameters;
void __interrupt(irq(IRQ_U1RX),base(0x0008)) U1RX_isr(){
    
    rx = U1RXB;
    cont_rx++;
    PIR3bits.U1RXIF = 0;
}

int main() {
    INTCON0 = 0x80;
    oscillator_module();
    while (1){
        if(cont_rx == 1){
            parameters.action = (char)rx;
        }else if(cont_rx == 2){
            parameters.quantity = (char)rx;
        }else if(cont_rx == 3){
            aux = rx<<8;
            parameters.address |= aux;
        }else if(cont_rx == 4){
            parameters.address |= rx;
        }else{
            //Queue function non-implemented yet
        }
    }
    return (EXIT_SUCCESS);
}

