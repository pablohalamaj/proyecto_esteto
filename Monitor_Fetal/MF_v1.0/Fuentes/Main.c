/*
===============================================================================
 Name        : main.c
 Author      : Sebastian Sisevich
 Version     : v2.5
 Copyright   : Copyright (C) 
 Description : main definition
===============================================================================
*/

#ifdef __USE_CMSIS
#include "LPC11xx.h"
#endif

#include <cr_section_macros.h>
#include <NXP/crp.h>

// Variable to store CRP value in. Will be placed automatically
// by the linker when "Enable Code Read Protect" selected.
// See crp.h header for more information
__CRP const unsigned int CRP_WORD = CRP_NO_CRP ;

#include "ssp.h"
#include "gpio.h"
#include "Menu.h"
#include "Touch.h"
#include "timer32.h"
#include "WG12864A.h"
#include "Definiciones.h"
//#include "Comunicaciones.h"
//---------------------------------------------------
//---------  Variables globales  --------------------
char 	cont=0,flagmm=0,flagbll=0,flag_1seg,flag_25ms;
char 	Rx[30];Rx1[30],Rx2[30],btens[16],toffset[16],movmp[16],version[16],version_prt[16];
char 	causaerr[11],causaerror[11],moduloerr[11],icomu[5],b_tog[16],sumaerr=0,lec=0;
extern char actualiza_fw;
// *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
int main(void)
{

//*-*-*-*-*-*-*-*-*-*-* Configuracion de la SPI. *-*-*-*-*-*-*-*-*-*-*-*

	SSP_IOConfig(SPI_0);		// Configuracion de SPI_0.
	SSP_Init(SPI_0);			// Inicializacion de SPI_0.
	SSP_IOConfig(SPI_1);		// Configuracion de SPI_1.
	SSP_Init(SPI_1);			// Inicializacion de SPI_1.

//*-*-*-*-*-*-*-*-*-*-* Configuracion del DISPLAY. *-*-*-*-*-*-*-*-*-*-*

	WG12864A_Init();			// Inicializacion del DISPLAY.

//*-*-*-*-*-*-*-*-*-*-*-* Configuracion del ADC. *-*-*-*-*-*-*-*-*-*-*-*

	ADCInit(ADC_CLK);			// Inicializacion de ADC.

//*-*-*-*-*-*-*-*-*-*-*-* Configuracion de TOUCH. *-*-*-*-*-*-*-*-*-*-*-*

	TOUCH_Init();				// Inicializacion del TOUCH.
	TOUCH_Standby();

//*-*-*-*-*-*-*-*-*-*-* Configuracion del TIMMER. *-*-*-*-*-*-*-*-*-*-*-*

	init_timer32 (0, 1);								// Timmer 0, para Delays.
	init_timer32 (1, (TIMMER_BASE_1S * TIMMER_SLEEP));	// Timmer 1, para Sleep Display.

//*-*-*-*-*-*-*-*-*-*-* MAIN. *-*-*-*-*-*-*-*-*-*-*-*

	Menu_Logo();				// Presento el Logo de la Empresa.
	enable_timer32(1);			// Disparo el Timmer que controla el Sleep del Display.
	GPIOIntClear(PORT1, 2);		// Limpio la Interrupcion y la habilito.
	GPIOIntEnable(PORT1, 2);
	actualiza_fw=0;
	while(1)
	{
		//Corro el Menu.
		Menu_Inicial();
	}
}
