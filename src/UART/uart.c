/*
 * uart.c
 *
 *  Created on: 14.10.2019
 *      Author: Arek
 */
#include "uart.h"
#include <stdlib.h>


/*******************************FUNCTIONS*************************/
//Definicja Funkcji wysy³aj¹cej ca³e stringi
void USART_puts ( char * s ) {
	while( *s ) USART_Transmit( *s++ );
	while(!(USART3->ISR & USART_ISR_TC));
}

//Definicja funkcji nadawczej
void USART_Transmit ( char data ) {
	/*czekaj a¿ bufor nadawczy bêdzie pusty*/
	while(!(USART3->ISR & (USART_ISR_TXE)) );
	/*wrzuc dane do bufora nadawczego, start transmisji */
	USART3 ->TDR = data;
}

//Definicja funkcji do wysy³ania liczb
void USART_putlong ( uint32_t liczba, uint8_t radix)
{
	char buf[17];
	itoa(liczba, buf, radix );
	USART_puts( buf );

}

void USART_Init(void){
	//Ustawienia USART - dla USART3 -
	USART3 -> BRR = 8000000/9600; //Ustawnieni predkosci na 9600
	USART3 -> CR1 |= USART_CR1_UE | USART_CR1_TE | USART_CR1_RE | USART_CR1_RXNEIE; // Wlaczenie mozliwosci wysylania TE, odbierania RE i calego USARTA UE
}

void tr_locate( uint8_t y, uint8_t x ) {

	USART_Transmit( 0x1b );	// <ESC>[y;xH
	USART_Transmit( '[' );
	USART_putlong( y, 10 );
	USART_Transmit( ';' );
	USART_putlong( x, 10 );
	USART_Transmit( 'H' );
}

void tr_cls (void){  //Funkcja czyszcz¹ca terminal

	USART_puts("\x1b""[m");
	USART_puts("\x1b""[2J");
	USART_puts("\x1b""[;H");
}

void tr_cursor_off (void){
	USART_puts("\x1b""[?25l"); //Ukryj kursor!
}
/*****************************************************************/
