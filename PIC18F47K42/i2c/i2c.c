#include "i2c.h"
#include <pic18f47k42.h>
int addr_w=0xAE;  //10101110 Slave address
int addr_r=0xAF;
void config_i2c(){
            /**
    LATx registers
    */
    LATE = 0x00;
    LATD = 0x00;
    LATA = 0x00;
    LATB = 0x00;
    LATC = 0x00;

    /**
    TRISx registers
    */

    TRISC &= 0xE7;
    /**
    ANSELx registers
    */
    ANSELC &= 0xE7;


    /**
    WPUx registers
    */
    WPUC = 0x00; // !!!Prueba 0x00
    /**
    RxyI2C registers
    */
    RC3I2C = 0x00;
    RC4I2C = 0x00;
    /**
    ODx registers
    */
    ODCONC = 0x18; // !!!Prueba 0x00
    /**
    SLRCONx registers
    */
    SLRCONC = 0x00; // !!!Prueba
    /**
    INLVLx registers
    */
    INLVLC = 0xFF;

    PMD5 = 0x00;
	
    I2C1SDAPPS = 0x14;   //RC4->I2C1:SDA1;    
    RC3PPS = 0x21;   //RC3->I2C1:SCL1;    
    RC4PPS = 0x22;   //RC4->I2C1:SDA1;  
    
    I2C1SCLPPS = 0x13;   //RC3->I2C1:SCL1;
    
    //EN enable; RSEN disabled; S Cleared by hardware after Start; CSTR Enable clocking; MODE 7-bit address; 
    I2C1CON0 = 0x84;
    //ACKCNT Acknowledge; ACKDT Acknowledge; ACKSTAT ACK received; ACKT 0; RXO 0; TXU 0; CSD Clock Stretching enabled; 
    I2C1CON1 = 0x80;
    //ACNT disabled; GCEN disabled; FME disabled; ABD enabled; SDAHT 300 ns hold time; BFRET 8 I2C Clock pulses; 
    I2C1CON2 = 0x00; //Probar 0x18
    //CLK MFINTOSC; 
    I2C1CLK = 0x03;
    //CNTIF 0; ACKTIF 0; WRIF 0; ADRIF 0; PCIF 0; RSCIF 0; SCIF 0; 
    I2C1PIR = 0x00;
    //CNTIE disabled; ACKTIE disabled; WRIE disabled; ADRIE disabled; PCIE disabled; RSCIE disabled; SCIE disabled; 
    I2C1PIE = 0x00;
    //BTOIF No bus timout; BCLIF No bus collision detected; NACKIF No NACK/Error detected; BTOIE disabled; BCLIE disabled; NACKIE disabled; 
    I2C1ERR = 0x00;
}

void write_bytes(QUEUE *queue, TX_PARAMETERS *parameters){
    
    I2C1CON0bits.EN = 1;
    
    I2C1CNT = parameters->bytes_to_write + 2;
    I2C1ADB1 = 0xA0; // CARGO LA DIRECCION EN EL BUFFER
    I2C1TXB = parameters->addr_high;
    
    I2C1CON0bits.S = 1;   //HABILITO LA TRANSMISION 
    
    while(I2C1STAT1bits.TXBE == 0);
    I2C1TXB = parameters->addr_low;
            
    while(I2C1CNT){
        while(I2C1STAT1bits.TXBE == 0);
        I2C1TXB = pop(queue);        
    }
    while(I2C1PIRbits.PCIF == 0);
    
    I2C1CON0bits.EN = 0;
    I2C1PIR = 0x00;
}

void read_bytes(QUEUE *queue, TX_PARAMETERS *parameters){
    int rx = 0;
    I2C1CON0bits.EN = 1;
    I2C1CNT = parameters->bytes_to_read;
    I2C1ADB1 = 0xA0; // Slave address to write
    I2C1TXB = parameters->addr_high;//MSB memory address to read
    I2C1CON0bits.RSEN = 1; 
    /*Enable the restart to not generate a stop condition when I2C1CNT == 0  */
    
   
    I2C1CON0bits.S = 1;// Start communication
            
            
    while(I2C1STAT1bits.TXBE == 0);
    I2C1TXB = parameters->addr_low;//LSB memory address to read
            
    while(I2C1STAT1bits.TXBE == 0);
    while(!I2C1CON0bits.MDR); // Master ready
            
    I2C1CNT = parameters->bytes_to_read;
    I2C1ADB1 = 0xA1; //Slave address to read
    I2C1CON0bits.RSEN = 0;
    I2C1CON0bits.S = 1;  
            
    while(I2C1CNT){
        while(!I2C1STAT1bits.RXBF);     // Wait to receive a data in the buffer
        rx = I2C1RXB;   
        push(queue,rx);//Push data into the queue
    }     
    I2C1CON0bits.EN = 0;
}

