
#include "i2c.h"
#include "uart.h"
#include <cstdlib>
#include <queue>
#include <pic18f47k42>

using namespace std;

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

void __interrupt(irq(IRQ_U1RX),base(0x0008)) U1RX_isr(){
    rx = U1RXB;
    counters.cont_rx++;

    PIR3bits.U1RXIF = 0;
}


void main() {
    INTCON0 = 0x80; 
    oscillator_module();
    
    return 0;
}

