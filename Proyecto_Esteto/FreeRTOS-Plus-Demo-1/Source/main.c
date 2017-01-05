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

#include "adc.h"
#include "dac.h"
#include "lowpass.h"
#include "bandpass.h"
#include "fir_q31.h"

#define USAR_FUNCIONES_ASSEMBLER	0
#define CTE_CONVERSION_VOLTS ((int)(3.3*65535/1024))
#define TIEMPO_DE_MUESTREO		20
#define BUFFER		0
#define FILTRO		(!BUFFER)

//-----------------  Funciones globales  ---------------------------
void vRegisterCLICommands( void );
static uint32_t ulClocksPer10thOfAMilliSecond = 0UL;

//-----------------  Variables globales  ---------------------------

int adcFlag=0;
int timerflag = 0;
int timer;
int datos[9] ={};
int y[500];
int history[LOWPASS_TAP_NUM];
fir_q31_t filtro;
#if BUFFER
unsigned int dac_data;
#elif FILTRO
#endif

//-----------------  Main  -----------------------------------------
int main( void )
{
	NVIC_SetPriorityGrouping( 0UL );								// Setea prioridad
	vGPIOSoftwareTimersStart();										// Dispara timers
	vUARTCommandConsoleStart();										// Dispara tarea de consola(SACAR)
	vSPIWriteTaskStart();											// Dispara tarea de display
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
