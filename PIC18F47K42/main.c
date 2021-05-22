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
#define LENGTH 1100

#define _XTAL_FREQ 64000000
short int vector[LENGTH];

typedef struct FLAGS{
    short int uart_rx_end;
    short int i2c_tx_end;
    short int error_handling;
    short int reading_done;
    short int queue_overflow;
}FLAGS;

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


int rx = 500;
int cont_rx;
short int error = 0;
int cont_tx = 0;
int cont = 0;
QUEUE queue;
TX_PARAMETERS tx_parameters; 
char vector_uart[5];
FLAGS flags;
char aux;
int aux_bytes = 0;
int amount;
void __interrupt(irq(IRQ_U1TX), base(0x0008)) U1TX_isr(){
    if(flags.reading_done == 1){
        if(queue.queue_empty == 1){
            U1CON0bits.TXEN = 0, queue_init(&queue,&vector), U1FIFObits.TXBE = 1;
            PIE3bits.U1TXIE = 0;
            flags.reading_done = 0;
        }else{
            U1TXB = pop(&queue);
        }
        
    }else{
        if(cont == 0){
            U1CON0bits.TXEN = 0, U1FIFObits.TXBE = 1, PIE3bits.U1TXIE = 0;
        }else{
            cont--;
            U1TXB = vector_uart[cont];
        }

        
    }
}



void __interrupt(irq(IRQ_U1RX),base(0x0008)) U1RX_isr(){
    
    rx = U1RXB;  
    PIR3bits.U1RXIF = 0;
    
}

short int error_evaluator(TX_PARAMETERS *tx_parameters){
    if(!(tx_parameters->action == 'W' || tx_parameters->action == 'R' || tx_parameters->action == 0)){
        tx_parameters->action = 0;
        return 1;
        
    }
    else if(!(tx_parameters->quantity == 'B' || tx_parameters->quantity == 'S' || tx_parameters->quantity == 0)){
        tx_parameters->quantity = 0;
        return 2;
        
    }
    else if(tx_parameters->addr_high > 127){
        tx_parameters->addr_high = 0;
        return 3;
        
    }else{
        return 0;
    }
}

void init_PIC(){
    config_UART();
    config_i2c();
    flags.error_handling = 0;
    flags.i2c_tx_end = 0;
    flags.uart_rx_end = 0;
    flags.queue_overflow = 0;
}


void parameters_reset(TX_PARAMETERS *tx_parameters){
    tx_parameters->action = 0;
    tx_parameters->quantity = 0;
    tx_parameters->addr_high = 0;
    tx_parameters->addr_low = 0;
    cont_rx = 0;
}
int per_error = 0;
int aux_quan;
int main() {
    INTCON0 = 0x80;
    oscillator_module();
    init_PIC();
    queue_init(&queue, &vector);
    //TRISD = 0x00;
    //ANSELD = 0x00;
    
    while (1){
        if((rx == 10) && (flags.error_handling == 0)){
            tx_parameters.action = pop(&queue); 
            tx_parameters.quantity = pop(&queue);
            tx_parameters.addr_high = (short int)pop(&queue); //MSB of memory address
            tx_parameters.addr_low = (short int)pop(&queue);//LSB of memory address
            error = error_evaluator(&tx_parameters);
            flags.uart_rx_end = 1;
            tx_parameters.bytes_to_write = cont_rx - 4;
            if(error){
                flags.error_handling = 1;
                flags.uart_rx_end = 0;
                per_error = error;
                error = 0;
            }
            flags.queue_overflow = 0;
            rx = 500;
            
            
        }
        if((rx != 500) && (rx != ',') && (flags.error_handling == 0) && (flags.queue_overflow == 0)){
            
            push(&queue,rx);
            rx = 500;
            cont_rx++;
            if(cont_rx >= LENGTH){
                flags.error_handling = 1;
                queue_init(&queue, &vector);
                parameters_reset(&tx_parameters);
                flags.queue_overflow = 1;
            }
        }
        if(flags.error_handling == 1){
            flags.error_handling = error_handler(&vector_uart, &cont, &per_error);
            parameters_reset(&tx_parameters);
        }
        if(flags.uart_rx_end == 1){
            if(tx_parameters.action == 'R'){
                if(tx_parameters.quantity == 'B'){
                    tx_parameters.bytes_to_read = 1;
                    queue_init(&queue,&vector);
                    read_bytes(&queue, &tx_parameters);
                }else{
                    aux_quan = pop(&queue);
                    tx_parameters.bytes_to_read = pop(&queue);
                    aux_bytes = tx_parameters.bytes_to_read;
                    queue_init(&queue,&vector);
                    
                    while(aux_quan){
                            tx_parameters.bytes_to_read = 254;
                            read_bytes(&queue, &tx_parameters);
                            tx_parameters.addr_high +=1;
                            i2c_reset();
                            aux_quan--;
                    }
                    tx_parameters.bytes_to_read = aux_bytes;
                    read_bytes(&queue, &tx_parameters);
 
                }
                
                if(queue.queue_empty == 0){
                    flags.reading_done = 1;
                    PIE3bits.U1TXIE = 1;// Enable interrups
                    U1FIFObits.TXBE = 1;// Clear buffers
                    U1CON0bits.TXEN = 1;// Enable module
                }
                parameters_reset(&tx_parameters);
                
                flags.uart_rx_end = 0;
                
            }else if(tx_parameters.action == 'W'){     
                if(tx_parameters.bytes_to_write > 63){
                    
                    aux_bytes = tx_parameters.bytes_to_write;
                    tx_parameters.bytes_to_write = 64;
                    error = write_bytes(&queue, &tx_parameters);
                    tx_parameters.bytes_to_write = aux_bytes - 64;
                    tx_parameters.addr_low += 64;
                   
                    if(tx_parameters.addr_low > 255){
                        tx_parameters.addr_high++;
                        tx_parameters.addr_low = 0x00FF & tx_parameters.addr_low;
                    }
                    __delay_ms(6);
                       
                    
                }
                else if(tx_parameters.bytes_to_write > 0){
                    error = write_bytes(&queue, &tx_parameters);
                    tx_parameters.bytes_to_write = 0;
                    
                }else{
                    if(error == 0){
                        int_transmit(&vector_uart,&cont);
                    }
                    queue_init(&queue, &vector);
                    parameters_reset(&tx_parameters);
                    flags.uart_rx_end = 0;
                    i2c_reset();
                }
                
                
                
            }
            /*Errores:
                 - Direccion erronea +
                 - Accion erronea  +
                 - Cantidad erronea (Buffer overflow) +
             *   - Transmision I2C +
             *   - Recepcion I2C +
             *  - 
             - Enviar por pagina
             
             
             Extras:
             * - Manejo no secuencial
             * - Quitar while de I2C
            */
        }
        
        
    }
    return (EXIT_SUCCESS);
}

