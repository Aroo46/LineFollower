/*
 * pwm.h
 *
 *  Created on: 31 mar 2019
 *      Author: Arkadiusz Kwaœniak
 */

#ifndef PWM_PWM_H_
#define PWM_PWM_H_

#include <stdbool.h>
#include <avr/io.h>

/*Definicja Portow Pinow do obslugi sygnalu PWM oraz driver'a dla silnikow*/
#define DRIVER_AIN2			1<<PC7
#define DRIVER_AIN1 		1<<PC6
#define DRIVER_STBY			1<<PC5
#define DRIVER_BIN1			1<<PC4
#define DRIVER_BIN2			1<<PC3
#define DRIVER_PWMA			1<<PD4
#define DRIVER_PWMB			1<<PD5

#define DRIVER_AIN2_OUT 	DDRC |= (DRIVER_AIN2)
#define DRIVER_AIN1_OUT		DDRC |= (DRIVER_AIN1)
#define DRIVER_STBY_OUT		DDRC |= (DRIVER_STBY)
#define DRIVER_BIN1_OUT		DDRC |= (DRIVER_BIN1)
#define DRIVER_BIN2_OUT		DDRC |= (DRIVER_BIN2)
#define DRIVER_PWMA_OUT		DDRD |= (DRIVER_PWMA)
#define DRIVER_PWMB_OUT		DDRD |= (DRIVER_PWMB)

#define DRIVER_AIN2_ON		PORTC |= (DRIVER_AIN2)
#define DRIVER_AIN1_ON		PORTC |= (DRIVER_AIN1)
#define DRIVER_STBY_ON		PORTC |= (DRIVER_STBY)
#define DRIVER_BIN1_ON		PORTC |= (DRIVER_BIN1)
#define DRIVER_BIN2_ON		PORTC |= (DRIVER_BIN2)

#define DRIVER_AIN2_OFF		PORTC &=~(DRIVER_AIN2)
#define DRIVER_AIN1_OFF		PORTC &=~(DRIVER_AIN1)
#define DRIVER_STBY_OFF		PORTC &=~(DRIVER_STBY)
#define DRIVER_BIN1_OFF		PORTC &=~(DRIVER_BIN1)
#define DRIVER_BIN2_OFF		PORTC &=~(DRIVER_BIN2)


void PWM_Driver_Init (void);


#endif /* PWM_PWM_H_ */
