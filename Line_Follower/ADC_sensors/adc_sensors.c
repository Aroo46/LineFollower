/*
 * adc_sensors.c
 *
 *  Created on: 21 sty 2018
 *      Author: Arkadiusz Kwaœniak
 */
#include "adc_sensors.h"
#include <avr/io.h>

/*****************Definitions of the VARIABLES****************/
uint16_t sensors[LICZBA_SENSOROW];


/*****************Definitions of the FUNCTIONS****************/
/*****Initialisation of the ADC*************/
void ADC_Init(void){

	//Inicjalizacja ADC
	ADMUX |= (1<<REFS0); 							//Wybranie wewnêtrznego napiêcia odniesiena = 5V
	ADMUX |= (1<<ADLAR);							//Wyrównanie wyniku do lewej strony - mo¿liwoœc odczytu na 8 bitach
	ADCSRA |= (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1);      //W³¹czenie przetwornika ADC oraz preskalera na 64

}

/*****Function to measure the voltage on one channel by the ADC***/
uint16_t ADC_Measure(uint8_t kanal){

	ADMUX = (ADMUX & 0xF8)|(kanal & 0x07);          //Wybór kana³u do pomiaru z jednoczesnym zamaskowaniem pozosta³ej czêœci rejestru ADMUX

	ADCSRA |= (1<<ADSC);                            //Start konwersji

	while(ADCSRA & (1<<ADSC));  					//Póki flaga ADSC jest ustawiona trwa konwersja

	return ADCH;									//Odczytujemy rejestr ADCH - 8 bitowa rozdzielczoœc
}

/*****Function to measure the voltage on all 8 channels*********/
void ADC_GetSensorState(void){

	uint8_t i;                                      //Zmienna do liczby iteracji pêtli
	for(i = 0; i < LICZBA_SENSOROW; i++){
	sensors[i] = ADC_Measure(i);					//Zebranie pomiarów z sensorów
	}
}
