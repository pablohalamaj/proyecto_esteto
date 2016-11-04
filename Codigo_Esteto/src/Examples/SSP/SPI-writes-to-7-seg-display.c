/*
    FreeRTOS V7.3.0 - Copyright (C) 2012 Real Time Engineers Ltd.


    ***************************************************************************
     *                                                                       *
     *    FreeRTOS tutorial books are available in pdf and paperback.        *
     *    Complete, revised, and edited pdf reference manuals are also       *
     *    available.                                                         *
     *                                                                       *
     *    Purchasing FreeRTOS documentation will not only help you, by       *
     *    ensuring you get running as quickly as possible and with an        *
     *    in-depth knowledge of how to use FreeRTOS, it will also help       *
     *    the FreeRTOS project to continue with its mission of providing     *
     *    professional grade, cross platform, de facto standard solutions    *
     *    for microcontrollers - completely free of charge!                  *
     *                                                                       *
     *    >>> See http://www.FreeRTOS.org/Documentation for details. <<<     *
     *                                                                       *
     *    Thank you for using FreeRTOS, and thank you for your support!      *
     *                                                                       *
    ***************************************************************************


    This file is part of the FreeRTOS distribution.

    FreeRTOS is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License (version 2) as published by the
    Free Software Foundation AND MODIFIED BY the FreeRTOS exception.
    >>>NOTE<<< The modification to the GPL is included to allow you to
    distribute a combined work that includes FreeRTOS without being obliged to
    provide the source code for proprietary components outside of the FreeRTOS
    kernel.  FreeRTOS is distributed in the hope that it will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
    or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
    more details. You should have received a copy of the GNU General Public
    License and the FreeRTOS license exception along with FreeRTOS; if not it
    can be viewed here: http://www.freertos.org/a00114.html and also obtained
    by writing to Richard Barry, contact details for whom are available on the
    FreeRTOS WEB site.

    1 tab == 4 spaces!

    http://www.FreeRTOS.org - Documentation, latest information, license and
    contact details.

    http://www.SafeRTOS.com - A version that is certified for use in safety
    critical systems.

    http://www.OpenRTOS.com - Commercial support, development, porting,
    licensing and training services.
*/

/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

/* FreeRTOS+IO includes. */
#include "FreeRTOS_IO.h"

/* Library includes. */
#include "lpc17xx_gpio.h"

/* Example includes. */
#include "SPI-writes-to-7-seg-display.h"

/* Place holder for calls to ioctl that don't use the value parameter. */
#define spiPARAMTER_NOT_USED			( ( void * ) 0 )
#define ON 	1
#define OFF 0

/* 7 segment display character map. */
static const uint8_t pcStringOfDigits[] = { 0x24, /* 0 */	0xAF, /* 1 */ 0xE0, /* 2 */	0xA2, /* 3 */ 0x2B, /* 4 */	0x32, /* 5 */ 0x30, /* 6 */	0xA7, /* 7 */ 0x20, /* 8 */	0x22 };
void set_value (uint8_t  port, uint8_t  pin, uint8_t value);
/*-----------------------------------------------------------*/

/*
 * The task that uses the SSP in SPI mode to write to the 7 segment display.
 */
static void prvSPIWriteTask( void *pvParameters );

/*-----------------------------------------------------------*/

void vSPIWriteTaskStart( void )
{
	xTaskCreate( 	prvSPIWriteTask,						/* The task that uses the SSP in SPI mode. */
					( const int8_t * const ) "SPIWrt", 		/* Text name assigned to the task.  This is just to assist debugging.  The kernel does not use this name itself. */
					configSPI_7_SEG_WRITE_TASK_STACK_SIZE,	/* The size of the stack allocated to the task. */
					NULL,									/* The parameter is not used, so NULL is passed. */
					configSPI_7_SEG_WRITE_TASK_PRIORITY,	/* The priority allocated to the task. */
					NULL );									/* A handle to the task being created is not required, so just pass in NULL. */
}
/*-----------------------------------------------------------*/

