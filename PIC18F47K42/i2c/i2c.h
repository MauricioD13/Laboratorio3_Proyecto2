#ifndef I2C_H
#define I2C_H
typedef struct TX_PARAMETERS{
    char action;
    char quantity;
    short int address_high;
    short int address_low;
}TX_PARAMETERS;
void config_i2c();
void read_memory(TX_PARAMETERS *tx_parameters);
#endif