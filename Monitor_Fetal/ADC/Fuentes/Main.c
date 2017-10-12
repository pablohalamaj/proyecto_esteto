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
#include "Menu.h"
#include "Touch.h"
#include "timer32.h"
#include "WG12864A.h"
#include "Definiciones.h"
//#include "Comunicaciones.h"
//---------------------------------------------------
//---------  Variables globales  --------------------
char 	cont=0,flagmm=0,flagbll=0;
char 	Rx[30],Rx1[30],Rx2[30],btens[16],movmp[16],version[16],version_prt[16];
char 	causaerr[11],causaerror[11],moduloerr[11],icomu[5],sumaerr=0,lec=0;
extern char actualiza_fw;
extern unsigned int 	lat_co[];
int val_adc;
extern int lati;
int valpru[100];
uint16_t leo_adc(char);
// *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
void main(void)
{

	int inc=0,aux1,aux2;
	 unsigned int i  = 0;
	inc=0;
	LPC_SYSCON->PDRUNCFG        &= ~(0x1<<4);  //power the ADC (sec. 3.5.35)
	LPC_SYSCON->SYSAHBCLKCTRL   |= (1<<13);    //enable clock for ADC (sec. 3.5.14)
/*
	LPC_IOCON->R_PIO0_11        &= 0xFFFFFF78; //clear FUNC field for pin 32, set to analog input (sec. 7.4.28)
	LPC_IOCON->R_PIO0_11        |= (1<<1);     //set to ADC mode for pin 32 (sec. 7.4.28)

	LPC_IOCON->R_PIO1_0&= 0xFFFFFF78; //clear FUNC field for pin 33, set to analog input (sec. 7.4.29)
	LPC_IOCON->R_PIO1_0      |= (1<<1);     //set to ADC mode for pin 33 (sec. 7.4.29)
*/
//	LPC_SYSCON->PDRUNCFG &=   ~(0x1<<4);  //power the ADC (sec. 3.5.35)
//	LPC_SYSCON->SYSAHBCLKCTRL |= (1<<13);    //enable clock for ADC (sec.3.5.14)
	LPC_IOCON->PIO1_4 &= ~(0x97);    //clear FUNC field for pin 40, set to   analog input (sec. 7.4.36)
	LPC_IOCON->PIO1_4 |= (1<<0);     //set to ADC mode for pin 40 (sec. 7.4.36)
	LPC_ADC->CR = 0x0B20;      //select ADC channel AD5 (pin 40), set up   clock (sec 25.5.1)

	     while(1)
	     {                                     //infinite loop
/*	    	 LPC_ADC->CR |= (1<<24);                            //startconversion by   setting "Start Conversion Now" bit (sec. 25.5.1)
	    	 while((LPC_ADC->DR[5] <  0x7FFFFFFF));                  //wait for"done" bit to   be   set (sec. 25.5.4)
	    	 valpru[inc]= ((LPC_ADC->DR[5] & 0xFFC0) >> 8);
*/
//	    	 aux1=leo_adc(0);
//	    	 aux2=leo_adc(1);
	    	 valpru[inc]=leo_adc(5);
	    	 inc++;
	    	 if(inc>=100)
	    		 inc=50;
	    	 for(i=0; i  <  0xFFFFF; ++i);                            //simple delay to   make scope shots easier to   view
	    }
}

uint16_t leo_adc(char Channel)
{

uint16_t Data = 0;

LPC_ADC->CR |= (1<<24);                         //start conversion by setting "Start Conversion Now" bit (sec. 25.5.1)
while((LPC_ADC->DR[Channel] < 0x7FFFFFFF));//wait for "done" bit to be set (sec. 25.5.4)
Data = (LPC_ADC->DR[Channel] & 0xFFC0) >> 4;//8;
return (Data);
}



