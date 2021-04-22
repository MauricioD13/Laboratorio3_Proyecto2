#include "i2c.h"
#include <pic18f47k42.h>

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
    
    


}