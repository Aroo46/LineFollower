/*
 * pwm.c
 *
 *  Created on: 25 mar 2019
 *      Author: Arkadiusz Kwaœniak
 */
/*Plik do obslugi sygnalu PWM dla silnikow Line Followera*/
#include <avr/io.h>
#include <stdbool.h>
#include "pwm.h"

void PWM_Driver_Init (void){

	/*Ustawienie Portow drivera/PWM jako wyjscia*/
	DRIVER_AIN1_OUT;
	DRIVER_AIN2_OUT;
	DRIVER_BIN1_OUT;
	DRIVER_BIN2_OUT;
	DRIVER_STBY_OUT;
	DRIVER_PWMB_OUT;
	DRIVER_PWMA_OUT;

}
