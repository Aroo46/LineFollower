/*
 * common.h
 *
 *  Created on: 23 sty 2018
 *      Author: Arkadiusz Kwaœniak
 */

#ifndef COMMON_H_
#define COMMON_H_

#include <stdbool.h>
#include <avr/io.h>

/****LED Macro*********************/
#define LED_RIDE_PIN 1<<PC0                					//Makra do obs³ugi diod LED
#define LED_SERVICE_PIN 1<<PC1
#define LED_RIDE_OUT DDRC |= (LED_RIDE_PIN)
#define LED_RIDE_ON PORTC &=~ (LED_RIDE_PIN)
#define LED_SERVICE_OUT DDRC |= (LED_SERVICE_PIN)
#define LED_SERVICE_ON PORTC &=~ (LED_SERVICE_PIN)
#define LED_RIDE_OFF PORTC |= (LED_RIDE_PIN)
#define LED_SERVICE_OFF PORTC |= (LED_SERVICE_PIN)
#define LED_RIDE_TOGGLE PORTC ^= (LED_RIDE_PIN)


/****Button Macro******************/
#define KL1 (1<<PB4)										//Makra do obs³ugi klawiszy
#define BUTTON_PORT_IN DDRB &=~ KL1
#define BUTTON_PULL_UP PORTB |= KL1

/****Variables******************/
extern volatile uint8_t ProgTimer1, ProgTimer2;

extern bool Program_state;

/****TIMER0 Initialisation******/
void Timer0_Init (void);

/****TIMER1-PWM Initialisation**/
void Timer1_Init (void);

/****External Interrupt*********/
void Ext_Interrupt_Init(void);

/****LED Initialisation*********/
void Led_Init (void);

/****Button Initialisation******/
void Button_Init(void);

/****Button usage***************/
void SuperDebounce ( volatile uint8_t *KPIN, uint8_t key_mask, uint16_t rep_time,
		uint16_t rep_wait, void (*push_proc) (void), void (*rep_proc) (void));

/****Change of program state - Run or Settings****/
void ChangeProgramState (void);



#endif /* COMMON_H_ */
