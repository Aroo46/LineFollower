/*
 * mk_term.c
 *
 *  Created on: 30-04-2013
 *      Author: Miros³aw Kardaœ
 */
#include <avr/io.h>
#include <avr/pgmspace.h>

#include "../MKUART/mkuart.h"
#include "mk_term.h"

// aby mieæ polskie ogonki w PUTTY ----> ustawiæ WINDOW / Translation / Win1250

const char UCLS[] PROGMEM = { "\x1b""[2J" };

const char UHOME[] PROGMEM = { "\x1b""[;H" };

const char UCUR_HIDE[] PROGMEM = { "\x1b""[?25l" };
const char UCUR_SHOW[] PROGMEM = { "\x1b""[?25h" };

const char U_ATTR_OFF[] PROGMEM = { "\x1b""[m" };



void tr_cursor_hide( uint8_t hide ) {
	if(hide) USART_puts_P( UCUR_HIDE );
	else USART_puts_P( UCUR_SHOW );
}


void tr_cls(uint8_t cur_onoff) {

	USART_puts_P( U_ATTR_OFF );
	tr_cursor_hide(cur_onoff);
	USART_puts_P( UCLS );

	USART_puts_P( UHOME );

}


void fill_line( char ascii, uint8_t cnt ) {
	for(uint8_t i=0; i<cnt; i++) USART_Transmit( ascii );
}


void tr_attr( uint8_t atr, uint8_t fg, uint8_t bg ) {
	USART_Transmit( 0x1b );		// <ESC>[0;32;44m
	USART_Transmit( '[' );
	USART_Transmit( atr+'0' );
	USART_Transmit( ';' );
	USART_Transmit( '3' );
	USART_Transmit( fg+'0' );
	USART_Transmit( ';' );
	USART_Transmit( '4' );
	USART_Transmit( bg+'0' );
	USART_Transmit( 'm' );
}


void tr_pen_color( uint8_t cl ) {
	USART_Transmit( 0x1b );		// <ESC>[34m
	USART_Transmit( '[' );
	USART_Transmit( '3' );
	USART_Transmit( cl+'0' );
	USART_Transmit( 'm' );
}

void tr_brush_color( uint8_t cl ) {
	USART_Transmit( 0x1b );		// <ESC>[44m
	USART_Transmit( '[' );
	USART_Transmit( '4' );
	USART_Transmit( cl+'0' );
	USART_Transmit( 'm' );
}


void tr_locate( uint8_t y, uint8_t x ) {

	USART_Transmit( 0x1b );	// <ESC>[y;xH
	USART_Transmit( '[' );
	USART_putlong( y, 10 );
	USART_Transmit( ';' );
	USART_putlong( x, 10 );
	USART_Transmit( 'H' );
}


