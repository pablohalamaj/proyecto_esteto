/***************************************************************************
 *  Proyecto Final-UTN.BA
 *  Proyecto: Monitor Fetal
 *  Versión: v1.0
 *  Fecha: 30-08-2017
 *  Autor: Sebastian Sisevich
****************************************************************************/
/***************************************************************************
 *	Comentarios:
 *
 *
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
#include "Touch.h"
#include "timer32.h"
#include "WG12864A.h"
#include "Definiciones.h"
//#include "Comunicaciones.h"
//---------------------------------------------------
//---------  Variables globales  --------------------
char 	cont=0,flagmm=0,flagbll=0,flag_1seg;
char 	Rx[30],Rx1[30],Rx2[30],btens[16],movmp[16],version[16],version_prt[16];
char 	causaerr[11],causaerror[11],moduloerr[11],icomu[5],sumaerr=0,lec=0;
extern char actualiza_fw,pos_y;
extern unsigned int 	lat_co[];
int val_adc;
extern int lati;
//int valpru[100];
uint16_t leo_adc(char);

extern int inc;

// *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
void Func_Monitoreo(void);
void inicializaciones (void);

void main(void)
{
//-------------------------------------------------------------------
	pos_y=4;
	inicializaciones();
	while(1)
	{                                     //infinite loop
		Func_Monitoreo();
	}
}

void inicializaciones (void)
{
//Inicializaciones
	SSP_IOConfig(SPI_0);		// Configuracion de SPI_0.
	SSP_Init(SPI_0);			// Inicializacion de SPI_0.
	SSP_IOConfig(SPI_1);		// Configuracion de SPI_1.
	SSP_Init(SPI_1);			// Inicializacion de SPI_1.
	WG12864A_Init();			// Inicializacion del DISPLAY.
	init_timer32 (0, 1,0x2DC8A01,0x00D2F8A);								// Timmer 0, para Delays.
	init_timer32 (1, (TIMMER_BASE_1S * TIMMER_SLEEP),0x2DC8A01,0x2DC8A01);	// Timmer 1, para Sleep Display.
//	enable_timer32(1);			// Disparo el Timmer que controla el Sleep del Display.
// 	Inicialización ADC 5
	LPC_SYSCON->PDRUNCFG        &= ~(0x1<<4);  //power the ADC (sec. 3.5.35)
	LPC_SYSCON->SYSAHBCLKCTRL   |= (1<<13);    //enable clock for ADC (sec. 3.5.14)
	LPC_IOCON->PIO1_4 &= ~(0x97);    //clear FUNC field for pin 40, set to   analog input (sec. 7.4.36)
	LPC_IOCON->PIO1_4 |= (1<<0);     //set to ADC mode for pin 40 (sec. 7.4.36)
	LPC_ADC->CR = 0x0B20;      //select ADC channel AD5 (pin 40), set up   clock (sec 25.5.1)
	GLCD_Output_High(BACKLIGHT);
}
