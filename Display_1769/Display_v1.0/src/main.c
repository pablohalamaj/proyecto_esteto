/*
===============================================================================
 Name        : main.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include "lpc17xx_ssp.h"
#include "lpc17xx_gpio.h"
#include "lpc_types.h"
#include "Definiciones.h"
//#include <cr_section_macros.h>

#define ON 	1
#define OFF 0
#define spiPARAMTER_NOT_USED			( ( void * ) 0 )
struct SSP_CFG_Type * SSP_InitStruct=NULL;

void set_value (uint8_t  port, uint8_t  pin, uint8_t value);


int main(void)
{
//------------ Seteo los puertos como salida ---------------
	GPIO_SetDir(0, (1<<22), 1); 							// Led Stick
	GPIO_SetDir(2, (1<<6), 1);
	GPIO_SetDir(0, (1<<4), 1);
	GPIO_SetDir(0, (1<<5), 1);
	GPIO_SetDir(0, (1<<10), 1);
	GPIO_SetDir(0, (1<<11), 1);

    volatile static int i,j,t,a=0;
    unsigned char 	led=0,leds=0,
    				PIN[]={4,5,10,11},
    				MASK[]={0x1,0x2,0x4,0x8};
    SSP_CFG_Type  *SSP_InitSt;
    SSP_CFG_Type SSP_InitStruct;
	SSP_InitStruct.CPHA = SSP_CPHA_FIRST;
	SSP_InitStruct.CPOL = SSP_CPOL_HI;
	SSP_InitStruct.ClockRate = 1000000;
	SSP_InitStruct.Databit = SSP_DATABIT_8;
	SSP_InitStruct.Mode = SSP_MASTER_MODE;
	SSP_InitStruct.FrameFormat = SSP_FRAME_SPI;
	SSP_InitSt=&SSP_InitStruct;
//    SSP_IOConfig(SPI_0);		// Configuracion de SPI_0.
//	SSP_Init(SPI_0,SSP_InitSt);			// Inicializacion de SPI_0.
	ini_spi1769(SPI_1);
	WG12864A_Init();			// Inicializacion del DISPLAY.
	Menu_Logo();				// Presento el Logo de la Empresa.

    while(1)
    {
    	for(i=0;i<1000;i++)									// Delay ciclado
    	{
    		for(j=0;j<1000;j++);
    	}
    	i=0;
//------------ Enciende el P0,22 parpadeante ---------------
    	if(led)
    		led=0;
    	else
    		led=1;
   	     set_value(0, 22, led);								// Led Stick Parpadeante

//----------------------------------------------------------
    	for(t=0;t<4;t++)
    	{
    		if(a==t)										// Enciende un puerto por vuelta (Nivel bajo)
    			leds=0;
    		else
    			leds=1;
      	     set_value(0, PIN[t], leds);					// Envia puerto, pin y estado
    	}
    	a++;												// Incrementa uno por vuelta
    	if(a==4)
    		a=0;
    }
    return 0 ;
}

void set_value (uint8_t  port, uint8_t  pin, uint8_t value)	// Función que maneja el puerto
{
	if( value == ON )
	{
		GPIO_SetValue(port, (1<<pin));
		return;
	}
	if( value == OFF )
	{
		GPIO_ClearValue(port, (1<<pin));
		return;
	}
}

