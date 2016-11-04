/*
===============================================================================
 Name        : dac.c
 Author      : ss
 Version     : v1.0
 date		 : 01/07/2016
 Description : conversión digital analógica
===============================================================================
*/

#include "lpc17xx_dac.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_nvic.h"

/* P0.26 -> AOUT */
void dacInit(void)
{
	PINSEL_CFG_Type ad0;

	ad0.Funcnum = PINSEL_FUNC_2;
	ad0.OpenDrain = PINSEL_PINMODE_NORMAL;
	ad0.Pinmode = PINSEL_PINMODE_PULLUP;
	ad0.Pinnum = PINSEL_PIN_26;
	ad0.Portnum = PINSEL_PORT_0;

	PINSEL_ConfigPin(&ad0);

	DAC_Init(LPC_DAC);
}

void dacWrite(uint32_t v)
{
	DAC_UpdateValue(LPC_DAC, v);
}
