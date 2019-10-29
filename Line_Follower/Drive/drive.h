/*
 * drive.h
 *
 *  Created on: 12 lut 2018
 *      Author: Arkadiusz Kwaœniak
 */

#ifndef DRIVE_DRIVE_H_
#define DRIVE_DRIVE_H_

#define TRESHOLD_FOR_SENSORS	100

/*****Variables******************/
uint16_t P_term_sens_tab; 				//Zmienna do sta³ej P - regulatora PD - do obs³ugi sensorów
uint16_t D_term_sens_tab; 				//Zmienna do sta³ej D - regulatora PD - do obs³ugi sensorów
pidData_t PD_Sensors; 					//Struktura do regulatora PD - regulator do obs³ugi sensorów

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


#endif /* DRIVE_DRIVE_H_ */
