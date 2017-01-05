/*
===============================================================================
 Name        : adc.c
 Author      : ss
 Version     : v1.0
 date		 : 01/07/2016
 Description : conversión analógica digital
===============================================================================
*/

#include "lpc17xx_adc.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_nvic.h"

/* P0.24 -> AD1 */
void adcInit(void)
{
	PINSEL_CFG_Type ad0;

	ad0.Funcnum = PINSEL_FUNC_1;
	ad0.OpenDrain = PINSEL_PINMODE_NORMAL;
	ad0.Pinmode = PINSEL_PINMODE_PULLUP;
	ad0.Pinnum = PINSEL_PIN_24;
	ad0.Portnum = PINSEL_PORT_0;

	PINSEL_ConfigPin(&ad0);
	ADC_Init(LPC_ADC, 1000);
	ADC_IntConfig(LPC_ADC, ADC_ADINTEN1, ENABLE);
	NVIC_EnableIRQ(ADC_IRQn);
}

