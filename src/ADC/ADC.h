/*
 * ADC.h
 *
 *  Created on: 14.10.2019
 *      Author: Arek
 */

#ifndef ADC_ADC_H_
#define ADC_ADC_H_

/**************************INCLUDES*************************/

#include "stm32f303xc.h"
/**********************************************************/

/**************************MACRO DEFINITIONS****************/
#define ADVREG_WAIT_TIME 	100000
/***********************************************************/

/**************************VARIABLES************************/

/***********************************************************/

/**************************FUNCTIONS************************/
void ADC1_Calib (void); // Kalibracja ADC1
void ADC3_Calib (void); //Kalibracja ADC3
void ADC1_Config (void); // Konfiguracja ADC1
void ADC3_Config (void); // Konfiguracja ADC3
/***********************************************************/
#endif /* ADC_ADC_H_ */
