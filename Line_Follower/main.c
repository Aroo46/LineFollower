/*
 * main.c
 *
 *  Created on: 21 sty 2018
 *  Author: Arkadiusz Kwaœniak
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#include "ADC_sensors/adc_sensors.h"
#include "PID/pid.h"
#include "PWM/pwm.h"
#include "common.h"
#include "Settings/settings.h"
#include "Drive/drive.h"
#include "MKUART/mkuart.h"
#include "MK_TERMINAL/mk_term.h"

/**********************TEST PRZERWAN ZEWNETRZNYCH******************/
volatile uint8_t calc_left = 0;
volatile uint8_t calc_right = 0;
/**************************Konic TESTU******************************/


int main ( void ){

#if REG_ON == 1
	wejscie_obiektu we = {0};
	wyjscie_obiektu wyj = {0};
	wyjscie_obiektu w_zad = {0};
#endif
#if REG_ON == 2
	wejscie_obiektu we = {0, 20, 0, 0, 0};
	wyjscie_obiektu wyj = {0, 0, 0};
	wyjscie_obiektu w_zad = {0, 0, 0};
#endif

	Ext_Interrupt_Init();													//Inicjalizacja przerwan zewnetrznych - enkodery
	ADC_Init();																//Inicjalizacja modu³u ADC
	Timer0_Init();															//Inicjalizacja Timer0
	Timer1_Init();															//Inicjalizacja Timer2 - PWM
	PWM_Driver_Init();
	Led_Init();																//Inicjalizacja diod LED
	Button_Init();															//Inicjalizacja klawiszy
	USART_Init(__UBRR);														//Inicjalizacja UART
	//TODO: Dokonczyc regulator PID dla sensorow!!

	sei();																	//W³¹czenie globalnych przerwañ
	/*PWM test - only this code will be executed*/
	/*
	while(1){
		SuperDebounce(&PINB, KL1, 5, 400, ChangeProgramState, 0);
		if(Program_state == true){
			//Drive() --> FUNKCJA DO OBSLUGI JAZDY BEZ PID'a
			Drive_Main();
			LED_RIDE_ON;
			LED_SERVICE_OFF;
		}
		if(Program_state == false){
			Brake();														//Wyhamuj robota - serwis
			//LED_SERVICE_ON;
			//LED_RIDE_OFF;
		}


	}
	*/
	/*end of PWM test code*/
	/*
	while(1){																//Pêtla g³ówna programu w której mamy 2 pêtle:

		tr_cls(1);															//Wyczyszczenie ekranu miêdzy przejsciami pêtli
		while(1){															//Jedna przeznaczona do pracy w trybie opcji - zmiana ustawieñ
				SuperDebounce(&PINB, KL1, 5, 400, ChangeProgramState, 0);
				if(Program_state == true){ 									//Druga przeznaczona do pracy w trybie jazdy - podczas pokonywaniu toru przez Line Followera
					Drive_Main();											//Prze³¹czanie pomiêdzy pêtlami nastêpuje przez naciœniêcie przycisku

				}
				else break;
			}
		tr_cls(1);
		while(1){
				SuperDebounce(&PINB, KL1, 5, 400, ChangeProgramState, 0);
				if(Program_state == false){
					Settings_Main();
				}
				else break;

			}
	}*/

	/*TEST PRZERWAN ZEWNETRZNYCH*/
	while(1){
#if REG_ON == 1
		w_zad.position = 0;
		//TIM2->CCR2 = 20;
		wyj.position = PID_obiekt(we);
		//Show_PID_test(we,wyj,w_zad);
		we.reg_speed = reg_PID(w_zad,wyj);
		//Menu(we, wyj, w_zad);
#endif

#if REG_ON == 2
		wyj.position = PD_obiekt(we);
		we.reg_speed = reg_PD_pozycja(w_zad, wyj);
		wyj.speed_of_left_eng = Odczyt_lewy_enkoder();
		wyj.speed_of_right_eng = Odczyt_prawy_enkoder();
		we.reg_speed_translation = reg_PD_translacji(we, wyj, w_zad);
		we.reg_speed_rotation = reg_PD_rotacji(we, wyj, w_zad);

#endif
		/*if(calc_left >= 35){
			for(uint8_t licz = 0; licz<500; licz++){
				LED_RIDE_ON;
			}
			calc_left = 0;
		}else{
			LED_RIDE_OFF;
		}
		if(calc_right >= 35){
			for(uint8_t zlicz = 0; zlicz<500; zlicz++){
				LED_SERVICE_ON;
			}
			calc_right = 0;
		}else {
			LED_SERVICE_OFF;
		}
		*/
	}
	/*KONIEC TESTU PRZERWAN ZEWNETRZNYCH*/


}

/****External interrupts for encoder******/
ISR(INT0_vect){
	LED_RIDE_TOGGLE;
}

ISR(INT1_vect){
	calc_right++;
}
