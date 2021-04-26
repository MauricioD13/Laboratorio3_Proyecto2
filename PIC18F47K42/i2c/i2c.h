#ifndef I2C_H
#define I2C_H

void config_i2c();
void read_memory(int addr_r, TX_PARAMETERS *tx_parameters);
#endif