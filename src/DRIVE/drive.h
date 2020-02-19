/*
 * drive.h
 * File stores Functions and necessary settings responsible for steering a Robot.
 *
 *  Created on: 29.10.2019
 *      Author: Arek
 */

#ifndef DRIVE_DRIVE_H_
#define DRIVE_DRIVE_H_

#include "stm32f303xc.h"		//Ten Include idzie pierwszy! zawsze
#include <core_cm4.h>
#include <core_cmFunc.h>
#include "../UART/uart.h"

/****************************MAKROS*******************************/
//Makro definicje dla regulatora PID
#define TRESHOLD_FOR_SENSORS		150
#define MAX_LEFT_VALUE				255
#define MAX_RIGHT_VALUE				255
#define MAX_SPEED_VALUE				30
#define NUMBER_OF_LEFT_SENSORS		4
#define NUMBER_OF_RIGHT_SENSORS		4
// 0 - BRAK REGULATORA, 1 - REGULATOR PD NA POZYCJE, 2 - REGULATOR + ENKODERY
#define REG_ON						2
/*****************************************************************/
/***************************VARIABLES*****************************/
#if REG_ON == 1
typedef struct{
	float reg_speed;  	//predkosc obliczona z regulatora
} wejscie_obiektu;

typedef struct{
	float position;		//pozycja LineFollowera
} wyjscie_obiektu;
#endif
#if REG_ON == 2
typedef struct{
	float reg_speed;  				//predkosc obliczona z regulatora polozenia - listwa czujnikow
	float reg_speed_translation;	//predkosc obliczona z regulatora translacji
	float reg_speed_rotation;		//predkosc obliczona z regulatora rotacji
	uint8_t speed_translation;		//predkosc translacji - obliczona z profilera
	int16_t speed_rotation;			//predkosc rotacji - obliczona z profilera
} wejscie_obiektu;

typedef struct{
	float position;					//pozycja LineFollowera
	uint16_t speed_of_left_eng;		//zmierzona predkosc lewego kola
	uint16_t speed_of_right_eng;	//zmierzona predkosc prawego kola
} wyjscie_obiektu;

volatile uint16_t ilosc_cwierc_obrotow_lewe;
volatile uint16_t ilosc_cwierc_obrotow_prawe;
#endif

#if (REG_ON == 2 || REG_ON == 1)
extern float K;		// Wspolczynnik K
extern float K_I;			// Wspolczynnik Ki
extern float K_D;			// Wspolczynnik Kd
#endif
#if REG_ON == 2
extern float K_t;		// Wspolczynnik K_t - dla regulatora translacji
extern float K_I_t;		// Wpsolczynnik K_I_t - dla regulatora translacji
extern float K_D_t;		// Wpsolczynnik K_D_t - dla regulatora translacji
extern float K_r;		// Wpsolczynnik K_r - dla regulatora rotacji
extern float K_I_r;		// Wpsolczynnik K_I_r - dla regulatora rotacji
extern float K_D_r;		// Wpsolczynnik K_D_r - dla regulatora rotacji
#endif

#if REG_ON == 0
uint8_t Lewy_PWM_w_prawo_soft;
uint8_t Prawy_PWM_w_prawo_soft;
uint8_t Lewy_PWM_w_prawo;
uint8_t Prawy_PWM_w_prawo;
uint8_t Lewy_PWM_w_prawo_hard;
uint8_t Prawy_PWM_w_prawo_hard;
uint8_t Lewy_PWM_w_lewo;
uint8_t Prawy_PWM_w_lewo;
uint8_t Lewy_PWM_w_lewo_soft;
uint8_t Prawy_PWM_w_lewo_soft;
uint8_t Lewy_PWM_w_lewo_hard;
uint8_t Prawy_PWM_w_lewo_hard;
#endif

uint8_t sensors_left[4];
uint8_t sensors_right[4];
/*****************************************************************/

/****************************FUNCTIONS****************************/
#if REG_ON == 0
uint8_t Check_sensor_state(void);
void Drive(void);
void Go_soft_Left(void);
void Go_Left(void);
void Go_harder_Left(void);
void Go_Right(void);
void Go_soft_Right(void);
void Go_harder_Right(void);
#endif

#if REG_ON == 1
void Show_PID_test(wejscie_obiektu we, wyjscie_obiektu wy, wyjscie_obiektu zad);
float PID_obiekt (wejscie_obiektu wejscie);
float reg_PID (wyjscie_obiektu w_zad, wyjscie_obiektu wy_o);
#endif


#if REG_ON == 2
float PD_obiekt(wejscie_obiektu we);
float reg_PD_pozycja(wyjscie_obiektu w_zad, wyjscie_obiektu wy_o);
float reg_PD_translacji(wejscie_obiektu we, wyjscie_obiektu wy, wyjscie_obiektu zad);
float reg_PD_rotacji(wejscie_obiektu we, wyjscie_obiektu wy, wyjscie_obiektu zad);
uint16_t Odczyt_lewy_enkoder(void);
uint16_t Odczyt_prawy_enkoder(void);
uint8_t Profiler_translacji(wejscie_obiektu we);
//int16_t Profiler_rotacji(wejscie_obiektu we, uint16_t enko_lewy_silnik, uint16_t enko_prawy_silnik);

#endif
void Show_sensors(void);
/*****************************************************************/


#endif /* DRIVE_DRIVE_H_ */
