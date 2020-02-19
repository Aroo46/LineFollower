/*
 * ADC.c
 *
 *  Created on: 14.10.2019
 *      Author: Arek
 */

#include "ADC.h"

void ADC1_Calib (void){
	ADC1 -> CR = 0x00000000;// wyzerowanie ADVREG - pocz¹tek konfiguracji regulatora ADC
	ADC1 -> CR |= ADC_CR_ADVREGEN_0; // w³¹czenie regulatora ADC
	for(int wa = 0; wa < ADVREG_WAIT_TIME; wa++ ){
		//Czekamy na koniec procesu wlaczenia konwertera
	}
	//ADCALDIF - juz ustawione na 0 - single ended
	ADC1 -> CR |= ADC_CR_ADCAL; // Start Kalibracji ADC
	while(ADC1->CR & ADC_CR_ADCAL);//czekamy na koniec kalibracji
	for(int wa = 0; wa < ADVREG_WAIT_TIME; wa++ ){
		//Czekamy na koniec procesu wlaczenia konwertera
	}
}

void ADC3_Calib (void){
	ADC3 -> CR = 0x00000000;// wyzerowanie ADVREG - pocz¹tek konfiguracji regulatora ADC
	ADC3 -> CR |= ADC_CR_ADVREGEN_0; // w³¹czenie regulatora ADC
	for(int wa = 0; wa < ADVREG_WAIT_TIME; wa++ ){
		//Czekamy na koniec procesu wlaczenia konwertera
	}
	//ADCALDIF - juz ustawione na 0 - single ended
	ADC3 -> CR |= ADC_CR_ADCAL; // Start Kalibracji ADC
	while(ADC3->CR & ADC_CR_ADCAL);//czekamy na koniec kalibracji
	for(int wa = 0; wa < ADVREG_WAIT_TIME; wa++ ){
		//Czekamy na koniec procesu wlaczenia konwertera
	}
}

void ADC3_Config (void){
	ADC3 -> CFGR |= ADC_CFGR_CONT | ADC_CFGR_RES_1 | ADC_CFGR_DMAEN | ADC_CFGR_DMACFG; //Ustawienie continous mode i rozdzielczosc na 8 bitow
	ADC3 -> SMPR2 |= ADC_SMPR2_SMP10_2 | ADC_SMPR2_SMP11_2 | ADC_SMPR2_SMP12_2 | ADC_SMPR2_SMP13_2; // Ustawienie sample time dla kanalu 10, 11, 12, 13
	ADC3 -> SQR1 |= ADC_SQR1_SQ1_0 | ADC_SQR1_SQ1_1 | ADC_SQR1_SQ1_2 | ADC_SQR1_SQ2_3 |ADC_SQR1_SQ3_0 |ADC_SQR1_SQ3_3 | ADC_SQR1_SQ4_1 | ADC_SQR1_SQ4_3 | ADC_SQR1_L_0 | ADC_SQR1_L_1; //Ustawienie ktory KANAL (WPISUJEMY NUMER KANALU NIE PINA) bedzie brany do konwersji - u nas 7, 8, 9, 10 oraz ile konwersji zostanie wykonanych - u nas 4
	ADC3 -> IER |= ADC_IER_EOC; //Wlaczenie przerwania od skonczenia konwersji regularnej grupy

	ADC3 -> CR = ADC_CR_ADVREGEN_0 | ADC_CR_ADEN;
	while(!(ADC3->ISR & ADC_ISR_ADRD));
}

void ADC1_Config (void){
	ADC1 -> CFGR |= ADC_CFGR_CONT | ADC_CFGR_RES_1 | ADC_CFGR_DMAEN | ADC_CFGR_DMACFG; //Ustawienie continous mode i rozdzielczosc na 8 bitow
	ADC1 -> SMPR1 |= ADC_SMPR1_SMP6_2 | ADC_SMPR1_SMP7_2 | ADC_SMPR1_SMP8_2 | ADC_SMPR1_SMP9_2;  //Ustawienie sample time register dla kanalu 6, 7, 8, 9
	ADC1 -> SQR1 |= ADC_SQR1_SQ1_1 | ADC_SQR1_SQ1_2 | ADC_SQR1_SQ2_0 |ADC_SQR1_SQ2_1 |ADC_SQR1_SQ2_2 | ADC_SQR1_SQ3_3 | ADC_SQR1_SQ4_3 | ADC_SQR1_SQ4_0 | ADC_SQR1_L_0 | ADC_SQR1_L_1; //Ustawienie ktory kanal bedzie brany do konwersji - u nas 10, 11, 12, 13 oraz ile konwersji zostanie wykonanych - u nas 4
	ADC1 -> IER |= ADC_IER_EOC; //Wlaczenie przerwania od skonczenia konwersji regularnej grupy

	ADC1 -> CR = ADC_CR_ADVREGEN_0 | ADC_CR_ADEN;
	while(!(ADC1->ISR & ADC_ISR_ADRD));
}
