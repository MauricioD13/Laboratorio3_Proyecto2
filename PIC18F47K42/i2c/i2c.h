#ifndef I2C_H
#define I2C_H
#include "C:/Users/guaso/Documents/Universidad/Proyecto II/Laboratorio3_Proyecto2/PIC18F47K42/queue/queue.h"
typedef struct TX_PARAMETERS{
    char action;
    char quantity;
    short int addr_high;
    short int addr_low;
    int bytes_to_write;
    int bytes_to_read;
}TX_PARAMETERS;

void config_i2c();
void read_bytes(QUEUE *queue, TX_PARAMETERS *tx_parameters);
short int write_bytes(QUEUE *queue, TX_PARAMETERS *tx_parameters);
void i2c_reset();
#endif