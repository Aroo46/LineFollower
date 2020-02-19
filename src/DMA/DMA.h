/*
 * DMA.h
 *
 *  Created on: 14.10.2019
 *      Author: Arek
 */

#ifndef DMA_DMA_H_
#define DMA_DMA_H_

#include "stm32f303xc.h"

/********************************FUNCTIONS*********************************/
void DMA1_Channel1_Config(void); //Konfiguracja DMA dla ADC12 - kanal 1
void DMA2_Channel5_Config(void); //Konfiguracja DMA dla ADC34 - kanal 5
/**************************************************************************/
#endif /* DMA_DMA_H_ */
