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
 *	Definiciones de constantes
****************************************************************************/

#ifndef DEFINICIONES_H_
#define DEFINICIONES_H_
// *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
#define SD_MMC
//	SPI.
#define SPI_0					0
#define SPI_1					1				//SPI1

//	OFFSET ADC.
#define OFFSET_ADC				0x0A			//10 Posiciones.

//  ESTADOS.
#define	SALIDA					1
#define	ENTRADA					0
#define	VCC						1
#define	GND						0
#define KO        0
#define OK        1

//	PINES GPIO TOUCH.
#define	TOUCH_X1M				R_PIO1_1		//AD2
#define	TOUCH_X2P				R_PIO0_11		//AD0
#define	TOUCH_Y1M				R_PIO1_2		//AD3
#define	TOUCH_Y2P				R_PIO1_0		//AD1
#define	GPIO_ADC_5				PIO1_4			//AD5
//	ADC's TOUCH.
#define	TOUCH_ADC2_X1M			2				//AD2
#define	TOUCH_ADC3_Y1M			3				//AD3
#define	TOUCH_ADC5				5				//AD5

//	TIEMPOS PARA DEMORAS.
//#define	TIMMER_DEMORA_IRQ			500			//0.5 Seg. para demorar la habilitacion de la IRQ del Touch.
#define	TIMMER_FONDO_BLANCO			400			//0.4 Seg. para los Fondos de los Menus que se seleccionan.
#define	TIMMER_FONDO_BLANCO_BOTONES	100			//0.2 Seg. para los Fondos de los Botones que se seleccionan.
#define	TIMMER_LOGO_AUTOTROL		1000		//1 Seg. para la presentacion del Logo Autotrol.
#define TIMMER_ACTUALIZA_FW			3000		//3 seg. para Mostrar en pantalla actulizando FW
#define	TIMMER_TIT_CURSOR			200			//0.2 Seg. para la presentacion del cursor del teclado.
#define	TIMMER_MUESTRO_MSJ			1000		//1 Seg. para Mostrar mensajes.
#define	TIMMER_BASE_1S				50000000	//1 Seg. para la rutina de IRQ del timmer.
#define	TIMMER_SLEEP				60			//60 Seg = 1 Min. Para dormir la pantalla.
#define MULTIPLICADOR_TIMMER_SLEEP	5			//Multiplica cuanto Timmer Sleep son necesarios para reiniciar la Pantalla.
#define TIMMER_BASE_25MS			0x124F80	//25ms->x=0.025*4*12MHz=1200000=0x124F80
//	CLAVE MENU ESPECIAL.
#define CLAVE					{3,0,6,0}		//Clave para el acceso del menu especial.
#define PICO_POSIT				0x6a//0x70//3800			// Umbral Para detectar pico +
// *-*-*-*-*-*-*-*-*-*-*-*-*- FUNCIONES -*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//							msf		m=menu	s=submenu	f=funcion
#define fyh			1	//	111
#define dds			2	//	112
#define det			3	//	113
#define ests		4	//	114
#define modp		5	//	115
#define modc		6	//	116
#define consp		7	//	121
#define errs		8	//	131
#define berr		9	//	132

//-------------------------------
//	Acciones dentro del menu
#define proxhora	10
#define proxfecha	11
#define cauerrs		12
#define flecha		13
#define flechamc	14
#define afw			41
#define MODO_CICLADO 21
#define ED_PARAM	25
#define comu		23
#define sinerr		37
#define comu_1200	1
#define comu_9600	2
#define sin_comu	3
#define NORMAL		1
#define PLAN		2
#define TITILANTE	3
#define APAGADO		4
#define EMERGENCIA	5

//------------------------------
//--------------------------------------------------------------------
#endif /* DEFINICIONES_H_ */
