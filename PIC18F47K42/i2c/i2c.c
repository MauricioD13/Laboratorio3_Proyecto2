#include "i2c.h"
#include <pic18f47k42.h>
int addr_w=0xAE;  //10101110 Slave address
int addr_r=0xAF;
void config_i2c(){
    I2C1SCLPPS=0x13; //Pin enable I2C
    I2C1SDAPPS=0x14;
    //Revisar esto
    RC3I2C=0x41;
    RC4I2C=0x41;  

     
    SLRCONC = 0xFF;  //TIEMPO DE SUBIDA Y DE BAJADA
    
    I2C1CON0=0x84; 
    //Bit 7 -> Enable module
    //Bit 3 -> Host Data Request
    
    I2C1CON1=0x80; 
    //Not acknowledge after received data
    
    I2C1CON2=0x00;
    //Auto-load disabled
    //8 clock pulses
    
    I2C1CLK=0x01;
    //Clock frequency = Fosc
    
    I2C1PIR=0x00;
    //Flags reset
    
    I2C1ERR=0x00;
    //Error flags reset
    PIE3bits.I2C1TXIF = 1;
    PIE3bits.I2C1RXIF = 1;
}

void read_memory(TX_PARAMETERS *tx_parameters,int parameters_sended){

    if(option == 'B'){ // Read a data
        if(parameters_sended == 1){
            I2C1ADB1 = addr_r;
            I2CTXB = tx_parameters->address_high;

<<<<<<< HEAD
        
        
=======
            I2C1CNT = 2 // Two bytes of address
            I2C1CON0bits.RSEN = 1; // Restart bits
            I2C1CON0bits.S = 1;     //Start bit
            if(I2CSTAT1bits.TXBE == 1){
                I2CTXB = tx_parameters->address_low;
            }
        }
>>>>>>> fedb177f91b79a77bc4a3056a6e338ca1349cf40
    }

}