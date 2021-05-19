/* 
 * File:   uart.h
 * Author: guasonito
 *
 * Created on 15 de marzo de 2021, 02:37 PM
 */

#ifndef UART_H
#define	UART_H

#define TX_FLAG U1ERRIRbits.U1TXMTIF //'1' shift register is empty
#define RX_BUFFER_EMPTY U1FIFObits.RXBE
#define TO_TRANSMIT U1TXB
#define RECEIVED U1RXB
#include "D:/Documentos/Universidad/Semestre 8/Proyecto 2/Laboratorio3_Proyecto2/PIC18F47K42/queue/queue.h"
void config_UART();
void transmit_UART(int value);
short int error_handler(char *vector, short int *cont);
int receive_UART();
void int_transmit(char vector[],short int *cont);

#endif	/* UART_H */

