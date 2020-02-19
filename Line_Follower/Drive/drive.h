/*
 * drive.h
 *
 *  Created on: 12 lut 2018
 *      Author: Arkadiusz Kwaœniak
 */

#ifndef DRIVE_DRIVE_H_
#define DRIVE_DRIVE_H_

#define TRESHOLD_FOR_SENSORS	100
#define MAX_SPEED_VALUE			30

/*** Zmienna decydujaca o wyborze algorytmu **/
// 0 - BRAK REGULATORA, 1 - REGULATOR PD NA POZYCJE, 2 - REGULATOR + ENKODERY
#define REG_ON 					1

/*****Variables******************/
#if REG_ON == 1
typedef struct{
	float reg_speed;  	//predkosc obliczona z regulatora
} wejscie_obiektu;

typedef struct{
	float position;		//pozycja LineFollowera
} wyjscie_obiektu;
#endif

#if (REG_ON == 2 || REG_ON == 1)
extern float K;		// Wspolczynnik K
extern float K_I;	// Wspolczynnik Ki
extern float K_D;	// Wspolczynnik Kd
#endif
#if REG_ON == 2
extern float K_t;		// Wspolczynnik K_t - dla regulatora translacji
extern float K_I_t;		// Wpsolczynnik K_I_t - dla regulatora translacji
extern float K_D_t;		// Wpsolczynnik K_D_t - dla regulatora translacji
extern float K_r;		// Wpsolczynnik K_r - dla regulatora rotacji
extern float K_I_r;		// Wpsolczynnik K_I_r - dla regulatora rotacji
extern float K_D_r;		// Wpsolczynnik K_D_r - dla regulatora rotacji
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

#if REG_ON == 0
/******Main Drive Function********/
void Drive_Main(void);

/**********Sensor object - for PID controller******/
uint8_t Sensors_regulator_object ();

/**********Get reference value for sensors controller******/
//uint8_t Get_reference_value_sensors (void);

/**********Get measurement value for sensors controller****/
//int16_t Get_measurement_value_sensors (void);

/**********Stop the robot - make a service functions*******/
void Brake(void);

/**********Settings for engies to go straights*************/
void Go_Straight(void);

/**********Settings for engies to go slightly left*********/
void Go_soft_Left(void);

/**********Settings for engies to go harder to left********/
void Go_harder_Left(void);

/**********Settings for engies to go completely left*******/
void Go_Left(void);

/**********Settings for engies to go slightly right********/
void Go_soft_Right(void);

/**********Settings for engies to go harder right**********/
void Go_harder_Right(void);

/**********Settings for engies to go completely right**********/
void Go_Right(void);
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


#endif /* DRIVE_DRIVE_H_ */
