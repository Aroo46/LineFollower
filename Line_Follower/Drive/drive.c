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
#include <math.h>

#if (REG_ON == 2 || REG_ON == 1)
float K = 4.1;		// Wspolczynnik K
float K_I;			// Wspolczynnik Ki
float K_D = 5.6;	// Wspolczynnik Kd
#endif
#if REG_ON == 2
float K_t = 1.5;		// Wspolczynnik K_t - dla regulatora translacji
float K_I_t;		// Wpsolczynnik K_I_t - dla regulatora translacji
float K_D_t;		// Wpsolczynnik K_D_t - dla regulatora translacji
float K_r = 1.8;		// Wpsolczynnik K_r - dla regulatora rotacji
float K_I_r;		// Wpsolczynnik K_I_r - dla regulatora rotacji
float K_D_r;		// Wpsolczynnik K_D_r - dla regulatora rotacji
#endif

/* WAGI CZUJNIKOW DO PONOWNEGO PRZYPISANIA NA TESTACH - W ZALEZNOSCI OD PODLACZENIA */
int8_t tab_of_weight_sensors [LICZBA_SENSOROW] = {-1, -3, -8, -20, 20, 8, 3, 1};
uint8_t Basic_speed = 20;

#if REG_ON == 1
static void Set_Speed(wejscie_obiektu we);
#endif


#if REG_ON == 0
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

/**********Sensor object - check the sensors value******/

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

#endif

#if REG_ON == 1
float PID_obiekt (wejscie_obiektu we){
	float position = 0;
	uint8_t ilosc_czujnikow = 0;

	// Ustawiamy aktualna - zadana predkosc
	Set_Speed(we);

	// Przeliczamy dane z czujników
	for(int8_t sens = LICZBA_SENSOROW - 1; sens >= 0; sens--){
		if(sensors[sens]>TRESHOLD_FOR_SENSORS){
			position += tab_of_weight_sensors[sens];
			ilosc_czujnikow++;
		}
	}

	if(ilosc_czujnikow == 0){
		OCR1A = 0; 		//W zaleznosci od silnika dodajemy badz odejmujemy wartosc
		OCR1B = 0;
		return position = 0;
	}

	return position = (position)/ilosc_czujnikow;
}

float reg_PID (wyjscie_obiektu w_zad, wyjscie_obiektu wy_o){

	//zmienne pomocnicze
	float p, i, d, reg;
	float uchyb;					//uchyb regulacji
	static float uchyb_pop = 0; 	// uchyb w poprzednim wywolaniu
	static float suma_uchybu = 0; 	// suma minionych uchybow

	uchyb = w_zad.position - wy_o.position;  	// aktualny uchyb

	// wyznaczenie skladnika proporcjonalnego (P)
	p = K*uchyb;

	//wyznaczenie skladnika calkowego (I)
	if(uchyb == 0) suma_uchybu = 0;
	suma_uchybu = suma_uchybu+uchyb;
	i= K_I * suma_uchybu;

	//wyznaczenie skladnika rozniczkowego(D)
	d = K_D * (uchyb-uchyb_pop);
	uchyb_pop = uchyb;				// zapamietaj chwilowa wartosc bledu

	reg = p+i+d; 				//sygnal wyjsciowy regulatora

	if(reg > 22) reg = 22;
	if(reg < -22) reg = -22;

	return reg;
}

static void Set_Speed(wejscie_obiektu we){
	//ustawienie predkosci silnikow - jezeli predkosc spada ponizej 0 - zerujemy PWM
	// Jezeli predkosc wykroczy poza bezpieczna wartosc obcinamy ja - saturacja
	// 1. Sprawdzenie czy nie przekraczamy dozwolonej predkosci

	if(MAX_SPEED_VALUE < (Basic_speed - we.reg_speed)){
		OCR1A = MAX_SPEED_VALUE;
	}else if (MAX_SPEED_VALUE < (Basic_speed + we.reg_speed)) {
		OCR1B = MAX_SPEED_VALUE;
	}

	// 2. Sprawdzenie czy ustawiana predkosc nie jest ujemna - wyhamujemy do 0
	// Jezeli nie przekraczamy zakresow - ustawiamy zadana predkosc
	if(0 > (Basic_speed - we.reg_speed)){
		OCR1A = 0;
	}
	else{
		OCR1A = Basic_speed - we.reg_speed; 	//W zaleznosci od silnika dodajemy badz odejmujemy wartosc
	}

	if(0 > (Basic_speed + we.reg_speed)){
		OCR1B = 0;
	}
	else{
		OCR1B = Basic_speed + we.reg_speed;
	}

}

#endif

