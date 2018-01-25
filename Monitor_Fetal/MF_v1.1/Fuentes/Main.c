/***************************************************************************
 *  Proyecto Final-UTN.BA
 *  Proyecto: Monitor Fetal
 *  Versión: v1.1
 *  Fecha: 30-08-2017
 *  Autor: Sebastian Sisevich
****************************************************************************/
/***************************************************************************
 *	Comentarios:
 *
 * El programa toma la señal del corazón, calcula las PPM y grafica en
 * pantalla el monitoreo.
****************************************************************************/

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
//--------------------------------------------------------------------------
//**********************  Variables globales  ******************************
char 	cont=0,flagmm=0,flagbll=0,flag_1seg,flag_25ms,cont_1min,cont_1seg,Hab_cont_500ms,cont_500ms;
char 	Rx[30];Rx1[30],Rx2[30],btens[16],toffset[16],movmp[16],version[16],version_prt[16];
char 	causaerr[11],causaerror[11],moduloerr[11],icomu[5],b_tog[16],sumaerr=0,lec=0;
extern char actualiza_fw,pos_x;

int main(void)
{
	SSP_IOConfig(SPI_0);													// Configuracion de SPI_0.
	SSP_Init(SPI_0);														// Inicializacion de SPI_0.
	SSP_IOConfig(SPI_1);													// Configuracion de SPI_1.
	SSP_Init(SPI_1);														// Inicializacion de SPI_1.
	WG12864A_Init();														// Inicializacion del DISPLAY.
	ADCInit(ADC_CLK);														// Inicializacion de ADC.
	TOUCH_Init();															// Inicializacion del TOUCH.
	TOUCH_Standby();
	init_timer32 (0, 1,1,1);													// Timmer 0, para Delays.
//	init_timer32(0,0x0001770F, 0x2DC8A01,0x0003A9A6);
//	init_timer32 (1, (TIMMER_BASE_1S)/* * TIMMER_SLEEP)*/,0x2DC8A01,0x0003A9A6);						// Timmer 1, para Sleep Display.
	init_timer32 (1, TIMMER_BASE_25MS,0x2DC8A01,0x0003A9A6);						// Timmer 1, para Sleep Display.
	Menu_Logo();															// Presento el Logo
//	enable_timer32(0);			// Disparo el Timmer que controla el Sleep del Display.
	enable_timer32(1);														// Disparo el Timmer que controla el Sleep del Display.
	GPIOInit();		 														//Inicializo GPIO
	GPIOIntClear(PORT1, 2);													// Limpio la Interrupcion y la habilito.
	GPIOIntEnable(PORT1, 2);
	actualiza_fw=0;
	pos_x=25;
	while(1)
	{
		Menu_Inicial();														//Corro el Menu.
	}
}
//--------------------------------------------------------------------------
// ss	2017
