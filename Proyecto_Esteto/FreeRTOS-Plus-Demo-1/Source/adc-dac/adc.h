/*
===============================================================================
 Name        : adc.h
 Author      : ss
 Version     : v1.0
 date		 : 01/07/2016
 Description : Archivos cabecera de adc
===============================================================================
*/

#ifndef ADC_H_
#define ADC_H_

#include "lpc17xx_adc.h"

void adcInit(void);
void ADC_IRQHandler(void); 											// Prototipo de la ISR ADC


#endif /* ADC_H_ */
