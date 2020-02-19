/*
 * common.c
 *
 *  Created on: 23 sty 2018
 *      Author: Arkadiusz Kwaœniak
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <util/delay.h>
#include "common.h"
#include "PWM/pwm.h"

//Wstêpnie ProgTimer1 = 5 aby przerwanie wykonywa³o siê co 5 ms. (ADC)
//ProgTimer2 = 50 ms - tyle czekamy a¿ skoñcz¹ siê drgania styków
/*****************Definitions of the VARIABLES****************/
volatile uint8_t ProgTimer1 = 5, ProgTimer2;
volatile uint8_t pwm2 = 60;
bool Program_state;

/*****************Definitions of the FUNCTIONS****************/

/****TIMER0 Initialisation******/
void Timer0_Init (void){
	TCCR0 |= (1<<WGM01);								//Ustawienie trybu CTC
	TCCR0 |= (1<<CS02)|(1<<CS00);						//Ustawienie preskalera na 1024
	OCR0 = 10;                                          //Przerwanie Compare Match co 1 ms
	TIMSK |= (1<<OCIE0);								//W³¹czenie (wektora przerwañ) przerwania Compare Match

}

/****TIMER1 Initialisation******/
void Timer1_Init (void){
	TCCR1A |= (1<<WGM10);
	TCCR1B |= (1<<WGM12);								//Ustawienie trybu FAST PWM
	TCCR1A |= (1<<COM1A1)|(1<<COM1B1);					//Wyzerowanie OC1 przy porównaniu, ustawienie przy wartoœci BOTTOM OCR1n
	TCCR1B |= (1<<CS11)|(1<<CS10);						//Preskaler = 64, fPWM = 1kHz
	OCR1A = 140;										//Wypelnienie PWMa - PIN PD5
	OCR1B = 140;										//Wypelnienie PWMa - PIN PD4

}


/****LED Initialisation*********/
void Led_Init (void){
	LED_SERVICE_OUT;
    LED_RIDE_OUT;
    LED_RIDE_ON;
    LED_SERVICE_ON;
    LED_SERVICE_OFF;
    LED_RIDE_OFF;

    /*LED do sprawdzenia ADCka*/
    DDRB |= (1<<PB0)|(1<<PB1)|(1<<PB2)|(1<<PB3);

}

/****Button Initialisation******/
void Button_Init(void){
	BUTTON_PORT_IN;
	BUTTON_PULL_UP;
}

/****External Interrupt*********/
void Ext_Interrupt_Init(void){
	DDRD &=~(1<<PD2);
	DDRD &=~(1<<PD3);
	PORTD |= (1<<PD2)|(1<<PD3);					//Ustawienia pinów jako wejœcia i podci¹gniecie do VCC

	MCUCR |= (1<<ISC11)|(1<<ISC01);				//set INT1 and INT0 ext. interrupt on falling edge
	GICR |= (1<<INT1)|(1<<INT0);				//Enable external int on PIN PD3, PD2
}

/****Button usage***********************************/
void SuperDebounce ( volatile uint8_t *KPIN, uint8_t key_mask, uint16_t rep_time,
		uint16_t rep_wait, void (*push_proc) (void), void (*rep_proc) (void)){

	enum KS {idle,debounce, go_rep, wait_rep, rep};

	//komórka w pamiêcie RAM przechowuj¹ca stan klawisza,
	//w jakim siê znajduje, gdy jest wciœniêty
	static enum KS key_state;
	static uint8_t last_key;  					//ostatio u¿yty klawisz

	//zmienna key_press to bufor stanu klawisza w trakcie dzia³ania funkcji
	uint8_t key_press;

	//zabezpieczenie przed jednoczesnym wciskaniem dwóch klawiszy
	if (last_key && last_key != key_mask ) return;

	key_press = !(*KPIN & key_mask);			//stan klawisza

	if( key_press && !key_state){
		key_state = debounce;  					//jeœli wciœniêty, poczekaj
		ProgTimer2 = 50;						//50 ms a¿ min¹ drgania styków
	}else
		if ( key_state){						//jeœli klawisz wciœniêty a minê³y ju¿ drgania

			if( key_press && debounce == key_state && !ProgTimer2) {
				key_state = go_rep;
				ProgTimer2 = 30; 				//poczekaj jeszcze 30 ms po drganiach
				last_key = key_mask;
			}else

				//jeœli klawisz zosta³ zwolniony w chwili czekania na funkcjonalnoœc REAPET
				if (!key_press && key_state > debounce && key_state< rep){
					//wykonaj funkcje jeœli wskaŸnik ró¿ny od zera
					if( push_proc)push_proc();   /***klawisz zwolniony wywo³aj funkcje***/
					key_state = idle; 			 //przejdŸ do stanu oczekiwania
					last_key = 0;
				}else
					//Jeœli min¹³ czas drgañ styków, zacznij odliczac czas na REAPET
					if ( key_press && go_rep == key_state && !ProgTimer2){
						//nadanie wartoœci domyœlnych jeœli parametr mia³ wartoœc 0
						if(!rep_time) rep_time = 20;
						if(!rep_wait) rep_wait = 150;
						key_state = wait_rep;
						ProgTimer2 = rep_wait;
					}else
						// jeœli min¹³ czas oczekiwania na REAPET - wykonaj akcje
						if(key_press && wait_rep == key_state && !ProgTimer2){
							key_state = rep;
						}else
							//tryb powtarzania akcji/funkcji rep_proc, jeœli wskaŸnik ró¿ny od 0
							//przez ca³y czas, gdy klawisz jest wci¹¿ wciœniêty
							if(key_press && rep == key_state && !ProgTimer2){
								ProgTimer2 = rep_time; // za³aduj ponownie czas powtarzania akcji
								if (rep_proc) rep_proc(); // KEY UP
							}
		}
	//jeœli klawisz zosta³ zwolniony w trybie rep, przejdŸ do stanu oczekiwania
	if( key_state >= wait_rep && !key_press){
		key_state = idle;
		last_key = 0;
	}
}

/******Function which will change program state****************/
void ChangeProgramState (void){
	if(Program_state == true){
		Program_state = false;														//Zmiana stanu "Program_state" na przeciwny
	}else{
		Program_state = true;
	}
}

/****Internal interrupt - Compare Match - TIMER0******/
ISR(TIMER0_COMP_vect){
	uint8_t sprawdz_timer;								//Zmienna do obs³ugi timera sprzêtowego
	sprawdz_timer = ProgTimer1;							//do sprawdz_timer przypisujemy wartoœc timera programowego
	if (sprawdz_timer) ProgTimer1 = --sprawdz_timer; 	//Je¿eli wiêksza od 0 - zmienjszamy j¹ o 1 i przypisujemy do timera programowego

	sprawdz_timer = ProgTimer2;
	if (sprawdz_timer) ProgTimer2 = --sprawdz_timer;   	//Wykonujemy to samo dla drugiego Timera

}

