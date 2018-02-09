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
 * Funciones de inicialización y manejo del Touch panel
****************************************************************************/

#include "adc.h"
#include "gpio.h"
#include "Menu.h"
#include "Touch.h"
#include "timer32.h"
#include "WG12864A.h"
#include "Definiciones.h"

//--------------------------------------------------------------------------
//***********************  Variables Externas  *****************************
extern unsigned int adc_valX, adc_valY,adc_val5;
extern unsigned int sleepmenu;

//--------------------------------------------------------------------------
//***********************  Variables Propias  ******************************
unsigned int 	menu, menu1, menu2, menu3,	menu32, menu4, menu41,
				menu41_1,menu41_2, menu41_3, menu41_4;
unsigned int 	sleep,														// Variable que a cierto valor duerme la pantalla.
				flagsleep,													// Variable que me indica si el Display esta dormido para despertarlo.
				flagirq;													// Bandera que me indica si habilitar o no la IRQ del TOUCH.
int 			a = 0;
//--------------------------------------------------------------------------

void TOUCH_Init(void)														// Configuracion de INTERRUPCION TOUCH por bajo nivel.
{
	GPIOInit();
	GPIOSetInterrupt(PORT1, 2, 0, 0, 0);
}
//--------------------------------------------------------------------------
void PIOINT1_IRQHandler(void)
{
	GPIOIntClear(PORT1, 2);													// Limpio la Interrupcion.
	if( flagsleep )															// Si la Bandera de Sleep esta arriba es que el Display duerme y los despiesto.
	{
		enable_timer32(1);													// Habilito el timer para el proximo Sleep.
		if( sleep >= MULTIPLICADOR_TIMMER_SLEEP ) 							// Si es Mayor a 5 reinicio por completo, sino solo prendo la pantalla.
		{
			flagsleep = 0;													// Bajo la bandera de que el Display duerme.
			sleepmenu = 1;													// Seleccion con que modalidad vuelve del Sleep.
			sleep = 0;														// Reset la variable que duerme la pantalla.
		}
		else
		{
			flagsleep = 0;													// Bajo la bandera de que el Display duerme.
			sleepmenu = 0;													// Seleccion con que modalidad vuelve del Sleep.
			sleep = 0;														// Reset la variable que duerme la pantalla.
		}
	}
	else
	{
		reset_timer32 (1);													// Reseteo el Timmer de Sleep porque la pantalla esta en uso.
		sleep = 0;															// Reset la variable que duerme la pantalla.
		flagirq = 0;														// Bandera que me indica si habilitar o no la IRQ del TOUCH.
		adc_valX = 0, adc_valY = 0;											// Reseteo el valor de X, Y del ADC.
		//Obtengo las Coordenadas donde se presiono y pongo el Touch en reposo.
		TOUCH_GetX();
		TOUCH_GetY();
//		Leo_ADC5();
		TOUCH_Standby();
	}
}
//--------------------------------------------------------------------------
void TOUCH_Standby(void)
{
	GPIOIntDisable(PORT1, 2);												//Desabilito la IRQ ya que modifico el estado de los pines.

	//X2P (+) GND.
	LPC_IOCON->TOUCH_X2P = 0xD1;											//Paso el Pin a Puerto GPIO.
	GPIOSetDir (PORT0,11,SALIDA);											//Seteo el Puerto como Salida!.
	GPIOSetValue(PORT0,11,GND);												//Pongo el Pin X2P a GND.

	//X1M (-) DISABLE PULL UP & PULL DOWN - GPIO.
	LPC_IOCON->TOUCH_X1M = 0xC1;											//Paso el Pin a Hi-z.
	GPIOSetDir (PORT1,1,ENTRADA);											//Seteo el Pin como Entrada.

	//Y1M (-) ENABLE PULL UP / INT.
	LPC_IOCON->TOUCH_Y1M = 0xD1;											//Paso el Pin a PULL UP.
	GPIOSetDir (PORT1,2,ENTRADA);											//Seteo el Pin como Entrada.

	//Y2P (+) DISABLE PULL UP & PULL DOWN - GPIO.
	LPC_IOCON->TOUCH_Y2P = 0xC1;											//Paso el Pin a Puerto GPIO.
	GPIOSetDir (PORT1,0,ENTRADA);											//Seteo el Pin Y2P como Entrada.
}
//--------------------------------------------------------------------------
void TOUCH_GetX(void)
{
	int g, auxX = 0;

	GPIOIntDisable(PORT1, 2);												//Desabilito la IRQ ya que modifico el estado de los pines.

	//X1M (-) GND.
	LPC_IOCON->TOUCH_X1M = 0xD1;											//Paso el Pin a Puerto GPIO.
	GPIOSetDir (PORT1,1,SALIDA);											//Seteo el Puerto como Salida!.
	GPIOSetValue(PORT1,1,GND);												//Pongo el Pin X1M a GND.

	//X2P (+) VCC.
	LPC_IOCON->TOUCH_X2P = 0xD1;											//Paso el Pin a Puerto GPIO.
	GPIOSetDir (PORT0,11,SALIDA);											//Seteo el Puerto como Salida!.
	GPIOSetValue(PORT0,11,VCC);												//Pongo el Pin X2P a VCC.

	//Y1M (-) DISABLE PULL UP / ADC3.
	LPC_IOCON->TOUCH_Y1M = 0x42;											//Paso el Pin a Hi-z y Open Drain.
	GPIOSetDir (PORT1,2,ENTRADA);											//Seteo el Pin como Entrada.

	//Y2P (+) ENABLE PULL DOWN y GPIO.
	LPC_IOCON->TOUCH_Y2P = 0xC9;											//Paso el Pin a Puerto GPIO.
	GPIOSetDir (PORT1,0,ENTRADA);											//Seteo el Pin Y2P como Entrada.

//	LPC_IOCON->GPIO_ADC_5 = 0xD1;											//Paso el Pin a Puerto GPIO.
//	GPIOSetDir (PORT1,4,SALIDA);											//Seteo el Puerto como Salida!.

	for( g = 0; g < 10; g++ )
	{
		//Tomo 10 Muestras del ADC.
		auxX += ADCRead (TOUCH_ADC3_Y1M);
	}
	//Obtengo el Promedio de los valores obtenidos.
	auxX = auxX/10;
	//Le sumo un desplazamiento al valor obtenido por un error de Offset.
	adc_valX = auxX + OFFSET_ADC;
//	adc_val5 = adc_valX;
}

