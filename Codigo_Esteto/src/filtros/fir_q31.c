/*
===============================================================================
 Name        : fir_q31.c
 Author      : ss
 Version     : v1.0
 date		 : 01/07/2016
 Description :
===============================================================================
*/

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif
/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
/* FreeRTOS+IO includes. */
#include "FreeRTOS_IO.h"
#include "adc.h"
#include "dac.h"
#include "lowpass.h"
#include "bandpass.h"
#include "fir_q31.h"

#define USAR_FUNCIONES_ASSEMBLER	0
#define CTE_CONVERSION_VOLTS ((int)(3.3*65535/1024))
#define BUFFER		0
#define FILTRO		(!BUFFER)

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


void ADC_IRQHandler(void)
{
	if (ADC_ChannelGetStatus(LPC_ADC,ADC_CHANNEL_1,ADC_DATA_DONE))
	{
		adcFlag=1;

#if BUFFER
		//dac_data = (ADC_ChannelGetData(LPC_ADC, ADC_CHANNEL_0)>>2);//*(CTE_CONVERSION_VOLTS);
#elif FILTRO
		fir_q31_put(&filtro, (ADC_ChannelGetData(LPC_ADC, ADC_CHANNEL_1)>>2));
#endif
		//dacWrite((ADC_ChannelGetData(LPC_ADC, ADC_CHANNEL_0)>>2));
		//LPC_GPIO2->FIOPIN ^=(1<<13); // PARA PROBAR SI ESTA MUESTREANDO EN EL TIEMPO DESEADO
		ADC_ChannelCmd(LPC_ADC,ADC_CHANNEL_1,DISABLE);
		NVIC_DisableIRQ(ADC_IRQn);
	}
}

/*-----------------------------------------------------------*/
static void prvFiltradoTask( void *pvParameters );

void vFILTERTaskStart( void )												// Creo la tarea de filtrado
{
	xTaskCreate( 	prvFiltradoTask,										// Puntero a función de tarea
					( const int8_t * const ) "FILTRO", 						// Etiqueta
					configFilter_TASK_STACK_SIZE,							// Definición de tamaño
					NULL,
					configFilter_TASK_PRIORITY,								// Prioridad de la tarea
					NULL );
}
/*-----------------------------------------------------------*/

static void prvFiltradoTask( void *pvParameters )							// Tarea de filtratado
{
	int i=0, offset;
	( void ) pvParameters;
	//int j = 50;
	fir_q31_init(&filtro, history, lowpass_taps, LOWPASS_TAP_NUM);
	offset=0;//(-1024);//(-512);											//*CTE_CONVERSION_VOLTS);
	SysTick_Config(SystemCoreClock/1000);
	adcInit();
	dacInit();
	ADC_ChannelCmd(LPC_ADC,ADC_CHANNEL_1,ENABLE);
	NVIC_EnableIRQ(ADC_IRQn);
	ADC_StartCmd(LPC_ADC,ADC_START_NOW);									// Start conversion

	while(1)
	{
		if(adcFlag)
		{
			adcFlag=0;
			y[i] = fir_q31_get(&filtro) + offset;
			if(y[i]>511)
				y[i] = 511;
			else if(y[i]<-512)
					y[i] = -512;

			dacWrite(y[i]+512);
			i++;
			if(i==500)
				i=0;
		}
		if(timerflag)
		{
			timerflag = 0;
			ADC_ChannelCmd(LPC_ADC,ADC_CHANNEL_1,ENABLE);
			NVIC_EnableIRQ(ADC_IRQn);
			ADC_StartCmd(LPC_ADC,ADC_START_NOW);							// Start conversion
		}
	}
}

void fir_q31_init(fir_q31_t * f, int * history, const int * kernel, int num_taps)
{
	int i;

	f->num_taps = num_taps;
	f->history = history;
	f->kernel = kernel;

	for(i = 0; i < f->num_taps; ++i)
		f->history[i] = 0;
	f->last_index = 0;
}

void fir_q31_put(fir_q31_t * f, int input)
{
  f->history[f->last_index++] = input;
  if(f->last_index == f->num_taps)
    f->last_index = 0;
}

int fir_q31_get(fir_q31_t * f)
{
	long long acc = 0;
	int index = f->last_index, i;
	for(i = 0; i < f->num_taps; ++i)
	{
		index = index != 0 ? index-1 : f->num_taps-1;
		acc += (long long)f->history[index] * f->kernel[i];
	};

	return acc >> 16;
}
