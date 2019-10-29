/*
 * settings.c
 *
 *  Created on: 6 lut 2018
 *      Author: Arkadiusz Kwaœniak
 */
#include <avr/io.h>

#include "settings.h"
#include "../common.h"
#include "../MKUART/mkuart.h"
#include "../MK_TERMINAL/mk_term.h"

/*********** Main settings function ***********/
void Settings_Main (void){
	char znak;
	Draw_menu();
	znak = uart_getc();
	if(znak == '1') PWM_settings();
	if(znak == '2') PID_settings();
	if(znak == 'q') ChangeProgramState();
}

/***** Function to change PID settings ********/
void PID_settings(void){

	tr_cls(1);
	char znak;
	while(1){
	znak = uart_getc();
	Draw_PID_settings();
	if('q'==znak) break;
	}
	tr_cls(1);
}

/***** Function to change PWM settings ********/
void PWM_settings(void){

	tr_cls(1);
	char znak;
	while(1){
	znak = uart_getc();
	Draw_PWM_settings();
	if('q'==znak) break;
	}
	tr_cls(1);

}

/*** Function to draw the settings menu *******/
void Draw_menu(void){


	tr_cursor_hide(1);
	tr_locate(2,5);
	USART_puts("Wybierz 1 aby zmienic ustawienia PWM,");
	tr_locate(3,5);
	USART_puts("lub 2 aby zmienic ustawienia regulatora PID: ");
	tr_locate(4,5);
	USART_puts("1. PWM.");
	tr_locate(5,5);
	USART_puts("2. Regulator PID. ");
	tr_locate(6,5);
	USART_puts("Nacisnij 'q' aby wrocic w tryb jazdy.");
}

/*** Function to draw the PID settings menu *******/
void Draw_PID_settings(void){
	tr_cursor_hide(1);
	tr_locate(2,5);
	USART_puts("Ustawienia regulatora PID:");
	tr_locate(5,5);
	USART_puts("Nacisnij 'q' aby wyjsc");
}
/*** Function to draw the PWM settings menu *******/
void Draw_PWM_settings(void){
	tr_cursor_hide(1);
	tr_locate(2,5);
	USART_puts("Ustawienia PWM:");
	tr_locate(5,5);
	USART_puts("Nacisnij 'q' aby wyjsc");
}
