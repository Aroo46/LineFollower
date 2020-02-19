/*
 * uart.h
 *
 *  Created on: 14.10.2019
 *      Author: Arek
 */

#ifndef UART_UART_H_
#define UART_UART_H_
/***********************INCLUDES*******************************/
#include "stm32f303xc.h"
/**************************************************************/
/***********************MAKROS*********************************/




/**********************************************************************/
/**********************************FUNCTIONS*************************************/
void USART_Init(void); //Funkcja do inicjalizacji USART
void USART_puts ( char * s ); //Funkcja do wyslania calych stringow przez USART
void USART_Transmit( char data ); //Funkcja do wysylania pojedynczego znaku przez USART
void USART_putlong ( uint32_t liczba, uint8_t radix); //Funkcja do wysylania liczb przez USART
void tr_locate( uint8_t y, uint8_t x ); //Funkcja pomocnicza do USARTa
void tr_cls (void);
void tr_cursor_off (void);
/********************************************************************************/
#endif /* UART_UART_H_ */
