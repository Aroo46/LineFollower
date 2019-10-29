/*
 * drive.c
 *
 *  Created on: 12 lut 2018
 *      Author: Arkadiusz Kwaœniak
 */
#include <avr/io.h>
#include "../common.h"
#include "../PID/pid.h"
#include "drive.h"
#include "../MKUART/mkuart.h"
#include "../MK_TERMINAL/mk_term.h"
#include "../common.h"
#include "../ADC_sensors/adc_sensors.h"
#include "../PWM/pwm.h"

/******Variables related to PID controller*****/
pidData_t PD_Sensors; 						//Struktura do regulatora PD - regulator do obs³ugi sensorów
uint16_t P_term_sens_tab = 8; 				//Zmienna do sta³ej P - regulatora PD - do obs³ugi sensorów
uint16_t D_term_sens_tab = 0; 				//Zmienna do sta³ej D - regulatora PD - do obs³ugi sensorów


/******Main Drive Function********/
void Drive_Main(void){

	//char znak;
	uint8_t sens = 10;
	DRIVER_STBY_OFF;
	DRIVER_AIN1_OFF;
	DRIVER_AIN2_ON;
	DRIVER_BIN2_OFF;
	DRIVER_BIN1_ON;
	//tr_cursor_hide(1);
	while(1){
		sens = Sensors_regulator_object();
		//Prosty program sterujacy robotem ver 1.3*/
		switch(sens){
			case 0:
				Go_soft_Left();
				break;
			case 1:
				Go_Left();
				break;
			case 2:
			case 3:
				Go_harder_Left();
				break;
			case 4:
				Go_Right();
				break;
			case 5:
				Go_soft_Right();
				break;
			case 6:
			case 7:
				Go_harder_Right();
				break;
			default:
				OCR1B = 0;
				OCR1A = 0;
				break;
		}
		//znak = uart_getc();
		tr_locate(2,5);
		USART_puts("Tablica sensorow:");
		tr_locate(3,5);
		USART_puts("sensors[0]: ");
		USART_putlong(sensors[0],10);
		USART_puts("  ");
		tr_locate(5,5);
		USART_puts("sensors[1]: ");
		USART_putlong(sensors[1],10);
		USART_puts("  ");
		tr_locate(7,5);
		USART_puts("sensors[2]: ");
		USART_putlong(sensors[2],10);
		USART_puts("  ");
		tr_locate(9,5);
		USART_puts("sensors[3]: ");
		USART_putlong(sensors[3],10);
		USART_puts("  ");
		tr_locate(11,5);
		USART_puts("sensors[4]: ");
		USART_putlong(sensors[4],10);
		USART_puts("  ");
		tr_locate(13,5);
		USART_puts("sensors[5]: ");
		USART_putlong(sensors[5],10);
		USART_puts("  ");
		tr_locate(15,5);
		USART_puts("sensors[6]: ");
		USART_putlong(sensors[6],10);
		USART_puts("  ");
		tr_locate(17,5);
		USART_puts("sensors[7]: ");
		USART_putlong(sensors[7],10);
		USART_puts("  ");
		tr_locate(20,5);
		USART_puts(" pokaz_wynik ");
		USART_putlong(sens,10);
		USART_puts("  ");
		tr_locate(22,5);

		//USART_puts("Nacisnij 'q' aby wrocic do trybu ustawien");
		//if(znak == 'q'){
		//	ChangeProgramState();
		//	break;
		//}
	}

}

/**********Sensor object - for PID controller******/

uint8_t Sensors_regulator_object (void){
	uint8_t num_of_sensor = 10;					//Zainicjalizowanie zmiennej wartoscia wieksza niz uzywana w switchu na wypadek problemu z odczytem z sensorow - wejdziemy do default

	ADC_GetSensorState();							// Wykonanie pomiaru przez ADC - uaktualnienie tablicy sensorów
	//Stare podejscie -->	sum = (((-1)*sensors[1])+((-2)*sensors[0])+((-3)*sensors[2])+sensors[4]+((-4)*sensors[3])+((2)*sensors[5])+((3)*sensors[7])+((4)*sensors[6]));
	for(uint8_t i = 0; i < LICZBA_SENSOROW; i++){
		if(sensors[i] > TRESHOLD_FOR_SENSORS){
			num_of_sensor = i;
			break;
		}
	}

	return num_of_sensor;
}

void Brake (void){

	DRIVER_AIN1_ON;
	DRIVER_AIN2_ON;
	DRIVER_BIN1_ON;
	DRIVER_BIN2_ON;

}

void Go_Straight(void){
	static uint8_t count = 0;
	if(count == 0){
		OCR1B = 80;
		OCR1A = 80;
		count = 1;
	}else{
		OCR1B = 50;
		OCR1A = 50;
	}

}

void Go_soft_Left(void){
	OCR1B = 100;
	OCR1A = 20;
}

void Go_harder_Left(void){
	/*DOBRE USTAWIENIA*/
	OCR1B = 100;
	OCR1A = 5;

}

void Go_Left(void){
	/*DOBRE USTAWIENIA*/
	OCR1B = 90;
	OCR1A = 40;
}

void Go_soft_Right(void){
	OCR1B = 20;
	OCR1A = 100;
}

void Go_harder_Right(void){
	/*DOBRE USTAWIENIA*/
	OCR1B = 5;
	OCR1A = 100;
}

void Go_Right(void){
	/*DOBRE USTAWIENIA*/
		OCR1B = 40;
		OCR1A = 90;

	//OCR1B = 30;
	//OCR1A = 140;
}
