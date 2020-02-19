/**
 * ADC - first program to try ADC on STM micro
 */

/*************************INCLUDES******************************/
#include "stm32f303xc.h" 		//Ten Include idzie pierwszy! zawsze
#include <stddef.h>
#include <stdlib.h>

#include "ADC/ADC.h"
#include "DMA/DMA.h"
#include "CONFIG/config.h"
#include "DRIVE/drive.h"
/****************************************************************/
/************************** Functions ****************************/
void Menu (wejscie_obiektu we, wyjscie_obiektu wyj, wyjscie_obiektu w_zad);
void Send_message(void);
/************************** Variables *****************************/
volatile char znak;
/******************************************************************/

/************************** Main Program **************************/

int main (void){
	RCC_Init();  //wlaczenie taktowania dla poszczegolnych modulow - zawsze na poczatku konfiguracji
	//Ustawienie CKMODE - na HCLK/4 oraz DUAL mode na Regular group only - dla ADC12 oraz ADC34
	ADC1_2_COMMON -> CCR = ADC12_CCR_CKMODE | 0b110;
	ADC3_4_COMMON -> CCR = ADC34_CCR_CKMODE | 0b110;
	ADC1_Calib(); // Kalibracja ADC1 oraz ADC3
	ADC3_Calib();
	ADC1_Config(); //Konfiguracja ADC1 oraz ADC3
	ADC3_Config();

	ADC1->CR |= ADC_CR_ADSTART; // Wystartowanie w koncu konwersji
	ADC3->CR |= ADC_CR_ADSTART; //Wystartowanie konwersji

	DMA1_Channel1_Config(); //Odpalenie DMA1 kanal 1 - obsluga ADC1
	DMA2_Channel5_Config(); // Odpalenie DMA2 kanal5 - obsluga ADC3

	USART_Init(); //inicjalizacja USART

	GPIO_init(); //Konfiguracja odpowiednich PINów/ Portów

	PWM_config();
#if REG_ON == 2
	Timer_Counter_init();
	Encoder_mode_config();
#endif
	//Wlaczenie Bloku NVIC - przerwania od peryferiow - do rdzenia
	NVIC_ClearPendingIRQ(TIM8_UP_IRQn);
	NVIC_ClearPendingIRQ(TIM1_UP_TIM16_IRQn);
	NVIC_EnableIRQ(USART3_IRQn);
	NVIC_EnableIRQ(TIM8_UP_IRQn);
	NVIC_EnableIRQ(TIM1_UP_TIM16_IRQn);
	NVIC_EnableIRQ(TIM3_IRQn);

	//DRIVER WYLACZONY!!!!!!!!!!! <<<<<--------------- ZOBACZ
	//Ustawienie driver'a do jazdy do przodu
	DRIVER_STBY_OFF;		// Stand by - stan wysoki - driver wlaczony. Stan niski - driver uspiony
	DRIVER_AIN1_OFF;
	DRIVER_AIN2_ON;		// Cztery PINy - definiuja kierunek obrotow silnikow.
	DRIVER_BIN2_ON;
	DRIVER_BIN1_OFF;
	tr_cls();			// Funkcja czyszczaca terminal
	tr_cursor_off();	// Funkcja ukrywajaca migajacy kursor na terminalu
#if REG_ON == 1
	wejscie_obiektu we = {0};
	wyjscie_obiektu wyj = {0};
	wyjscie_obiektu w_zad = {0};
#endif

#if REG_ON == 2
	wejscie_obiektu we = {0, 20, 0, 0, 0};
	wyjscie_obiektu wyj = {0, 0, 0};
	wyjscie_obiektu w_zad = {0, 0, 0};
#endif
	while(1){
#if REG_ON == 1
		w_zad.position = 0;
		//TIM2->CCR2 = 20;
		wyj.position = PID_obiekt(we);
		//Show_PID_test(we,wyj,w_zad);
		we.reg_speed = reg_PID(w_zad,wyj);
		//Menu(we, wyj, w_zad);
#endif
		//Show_sensors();
#if REG_ON == 0
		//Drive();
		//TIM2->CCR2 = 40;
		//TIM2->CCR3 = 40;
		//Show_sensors();
#endif

#if REG_ON == 2
		Menu(we,wyj,w_zad);
		wyj.position = PD_obiekt(we);
		we.reg_speed = reg_PD_pozycja(w_zad, wyj);
		wyj.speed_of_left_eng = Odczyt_lewy_enkoder();
		wyj.speed_of_right_eng = Odczyt_prawy_enkoder();
		we.reg_speed_translation = reg_PD_translacji(we, wyj, w_zad);
		we.reg_speed_rotation = reg_PD_rotacji(we, wyj, w_zad);
		//Show_sensors();

#endif

	}

	return 0;
}

