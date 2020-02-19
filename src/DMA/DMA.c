/*
 * DMA.c
 *
 *  Created on: 14.10.2019
 *      Author: Arek
 */
/****************************INCLUDES****************************/
#include "DMA.h"
#include "../DRIVE/drive.h"
/****************************************************************/


/*****************************FUNCTIONS**************************/
void DMA1_Channel1_Config(void){
	DMA1_Channel1 -> CPAR = (uint32_t)&ADC1->DR;
	DMA1_Channel1 -> CMAR =(uint32_t)sensors_left;
	DMA1_Channel1 -> CNDTR = 4;
	DMA1_Channel1 -> CCR |= DMA_CCR_MINC | DMA_CCR_EN | DMA_CCR_CIRC;
}

void DMA2_Channel5_Config(void){
	DMA2_Channel5 -> CPAR = (uint32_t)&ADC3->DR;
	DMA2_Channel5 -> CMAR = (uint32_t)sensors_right;
	DMA2_Channel5 -> CNDTR = 4;
	DMA2_Channel5 -> CCR |= DMA_CCR_MINC | DMA_CCR_EN | DMA_CCR_CIRC;
}
/****************************************************************/
