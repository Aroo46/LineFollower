/*
 * config.c
 *
 *  Created on: 14.10.2019
 *      Author: Arek
 */
/********************************INCLUDES******************************************/
#include "config.h"
/**********************************************************************************/


/********************************FUNCTIONS******************************************/
void RCC_Init(void){
	// wlaczenie zegarow dla poszczegolnych grup
	RCC -> AHBENR = RCC_AHBENR_GPIOAEN | RCC_AHBENR_DMA1EN | RCC_AHBENR_GPIODEN | RCC_AHBENR_GPIOBEN | RCC_AHBENR_GPIOEEN | RCC_AHBENR_GPIOCEN | RCC_AHBENR_ADC12EN | RCC_AHBENR_ADC34EN | RCC_AHBENR_DMA2EN;
	RCC -> APB1ENR = RCC_APB1ENR_USART3EN | RCC_APB1ENR_TIM2EN | RCC_APB1ENR_TIM3EN;
	RCC -> APB2ENR |= RCC_APB2ENR_TIM1EN | RCC_APB2ENR_TIM8EN;
}

void PWM_config (void){
	TIM2->CCMR2 |= TIM_CCMR2_OC3PE | TIM_CCMR2_OC3M_2 | TIM_CCMR2_OC3M_1;
	TIM2->CCMR1 |= TIM_CCMR1_OC2PE | TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_2;

	TIM2->CCER |= TIM_CCER_CC3E | TIM_CCER_CC2E;
	TIM2->BDTR |= TIM_BDTR_MOE;

	TIM2->PSC = 800-1;
	TIM2->ARR = 99;
	TIM2->CCR3 = 0; //Wypelnienie sygnalu PWM dla PINu 3 LEWY SILNIK
	TIM2->CCR2 = 0; //Wypelnienie sygnalu PWM dla PINu 2 PRAWY SILNIK

	TIM2 -> EGR = TIM_EGR_UG;
	TIM2 -> CR1 = TIM_CR1_ARPE|TIM_CR1_CEN;

}
//Ustawienie Enkodera na TIM1_CH1, TIM1_CH2 oraz TIM8_CH1, TIM8_CH2
void Encoder_mode_config (void){
	TIM1 -> SMCR |= TIM_SMCR_SMS_0 | TIM_SMCR_SMS_1;	// Zliczanie na obu kanalach enkodera
	TIM1 -> CCMR1 |= TIM_CCMR1_CC1S_0 | TIM_CCMR1_CC2S_0;// Oba kanaly TI1 oraz TI2 sa wybrane jako wejscia
	TIM1 -> CCER |= TIM_CCER_CC2P;
	TIM1 -> ARR = 30; 									//Ustawienie compare register na 30 1/4 obrotu kola - z ta wartoscia porownujemy nasz zliczajacy counter
	TIM1 -> DIER |= TIM_DIER_UIE;
	TIM1 -> CR1 |= TIM_CR1_CEN; 						//Wlaczenie countera


	TIM8 -> SMCR |= TIM_SMCR_SMS_0 | TIM_SMCR_SMS_1; 					// Zliczanie na kanale obu kanalach enkodera
	TIM8 -> CCMR1 |= TIM_CCMR1_CC1S_0 | TIM_CCMR1_CC2S_0; // Oba kanaly TI1 oraz TI2 sa wybrane jako wejscia
	TIM8 -> CCER |= TIM_CCER_CC2P;
	TIM8 -> ARR = 30; 									//Ustawienie compare register na 30 1/4 obrotu kola- z ta wartoscia porownujemy nasz zliczajacy counter
	TIM8 -> DIER |= TIM_DIER_UIE;
	TIM8 -> CR1 |= TIM_CR1_CEN; 						//Wlaczenie countera
}
// Inicjalizacja Timer'a Counter'a - Przerwanie co 100 ms
/*
 * czestotliwosc przerwania Fuev = Ftim(czest. sygnalu zegarowego)/(ARR + 1)*(PSC + 1)
 *
 */
