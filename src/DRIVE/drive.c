/*
 * drive.c
 * File stores Functions and necessary settings responsible for steering a Robot.
 *
 *  Created on: 29.10.2019
 *      Author: Arek
 */

#include "drive.h"
#include "../UART/uart.h"
#include <math.h>
#include "../CONFIG/config.h"

#if (REG_ON == 2 || REG_ON == 1)
float K = 4.1;		// Wspolczynnik K
float K_I;			// Wspolczynnik Ki
float K_D = 5.6;	// Wspolczynnik Kd
#endif
#if REG_ON == 2
float K_t = 1.2;		// Wspolczynnik K_t - dla regulatora translacji
float K_I_t;		// Wpsolczynnik K_I_t - dla regulatora translacji
float K_D_t;		// Wpsolczynnik K_D_t - dla regulatora translacji
float K_r = 3.6;		// Wpsolczynnik K_r - dla regulatora rotacji
float K_I_r;		// Wpsolczynnik K_I_r - dla regulatora rotacji
float K_D_r;		// Wpsolczynnik K_D_r - dla regulatora rotacji
#endif

#if REG_ON == 0
uint8_t Lewy_PWM_w_prawo_soft = 13;
uint8_t Prawy_PWM_w_prawo_soft = 13;
uint8_t Lewy_PWM_w_prawo = 18;
uint8_t Prawy_PWM_w_prawo = 18;
uint8_t Lewy_PWM_w_prawo_hard = 20;
uint8_t Prawy_PWM_w_prawo_hard = 20;
uint8_t Lewy_PWM_w_lewo = 18;
uint8_t Prawy_PWM_w_lewo = 18;
uint8_t Lewy_PWM_w_lewo_soft = 13;
uint8_t Prawy_PWM_w_lewo_soft = 13;
uint8_t Lewy_PWM_w_lewo_hard = 20;
uint8_t Prawy_PWM_w_lewo_hard = 20;
#endif

int8_t tab_of_weight_left [NUMBER_OF_LEFT_SENSORS] = {-1, -3, -8, -20};
int8_t tab_of_weight_right [NUMBER_OF_RIGHT_SENSORS] = {20, 1, 3, 8};
uint8_t Basic_speed = 20;

#if REG_ON == 1
static void Set_Speed(wejscie_obiektu we);
#endif


#if REG_ON == 0
uint8_t Check_sensor_state(void){

	uint8_t num_of_sensor = 10;

	for(uint8_t i = 0; i<4;i++){
		if(sensors_left[i]>TRESHOLD_FOR_SENSORS){
			num_of_sensor = i;
			break;
		}
		if(sensors_right[i]>TRESHOLD_FOR_SENSORS){
			num_of_sensor = i+4;
			break;
		}
	}

	return num_of_sensor;
}

void Drive(void){
	uint8_t sens = Check_sensor_state();

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
	case 6:
		Go_Right();
		break;
	case 5:
		Go_soft_Right();
		break;
	case 4:
	case 7:
		Go_harder_Right();
		break;
	default:
		TIM2->CCR3 = 00; //Wyzerowanie PWM'a - Silniki stop
		TIM2->CCR2 = 00;
		break;
	}

}

void Go_soft_Right(void){
	TIM2->CCR3 = Lewy_PWM_w_prawo_soft;
	TIM2->CCR2 = Prawy_PWM_w_prawo_soft;
}
void Go_Right(void){
	TIM2->CCR3 = Lewy_PWM_w_prawo;
	TIM2->CCR2 = Prawy_PWM_w_prawo;
}
void Go_harder_Right(void){
	TIM2->CCR3 = Lewy_PWM_w_prawo_hard;
	TIM2->CCR2 = Prawy_PWM_w_prawo_hard;
}
void Go_Left(void){
	TIM2->CCR3 = Lewy_PWM_w_lewo;
	TIM2->CCR2 = Prawy_PWM_w_lewo;
}
void Go_soft_Left(void){
	TIM2->CCR3 = Lewy_PWM_w_lewo_soft;
	TIM2->CCR2 = Prawy_PWM_w_lewo_soft;
}
void Go_harder_Left(void){
	TIM2->CCR3 = Lewy_PWM_w_lewo_hard;
	TIM2->CCR2 = Prawy_PWM_w_lewo_hard;
}

#endif

#if REG_ON == 1
void Show_PID_test(wejscie_obiektu we, wyjscie_obiektu wy, wyjscie_obiektu zad){
	tr_locate(4,5);
	USART_puts("left engine PWM ");
	USART_putlong(we.reg_speed, 10);
	USART_puts(" ");
	tr_locate(12,5);
	USART_puts("Pozycja ");
	USART_putlong(wy.position,10);
	USART_puts(" ");
}

float PID_obiekt (wejscie_obiektu we){
	float position = 0, position_left = 0, position_right = 0;
	uint8_t ilosc_czujnikow = 0;

	// Ustawiamy aktualna - zadana predkosc
	Set_Speed(we);

	//DEBUG LED PE8 - COUNT THE SENSORS VALUE - TURN ON LED
	//LED_PE8_ON;

	// Przeliczamy dane z czujników
	for(int8_t sens = NUMBER_OF_LEFT_SENSORS - 1; sens >= 0; sens--){
		if(sensors_left[sens]>TRESHOLD_FOR_SENSORS){
			position_left += tab_of_weight_left[sens];
			ilosc_czujnikow++;
		}
	}
	for(uint8_t sens = 0; sens < NUMBER_OF_RIGHT_SENSORS; sens++){
		if(sensors_right[sens]>TRESHOLD_FOR_SENSORS){
			position_right += tab_of_weight_right[sens];
			ilosc_czujnikow++;
		}
	}
	if(ilosc_czujnikow == 0){
		TIM2->CCR3 = 0; 		//W zaleznosci od silnika dodajemy badz odejmujemy wartosc
		TIM2->CCR2 = 0;
		return position = 0;
	}
	// DEBUG LED PE8 - TURN OFF LED
	//LED_PE8_OFF;
	return position = (position_left + position_right)/ilosc_czujnikow;
}

