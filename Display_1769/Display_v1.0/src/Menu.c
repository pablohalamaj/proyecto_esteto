/********************************************************
 Name          	: Menu.c
 Created on		: 08/11/2013
 Author        	: Sebastian Sisevich/potero
 Copyright     	:
 **********************************************************/

//#include "ssp.h"
#include "lpc17xx_ssp.h"
#include "lpc17xx_gpio.h"
//#include "gpio.h"
#include "Menu.h"
#include "Touch.h"
//#include "timer32.h"
#include "WG12864A.h"
#include "Funciones.h"
#include "Definiciones.h"

// *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

// Variables GLOBALES para posicionamiento en todo el Menú.
extern unsigned int menu = 0, menu1 = -1, menu2 = -1, menu3 = -1,
					menu32 = -1, menu4 = -1, menu41 = -1, menu41_1 = -1,
					menu41_2 = -1, menu41_3 = -1, menu41_4 = -1;
extern char			flagmm;
// Variables GLOBALES con los valores de posicion en X e Y.
unsigned int adc_valX, adc_valY;

// Vectores para validar la clave del Menú Especial.
unsigned int clave[] = CLAVE, cla[4];

// Variable Globales.
unsigned int 	cur=76,			// Posicion del cursor dentro de la pantalla.
				indice=0,			// Variable dinamica para moverse por el vector de la clave ingresada.
				sleepmenu=0,		// Variable que me indica con que menu vuelvo del Sleep en la funcion Sleep.
				flagirq=-1;		// Bandera que me indica si habilitar o no la IRQ del TOUCH.

// Variable para usos varios.
int var_aux;

// *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
void Menu_Logo(void){
	int i;

	// "AUTOTROL" (Mensaje de inicio).
	WG12864A_posXY(15,3);
	WG12864A_print_symbol(a16x16, BLANCO);
	WG12864A_posXY(31,3);
	WG12864A_print_symbol(b16x16, BLANCO);
	WG12864A_posXY(47,3);
	WG12864A_print_symbol(c16x16, BLANCO);
	WG12864A_posXY(63,3);
	WG12864A_print_symbol(d16x16, BLANCO);
	WG12864A_posXY(79,3);
	WG12864A_print_symbol(e16x16, BLANCO);
	WG12864A_posXY(95,3);
	WG12864A_print_symbol(f16x16, BLANCO);
	WG12864A_posXY(15,5);
	WG12864A_print_symbol(g16x16, BLANCO);
	WG12864A_posXY(31,5);
	WG12864A_print_symbol(h16x16, BLANCO);
	WG12864A_posXY(47,5);
	WG12864A_print_symbol(i16x16, BLANCO);
	WG12864A_posXY(63,5);
	WG12864A_print_symbol(j16x16, BLANCO);
	WG12864A_posXY(79,5);
	WG12864A_print_symbol(k16x16, BLANCO);
	WG12864A_posXY(95,5);
	WG12864A_print_symbol(l16x16, BLANCO);
	GLCD_Output_High(BACKLIGHT);

	for(i=0;i<10000;i++);

//	delay32Ms(0, TIMMER_LOGO_AUTOTROL);
}

