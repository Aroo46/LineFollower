/*
 * config.h
 *
 *  Created on: 14.10.2019
 *      Author: Arek
 */

#ifndef CONFIG_CONFIG_H_
#define CONFIG_CONFIG_H_

/******************************INCLUDES*******************************************/
#include "stm32f303xc.h"
#include <stdlib.h>
/*********************************************************************************/
/******************************DEFINES********************************************/


//Makro definicje do obslugi driver'a silnika
#define DRIVER_AIN2_ON		GPIOD->BSRRL=GPIO_BSRR_BS_2
#define DRIVER_AIN1_ON		GPIOD->BSRRL=GPIO_BSRR_BS_3
#define DRIVER_BIN1_ON		GPIOD->BSRRL=GPIO_BSRR_BS_4
#define DRIVER_STBY_ON		GPIOD->BSRRL=GPIO_BSRR_BS_5
#define DRIVER_BIN2_ON		GPIOD->BSRRL=GPIO_BSRR_BS_6

#define DRIVER_AIN2_OFF		GPIOD->BRR=GPIO_BRR_BR_2
#define DRIVER_AIN1_OFF		GPIOD->BRR=GPIO_BRR_BR_3
#define DRIVER_BIN1_OFF		GPIOD->BRR=GPIO_BRR_BR_4
#define DRIVER_STBY_OFF		GPIOD->BRR=GPIO_BRR_BR_5
#define DRIVER_BIN2_OFF		GPIOD->BRR=GPIO_BRR_BR_6

//Makro definicje do obslugi diod LED
#define LED_PE8_ON			GPIOE->BSRRL=GPIO_BSRR_BS_8
#define LED_PE9_ON			GPIOE->BSRRL=GPIO_BSRR_BS_9
#define LED_PE10_ON			GPIOE->BSRRL=GPIO_BSRR_BS_10

#define LED_PE8_OFF			GPIOE->BRR=GPIO_BRR_BR_8
#define LED_PE9_OFF			GPIOE->BRR=GPIO_BRR_BR_9
#define LED_PE10_OFF		GPIOE->BRR=GPIO_BRR_BR_10
/*********************************************************************************/
/*******************************VARIABLES******************************************/
extern volatile uint16_t ProgTimer2, ProgTimer;
extern uint8_t Program_state;

/*********************************************************************************/


/********************************FUNCTIONS*****************************************/
void PWM_config (void); //Funkcja konfiguruj¹ca PWM
void RCC_Init(void); // Funkcja inicjalizacyjna, uruchamiajaca zegar RCC dla odpowiednich modulow
void Encoder_mode_config (void); //Konfiguracja wejsc enkodera
void GPIO_init(void); //Konfiguracja portów I/O
void Timer_Counter_init(void);	//Inicjalizacja timer'a/counter'a
void SuperDebounce (volatile uint32_t *KPIN, uint32_t key_mask, uint16_t rep_time,
		uint16_t rep_wait, void (*push_proc) (void), void (*rep_proc) (void));
void ChangeProgramState (void);
/**********************************************************************************/

#endif /* CONFIG_CONFIG_H_ */