//--------------------------------------------------------------------------
void TOUCH_GetY(void)
{
	int g, auxY = 0;

	GPIOIntDisable(PORT1, 2);												//Desabilito la IRQ ya que modifico el estado de los pines.

	//Y1M (-) GND.
	LPC_IOCON->TOUCH_Y1M = 0x0D1;											//Paso el Pin a Puerto GPIO.
	GPIOSetDir (PORT1,2,SALIDA);											//Seteo el Puerto como Salida!.
	GPIOSetValue(PORT1,2,GND);												//Pongo el Pin Y1N a GND.

	//Y2P (+) VCC.
	LPC_IOCON->TOUCH_Y2P = 0x0D1;											//Paso el Pin a Puerto GPIO.
	GPIOSetDir (PORT1,0,SALIDA);											//Seteo el Puerto como Salida!.
	GPIOSetValue(PORT1,0,VCC);												//Pongo el Pin Y2P a VCC.

	//X1M (-) DISABLE PULL UP / ADC2.
	LPC_IOCON->TOUCH_X1M = 0x42;											//Paso el Pin a Hi-z y Open Drain.
	GPIOSetDir (PORT1,1,ENTRADA);											//Seteo el Pin como Entrada.

	//X2P (+) ENABLE PULL DOWN y GPIO.
	LPC_IOCON->TOUCH_X2P = 0xC9;											//Paso el Pin a Hi-z, Open Drain y GPIO.
	GPIOSetDir (PORT0,11,ENTRADA);											//Seteo el Pin como Salida.

//	LPC_IOCON->GPIO_ADC_5 = 0xD1;											//Paso el Pin a Puerto GPIO.
//	GPIOSetDir (PORT1,4,SALIDA);											//Seteo el Puerto como Salida!.

	for( g = 0; g < 10; g++ )
	{
		//Tomo 10 Muestras del ADC.
		auxY += ADCRead (TOUCH_ADC2_X1M);
	}

	//Obtengo el Promedio de los valores obtenidos.
	auxY = auxY/10;
	//Le sumo un desplazamiento al valor obtenido por un error de Offset.
	adc_valY = auxY + OFFSET_ADC;
}
//--------------------------------------------------------------------------
void Leo_ADC5(void)
{
	int g, aux5 = 0;

	// DISABLE PULL UP / ADC5.
	LPC_IOCON->GPIO_ADC_5 = 0x51;											//Paso el Pin a Hi-z y Open Drain.
	GPIOSetDir (PORT1,4,ENTRADA);											//Seteo el Pin como Entrada.
	adc_val5=ADCRead (TOUCH_ADC5);

/*	for( g = 0; g < 10; g++ )												//Tomo 10 Muestras del ADC.
	{
		aux5 += ADCRead (TOUCH_ADC5);
	}
	aux5 = aux5/10;															//Obtengo el Promedio de los valores obtenidos.
	adc_val5 = aux5;// + OFFSET_ADC;
*/
}