// *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
void Menu_Inicial(void){

	// Funcion que maneja el Sleep de la pantalla y la IRQ del TOUCH.
	Func_Sleep (flagirq, sleepmenu);
	switch( menu ){

	case 0:
		adc_valX = 0, adc_valY = 0;			// Reseteo el valor de X, Y del ADC.
		WG12864A_posXY(1, 1);
		WG12864A_printf("  *Menu Principal*   ", Arial8x6, BLANCO);
		WG12864A_posXY(1, 2);
		WG12864A_printf("1. Menu de Consultas ", Arial8x6, NEGRO);
		WG12864A_posXY(1, 3);
		WG12864A_printf("                     ", Arial8x6, NEGRO);
		WG12864A_posXY(1, 4);
		WG12864A_printf("2. Menu de Parametros", Arial8x6, NEGRO);
		WG12864A_posXY(1, 5);
		WG12864A_printf("                     ", Arial8x6, NEGRO);
		WG12864A_posXY(1, 6);
		WG12864A_printf("3. Menu de Reloj     ", Arial8x6, NEGRO);
		WG12864A_posXY(1, 7);
		WG12864A_printf("                     ", Arial8x6, NEGRO);
		WG12864A_posXY(1, 8);
		WG12864A_printf("4. Menu Especial     ", Arial8x6, NEGRO);

/*//-------------------- Muestra el hexa en la pantalla ----------------------
			WG12864A_posXY(56, 7);
			WG12864A_putchar((((adc_valX >> 4) & 0x0F)+0x20), Hexa8x6, NEGRO);
			WG12864A_putchar(((adc_valX & 0x0F)+0x20), Hexa8x6, NEGRO);
			WG12864A_posXY(70, 7);
			WG12864A_putchar((((adc_valY >> 4) & 0x0F)+0x20), Hexa8x6, NEGRO);
			WG12864A_putchar(((adc_valY & 0x0F)+0x20), Hexa8x6, NEGRO);
*///--------------------------------------------------------------------------

		// Consulta que menu se presionó.
		if( (0x1A < adc_valX) && (adc_valX < 0xEA) &&
			(0xAA < adc_valY) && (adc_valY < 0xC0) ){

			menu = 1;
		}
		if( (0x1A < adc_valX) && (adc_valX < 0xEA) &&
			(0x85 < adc_valY) && (adc_valY < 0xA0) ){

			menu = 2;
		}
		if( (0x1A < adc_valX) && (adc_valX < 0xC0) &&
			(0x5A < adc_valY) && (adc_valY < 0x7A) ){

			menu = 3;
		}
		if( (0x1A < adc_valX) && (adc_valX < 0xC0) &&
			(0x3D < adc_valY) && (adc_valY < 0x50) ){

			menu = 4;
		}
		if( !( ((0x1A < adc_valX) && (adc_valX < 0xEA) && (0xAA < adc_valY) && (adc_valY < 0xC0)) ||
			   ((0x1A < adc_valX) && (adc_valX < 0xEA) && (0x85 < adc_valY) && (adc_valY < 0xA0)) ||
		       ((0x1A < adc_valX) && (adc_valX < 0xC0) && (0x5A < adc_valY) && (adc_valY < 0x7A)) ||
		       ((0x1A < adc_valX) && (adc_valX < 0xC0) && (0x3D < adc_valY) && (adc_valY < 0x50)) )){

			adc_valX = 0, adc_valY = 0;		// Reseteo el valor de X, Y del ADC.
		}
		break;

	case 1:
		WG12864A_posXY(1, 2);
		WG12864A_printf("1. Menu de Consultas", Arial8x6, BLANCO);
		delay32Ms(0, TIMMER_FONDO_BLANCO);
		adc_valX = 0, adc_valY = 0;			// Reseteo el valor de X, Y del ADC.
		menu = -1;							// Desabilito que entre al Menú Principal.
		menu1 = 0;							// Habilito que entre al SubMenu Seleccionado.
		break;

	case 2:
		WG12864A_posXY(1, 4);
		WG12864A_printf("2. Menu de Parametros", Arial8x6, BLANCO);
		delay32Ms(0, TIMMER_FONDO_BLANCO);
		adc_valX = 0, adc_valY = 0;			// Reseteo el valor de X, Y del ADC.
		menu = -1;							// Desabilito que entre al Menú Principal.
		menu2 = 0;							// Habilito que entre al SubMenu Seleccionado.
		break;

	case 3:
		WG12864A_posXY(1, 6);
		WG12864A_printf("3. Menu de Reloj", Arial8x6, BLANCO);
		delay32Ms(0, TIMMER_FONDO_BLANCO);
		adc_valX = 0, adc_valY = 0;			// Reseteo el valor de X, Y del ADC.
		menu = -1;							// Desabilito que entre al Menú Principal.
		menu3 = 0;							// Habilito que entre al SubMenu Seleccionado.
		break;

	case 4:
		WG12864A_posXY(1, 8);
		WG12864A_printf("4. Menu Especial", Arial8x6, BLANCO);
		delay32Ms(0, TIMMER_FONDO_BLANCO);
		adc_valX = 0, adc_valY = 0;			// Reseteo el valor de X, Y del ADC.
		menu = -1;							// Desabilito que entre al Menú Principal.
		menu4 = 0;							// Habilito que entre al SubMenu Seleccionado.
		break;

	default:
		break;
	}

//-------------------------------------------------------------------------------------------------------------------------------
/*******************************************  SUBMENU 1  ***********************************************************************/
//-------------------------------------------------------------------------------------------------------------------------------

	switch( menu1 ){

	case 0:
		adc_valX = 0, adc_valY = 0;			// Reseteo el valor de X, Y del ADC.
		WG12864A_posXY(1, 1);
		WG12864A_printf(" *Menu de Consultas* ", Arial8x6, BLANCO);
		WG12864A_posXY(1, 2);
		WG12864A_printf("1.1 Consulta Estados ", Arial8x6, NEGRO);
		WG12864A_posXY(1, 4);
		WG12864A_printf("1.2 Consulta Ptros   ", Arial8x6, NEGRO);
		WG12864A_posXY(1, 6);
		WG12864A_printf("1.3 Consulta Errores ", Arial8x6, NEGRO);
		WG12864A_posXY(1, 8);
		WG12864A_printf("1.4 Borrar Errores", Arial8x6, NEGRO);
		WG12864A_posXY(110, 7);
		WG12864A_print_symbol(BACK16x16, BLANCO);
		menu1 = 0;								//??

		// Se presionó Back?
		if( (0xCA < adc_valX) && (adc_valX < 0xEA) &&
			(0x2A < adc_valY) && (adc_valY < 0x65) ){

			WG12864A_posXY(110, 7);
			WG12864A_print_symbol(BACK16x16, NEGRO);
			delay32Ms(0, TIMMER_FONDO_BLANCO);
			adc_valX = 0, adc_valY = 0;			// Reseteo el valor de X, Y del ADC.
			menu1 = -1;							// Desabilito que entre al SubMenú.
			menu = 0;							// Habilito que entre al Menu Seleccionado.
			WG12864A_posXY(110, 7);
			WG12864A_print_symbol(BACK16x16, BLANCO);
		}

		// Consulta que menu se presionó.
		if( (0x1A < adc_valX) && (adc_valX < 0xEA) &&
			(0xAA < adc_valY) && (adc_valY < 0xC0) ){

			menu1 = 1;
		}
		if( (0x1A < adc_valX) && (adc_valX < 0xEA) &&
			(0x85 < adc_valY) && (adc_valY < 0xA0) ){

			menu1 = 2;
		}
		if( (0x1A < adc_valX) && (adc_valX < 0xC0) &&
			(0x5A < adc_valY) && (adc_valY < 0x7A) ){

			menu1 = 3;
		}
		if( (0x1A < adc_valX) && (adc_valX < 0xC0) &&
			(0x3D < adc_valY) && (adc_valY < 0x50) ){

			menu1 = 4;
		}
		if( !( ((0x1A < adc_valX) && (adc_valX < 0xEA) && (0xAA < adc_valY) && (adc_valY < 0xC0)) ||
			   ((0x1A < adc_valX) && (adc_valX < 0xEA) && (0x85 < adc_valY) && (adc_valY < 0xA0)) ||
		       ((0x1A < adc_valX) && (adc_valX < 0xC0) && (0x5A < adc_valY) && (adc_valY < 0x7A)) ||
		       ((0x1A < adc_valX) && (adc_valX < 0xC0) && (0x3D < adc_valY) && (adc_valY < 0x50)) )){

			adc_valX = 0, adc_valY = 0;			// Reseteo el valor de X, Y del ADC.
		}
		break;

	case 1:
		WG12864A_posXY(1, 2);
		WG12864A_printf("1.1 Consulta Estados", Arial8x6, BLANCO);
		delay32Ms(0, TIMMER_FONDO_BLANCO);
		adc_valX = 0, adc_valY = 0;				// Reseteo el valor de X, Y del ADC.
		WG12864A_Limpiar(NEGRO); 				// Limpio la pantalla y entro al submenu 1.1

		func_consultaestados();					// Entra en el submenu de consulta de estados
		break;

	case 2:
		WG12864A_posXY(1, 4);
		WG12864A_printf("1.2 Consulta Ptros", Arial8x6, BLANCO);
		delay32Ms(0, TIMMER_FONDO_BLANCO);
		adc_valX = 0, adc_valY = 0;				// Reseteo el valor de X, Y del ADC.
		WG12864A_Limpiar(NEGRO); 				// Limpio la pantalla y entro al submenu 1.2

		func_consultaptros();
		break;

	case 3:
		WG12864A_posXY(1, 6);
		WG12864A_printf("1.3 Consulta Errores", Arial8x6, BLANCO);
		delay32Ms(0, TIMMER_FONDO_BLANCO);
		adc_valX = 0, adc_valY = 0;				// Reseteo el valor de X, Y del ADC.
		WG12864A_Limpiar(NEGRO); 				// Limpio la pantalla y entro al submenu 1.3

		func_conserr();							// Entra en el submenu de consulta de errores.
		break;

	case 4:
		WG12864A_posXY(1, 8);
		WG12864A_printf("1.4 Borrar Errores", Arial8x6, BLANCO);
		delay32Ms(0, TIMMER_FONDO_BLANCO);
		adc_valX = 0, adc_valY = 0;				// Reseteo el valor de X, Y del ADC.
		WG12864A_Limpiar(NEGRO);				// Limpio la pantalla y entro al submenu 1.4

		func_borrarerr();						// Entra en el submenu borrar errores.
		break;

	default:
		break;
	}

//-------------------------------------------------------------------------------------------------------------------------------
/*******************************************  SUBMENU 2  ***********************************************************************/
//-------------------------------------------------------------------------------------------------------------------------------

	switch( menu2 ){

	case 0:
		adc_valX = 0, adc_valY = 0;					// Reseteo el valor de X, Y del ADC.
		WG12864A_posXY(1, 1);
		WG12864A_printf("     *Parametros*    ", Arial8x6, BLANCO);
		WG12864A_posXY(1, 2);
		WG12864A_printf("2.1 Modo Cilado      ", Arial8x6, NEGRO);
		WG12864A_posXY(1, 4);
		WG12864A_printf("2.2 Editar Parametros", Arial8x6, NEGRO);
		WG12864A_posXY(1, 6);
		WG12864A_printf("2.3 Comunicaciones   ", Arial8x6, NEGRO);
		WG12864A_posXY(110, 7);
		WG12864A_print_symbol(BACK16x16, BLANCO);
		WG12864A_posXY(1, 8);
		WG12864A_printf("                 ", Arial8x6, NEGRO);
		menu2 = 0;							// ??

		// Se presionó Back?
		if( (0xCA < adc_valX) && (adc_valX < 0xEA) &&
			(0x2A < adc_valY) && (adc_valY < 0x65) ){

			WG12864A_posXY(110, 7);
			WG12864A_print_symbol(BACK16x16, NEGRO);
			delay32Ms(0, TIMMER_FONDO_BLANCO);
			adc_valX = 0, adc_valY = 0;			// Reseteo el valor de X, Y del ADC.
			menu2 = -1;							// Desabilito que entre al SubMenú.
			menu = 0;							// Habilito que entre al Menu Seleccionado.
			WG12864A_posXY(110, 7);
			WG12864A_print_symbol(BACK16x16, BLANCO);
		}

		// Consulta que menu se presionó.
		if( (0x1A < adc_valX) && (adc_valX < 0xEA) &&
			(0xAA < adc_valY) && (adc_valY < 0xC0) ){

			menu2 = 1;
		}
		if( (0x1A < adc_valX) && (adc_valX < 0xEA) &&
			(0x85 < adc_valY) && (adc_valY < 0xA0) ){

			menu2 = 2;
		}
		if( (0x1A < adc_valX) && (adc_valX < 0xC0) &&
			(0x5A < adc_valY) && (adc_valY < 0x7A) ){

			menu2 = 3;
		}
		if( !( ((0x1A < adc_valX) && (adc_valX < 0xEA) && (0xAA < adc_valY) && (adc_valY < 0xC0)) ||
			   ((0x1A < adc_valX) && (adc_valX < 0xEA) && (0x85 < adc_valY) && (adc_valY < 0xA0)) ||
		       ((0x1A < adc_valX) && (adc_valX < 0xC0) && (0x5A < adc_valY) && (adc_valY < 0x7A)) )){

			adc_valX = 0, adc_valY = 0;			// Reseteo el valor de X, Y del ADC.
		}
		break;

	case 1:
		WG12864A_posXY(1, 2);
		WG12864A_printf("2.1 Modo Ciclado    ", Arial8x6, BLANCO);
		delay32Ms(0, TIMMER_FONDO_BLANCO);
		adc_valX = 0, adc_valY = 0;				// Reseteo el valor de X, Y del ADC.
		WG12864A_Limpiar(NEGRO); 				// Limpio la pantalla y entro al submenu 2.1

		Func_Atributos();						// Funcion del SubMenu seleccionado.

		break;

	case 2:
		WG12864A_posXY(1, 4);
		WG12864A_printf("2.2 Editar Parametros", Arial8x6, BLANCO);
		delay32Ms(0, TIMMER_FONDO_BLANCO);
		adc_valX = 0, adc_valY = 0;				// Reseteo el valor de X, Y del ADC.
		WG12864A_Limpiar(NEGRO); 				// Limpio la pantalla y entro al submenu 2.2

		Func_Semaforos();						// Funcion del SubMenu seleccionado.

		break;

	case 3:
		WG12864A_posXY(1, 6);
		WG12864A_printf("2.3 Comunicaciones   ", Arial8x6, BLANCO);
		delay32Ms(0, TIMMER_FONDO_BLANCO);
		adc_valX = 0, adc_valY = 0;				// Reseteo el valor de X, Y del ADC.
		WG12864A_Limpiar(NEGRO); 				// Limpio la pantalla y entro al submenu 2.3

		Func_Direccion();						// Funcion del SubMenu seleccionado.

		break;

	default:
		break;
	}

//-------------------------------------------------------------------------------------------------------------------------------
/*******************************************  SUBMENU 3  ***********************************************************************/
//-------------------------------------------------------------------------------------------------------------------------------

	switch( menu3 ){

	case 0:
		adc_valX = 0, adc_valY = 0;				// Reseteo el valor de X, Y del ADC.
		WG12864A_posXY(1, 1);
		WG12864A_printf("    *Menu reloj*     ", Arial8x6, BLANCO);
		WG12864A_posXY(1, 2);
		WG12864A_printf("3.1 Consulta Reloj   ", Arial8x6, NEGRO);
		WG12864A_posXY(1, 4);
		WG12864A_printf("3.2 Modificar Reloj  ", Arial8x6, NEGRO);
		WG12864A_posXY(1,6);
		WG12864A_printf("                     ", Arial8x6, NEGRO);
		WG12864A_posXY(110, 7);
		WG12864A_print_symbol(BACK16x16, BLANCO);
		WG12864A_posXY(1,8);
		WG12864A_printf("                 ", Arial8x6, NEGRO);
		menu3 = 0;							//??

		// Se presionó Back?
		if( (0xCA < adc_valX) && (adc_valX < 0xEA) &&
			(0x2A < adc_valY) && (adc_valY < 0x65) ){

			WG12864A_posXY(110, 7);
			WG12864A_print_symbol(BACK16x16, NEGRO);
			delay32Ms(0, TIMMER_FONDO_BLANCO);
			adc_valX = 0, adc_valY = 0;			// Reseteo el valor de X, Y del ADC.
			menu3 = -1;							// Desabilito que entre al SubMenú.
			menu = 0;							// Habilito que entre al Menu Seleccionado.
			WG12864A_posXY(110, 7);
			WG12864A_print_symbol(BACK16x16, BLANCO);
		}

		// Consulta que menu se presionó.
		if( (0x1A < adc_valX) && (adc_valX < 0xEA) &&
			(0xAA < adc_valY) && (adc_valY < 0xC0) ){

			menu3 = 1;
		}
		if( (0x1A < adc_valX) && (adc_valX < 0xEA) &&
			(0x85 < adc_valY) && (adc_valY < 0xA0) ){

			menu3 = 2;
		}
		if( !( ((0x1A < adc_valX) && (adc_valX < 0xEA) && (0xAA < adc_valY) && (adc_valY < 0xC0)) ||
			   ((0x1A < adc_valX) && (adc_valX < 0xEA) && (0x85 < adc_valY) && (adc_valY < 0xA0)) )){

			adc_valX = 0, adc_valY = 0;				// Reseteo el valor de X, Y del ADC.
		}
		break;

//-------------------------------------------------------------------------------------------------------------------------------
//******************************************  Muestra Fecha y Hora  *************************************************************
	case 1:
		WG12864A_posXY(1, 2);
		WG12864A_printf("3.1 Consulta Reloj", Arial8x6, BLANCO);
		delay32Ms(0, TIMMER_FONDO_BLANCO);
		adc_valX = 0, adc_valY = 0;				// Reseteo el valor de X, Y del ADC.
		WG12864A_Limpiar(NEGRO); 				// Limpio la pantalla y entro al submenu 3.1

		func_muestrafyh();						// Entra en la funcion muestra fecha y hora.
		break;

//-------------------------------------------------------------------------------------------------------------------------------
//******************************************  Modifica Fecha y Hora  ************************************************************
	case 2:
		WG12864A_posXY(1, 4);
		WG12864A_printf("3.2 Modificar Reloj", Arial8x6, BLANCO);
		delay32Ms(0, TIMMER_FONDO_BLANCO);
		adc_valX = 0, adc_valY = 0;				// Reseteo el valor de X, Y del ADC.
		WG12864A_Limpiar(NEGRO); 				// Limpio la pantalla y entro al submenu 3.2

		func_modfyh();							// Entra en submenu modifica fecha y hora.
		break;

	default:
		break;
	}

//-------------------------------------------------------------------------------------------------------------------------------
/*******************************************  SUBMENU 4  ***********************************************************************/
//-------------------------------------------------------------------------------------------------------------------------------

	switch( menu4 ){

	case 0:
		adc_valX = 0, adc_valY = 0;					// Reseteo el valor de X, Y del ADC.
		WG12864A_posXY(1, 1);
		WG12864A_printf("   *Menu Especial*   ", Arial8x6, BLANCO);
		WG12864A_posXY(1,2);
		WG12864A_printf("                     ", Arial8x6, NEGRO);
		WG12864A_posXY(1, 4);
		WG12864A_printf("Ingrese Clave de           Acceso", Arial8x6, NEGRO);
		WG12864A_posXY(1,6);
		WG12864A_printf("                     ", Arial8x6, NEGRO);
		WG12864A_posXY(110, 7);
		WG12864A_print_symbol(BACK16x16, BLANCO);
		WG12864A_posXY(1, 8);
		WG12864A_printf("   * _ _ _ _ *    ", Arial8x6, NEGRO);
		menu4 = 0;								//??

		// Se presionó Back?
		if( (0xCA < adc_valX) && (adc_valX < 0xEA) &&
			(0x2A < adc_valY) && (adc_valY < 0x65) ){

			WG12864A_posXY(110, 7);
			WG12864A_print_symbol(BACK16x16, NEGRO);
			delay32Ms(0, TIMMER_FONDO_BLANCO);
			adc_valX = 0, adc_valY = 0;				// Reseteo el valor de X, Y del ADC.
			menu4 = -1;								// Desabilito que entre al SubMenú.
			menu = 0;								// Habilito que entre al Menu Seleccionado.
			WG12864A_posXY(110, 7);
			WG12864A_print_symbol(BACK16x16, BLANCO);
		}

		//Si Presionó sobre los renglones?.
		if( (0x20 < adc_valX) && (adc_valX < 0xD0) &&
			(0x35 < adc_valY) && (adc_valY < 0x50) ){

			WG12864A_posXY(1, 8);
			WG12864A_printf("  * _ _ _ _ *  ", Arial8x6, BLANCO);
			delay32Ms(0, TIMMER_FONDO_BLANCO);
			adc_valX = 0, adc_valY = 0;				// Reseteo el valor de X, Y del ADC.
			WG12864A_Limpiar(NEGRO); 				// Limpio la pantalla.
			menu4= 1;
			indice = 0;								// Preparo el indice del vector de la clave.
			cur = 76;							// Preparo el cursor para la proxima vez.
		}
		break;

	case 1:

//		adc_valX = 0, adc_valY = 0;					// Reseteo el valor de X, Y del ADC.
		var_aux = Func_MuestraTeclado(1);			// Muestra Teclado en pantalla.

		if( var_aux != -1 ){						// Valida el numero del teclado que se pulso.

			if (indice < 4){						// Si no cargo los 4 Digitos, lo carga sino limpia la clave.

				cla[indice] = var_aux;				// Carga en el vector los digitos de la clave.
				indice ++;
			}else{

				indice = 0;							// Reseteo el indice del vector de la clave.
				cur = 76;						// Acomodo el cursor para la proxima vez.
				WG12864A_posXY(cur, 2);			// Limpio la clave en pantalla.
				WG12864A_printf(" ", Arial8x6, NEGRO);
				WG12864A_posXY(88, 2);
				WG12864A_printf(" ", Arial8x6, NEGRO);
				WG12864A_posXY(100, 2);
				WG12864A_printf(" ", Arial8x6, NEGRO);
				WG12864A_posXY(112, 2);
				WG12864A_printf(" ", Arial8x6, NEGRO);
			}
		}

		// Se presionó Back?
		if( (0xCA < adc_valX) && (adc_valX < 0xEA) &&
			(0x2A < adc_valY) && (adc_valY < 0x65) ){

			WG12864A_posXY(110, 7);
			WG12864A_print_symbol(BACK16x16, NEGRO);
			delay32Ms(0, TIMMER_FONDO_BLANCO);
			adc_valX = 0, adc_valY = 0;				// Reseteo el valor de X, Y del ADC.
			menu4 = 0;								// Habilito que entre al Menu Seleccionado.
			WG12864A_Limpiar(NEGRO); 				// Limpio la pantalla y vuelvo al menu anterior.
			cur = 76;							// Acomodo el cursor para la proxima vez.
			indice = 0;								// Reseteo el indice del vector de la clave.
		}

		//Se presionó OK?
		if( (0xA0 < adc_valX) && (adc_valX < 0xB9) &&
			(0x2A < adc_valY) && (adc_valY < 0x65) ){

			WG12864A_posXY(80, 7);
			WG12864A_print_symbol(OK16x16, NEGRO);
			delay32Ms(0, TIMMER_FONDO_BLANCO);
			adc_valX = 0, adc_valY = 0;				// Reseteo el valor de X, Y del ADC.
			indice = 0;								// Reseteo el indice del vector de la clave.
			WG12864A_Limpiar(NEGRO);				// Limpio la pantalla.

			//Clave correcta?
			if( (cla[0] == clave[0]) && (cla[1] == clave[1]) &&
				(cla[2] == clave[2]) && (cla[3] == clave[3]) ){

				menu4 = -1;							// Desabilito que entre al Menu Seleccionado.
				menu41 = 0;
				cla[0] = 0, cla[1] = 0,	cla[2] = 0,	cla[3] = 0;
				indice = 0;
			}else{

				WG12864A_Limpiar(NEGRO);
				WG12864A_posXY(1, 4);
				WG12864A_printf("   Clave Incorrecta!  ", Arial8x6, NEGRO);
				delay32Ms(0, TIMMER_MUESTRO_MSJ);
				WG12864A_Limpiar(NEGRO);
				menu4 = 0;
				indice = 0;							// Reseteo el indice del vector de la clave.
				cur = 76;						// Acomodo el cursor para la proxima vez.
			}
		}
		break;

	default:
		break;
	}

//-------------------------------------------------------------------------------------------------------------------------------
/***********************************  SUBMENU 41 CLAVE OK! *********************************************************************/
//-------------------------------------------------------------------------------------------------------------------------------

	switch( menu41 ){

	case 0:
		adc_valX = 0, adc_valY = 0;					// Reseteo el valor de X, Y del ADC.
		WG12864A_posXY(1, 1);
		WG12864A_printf("   *Menu Especial*   ", Arial8x6, BLANCO);
		WG12864A_posXY(110, 7);
		WG12864A_print_symbol(BACK16x16, BLANCO);
		WG12864A_posXY(1, 2);
		WG12864A_printf("4.1 Prog por USB", Arial8x6, NEGRO);
		WG12864A_posXY(1, 4);
		WG12864A_printf("4.2 Prog por SD", Arial8x6, NEGRO);
		WG12864A_posXY(1, 6);
		WG12864A_printf("4.3 Grabar en USB", Arial8x6, NEGRO);
		WG12864A_posXY(1, 8);
		WG12864A_printf("4.4 Grabar en SD", Arial8x6, NEGRO);
		menu41=0;

		// Se presionó Back?
		if( (0xCA < adc_valX) && (adc_valX < 0xEA) &&
			(0x2A < adc_valY) && (adc_valY < 0x65) ){

			WG12864A_posXY(110, 7);
			WG12864A_print_symbol(BACK16x16, NEGRO);
			delay32Ms(0, TIMMER_FONDO_BLANCO);
			adc_valX = 0, adc_valY = 0;				// Reseteo el valor de X, Y del ADC.
			menu41 = -1;							// Desabilito que entre al SubMenú.
			menu4 = 0;								// Habilito que entre al Menu Seleccionado.
			WG12864A_posXY(110, 7);
			WG12864A_print_symbol(BACK16x16, BLANCO);
		}

		// Consulta que menu se presionó.
		if( (0x1A < adc_valX) && (adc_valX < 0xC0) &&
			(0xAA < adc_valY) && (adc_valY < 0xC0) ){

			menu41 = 1;
		}
		if( (0x1A < adc_valX) && (adc_valX < 0xC0) &&
			(0x85 < adc_valY) && (adc_valY < 0xA0) ){

			menu41 = 2;
		}
		if( (0x1A < adc_valX) && (adc_valX < 0xC0) &&
			(0x5A < adc_valY) && (adc_valY < 0x7A) ){

			menu41 = 3;
		}
		if( (0x1A < adc_valX) && (adc_valX < 0xC0) &&
			(0x3D < adc_valY) && (adc_valY < 0x50) ){

			menu41 = 4;
		}
		if( !( ((0x1A < adc_valX) && (adc_valX < 0xEA) && (0xAA < adc_valY) && (adc_valY < 0xC0)) ||
			   ((0x1A < adc_valX) && (adc_valX < 0xEA) && (0x85 < adc_valY) && (adc_valY < 0xA0)) ||
		       ((0x1A < adc_valX) && (adc_valX < 0xC0) && (0x5A < adc_valY) && (adc_valY < 0x7A)) ||
		       ((0x1A < adc_valX) && (adc_valX < 0xC0) && (0x3D < adc_valY) && (adc_valY < 0x50)) )){

			adc_valX = 0, adc_valY = 0;				// Reseteo el valor de X, Y del ADC.
		}
		break;

	case 1:
			WG12864A_posXY(1, 2);
			WG12864A_printf("4.1 Prog por USB", Arial8x6, BLANCO);
			delay32Ms(0, TIMMER_FONDO_BLANCO);
			adc_valX = 0, adc_valY = 0;				// Reseteo el valor de X, Y del ADC.
			WG12864A_Limpiar(NEGRO); 				// Limpio la pantalla.
			menu41 = -1;							// Desabilito que entre al SubMenú.
			menu41_1 = 0;							// Habilito que entre al Menu Seleccionado.
			break;

	case 2:
			WG12864A_posXY(1, 4);
			WG12864A_printf("4.2 Prog por SD", Arial8x6, BLANCO);
			delay32Ms(0, TIMMER_FONDO_BLANCO);
			adc_valX = 0, adc_valY = 0;				// Reseteo el valor de X, Y del ADC.
			WG12864A_Limpiar(NEGRO); 				// Limpio la pantalla.
			menu41 = -1;							// Desabilito que entre al SubMenú.
			menu41_2 = 0;							// Habilito que entre al Menu Seleccionado.
			break;

	case 3:
			WG12864A_posXY(1, 6);
			WG12864A_printf("4.3 Grabar en USB", Arial8x6, BLANCO);
			delay32Ms(0, TIMMER_FONDO_BLANCO);
			adc_valX = 0, adc_valY = 0;				// Reseteo el valor de X, Y del ADC.
			WG12864A_Limpiar(NEGRO); 				// Limpio la pantalla.
			menu41 = -1;							// Desabilito que entre al SubMenú.
			menu41_3 = 0;							// Habilito que entre al Menu Seleccionado.
			break;

	case 4:
			WG12864A_posXY(1, 8);
			WG12864A_printf("4.4 Grabar en SD", Arial8x6, BLANCO);
			delay32Ms(0, TIMMER_FONDO_BLANCO);
			adc_valX = 0, adc_valY = 0;				// Reseteo el valor de X, Y del ADC.
			WG12864A_Limpiar(NEGRO);				// Limpio la pantalla.
			menu41 = -1;							// Desabilito que entre al SubMenú.
			menu41_4 = 0;							// Habilito que entre al Menu Seleccionado.
			break;

	default:
			break;
	}

//-----------------------------------------------------------------------------------------------------------------------
/***********************************  SUBMENU 41.1 ***********************************************************************/
//-----------------------------------------------------------------------------------------------------------------------

	switch( menu41_1 ){

	case 0:
		adc_valX = 0, adc_valY = 0;					// Reseteo el valor de X, Y del ADC.
		WG12864A_posXY(1, 1);
		WG12864A_printf(" *Programar por USB* ", Arial8x6, BLANCO);
		WG12864A_posXY(1, 3);
		WG12864A_printf("Actualizar Firmware", Arial8x6, NEGRO);
		WG12864A_posXY(1, 5);
		WG12864A_printf("Actualizar Software", Arial8x6, NEGRO);
		WG12864A_posXY(110, 7);
		WG12864A_print_symbol(BACK16x16, BLANCO);

		// Se presionó Back?
		if( (0xCA < adc_valX) && (adc_valX < 0xEA) &&
			(0x2A < adc_valY) && (adc_valY < 0x65) ){

			WG12864A_posXY(110, 7);
			WG12864A_print_symbol(BACK16x16, NEGRO);
			delay32Ms(0, TIMMER_FONDO_BLANCO);
			adc_valX = 0, adc_valY = 0;				// Reseteo el valor de X, Y del ADC.
			menu41_1 = -1;							// Desabilito que entre al SubMenú.
			menu41 = 0;								// Habilito que entre al Menu Seleccionado.
			WG12864A_posXY(110, 7);
			WG12864A_print_symbol(BACK16x16, BLANCO);
			WG12864A_Limpiar(NEGRO); 				// Limpio la pantalla.
		}

		// Consulta que menu se presionó.
		if( (0x1A < adc_valX) && (adc_valX < 0xB0) &&
			(0x9A < adc_valY) && (adc_valY < 0xB0) ){

			menu41_1 = 1;
		}
		if( (0x1A < adc_valX) && (adc_valX < 0xB0) &&
			(0x70 < adc_valY) && (adc_valY < 0x90) ){

			menu41_1 = 2;
		}
		if( !( ((0x1A < adc_valX) && (adc_valX < 0xB0) && (0x9A < adc_valY) && (adc_valY < 0xB0)) ||
			   ((0x1A < adc_valX) && (adc_valX < 0xB0) && (0x70 < adc_valY) && (adc_valY < 0x90)) )){

			adc_valX = 0, adc_valY = 0;				// Reseteo el valor de X, Y del ADC.
		}
		break;

	case 1:
		WG12864A_posXY(1, 3);
		WG12864A_printf("Actualizar Firmware", Arial8x6, BLANCO);
		delay32Ms(0, TIMMER_FONDO_BLANCO);
		adc_valX = 0, adc_valY = 0;					// Reseteo el valor de X, Y del ADC.
		WG12864A_Limpiar(NEGRO); 					// Limpio la pantalla.

		// FUNCION.
		// HACER Q VUELVA A DONDE CORRESPONDE SEGUN LA ACCION DE LA FUNCION.
		menu41 = 0;									// Desabilito que entre al SubMenú.
		menu41_1 = -1;								// Habilito que entre al Menu Seleccionado.
		break;

	case 2:
		WG12864A_posXY(1, 5);
		WG12864A_printf("Actualizar Software", Arial8x6, BLANCO);
		delay32Ms(0, TIMMER_FONDO_BLANCO);
		adc_valX = 0, adc_valY = 0;					// Reseteo el valor de X, Y del ADC.
		WG12864A_Limpiar(NEGRO); 					// Limpio la pantalla.

		// FUNCION.
		// HACER Q VUELVA A DONDE CORRESPONDE SEGUN LA ACCION DE LA FUNCION.
		menu41_1 = -1;								// Desabilito que entre al SubMenú.
		menu41 = 0;									// Habilito que entre al Menu Seleccionado.
		break;

	default:
		break;
	}

//-----------------------------------------------------------------------------------------------------------------------
/***********************************  SUBMENU 41.2 ***********************************************************************/
//-----------------------------------------------------------------------------------------------------------------------

	switch( menu41_2 ){

	case 0:
		adc_valX = 0, adc_valY = 0;					// Reseteo el valor de X, Y del ADC.
		WG12864A_posXY(1, 1);
		WG12864A_printf(" *Programar por SD*  ", Arial8x6, BLANCO);
		WG12864A_posXY(1, 3);
		WG12864A_printf("Actualizar Firmware", Arial8x6, NEGRO);
		WG12864A_posXY(1, 5);
		WG12864A_printf("Actualizar Software", Arial8x6, NEGRO);
		WG12864A_posXY(110, 7);
		WG12864A_print_symbol(BACK16x16, BLANCO);

		// Se presionó Back?
		if( (0xCA < adc_valX) && (adc_valX < 0xEA) &&
			(0x2A < adc_valY) && (adc_valY < 0x65) ){

			WG12864A_posXY(110, 7);
			WG12864A_print_symbol(BACK16x16, NEGRO);
			delay32Ms(0, TIMMER_FONDO_BLANCO);
			adc_valX = 0, adc_valY = 0;				// Reseteo el valor de X, Y del ADC.
			menu41_2 = -1;							// Desabilito que entre al SubMenú.
			menu41 = 0;								// Habilito que entre al Menu Seleccionado.
			WG12864A_posXY(110, 7);
			WG12864A_print_symbol(BACK16x16, BLANCO);
			WG12864A_Limpiar(NEGRO); 				// Limpio la pantalla.
		}

		// Consulta que menu se presionó.
		if( (0x1A < adc_valX) && (adc_valX < 0xB0) &&
			(0x9A < adc_valY) && (adc_valY < 0xB0) ){

			menu41_2 = 1;
		}
		if( (0x1A < adc_valX) && (adc_valX < 0xB0) &&
			(0x70 < adc_valY) && (adc_valY < 0x90) ){

			menu41_2 = 2;
		}
		if( !( ((0x1A < adc_valX) && (adc_valX < 0xB0) && (0x9A < adc_valY) && (adc_valY < 0xB0)) ||
			   ((0x1A < adc_valX) && (adc_valX < 0xB0) && (0x70 < adc_valY) && (adc_valY < 0x90)) )){

			adc_valX = 0, adc_valY = 0;				// Reseteo el valor de X, Y del ADC.
		}
		break;

	case 1:
		WG12864A_posXY(1, 3);
		WG12864A_printf("Actualizar Firmware", Arial8x6, BLANCO);
		delay32Ms(0, TIMMER_FONDO_BLANCO);
		adc_valX = 0, adc_valY = 0;					// Reseteo el valor de X, Y del ADC.
		WG12864A_Limpiar(NEGRO); 					// Limpio la pantalla.

		// FUNCION.
		// HACER Q VUELVA A DONDE CORRESPONDE SEGUN LA ACCION DE LA FUNCION.
		menu41_2 = -1;								// Desabilito que entre al SubMenú.
		menu41 = 0;									// Habilito que entre al Menu Seleccionado.
		break;

	case 2:
		WG12864A_posXY(1, 5);
		WG12864A_printf("Actualizar Software", Arial8x6, BLANCO);
		delay32Ms(0, TIMMER_FONDO_BLANCO);
		adc_valX = 0, adc_valY = 0;					// Reseteo el valor de X, Y del ADC.
		WG12864A_Limpiar(NEGRO); 					// Limpio la pantalla.

		// FUNCION.
		// HACER Q VUELVA A DONDE CORRESPONDE SEGUN LA ACCION DE LA FUNCION.
		menu41_2 = -1;								// Desabilito que entre al SubMenú.
		menu41 = 0;									// Habilito que entre al Menu Seleccionado.
		break;

	default:
		break;
	}

//-----------------------------------------------------------------------------------------------------------------------
/***********************************  SUBMENU 41.3 ***********************************************************************/
//-----------------------------------------------------------------------------------------------------------------------

	switch( menu41_3 ){

	case 0:
		adc_valX = 0, adc_valY = 0;					// Reseteo el valor de X, Y del ADC.
		WG12864A_posXY(1, 1);
		WG12864A_printf("   *Grabar en USB*   ", Arial8x6, BLANCO);
		WG12864A_posXY(1, 3);
		WG12864A_printf("Grabar Errores", Arial8x6, NEGRO);
		WG12864A_posXY(1, 5);
		WG12864A_printf("Grabar Firmware", Arial8x6, NEGRO);
		WG12864A_posXY(1, 7);
		WG12864A_printf("Grabar Software", Arial8x6, NEGRO);
		WG12864A_posXY(110, 7);
		WG12864A_print_symbol(BACK16x16, BLANCO);

		// Se presionó Back?
		if( (0xCA < adc_valX) && (adc_valX < 0xEA) &&
			(0x2A < adc_valY) && (adc_valY < 0x65) ){

			WG12864A_posXY(110, 7);
			WG12864A_print_symbol(BACK16x16, NEGRO);
			delay32Ms(0, TIMMER_FONDO_BLANCO);
			adc_valX = 0, adc_valY = 0;				// Reseteo el valor de X, Y del ADC.
			menu41 = 0;								// Desabilito que entre al SubMenú.
			menu41_3 = -1;							// Habilito que entre al Menu Seleccionado.
			WG12864A_posXY(110, 7);
			WG12864A_print_symbol(BACK16x16, BLANCO);
			WG12864A_Limpiar(NEGRO); 				// Limpio la pantalla.
		}

		// Consulta que menu se presionó.
		if( (0x1A < adc_valX) && (adc_valX < 0xB0) &&
			(0x9A < adc_valY) && (adc_valY < 0xB0) ){

			menu41_3 = 1;
		}
		if( (0x1A < adc_valX) && (adc_valX < 0xB0) &&
			(0x70 < adc_valY) && (adc_valY < 0x90) ){

			menu41_3 = 2;
		}
		if( (0x1A < adc_valX) && (adc_valX < 0xB0) &&
			(0x45 < adc_valY) && (adc_valY < 0x65) ){

			menu41_3 = 3;
		}
		if( !( ((0x1A < adc_valX) && (adc_valX < 0xB0) && (0x9A < adc_valY) && (adc_valY < 0xB0)) ||
			   ((0x1A < adc_valX) && (adc_valX < 0xB0) && (0x70 < adc_valY) && (adc_valY < 0x90)) ||
			   ((0x1A < adc_valX) && (adc_valX < 0xB0) && (0x45 < adc_valY) && (adc_valY < 0x65)) )){

			adc_valX = 0, adc_valY = 0;				// Reseteo el valor de X, Y del ADC.
		}
		break;

	case 1:
		WG12864A_posXY(1, 3);
		WG12864A_printf("Grabar Errores", Arial8x6, BLANCO);
		delay32Ms(0, TIMMER_FONDO_BLANCO);
		adc_valX = 0, adc_valY = 0;					// Reseteo el valor de X, Y del ADC.
		WG12864A_Limpiar(NEGRO); 					// Limpio la pantalla.

		// FUNCION.
		// HACER Q VUELVA A DONDE CORRESPONDE SEGUN LA ACCION DE LA FUNCION.
		menu41_3 = -1;								// Desabilito que entre al SubMenú.
		menu41 = 0;									// Habilito que entre al Menu Seleccionado.
		break;

	case 2:
		WG12864A_posXY(1, 5);
		WG12864A_printf("Grabar Firmware", Arial8x6, BLANCO);
		delay32Ms(0, TIMMER_FONDO_BLANCO);
		adc_valX = 0, adc_valY = 0;					// Reseteo el valor de X, Y del ADC.
		WG12864A_Limpiar(NEGRO); 					// Limpio la pantalla.

		// FUNCION.
		// HACER Q VUELVA A DONDE CORRESPONDE SEGUN LA ACCION DE LA FUNCION.
		menu41_3 = -1;								// Desabilito que entre al SubMenú.
		menu41 = 0;									// Habilito que entre al Menu Seleccionado.
		break;

	case 3:
		WG12864A_posXY(1, 7);
		WG12864A_printf("Grabar Software", Arial8x6, BLANCO);
		delay32Ms(0, TIMMER_FONDO_BLANCO);
		adc_valX = 0, adc_valY = 0;					// Reseteo el valor de X, Y del ADC.
		WG12864A_Limpiar(NEGRO); 					// Limpio la pantalla.

		// FUNCION.
		// HACER Q VUELVA A DONDE CORRESPONDE SEGUN LA ACCION DE LA FUNCION.
		menu41_3 = -1;								// Desabilito que entre al SubMenú.
		menu41 = 0;									// Habilito que entre al Menu Seleccionado.
		break;

	default:
		break;
	}

//-----------------------------------------------------------------------------------------------------------------------
/***********************************  SUBMENU 41.4 ***********************************************************************/
//-----------------------------------------------------------------------------------------------------------------------

	switch( menu41_4 ){

	case 0:
		adc_valX = 0, adc_valY = 0;					// Reseteo el valor de X, Y del ADC.
		WG12864A_posXY(1, 1);
		WG12864A_printf("   *Grabar en SD*    ", Arial8x6, BLANCO);
		WG12864A_posXY(1, 3);
		WG12864A_printf("Grabar Errores", Arial8x6, NEGRO);
		WG12864A_posXY(1, 5);
		WG12864A_printf("Grabar Firmware", Arial8x6, NEGRO);
		WG12864A_posXY(1, 7);
		WG12864A_printf("Grabar Software", Arial8x6, NEGRO);
		WG12864A_posXY(110, 7);
		WG12864A_print_symbol(BACK16x16, BLANCO);

		// Se presionó Back?
		if( (0xCA < adc_valX) && (adc_valX < 0xEA) &&
			(0x2A < adc_valY) && (adc_valY < 0x65) ){

			WG12864A_posXY(110, 7);
			WG12864A_print_symbol(BACK16x16, NEGRO);
			delay32Ms(0, TIMMER_FONDO_BLANCO);
			adc_valX = 0, adc_valY = 0;				// Reseteo el valor de X, Y del ADC.
			menu41_4 = -1;							// Desabilito que entre al SubMenú.
			menu41 = 0;								// Habilito que entre al Menu Seleccionado.
			WG12864A_posXY(110, 7);
			WG12864A_print_symbol(BACK16x16, BLANCO);
			WG12864A_Limpiar(NEGRO); 				// Limpio la pantalla.
		}

		// Consulta que menu se presionó.
		if( (0x1A < adc_valX) && (adc_valX < 0xB0) &&
			(0x9A < adc_valY) && (adc_valY < 0xB0) ){

			menu41_4 = 1;
		}
		if( (0x1A < adc_valX) && (adc_valX < 0xB0) &&
			(0x70 < adc_valY) && (adc_valY < 0x90) ){

			menu41_4 = 2;
		}
		if( (0x1A < adc_valX) && (adc_valX < 0xB0) &&
			(0x45 < adc_valY) && (adc_valY < 0x65) ){

			menu41_4 = 3;
		}
		if( !( ((0x1A < adc_valX) && (adc_valX < 0xB0) && (0x9A < adc_valY) && (adc_valY < 0xB0)) ||
			   ((0x1A < adc_valX) && (adc_valX < 0xB0) && (0x70 < adc_valY) && (adc_valY < 0x90)) ||
			   ((0x1A < adc_valX) && (adc_valX < 0xB0) && (0x45 < adc_valY) && (adc_valY < 0x65)) )){

			adc_valX = 0, adc_valY = 0;				// Reseteo el valor de X, Y del ADC.
		}
		break;

	case 1:
		WG12864A_posXY(1, 3);
		WG12864A_printf("Grabar Errores", Arial8x6, BLANCO);
		delay32Ms(0, TIMMER_FONDO_BLANCO);
		adc_valX = 0, adc_valY = 0;					// Reseteo el valor de X, Y del ADC.
		WG12864A_Limpiar(NEGRO); 					// Limpio la pantalla.

		// FUNCION.
		// HACER Q VUELVA A DONDE CORRESPONDE SEGUN LA ACCION DE LA FUNCION.
		menu41_4 = -1;								// Desabilito que entre al SubMenú.
		menu41 = 0;									// Habilito que entre al Menu Seleccionado.
		break;

	case 2:
		WG12864A_posXY(1, 5);
		WG12864A_printf("Grabar Firmware", Arial8x6, BLANCO);
		delay32Ms(0, TIMMER_FONDO_BLANCO);
		adc_valX = 0, adc_valY = 0;					// Reseteo el valor de X, Y del ADC.
		WG12864A_Limpiar(NEGRO); 					// Limpio la pantalla.

		// FUNCION.
		// HACER Q VUELVA A DONDE CORRESPONDE SEGUN LA ACCION DE LA FUNCION.
		menu41_4 = -1;								// Desabilito que entre al SubMenú.
		menu41 = 0;									// Habilito que entre al Menu Seleccionado.
		break;

	case 3:
		WG12864A_posXY(1, 7);
		WG12864A_printf("Grabar Software", Arial8x6, BLANCO);
		delay32Ms(0, TIMMER_FONDO_BLANCO);
		adc_valX = 0, adc_valY = 0;					// Reseteo el valor de X, Y del ADC.
		WG12864A_Limpiar(NEGRO); 					// Limpio la pantalla.

		// FUNCION.
		// HACER Q VUELVA A DONDE CORRESPONDE SEGUN LA ACCION DE LA FUNCION.
		menu41_4 = -1;								// Desabilito que entre al SubMenú.
		menu41 = 0;									// Habilito que entre al Menu Seleccionado.
		break;

	default:
		break;
	}
}