#if REG_ON == 2
float PD_obiekt(wejscie_obiektu we){
	float position = 0;
	uint8_t ilosc_czujnikow = 0;

	//ustawienie predkosci silnikow
	//CCR3 - lewy silnik
	//CCR2 - prawy silnik
	if((we.reg_speed_translation - we.reg_speed_rotation) < 0) OCR1A = 0;
	else OCR1A = we.reg_speed_translation - we.reg_speed_rotation;		//W zaleznosci od silnika dodajemy badz odejmujemy wartosc
	if((we.speed_translation + we.reg_speed_rotation) < 0) OCR1B = 0;
	else OCR1B = we.reg_speed_translation + we.reg_speed_rotation;

	// Przeliczamy dane z czujników
	for(int8_t sens = LICZBA_SENSOROW - 1; sens >= 0; sens--){
		if(sensors[sens]>TRESHOLD_FOR_SENSORS){
			position += tab_of_weight_sensors[sens];
			ilosc_czujnikow++;
		}
	}

	if(ilosc_czujnikow == 0){
		OCR1A = 0; 		//W zaleznosci od silnika dodajemy badz odejmujemy wartosc
		OCR1B = 0;
		return position = 0;
	}

	return position = (position)/ilosc_czujnikow;
}

float reg_PD_pozycja(wyjscie_obiektu w_zad, wyjscie_obiektu wy_o){

	//zmienne pomocnicze
	float p, d, reg;
	float uchyb;					//uchyb regulacji
	static float uchyb_pop = 0; 	// uchyb w poprzednim wywolaniu

	uchyb = w_zad.position - wy_o.position;  	// aktualny uchyb

	// wyznaczenie skladnika proporcjonalnego (P)
	p = K*uchyb;

	//wyznaczenie skladnika rozniczkowego(D)
	d = K_D * (uchyb-uchyb_pop);
	uchyb_pop = uchyb;				// zapamietaj chwilowa wartosc bledu

	reg = p+d; 				//sygnal wyjsciowy regulatora

	if(reg > 30) reg = 30;
	if(reg < -30) reg = -30;

	return reg;
}

float reg_PD_translacji(wejscie_obiektu we, wyjscie_obiektu wy, wyjscie_obiektu zad){
	//zmienne pomocnicze
	float p_t = 0, i_t = 0, d_t = 0, reg_t = 0;
	float uchyb_t = 0;						//uchyb regulacji
	static float uchyb_pop_t = 0; 		// uchyb w poprzednim wywolaniu
	//static float suma_uchybu_t = 0; 	// suma minionych uchybow

	uchyb_t = Basic_speed - fabs(we.reg_speed);  // aktualny uchyb

	// wyznaczenie skladnika proporcjonalnego (P)
	p_t = K_t*uchyb_t;

	//wyznaczenie skladnika calkowego (I)
	//if(uchyb_t == 0) suma_uchybu_t = 0;
	//suma_uchybu_t = suma_uchybu_t+uchyb_t;
	//i_t= K_I_t * suma_uchybu_t;

	//wyznaczenie skladnika rozniczkowego(D)
	d_t = K_D_t * (uchyb_t-uchyb_pop_t);
	uchyb_pop_t = uchyb_t;				// zapamietaj chwilowa wartosc bledu

	reg_t = p_t+i_t+d_t; 				//sygnal wyjsciowy regulatora

	if(reg_t > 30) reg_t = 30;
	if(reg_t <= 0) reg_t = 0;

	return reg_t;
}

float reg_PD_rotacji(wejscie_obiektu we, wyjscie_obiektu wy, wyjscie_obiektu zad){

	//zmienne pomocnicze
	float p_r = 0, i_r = 0, d_r = 0, reg_r = 0;
	float uchyb_r = 0;					//uchyb regulacji
	static float uchyb_pop_r = 0; 	// uchyb w poprzednim wywolaniu
	//static float suma_uchybu_r = 0; 	// suma minionych uchybow

	uchyb_r = we.reg_speed - (wy.speed_of_right_eng - wy.speed_of_left_eng); // aktualny uchyb


	// wyznaczenie skladnika proporcjonalnego (P)
	p_r = K_r*uchyb_r;

	//wyznaczenie skladnika calkowego (I)
	//if(uchyb_r == 0) suma_uchybu_r = 0;
	//suma_uchybu_r = suma_uchybu_r+uchyb_r;
	//i_r= K_I_r * suma_uchybu_r;

	//wyznaczenie skladnika rozniczkowego(D)
	d_r = K_D_r * (uchyb_r-uchyb_pop_r);
	uchyb_pop_r = uchyb_r;				// zapamietaj chwilowa wartosc bledu

	reg_r = p_r+i_r+d_r; 				//sygnal wyjsciowy regulatora

	if(reg_r > 40) reg_r = 40;
	if(reg_r < -40) reg_r = -40;

	return reg_r;
}

uint16_t Odczyt_lewy_enkoder(void){
	uint16_t odczyt_lewy = ilosc_cwierc_obrotow_lewe;
	ilosc_cwierc_obrotow_lewe = 0;
	return odczyt_lewy;
}
uint16_t Odczyt_prawy_enkoder(void){
	uint16_t odczyt_prawy = ilosc_cwierc_obrotow_prawe;
	ilosc_cwierc_obrotow_prawe = 0;
	return odczyt_prawy;
}

#endif