void Timer_Counter_init(void){
	//TIMER WYLACZONY!!		- Nie wysylamy informacji o wartosciach liczonych przez enkoder
	TIM3 -> PSC = 3999;				// Wlaczenie preskalera
	TIM3 -> ARR = 1999;				// ustalenie wartosci bloku zliczajacego
	TIM3 -> DIER = TIM_DIER_UIE;	// Wlaczenie przerwan dla licznika
	//TIM3 ->CR1 = TIM_CR1_CEN;		//Wlaczenie licznika
}

//Konfiguracja port�w I/O - funkcja alternatywne, odpowiednie opcje pracy.
void GPIO_init(void){
	//Ustawienie alternatywnej funkcji dla portu
	GPIOA -> AFR[0] = 0x110; // Alternatywna funkcja dla PA1, PA2 - TIM2
	GPIOA -> AFR[1] = 0x66; // AF6 dla pinow PA8, PA9- funkcja TIM1_CH1, TIM1_CH2 - enkodery
	GPIOB -> AFR[1] = 0x7700; // Alternatywna funkcja AF7 - USART dla pin�w PB10 i PB11
	GPIOC -> AFR[0] = 0x44000000; //Alternatywna funkcja dla PC6 i PC7 - tryb enkodera TIM8_CH1 TIM8_CH2

	//Ustawienie odpowiedniego trybu pracy pin�w
	GPIOA -> MODER |= GPIO_MODER_MODER1_1 | GPIO_MODER_MODER2_1 | GPIO_MODER_MODER8_1 | GPIO_MODER_MODER9_1; // ALternatywna funkcja dla PA8 PA9
	GPIOC -> MODER |= GPIO_MODER_MODER0 | GPIO_MODER_MODER1 | GPIO_MODER_MODER2 | GPIO_MODER_MODER3 | GPIO_MODER_MODER6_1 | GPIO_MODER_MODER7_1; //Ustawienie pinu PC0, PC1, PC2, PC3 - w analog mode - ADC / oraz PC6 i PC7 w tryb alternatywny (enkoder)
	GPIOE -> MODER |= GPIO_MODER_MODER9_0 | GPIO_MODER_MODER8_0 | GPIO_MODER_MODER10_0; // Ustawienie pinu w general output
	GPIOB -> MODER |= GPIO_MODER_MODER10_1 | GPIO_MODER_MODER11_1; //Ustawienie pin�w USART w tryb alternatywny
	GPIOD -> MODER |= GPIO_MODER_MODER2_0 | GPIO_MODER_MODER3_0 | GPIO_MODER_MODER4_0 | GPIO_MODER_MODER5_0 | GPIO_MODER_MODER6_0 | GPIO_MODER_MODER10 | GPIO_MODER_MODER11 | GPIO_MODER_MODER12 | GPIO_MODER_MODER13; // Ustawienie - general output PD2, PD3, PD4, PD5, PD6, oraz - analog mode dla PD10, PD11, PD12, PD13
	//Ustawienie predkosci
	GPIOD -> OSPEEDR |= GPIO_OSPEEDER_OSPEEDR2_0 | GPIO_OSPEEDER_OSPEEDR3_0 | GPIO_OSPEEDER_OSPEEDR4_0 | GPIO_OSPEEDER_OSPEEDR5_0 | GPIO_OSPEEDER_OSPEEDR6_0;

	//Pull up or pull down - PUPDR register
	GPIOA -> PUPDR = GPIO_PUPDR_PUPDR8_0 | GPIO_PUPDR_PUPDR9_0; // Pull up mode
	GPIOB -> PUPDR = GPIO_PUPDR_PUPDR11_1; // Pull down
	GPIOC -> PUPDR = GPIO_PUPDR_PUPDR6_0 | GPIO_PUPDR_PUPDR7_0; // Pull up mode
	GPIOD -> PUPDR = GPIO_PUPDR_PUPDR2_0 | GPIO_PUPDR_PUPDR3_0 | GPIO_PUPDR_PUPDR4_0 | GPIO_PUPDR_PUPDR5_0 | GPIO_PUPDR_PUPDR6_0; //Pull up mode

	GPIOB -> OTYPER = GPIO_OTYPER_OT_11; //Open drain type

}
/**********************************************************************************/