float reg_PID (wyjscie_obiektu w_zad, wyjscie_obiektu wy_o){

	//DEBUG LED PE10 - START PD CONTROLER - TURN ON LED
	//LED_PE10_ON;

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
	uchyb_pop = uchyb;			// zapamietaj chwilowa wartosc bledu

	reg = p+i+d; 				//sygnal wyjsciowy regulatora

	if(reg > 22) reg = 22;
	if(reg < -22) reg = -22;

	//DEBUG LED PE10 - TURN OFF LED
	//LED_PE10_OFF;
	return reg;
}

static void Set_Speed(wejscie_obiektu we){
	//ustawienie predkosci silnikow - jezeli predkosc spada ponizej 0 - zerujemy PWM
	// Jezeli predkosc wykroczy poza bezpieczna wartosc obcinamy ja - saturacja
	// 1. Sprawdzenie czy nie przekraczamy dozwolonej predkosci

	//DEBUG LED PE9 - START OF THE SETTING SPEED FUNCTION - TURN ON LED
	//LED_PE9_ON;

	if(MAX_SPEED_VALUE < (Basic_speed - we.reg_speed)){
		TIM2->CCR3 = MAX_SPEED_VALUE;
	}else if (MAX_SPEED_VALUE < (Basic_speed + we.reg_speed)) {
		TIM2->CCR2 = MAX_SPEED_VALUE;
	}

	// 2. Sprawdzenie czy ustawiana predkosc nie jest ujemna - wyhamujemy do 0
	// Jezeli nie przekraczamy zakresow - ustawiamy zadana predkosc
	if(0 > (Basic_speed - we.reg_speed)){
		TIM2->CCR3 = 0;
	}
	else{
		TIM2->CCR3 = Basic_speed - we.reg_speed; 	//W zaleznosci od silnika dodajemy badz odejmujemy wartosc
	}

	if(0 > (Basic_speed + we.reg_speed)){
		TIM2->CCR2 = 0;
	}
	else{
		TIM2->CCR2 = Basic_speed + we.reg_speed;
	}

	//DEBUG LED PE9 - FINISH - TURN OFF LED
	//LED_PE9_OFF;
}

#endif

void Show_sensors(void){
	tr_locate(4,5);
	USART_puts("Pomiar z ADC sensor_left[0] ");
	USART_putlong(sensors_left[0],10);
	USART_puts(" ");
	tr_locate(6,5);
	USART_puts("sensor_left[1] ");
	USART_putlong(sensors_left[1],10);
	USART_puts(" ");
	tr_locate(8,5);
	USART_puts("sensor_left[2] ");
	USART_putlong(sensors_left[2],10);
	USART_puts(" ");
	tr_locate(10,5);
	USART_puts("sensor_left[3] ");
	USART_putlong(sensors_left[3],10);
	USART_puts(" ");
	tr_locate(12,5);
	USART_puts("sensor_right[0] ");
	USART_putlong(sensors_right[0],10);
	USART_puts(" ");
	tr_locate(14,5);
	USART_puts("sensor_right[1] ");
	USART_putlong(sensors_right[1],10);
	USART_puts(" ");
	tr_locate(16,5);
	USART_puts("sensor_right[2] ");
	USART_putlong(sensors_right[2],10);
	USART_puts(" ");
	tr_locate(18,5);
	USART_puts("sensor_right[3] ");
	USART_putlong(sensors_right[3],10);
	USART_puts(" ");
}


#if REG_ON == 2
float PD_obiekt(wejscie_obiektu we){
	float position = 0, position_left = 0, position_right = 0;
	uint8_t ilosc_czujnikow = 0;

	//ustawienie predkosci silnikow
	//CCR3 - lewy silnik
	//CCR2 - prawy silnik
	if((we.reg_speed_translation - we.reg_speed_rotation) < 0) TIM2->CCR3 = 0;
	else TIM2->CCR3 = we.reg_speed_translation - we.reg_speed_rotation;		//W zaleznosci od silnika dodajemy badz odejmujemy wartosc
	if((we.speed_translation + we.reg_speed_rotation) < 0) TIM2->CCR2 = 0;
	else TIM2->CCR2 = we.reg_speed_translation + we.reg_speed_rotation;

	// Przeliczamy dane z czujników
	for(int8_t sens = NUMBER_OF_LEFT_SENSORS - 1; sens >= 0; sens--){
		if(sensors_left[sens]>TRESHOLD_FOR_SENSORS){
			position_left += tab_of_weight_left[sens];
			ilosc_czujnikow++;
		}
	}
	for(uint8_t sens = 0; sens < NUMBER_OF_RIGHT_SENSORS; sens++){
		if(sensors_right[sens]>TRESHOLD_FOR_SENSORS){
			position_right += tab_of_weight_right[sens];
			ilosc_czujnikow++;
		}
	}
	if(ilosc_czujnikow == 0){
		TIM2->CCR3 = 0; 		//W zaleznosci od silnika dodajemy badz odejmujemy wartosc
		TIM2->CCR2 = 0;
		return position = 0;
	}

	return position = (position_left + position_right)/ilosc_czujnikow;
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
