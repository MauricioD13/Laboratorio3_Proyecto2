/* 
 * File:   newmain.c
 * Author: guasonito
 *
 * Created on April 21, 2021, 10:10 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include "i2c/i2c.h"
#include "uart/uart.h"
#include "queue/queue.h"
#include <pic18f47k42.h>
#include <xc.h>
// CONFIG1L
#pragma config FEXTOSC = OFF    // External Oscillator Selection (Oscillator not enabled)
#pragma config RSTOSC = HFINTOSC_64MHZ// Reset Oscillator Selection (HFINTOSC with HFFRQ = 64 MHz and CDIV = 1:1)

// CONFIG1H
#pragma config CLKOUTEN = OFF   // Clock out Enable bit (CLKOUT function is disabled)
#pragma config PR1WAY = ON     // PRLOCKED One-Way Set Enable bit (PRLOCK bit can be cleared and set only once)
#pragma config CSWEN = OFF      // Clock Switch Enable bit (Writing to NOSC and NDIV is allowed)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor enabled)

// CONFIG2L
#pragma config MCLRE = EXTMCLR  // MCLR Enable bit (If LVP = 0, MCLR pin is MCLR; If LVP = 1, RE3 pin function is MCLR )
#pragma config PWRTS = PWRT_OFF // Power-up timer selection bits (PWRT is disabled)
#pragma config MVECEN = ON      // Multi-vector enable bit (Multi-vector enabled, Vector table used for interrupts)
#pragma config IVT1WAY = ON     // IVTLOCK bit One-way set enable bit (IVTLOCK bit can be cleared and set only once)
#pragma config LPBOREN = OFF    // Low Power BOR Enable bit (ULPBOR disabled)
#pragma config BOREN = SBORDIS  // Brown-out Reset Enable bits (Brown-out Reset enabled , SBOREN bit is ignored)

// CONFIG2H
#pragma config BORV = VBOR_2P45 // Brown-out Reset Voltage Selection bits (Brown-out Reset Voltage (VBOR) set to 2.45V)
#pragma config ZCD = OFF        // ZCD Disable bit (ZCD disabled. ZCD can be enabled by setting the ZCDSEN bit of ZCDCON)
#pragma config PPS1WAY = ON     // PPSLOCK bit One-Way Set Enable bit (PPSLOCK bit can be cleared and set only once; PPS registers remain locked after one clear/set cycle)
#pragma config STVREN = ON     // Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
#pragma config DEBUG = OFF      // Debugger Enable bit (Background debugger disabled)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Extended Instruction Set and Indexed Addressing Mode disabled)

// CONFIG3L
#pragma config WDTCPS = WDTCPS_31// WDT Period selection bits (Divider ratio 1:65536; software control of WDTPS)
#pragma config WDTE = OFF       // WDT operating mode (WDT Disabled; SWDTEN is ignored)

// CONFIG3H
#pragma config WDTCWS = WDTCWS_7// WDT Window Select bits (window always open (100%); software control; keyed access not required)
#pragma config WDTCCS = SC      // WDT input clock selector (Software Control)

// CONFIG4L
#pragma config BBSIZE = BBSIZE_512// Boot Block Size selection bits (Boot Block size is 512 words)
#pragma config BBEN = OFF       // Boot Block enable bit (Boot block disabled)
#pragma config SAFEN = OFF      // Storage Area Flash enable bit (SAF disabled)
#pragma config WRTAPP = OFF     // Application Block write protection bit (Application Block not write protected)

// CONFIG4H
#pragma config WRTB = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-30000Bh) not write-protected)
#pragma config WRTC = OFF       // Boot Block Write Protection bit (Boot Block (000000-0007FFh) not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM not write-protected)
#pragma config WRTSAF = OFF     // SAF Write protection bit (SAF not Write Protected)
#pragma config LVP = ON         // Low Voltage Programming Enable bit (Low voltage programming enabled. MCLR/VPP pin function is MCLR. MCLRE configuration bit is ignored)

// CONFIG5L
#pragma config CP = OFF         // PFM and Data EEPROM Code Protection bit (PFM and Data EEPROM code protection disabled)

short int vector[LENGTH];

typedef struct FLAGS{
    short int uart_rx_end;
    short int i2c_tx_end;
    short int error_handling;
}FLAGS;

char buffer[8];
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
int error = 0;
int cont_tx = 0;
QUEUE queue;
TX_PARAMETERS tx_parameters; 
void __interrupt(irq(IRQ_U1RX),base(0x0008)) U1RX_isr(){
    
    rx = U1RXB;  
    PIR3bits.U1RXIF = 0;
    
}


void init_PIC(){
    config_UART();
    config_i2c();
}


FLAGS flags;


int main() {
    INTCON0 = 0x80;
    oscillator_module();
    init_PIC();
    queue_init(&queue, &vector);
    
    
    while (1){
        if((rx == '\n') && (flags.error_handling == 0)){
            tx_parameters.action = pop(&queue); 
            if(!(tx_parameters.action == 'W' || tx_parameters.action == 'R' || tx_parameters.action == 0)){
                error = 1;
                tx_parameters.action = 0;
            }
            tx_parameters.quantity = pop(&queue);
            if(!(tx_parameters.quantity == 'B' || tx_parameters.quantity == 'S' || tx_parameters.quantity == 0)){
                error = 2;
                tx_parameters.quantity = 0;
            }
            tx_parameters.addr_high = (short int)pop(&queue) - 48; //MSB of memory address
            if(tx_parameters.addr_high > 127){
                error = 3;
                tx_parameters.addr_high = 0;
            }
            tx_parameters.addr_low = (short int)pop(&queue)- 48;//Complete memory address
            flags.uart_rx_end = 1;
            if(error){
                flags.error_handling = 1;
                flags.uart_rx_end = 0;
            }
            
            
        }
        else if((rx != 0) && (rx != ',') && (flags.error_handling == 0)){
            
            push(&queue,rx);
            rx = 0;
            cont_rx++;
        }
        if(flags.error_handling == 1){
            flags.error_handling = error_handler(&cont_tx, flags.error_handling);
        }
        if(flags.uart_rx_end == 1){
            if(tx_parameters.action == 'R'){
                tx_parameters.bytes_to_read = cont_rx - 4;
                read_bytes(&queue, &tx_parameters);
                
            }else if(tx_parameters.action == 'W'){
                tx_parameters.bytes_to_write = cont_rx - 4;
                write_bytes(&queue, &tx_parameters); 
                
            }
            /*Errores:
                 - Cola vacia
                 - Direccion erronea 
                 - Accion erronea
                 - Cantidad erronea
             *   - Transmision I2C
             *   - Recepcion I2C
             *  
             - Enviar por pagina
             - Tamaño de la cola
             
             Extras:
             * - Manejo no secuencial
             * - Quitar while de I2C
            */
        }
        
        
    }
    return (EXIT_SUCCESS);
}