static void prvSPIWriteTask( void *pvParameters )
{
Peripheral_Descriptor_t xSPIPort;
const portTickType xTaskPeriod_ms = 500UL / portTICK_RATE_MS;
portTickType xLastExecutionTime;
uint8_t ucChar = 0U;
const uint8_t ucMaxChar = 10U;
const uint32_t xClockSpeed = 500000UL;

	( void ) pvParameters;


//------------ Seteo los puertos como salida ---------------
GPIO_SetDir(2, (1<<6), 1);
GPIO_SetDir(0, (1<<4), 1);
GPIO_SetDir(0, (1<<5), 1);
GPIO_SetDir(0, (1<<10), 1);
GPIO_SetDir(0, (1<<11), 1);

volatile static int i,j,t,a=0;
unsigned char 	led=0,leds=0,
   				PIN[]={4,5,10,11},
   				MASK[]={0x1,0x2,0x4,0x8};

while(1)
{
   	for(i=0;i<1000;i++)									// Delay ciclado
   	{
   		for(j=0;j<1000;j++);
   	}
   	i=0;
//------------ Enciende el P2,6 parpadeante ----------------
/*   	if(led)
   		led=0;
    	else
   		led=1;
     	set_value(2, 6, led);
*/
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
//	    return 0 ;


	/* Configure the IO pin used for the 7 segment display CS line. */
	GPIO_SetDir( board7SEG_CS_PORT, board7SEG_CS_PIN, boardGPIO_OUTPUT );
	board7SEG_DEASSERT_CS();

	/* Ensure the OLED, which is on the same SSP bus, is not selected. */
	GPIO_SetDir( boardOLED_CS_PORT, boardOLED_CS_PIN, boardGPIO_OUTPUT );
	boardOLED_DEASSERT_CS();

	
	/* Open the SSP port used for writing to the 7 segment display.  The second 
	parameter (ulFlags) is not used in this case.  By default, the SSP will open
	in SPI/polling mode. */
	xSPIPort = FreeRTOS_open( board7SEGMENT_SSP_PORT, ( uint32_t ) spiPARAMTER_NOT_USED );
	configASSERT( xSPIPort );
	
	/* Decrease the bit rate a bit, just to demonstrate an ioctl function being
	used.  By default it was 1MHz, this sets it to 500KHz. */
	FreeRTOS_ioctl( xSPIPort, ioctlSET_SPEED, ( void * ) xClockSpeed );

	/* At the time of writing, by default, the SSP will open all its parameters
	set correctly for communicating with the 7-segment display, but explicitly
	set all	parameters anyway, in case the defaults	changes in the future. */
	FreeRTOS_ioctl( xSPIPort, ioctlSET_SPI_DATA_BITS, 		( void * ) boardSSP_DATABIT_8 );
	FreeRTOS_ioctl( xSPIPort, ioctlSET_SPI_CLOCK_PHASE, 	( void * ) boardSPI_SAMPLE_ON_LEADING_EDGE_CPHA_0 );
	FreeRTOS_ioctl( xSPIPort, ioctlSET_SPI_CLOCK_POLARITY, 	( void * ) boardSPI_CLOCK_BASE_VALUE_CPOL_0 );
	FreeRTOS_ioctl( xSPIPort, ioctlSET_SPI_MODE, 			( void * ) boardSPI_MASTER_MODE );
	FreeRTOS_ioctl( xSPIPort, ioctlSET_SSP_FRAME_FORMAT, 	( void * ) boardSSP_FRAME_SPI );

	/* Initialise xLastExecutionTime prior to the first call to 
	vTaskDelayUntil().  This only needs to be done once, as after this call,
	xLastExectionTime is updated inside vTaskDelayUntil. */
	xLastExecutionTime = xTaskGetTickCount();
	
	for( ;; )
	{
		/* Send down a character at a time, using polling mode Tx. */
		for( ucChar = 0U; ucChar < ucMaxChar; ucChar++ )
		{
			/* Delay until it is time to update the display with a new digit. */
			vTaskDelayUntil( &xLastExecutionTime, xTaskPeriod_ms );

			board7SEG_ASSERT_CS();
			FreeRTOS_write( xSPIPort, &( pcStringOfDigits[ ucChar ] ), sizeof( uint8_t ) );
			board7SEG_DEASSERT_CS();
		}
	}
}
/*-----------------------------------------------------------*/

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

