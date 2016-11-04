/*
===============================================================================
 Name        : main.c
 Author      : ss
 Version     : v1.0
 date		 : 01/07/2016
 Description : Programa principal
===============================================================================
*/

//-----------------  Standard includes  ----------------------------
#include <string.h>
#include "FreeRTOS.h"
#include "task.h"
#include "GPIO-output-and-software-timers.h"
#include "UART-interrupt-driven-command-console.h"
#include "UART-operation-modes-test.h"
#include "SPI-writes-to-7-seg-display.h"
#include "I2C-coordinator.h"
#include "FreeRTOSConfig.h"
#include "FreeRTOS_CLI.h"
#include "LPC17xx.h"
#include "fir_q31.h"


//#include "ssp.h"
//#include "gpio.h"
//#include "Menu.h"
#include "Touch.h"
//#include "timer32.h"
#include "WG12864A.h"
#include "Definiciones.h"

extern int 	timerflag;
extern int 	timer;
#define 	TIEMPO_DE_MUESTREO		20
//-----------------  Funciones globales  ---------------------------
void vRegisterCLICommands( void );
static uint32_t ulClocksPer10thOfAMilliSecond = 0UL;


//-----------------  Main  -----------------------------------------
int main( void )
{

//	SSP_IOConfig(SPI_0);											// Configuracion de SPI_0.
//	SSP_Init(SPI_0);												// Inicializacion de SPI_0.
//	SSP_IOConfig(SPI_0);											// Configuracion de SPI_1.
//	SSP_Init(SPI_0);												// Inicializacion de SPI_1.
	//WG12864A_Init();												// Inicializacion del DISPLAY.
//	ADCInit(ADC_CLK);												// Inicializacion de ADC.
	//TOUCH_Init();													// Inicializacion del TOUCH.
//	TOUCH_Standby();
//	init_timer32 (0, 1);											// Timmer 0, para Delays.
//	init_timer32 (1, (TIMMER_BASE_1S * TIMMER_SLEEP));				// Timmer 1, para Sleep Display.

	NVIC_SetPriorityGrouping( 0UL );								// Setea prioridad
	vGPIOSoftwareTimersStart();										// Dispara timers
	vUARTCommandConsoleStart();										// Dispara tarea de consola(SACAR)
	vSPIWriteTaskStart();											// Dispara tarea de display
	vFILTERTaskStart();												// Dispara tarea de filtrado
	vI2CTaskStart();												// Dispara tarea I2C
	vRegisterCLICommands();											// Muestra tareas RUN
	vTaskStartScheduler();											// Dispara tareas
	for( ;; );														// Nunca deberia llegar aca
}

//-----------------  Funciones  ------------------------------------
void vMainConfigureTimerForRunTimeStats( void )
{
	/* How many clocks are there per tenth of a millisecond? */
	ulClocksPer10thOfAMilliSecond = configCPU_CLOCK_HZ / 10000UL;
}

//------------------------------------------------------------------
uint32_t ulMainGetRunTimeCounterValue( void )
{
	uint32_t 			ulSysTickCounts, ulTickCount, ulReturn;
	const uint32_t 		ulSysTickReloadValue = ( configCPU_CLOCK_HZ / configTICK_RATE_HZ ) - 1UL;
	volatile uint32_t * const pulCurrentSysTickCount = ( ( volatile uint32_t *) 0xe000e018 );
	volatile uint32_t * const pulInterruptCTRLState = ( ( volatile uint32_t *) 0xe000ed04 );
	const uint32_t 		ulSysTickPendingBit = 0x04000000UL;

	ulSysTickCounts = ulSysTickReloadValue - *pulCurrentSysTickCount;
	ulTickCount = xTaskGetTickCountFromISR();
	if( ( *pulInterruptCTRLState & ulSysTickPendingBit ) != 0UL )
	{
		ulTickCount++;
		ulSysTickCounts = ulSysTickReloadValue - *pulCurrentSysTickCount;
	}
	ulReturn = ( ulTickCount * 10UL ) ;
	ulReturn += ( ulSysTickCounts / ulClocksPer10thOfAMilliSecond );

	return ulReturn;
}
void SysTick_Handler(void)
{
	if(timer>0)
	{
		timer--;
	}
	else
	{
		if(timer == 0)
		{
			timerflag = 1;
			//estado = !estado;
			timer = TIEMPO_DE_MUESTREO;
		}
	}
}

//------------------------------------------------------------------

void vApplicationStackOverflowHook( xTaskHandle pxTask, signed char *pcTaskName )
{
	( void ) pcTaskName;
	( void ) pxTask;
	taskDISABLE_INTERRUPTS();
	for( ;; );
}
//------------------------------------------------------------------

void vApplicationMallocFailedHook( void )
{
	for( ;; );
}
//------------------------------------------------------------------
