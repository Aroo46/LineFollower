/*
 * adc_sensors.h
 *
 *  Created on: 21 sty 2018
 *      Author: Arkadiusz Kwaœniak
 */

#ifndef ADC_SENSORS_ADC_SENSORS_H_
#define ADC_SENSORS_ADC_SENSORS_H_

#include <avr/io.h>

/*****************Macros***************************************/
#define LICZBA_SENSOROW 	8

/*****************Declarations of the VARIABLES****************/

uint16_t sensors[LICZBA_SENSOROW];							/*Array with the result of a measurement from the sensors*/

/*****************Declarations of the FUNCTIONS****************/

void ADC_Init(void);										/*Function which initiate the ADC converter              */

uint16_t ADC_Measure(uint8_t kanal);                        /*Function which measure the voltage on one channel      */

void ADC_GetSensorState(void);								/*Function which measure the voltage on all channels     */
#endif /* ADC_SENSORS_ADC_SENSORS_H_ */
