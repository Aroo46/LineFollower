/*
 * settings.h
 *
 *  Created on: 6 lut 2018
 *      Author: Arkadiusz Kwaœniak
 */

#ifndef SETTINGS_SETTINGS_H_
#define SETTINGS_SETTINGS_H_

/*********** Main settings function ***********/
void Settings_Main (void);

/***** Function to change PID settings ********/
void PID_settings(void);

/***** Function to change PWM settings ********/
void PWM_settings(void);

/*** Function to draw the settings menu *******/
void Draw_menu(void);

/*** Function to draw the PID settings menu *******/
void Draw_PID_settings(void);

/*** Function to draw the PWM settings menu *******/
void Draw_PWM_settings(void);
#endif /* SETTINGS_SETTINGS_H_ */