/******************* Interrupts handlers **********************/
#if REG_ON == 2
void TIM8_UP_IRQHandler (void){
	if(TIM8->SR & TIM_SR_UIF){
		TIM8->SR=~TIM_SR_UIF;
		//GPIOE->ODR ^= GPIO_ODR_9;
		ilosc_cwierc_obrotow_lewe++;
	}
}

void TIM1_UP_TIM16_IRQHandler (void){
	if(TIM1->SR & TIM_SR_UIF){
		TIM1->SR =~TIM_SR_UIF;
		ilosc_cwierc_obrotow_prawe++;
	}
}
#endif
void USART3_IRQHandler (void){
	if(USART3->ISR & USART_ISR_RXNE){
		znak = USART3->RDR;
	}
}

void TIM3_IRQHandler (void){
	if(TIM3->SR & TIM_SR_UIF){
		TIM3->SR =~TIM_SR_UIF;
	}
}

/************************************************************/

void Menu (wejscie_obiektu we, wyjscie_obiektu wyj, wyjscie_obiektu w_zad){
	tr_locate(1,2);
	USART_puts("1 - Ustawienia PWM \n");
	tr_locate(3,2);
	USART_puts("2 - Utawienia PID \n");
	tr_locate(5,2);
	USART_puts("3 - Wlacz logowanie \n");
	if(znak == '1'){
		tr_cls();
#if REG_ON == 0
		while(1){
			TIM2->CCR3 = 0;
			TIM2->CCR2 = 0;
			tr_locate(1,2);
			USART_puts("Ustawienia PWM: ");
			tr_locate(3,2);
			USART_puts("Lewy_PWM_w_prawo_soft -> u w gore, d w dol = ");
			USART_putlong(Lewy_PWM_w_prawo_soft,10);
			USART_puts(" ");
			if(znak == 'u'){
				Lewy_PWM_w_prawo_soft++;
				znak = ' ';
			}
			if(znak == 'd'){
				Lewy_PWM_w_prawo_soft--;
				znak = ' ';
			}
			tr_locate(5,2);
			USART_puts("Prawy_PWM_w_prawo_soft -> k w gore, l w dol = ");
			USART_putlong(Prawy_PWM_w_prawo_soft,10);
			USART_puts(" ");
			if(znak == 'k'){
				Prawy_PWM_w_prawo_soft++;
				znak = ' ';
			}
			if(znak == 'l'){
				Prawy_PWM_w_prawo_soft--;
				znak = ' ';
			}
			tr_locate(7,2);
			USART_puts("Lewy_PWM_w_prawo -> n w gore, m w dol = ");
			USART_putlong(Lewy_PWM_w_prawo,10);
			USART_puts(" ");
			if(znak == 'n'){
				Lewy_PWM_w_prawo++;
				znak = ' ';
			}
			if(znak == 'm'){
				Lewy_PWM_w_prawo--;
				znak = ' ';
			}
			tr_locate(9,2);
			USART_puts("Prawy_PWM_w_prawo -> v w gore, b w dol = ");
			USART_putlong(Prawy_PWM_w_prawo,10);
			USART_puts(" ");
			if(znak == 'v'){
				Prawy_PWM_w_prawo++;
				znak = ' ';
			}
			if(znak == 'b'){
				Prawy_PWM_w_prawo--;
				znak = ' ';
			}
			tr_locate(11,2);
			USART_puts("Lewy_PWM_w_prawo_hard -> z w gore, x w dol = ");
			USART_putlong(Lewy_PWM_w_prawo_hard,10);
			USART_puts(" ");
			if(znak == 'z'){
				Lewy_PWM_w_prawo_hard++;
				znak = ' ';
			}
			if(znak == 'x'){
				Lewy_PWM_w_prawo_hard--;
				znak = ' ';
			}
			tr_locate(13,2);
			USART_puts("Prawy_PWM_w_prawo_hard -> a w gore, s w dol = ");
			USART_putlong(Prawy_PWM_w_prawo_hard,10);
			USART_puts(" ");
			if(znak == 'a'){
				Prawy_PWM_w_prawo_hard++;
				znak = ' ';
			}
			if(znak == 's'){
				Prawy_PWM_w_prawo_hard--;
				znak = ' ';
			}
			tr_locate(15,2);
			USART_puts("Lewy_PWM_w_lewo_soft -> w w gore, e w dol = ");
			USART_putlong(Lewy_PWM_w_lewo_soft,10);
			USART_puts(" ");
			if(znak == 'w'){
				Lewy_PWM_w_lewo_soft++;
				znak = ' ';
			}
			if(znak == 'e'){
				Lewy_PWM_w_lewo_soft--;
				znak = ' ';
			}
			tr_locate(17,2);
			USART_puts("Prawy_PWM_w_lewo_soft -> r w gore, t w dol = ");
			USART_putlong(Prawy_PWM_w_lewo_soft,10);
			USART_puts(" ");
			if(znak == 'r'){
				Prawy_PWM_w_lewo_soft++;
				znak = ' ';
			}
			if(znak == 't'){
				Prawy_PWM_w_lewo_soft--;
				znak = ' ';
			}
			tr_locate(19,2);
			USART_puts("Lewy_PWM_w_lewo -> o w gore, p w dol = ");
			USART_putlong(Lewy_PWM_w_lewo,10);
			USART_puts(" ");
			if(znak == 'o'){
				Lewy_PWM_w_lewo++;
				znak = ' ';
			}
			if(znak == 'p'){
				Lewy_PWM_w_lewo--;
				znak = ' ';
			}
			tr_locate(21,2);
			USART_puts("Prawy_PWM_w_lewo -> f w gore, g w dol = ");
			USART_putlong(Prawy_PWM_w_lewo,10);
			USART_puts(" ");
			if(znak == 'f'){
				Prawy_PWM_w_lewo++;
				znak = ' ';
			}
			if(znak == 'g'){
				Prawy_PWM_w_lewo--;
				znak = ' ';
			}
			tr_locate(23,2);
			USART_puts("Lewy_PWM_w_lewo_hard -> h w gore, j w dol = ");
			USART_putlong(Lewy_PWM_w_lewo_hard,10);
			USART_puts(" ");
			if(znak == 'h'){
				Lewy_PWM_w_lewo_hard++;
				znak = ' ';
			}
			if(znak == 'j'){
				Lewy_PWM_w_lewo_hard--;
				znak = ' ';
			}
			tr_locate(25,2);
			USART_puts("Prawy_PWM_w_lewo_hard -> 7 w gore, 8 w dol = ");
			USART_putlong(Prawy_PWM_w_lewo_hard,10);
			USART_puts(" ");
			if(znak == '7'){
				Prawy_PWM_w_lewo_hard++;
				znak = ' ';
			}
			if(znak == '8'){
				Prawy_PWM_w_lewo_hard--;
				znak = ' ';
			}
			if(znak == 'q') break;
		}
		tr_cls();
#endif
	}
	if(znak == '2'){
		tr_cls();
		while(1){
			tr_locate(1,2);
			USART_puts("Ustawienia PID: \n");
			tr_locate(3,2);
			USART_puts("Wspolczynnik K -> u w gore, d w dol = ");
			USART_putlong(K,10);
			USART_puts(",");
			USART_putlong((100*(K-(unsigned int)K)),10);
			USART_puts(" ");
			if(znak == 'u'){
				K=K+0.1;
				znak = ' ';
			}
			if(znak == 'd'){
				K=K-0.1;
				znak = ' ';
			}
			tr_locate(5,2);
			USART_puts("\n Wspolczynnik Ki -> k w gore, l w dol = ");
			USART_putlong(K_I,10);
			USART_puts(",");
			USART_putlong((100*(K_I-(unsigned int)K_I)),10);
			USART_puts(" ");
			if(znak == 'k'){
				K_I=K_I+0.1;
				znak = ' ';
			}
			if(znak == 'l'){
				K_I=K_I-0.1;
				znak = ' ';
			}
			tr_locate(7,2);
			USART_puts("\n Wspolczynnik Kd -> c w gore, v w dol = ");
			USART_putlong(K_D,10);
			USART_puts(",");
			USART_putlong((100*(K_D-(unsigned int)K_D)),10);
			USART_puts(" ");
			if(znak == 'c'){
				K_D=K_D+0.1;
				znak = ' ';
			}
			if(znak == 'v'){
				K_D=K_D-0.1;
				znak = ' ';
			}
#if REG_ON == 2
			tr_locate(9,2);
			USART_puts("\n Wspolczynnik K_t -> f w gore, g w dol = ");
			USART_putlong(K_t,10);
			USART_puts(",");
			USART_putlong((100*(K_t-(unsigned int)K_t)),10);
			USART_puts(" ");
			if(znak == 'f'){
				K_t=K_t+0.1;
				znak = ' ';
			}
			if(znak == 'g'){
				K_t=K_t-0.1;
				znak = ' ';
			}
			tr_locate(11,2);
			USART_puts("\n Wspolczynnik K_I_t -> h w gore, j w dol = ");
			USART_putlong(K_I_t,10);
			USART_puts(",");
			USART_putlong((100*(K_I_t-(unsigned int)K_I_t)),10);
			USART_puts(" ");
			if(znak == 'h'){
				K_I_t=K_I_t+0.1;
				znak = ' ';
			}
			if(znak == 'j'){
				K_I_t=K_I_t-0.1;
				znak = ' ';
			}
			tr_locate(13,2);
			USART_puts("\n Wspolczynnik K_D_t -> t w gore, y w dol = ");
			USART_putlong(K_D_t,10);
			USART_puts(",");
			USART_putlong((100*(K_D_t-(unsigned int)K_D_t)),10);
			USART_puts(" ");
			if(znak == 't'){
				K_D_t=K_D_t+0.1;
				znak = ' ';
			}
			if(znak == 'y'){
				K_D_t=K_D_t-0.1;
				znak = ' ';
			}
			tr_locate(15,2);
			USART_puts("\n Wspolczynnik K_r -> , w gore, . w dol = ");
			USART_putlong(K_r,10);
			USART_puts(",");
			USART_putlong((100*(K_r-(unsigned int)K_r)),10);
			USART_puts(" ");
			if(znak == ','){
				K_r=K_r+0.1;
				znak = ' ';
			}
			if(znak == '.'){
				K_r=K_r-0.1;
				znak = ' ';
			}
			tr_locate(17,2);
			USART_puts("\n Wspolczynnik K_I_r -> s w gore, a w dol = ");
			USART_putlong(K_I_r,10);
			USART_puts(",");
			USART_putlong((100*(K_I_r-(unsigned int)K_I_r)),10);
			USART_puts(" ");
			if(znak == 's'){
				K_I_r=K_I_r+0.1;
				znak = ' ';
			}
			if(znak == 'a'){
				K_I_r=K_I_r-0.1;
				znak = ' ';
			}
			tr_locate(19,2);
			USART_puts("\n Wspolczynnik K_D_r -> e w gore, r w dol = ");
			USART_putlong(K_D_r,10);
			USART_puts(",");
			USART_putlong((100*(K_D_r-(unsigned int)K_D_r)),10);
			USART_puts(" ");
			if(znak == 'e'){
				K_D_r=K_D_r+0.1;
				znak = ' ';
			}
			if(znak == 'r'){
				K_D_r=K_D_r-0.1;
				znak = ' ';
			}
#endif
			if(znak == 'q') break;
		}

		tr_cls();
	}
#if (REG_ON == 2 || REG_ON == 1)
	if((znak != '1' && znak != '2')|| znak == '3'){
		tr_cls();
#if REG_ON == 2
		USART_puts("\n Pozycja we.reg_speed: ");
		USART_putlong(we.reg_speed, 10);
		USART_puts("\n we.reg_speed_translation: ");
		USART_putlong(we.reg_speed_translation,10);
		USART_puts("\n we.reg_speed_rotation: ");
		USART_putlong(we.reg_speed_rotation,10);
		USART_puts("\n we.speed_translation: ");
		USART_putlong(we.speed_translation,10);
		USART_puts("\n wyj.speed_of_left_eng: ");
		USART_putlong(wyj.speed_of_left_eng,10);
		USART_puts("\n wyj.speed_of_right_eng: ");
		USART_putlong(wyj.speed_of_right_eng,10);
		USART_puts("\n wyj.position - srednia z sensorow: ");
		USART_putlong(wyj.position,10);
		USART_puts("\n");
#endif
#if REG_ON == 1
		USART_puts("\n Srednia z listwy czujnikow: ");
		USART_putlong(wyj.position, 10);
		USART_puts("\n Wartosc z regulatora PD: ");
		USART_putlong(we.reg_speed,10);
		USART_puts("\n");
#endif
	}
#endif

}

#if REG_ON == 2
void Send_message(void){

 	tr_locate(2,2);
	USART_puts("Lewe kolo: ");
	USART_putlong(ilosc_cwierc_obrotow_lewe,10);
	USART_puts(" cwierc obr/s");
	tr_locate(5,2);
	USART_puts("Prawe kolo: ");
	USART_putlong(ilosc_cwierc_obrotow_prawe,10);
	USART_puts(" cwierc obr/s");
	ilosc_cwierc_obrotow_lewe = 0;
	ilosc_cwierc_obrotow_prawe = 0;
}
#endif
