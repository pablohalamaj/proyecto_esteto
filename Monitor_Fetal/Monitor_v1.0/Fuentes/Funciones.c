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

#include "Menu.h"
#include "Touch.h"
#include "timer32.h"
#include "WG12864A.h"
#include "Funciones.h"
#include "Subfunciones.h"
#include "Definiciones.h"

// *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
extern void func_teclado(void);
//--------------------------------------------------------------------------
//***********************  Variables Externas  *****************************

extern unsigned int cur, 			// Posicion del cursor dentro de la pantalla.
					indice,			// Variable dinamica para moverse por el vector de la clave ingresada.
					sleep,			// Variable que a cierto valor duerme la pantalla.
					flagsleep ,		// Variable que me indica si el Display esta dormido para despertarlo.
					flagirq ;		// Bandera que me indica si habilitar o no la IRQ del TOUCH.

extern char			flagmm,flagbll,cantver,cantprt,version[],version_prt[],no_recibo,queenv,
					causaerr[],causaerror[],moduloerr[],sumaerr,lec;
extern int 			h,mi,seg,d,m,anio,ds,
					proxf[],proxh[],
					var_aux;

//--------------------------------------------------------------------------
//***********************  Variables Propias  ******************************

int					norecarga=0,muefunc=0;
char				menuactual,toca,toca1,modo_func,actualiza_fw=0,
					t,pressok,prs,velc,dir_comu,
					flagesp=0,salir=0,mod=0;
char pos_x=25,pos_y,PUNTO,BORRA,num_pulso,hab_corazon,FILTRO_DIG=0;
unsigned long int v=0,ha=0;
unsigned int 		cont_tra=0,tcla[4],
					adc_valX, adc_valY,	// Variables GLOBALES con los valores de posicion en X e Y.
					menu, menu1, menu2,	// Variables GLOBALES para posicionamiento en todo el Menú.
					menu3,	menu32, menu4,
					menu41, menu41_1,menu41_2,
					menu41_3, menu41_4,
					tit = 0,			// Variables para hacer titilante el cursor al ingresar datos del teclado numerico.
					num = -1,			// Variable para capturar el numero del teclado en pantalla.
					cla[4],				// Variable para capturar el numero del teclado en pantalla.
					sleepmenu,			// Variable que me indica con que menu vuelvo del Sleep en la funcion Sleep.
					flag_sleepsubmenu = 1,// Bandera que me indica si se durmio en un SubMenu y evita el codigo del boton Back.
					varmod=0,backmod=0,s=0,tcl;
static char  	buffer_pulso[44] ;

// *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
void TIMER32_1_IRQHandler(void)
{
	// La interrupcion la genero el MRO??
	if( LPC_TMR32B1->IR & 0x01 ){

		LPC_TMR32B1->IR = 1;					// Reseteo el Flag de IRQ.

		if( sleep == 0 ){

			flagsleep = 1;						// Activo la Bandera de que el Display duerme para en la IRQ despertarlo.
			GLCD_Output_Low(BACKLIGHT);			// Apaga el display
			TOUCH_Standby();					// Standby, en espera de si activacion.
			flagirq = 0;						// Bandera que me indica si habilitar o no la IRQ del TOUCH.
		}

		if( sleep == MULTIPLICADOR_TIMMER_SLEEP ){

			flagirq = 0;						// Bandera que me indica si habilitar o no la IRQ del TOUCH.
			disable_timer32(1); 				// Desabilito el timmer porque ya se durmio el Display.
			cla[0] = 0, cla[1] = 0,	cla[2] = 0, cla[3] = 0;	// Limpio la clave del ingreso al menu especial.
			indice = 0;							// Acomodo el indicel del vector de clave al inicio.
			cur = 76;						// Preparo el cursor para la proxima vez.
		}

		sleep ++;								// Incremento la variable que duerme la pantalla.
	}
}

// *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
void Func_Sleep(int opcion_1, int opcion_2 )
{
	int i;
	switch( opcion_1 ){

		case 0:

			GPIOIntClear(PORT1, 2);				// Limpio la Interrupcion.
			for(i=0; i == 10; i ++){;}			// Pequeña demora (Ciclos de Clock para que baje la Bandera).
			GPIOIntEnable(PORT1, 2);			// Habilito la Interrupcion.
			flagirq = -1;						// Bandera que me indica si habilitar o no la IRQ del TOUCH.
			menu = menu, menu1 = menu1, menu2 = menu2, menu3 = menu3,
			menu32 = menu32, menu4 = menu4, menu41 = menu41, menu41_1 = menu41_1,
			menu41_2 = menu41_2, menu41_3 = menu41_3, menu41_4 = menu41_4;
			break;

		default:
			break;
	}

	switch( opcion_2 ){							// Si es Mayor a 5 reinicio por completo, sino solo prendo la pantalla.

		case 0:

			WG12864A_Limpiar(NEGRO); 			// Limpio la pantalla de lo que tenia.
			Menu_Logo();						// Presento el Logo de la Empresa.
			WG12864A_Limpiar(NEGRO); 			// Limpio el logo de la pantalla.
			sleepmenu = -1;						// Seleccion con que modalidad vuelve del Sleep.
			adc_valX = 0, adc_valY = 0;			// Reseteo el valor de X, Y del ADC.
			break;

		case 1:

			WG12864A_Limpiar(NEGRO); 			// Limpio la pantalla de lo que tenia.
			Menu_Logo();						// Presento el Logo de la Empresa.
			WG12864A_Limpiar(NEGRO); 			// Limpio el logo de la pantalla.
			sleepmenu = -1;						// Seleccion con que modalidad vuelve del Sleep.
			flag_sleepsubmenu = 0;				// Aviso por medio de esta bandera que vuelve al menu principal por medio de Sleep.

			// Si me encuentro en un SubMenu le paso a X e Y las coordenadas del BACK y acomodo el menu.
			if( (menu2 >= 1 || menu2 <= 3) ||
				(menu3 >= 1 || menu3 <= 2) ){

				adc_valX = 0xDF, adc_valY = 0x40;				// Selecciono las coordenadas del BACK.
				menu = 0, menu1 = -1, menu2 = -1, menu3 = -1,
				menu32 = -1, menu4 = -1, menu41 = -1, menu41_1 = -1,
				menu41_2 = -1, menu41_3 = -1, menu41_4 = -1;
			}else{

				adc_valX = 0, adc_valY = 0;						// Reseteo el valor de X, Y del ADC.
				menu = 0, menu1 = -1, menu2 = -1, menu3 = -1,
				menu32 = -1, menu4 = -1, menu41 = -1, menu41_1 = -1,
				menu41_2 = -1, menu41_3 = -1, menu41_4 = -1;
			}
			break;
	}
	if(flagbll)													// Flag buffer lleno
	{
		flagbll=0;
		leobuffer(flagmm);										// Leo buffer de recepción
	}
}

// *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
uint32_t Func_MuestraTeclado(int opcion)
{
	num = -1;
	// Muestra Teclado Numerico.
	WG12864A_posXY(1, 1);
	WG12864A_printf(" 1   2   3 ", Arial8x6, NEGRO);
	WG12864A_posXY(1, 3);
	WG12864A_printf(" 4   5   6 ", Arial8x6, NEGRO);
	WG12864A_posXY(1, 5);
	WG12864A_printf(" 7   8   9 ", Arial8x6, NEGRO);
	WG12864A_posXY(1, 7);
	WG12864A_printf(" *   0   # ", Arial8x6, NEGRO);
	WG12864A_posXY(70, 2);
	WG12864A_printf("*", Arial8x6, NEGRO);

	switch (opcion){
	case 1:
			WG12864A_posXY(83, 1);
			WG12864A_printf("Clave", Arial8x6, NEGRO);
			WG12864A_posXY(118, 2);
			WG12864A_printf("*", Arial8x6, NEGRO);
			break;
	case 2:
			WG12864A_posXY(83, 1);
			WG12864A_printf("Fecha", Arial8x6, NEGRO);
			WG12864A_posXY(94, 2);
			WG12864A_printf("*", Arial8x6, NEGRO);
			break;
	case 3:
			WG12864A_posXY(83, 1);
			WG12864A_printf("Mes  ", Arial8x6, NEGRO);
			WG12864A_posXY(94, 2);
			WG12864A_printf("*", Arial8x6, NEGRO);
			break;
	case 4:
			WG12864A_posXY(83, 1);
			WG12864A_printf("A;o  ", Arial8x6, NEGRO);
			WG12864A_posXY(118, 2);
			WG12864A_printf("*", Arial8x6, NEGRO);
			break;
	case 5:
			WG12864A_posXY(83, 1);
			WG12864A_printf("Hora ", Arial8x6, NEGRO);
			WG12864A_posXY(94, 2);
			WG12864A_printf("*", Arial8x6, NEGRO);
			break;
	case 6:
			WG12864A_posXY(83, 1);
			WG12864A_printf("Minutos", Arial8x6, NEGRO);
			WG12864A_posXY(94, 2);
			WG12864A_printf("*", Arial8x6, NEGRO);
			break;
	case 7:
		    WG12864A_posXY(83, 1);
			WG12864A_printf("Dia   ", Arial8x6, NEGRO);
			WG12864A_posXY(82, 2);
			WG12864A_printf("*      ", Arial8x6, NEGRO);
			break;
	default:
			break;
	}

	WG12864A_posXY(80, 4);							// Muestra Flecha Izq.
	WG12864A_print_symbol(LeftArrow16x16, BLANCO);
	WG12864A_posXY(110, 4);							// Muestra Flecha Der.
	WG12864A_print_symbol(RightArrow16x16, BLANCO);
	WG12864A_posXY(80, 7);							// Muestra Boton Ok.
	WG12864A_print_symbol(OK16x16, BLANCO);
	WG12864A_posXY(110, 7);							// Muestra Boton Back.
	WG12864A_print_symbol(BACK16x16, BLANCO);

	// Se Presionó Flecha Izquerda?
	if( (0x95 < adc_valX) && (adc_valX < 0xB5) &&
		(0x70 < adc_valY) && (adc_valY < 0x9A) ){

		WG12864A_posXY(80, 4);						// Animacion del dibujo.
		WG12864A_print_symbol(LeftArrow16x16, NEGRO);
		delay32Ms(0, TIMMER_FONDO_BLANCO_BOTONES);
		adc_valX = 0, adc_valY = 0;					// Reseteo el valor de X, Y del ADC.
		WG12864A_posXY(cur, 2);					// Presento el cursor donde corresponde.
		WG12864A_printf(" ", Arial8x6, NEGRO);
		cur -= 12;								// Corro el cursor 1 lugar.
		norecarga=0;
		indice --;									// Decremento el Indice del vector que guarda la clave ingresada.

		// Acomodo el cursor dentro de los 4 casilleros de la clave a ingresar y el indice del vector de la clave.
		if( cur < 76 ){

			cur = 112;
			indice = 3;
		}
	}

	// Se Presionó Flecha Dererecha?
	if( (0xC5 < adc_valX) && (adc_valX < 0xE5) &&
		(0x70 < adc_valY) && (adc_valY < 0x9A) ){

		WG12864A_posXY(110, 4);						// Animacion del dibujo.
		WG12864A_print_symbol(RightArrow16x16, NEGRO);
		delay32Ms(0, TIMMER_FONDO_BLANCO_BOTONES);
		adc_valX = 0, adc_valY = 0;					// Reseteo el valor de X, Y del ADC.
		WG12864A_posXY(cur, 2);					// Presento el cursor donde corresponde.
		WG12864A_printf(" ", Arial8x6, NEGRO);
		cur += 12;								// Corro el cursor 1 lugar.
		norecarga=0;
		indice ++;									// Incremento el Indice del vector que guarda la clave ingresada.

		// Acomodo el cursor dentro de los 4 casilleros de la clave a ingresar y el indice del vector de la clave.
		if( cur > 112 ){

			cur = 76;
			norecarga=0;
			indice = 0;
		}
	}

	delay32Ms(0, TIMMER_TIT_CURSOR);				// Demora para que titile el cursor.
	WG12864A_posXY(cur, 2);

	if( tit )	{									// Cursor titilante.

		WG12864A_printf("_", Arial8x6, NEGRO);
		tit = 0;

	}else{

		WG12864A_printf(" ", Arial8x6, NEGRO);
		tit = 1;
	}

	// Se presionó 1?
	if( (0x10 < adc_valX) && (adc_valX < 0x35) &&
		(0xB5 < adc_valY) && (adc_valY < 0xC9) ){

		WG12864A_posXY(1, 1);
		WG12864A_printf(" 1 ", Arial8x6, BLANCO);
		delay32Ms(0, TIMMER_FONDO_BLANCO);
		adc_valX = 0, adc_valY = 0;					// Reseteo el valor de X, Y del ADC.
		WG12864A_posXY(cur, 2);
		WG12864A_printf("1", Arial8x6, NEGRO);
		cur += 12;								// Corro el cursor 1 lugar.
		norecarga=0;
		num = 1;									// Guardo el numero seleccionado para que la funcion lo devuelva.
	}

	// Se presionó 2?
	if( (0x40 < adc_valX) && (adc_valX < 0x55) &&
		(0xB5 < adc_valY) && (adc_valY < 0xC9) ){

		WG12864A_posXY(24, 1);
		WG12864A_printf(" 2 ", Arial8x6, BLANCO);
		delay32Ms(0, TIMMER_FONDO_BLANCO);
		adc_valX = 0, adc_valY = 0;
		WG12864A_posXY(cur, 2);
		WG12864A_printf("2", Arial8x6, NEGRO);
		cur += 12;
		norecarga=0;
		num = 2;
	}

	// Se presionó 3?
	if( (0x65 < adc_valX) && (adc_valX < 0x85) &&
		(0xB5 < adc_valY) && (adc_valY < 0xC9) ){

		WG12864A_posXY(48, 1);
		WG12864A_printf(" 3 ", Arial8x6, BLANCO);
		delay32Ms(0, TIMMER_FONDO_BLANCO);
		adc_valX = 0, adc_valY = 0;
		WG12864A_posXY(cur, 2);
		WG12864A_printf("3", Arial8x6, NEGRO);
		cur += 12;
		norecarga=0;
		num = 3;
	}

	// Se presionó 4?
	if( (0x10 < adc_valX) && (adc_valX < 0x35) &&
		(0x90 < adc_valY) && (adc_valY < 0xA9) ){

		WG12864A_posXY(1, 3);
		WG12864A_printf(" 4 ", Arial8x6, BLANCO);
		delay32Ms(0, TIMMER_FONDO_BLANCO);
		adc_valX = 0, adc_valY = 0;
		WG12864A_posXY(cur, 2);
		WG12864A_printf("4", Arial8x6, NEGRO);
		cur += 12;
		norecarga=0;
		num = 4;
	}

	// Se presionó 5?
	if( (0x40 < adc_valX) && (adc_valX < 0x55) &&
		(0x90 < adc_valY) && (adc_valY < 0xA9) ){

		WG12864A_posXY(24, 3);
		WG12864A_printf(" 5 ", Arial8x6, BLANCO);
		delay32Ms(0, TIMMER_FONDO_BLANCO);
		adc_valX = 0, adc_valY = 0;
		WG12864A_posXY(cur, 2);
		WG12864A_printf("5", Arial8x6, NEGRO);
		cur += 12;
		norecarga=0;
		num = 5;
	}

	// Se presionó 6?
	if( (0x65 < adc_valX) && (adc_valX < 0x85) &&
		(0x90 < adc_valY) && (adc_valY < 0xA9) ){

		WG12864A_posXY(48, 3);
		WG12864A_printf(" 6 ", Arial8x6, BLANCO);
		delay32Ms(0, TIMMER_FONDO_BLANCO);
		adc_valX = 0, adc_valY = 0;
		WG12864A_posXY(cur, 2);
		WG12864A_printf("6", Arial8x6, NEGRO);
		cur += 12;
		norecarga=0;
		num = 6;
	}

	// Se presionó 7?
	if( (0x10 < adc_valX) && (adc_valX < 0x35) &&
		(0x65 < adc_valY) && (adc_valY < 0x85) ){

		WG12864A_posXY(1, 5);
		WG12864A_printf(" 7 ", Arial8x6, BLANCO);
		delay32Ms(0, TIMMER_FONDO_BLANCO);
		adc_valX = 0, adc_valY = 0;
		WG12864A_posXY(cur, 2);
		WG12864A_printf("7", Arial8x6, NEGRO);
		cur += 12;
		norecarga=0;
		num = 7;
	}

	// Se presionó 8?
	if( (0x40 < adc_valX) && (adc_valX < 0x55) &&
		(0x65 < adc_valY) && (adc_valY < 0x85) ){

		WG12864A_posXY(24, 5);
		WG12864A_printf(" 8 ", Arial8x6, BLANCO);
		delay32Ms(0, TIMMER_FONDO_BLANCO);
		adc_valX = 0, adc_valY = 0;
		WG12864A_posXY(cur, 2);
		WG12864A_printf("8", Arial8x6, NEGRO);
		cur += 12;
		norecarga=0;
		num = 8;
	}

	// Se presionó 9?
	if( (0x65 < adc_valX) && (adc_valX < 0x85) &&
		(0x65 < adc_valY) && (adc_valY < 0x85) ){

		WG12864A_posXY(48, 5);
		WG12864A_printf(" 9 ", Arial8x6, BLANCO);
		delay32Ms(0, TIMMER_FONDO_BLANCO);
		adc_valX = 0, adc_valY = 0;
		WG12864A_posXY(cur, 2);
		WG12864A_printf("9", Arial8x6, NEGRO);
		cur += 12;
		norecarga=0;
		num = 9;
	}

	// Se presionó *?
	if( (0x10 < adc_valX) && (adc_valX < 0x35) &&
		(0x40 < adc_valY) && (adc_valY < 0x55) ){

		WG12864A_posXY(1, 7);
		WG12864A_printf(" * ", Arial8x6, BLANCO);
		delay32Ms(0, TIMMER_FONDO_BLANCO);
		adc_valX = 0, adc_valY = 0;
		WG12864A_posXY(cur, 2);
		WG12864A_printf("*", Arial8x6, NEGRO);
		cur += 12;
		norecarga=0;
		num = '*';
	}

	// Se presionó 0?
	if( (0x40 < adc_valX) && (adc_valX < 0x55) &&
		(0x40 < adc_valY) && (adc_valY < 0x55) ){

		WG12864A_posXY(24, 7);
		WG12864A_printf(" 0 ", Arial8x6, BLANCO);
		delay32Ms(0, TIMMER_FONDO_BLANCO);
		adc_valX = 0, adc_valY = 0;
		WG12864A_posXY(cur, 2);
		WG12864A_printf("0", Arial8x6, NEGRO);
		cur += 12;
		norecarga=0;
		num = 0;
	}

	// Se presionó #?
	if( (0x65 < adc_valX) && (adc_valX < 0x85) &&
		(0x40 < adc_valY) && (adc_valY < 0x55) ){

		WG12864A_posXY(48, 7);
		WG12864A_printf(" # ", Arial8x6, BLANCO);
		delay32Ms(0, TIMMER_FONDO_BLANCO);
		adc_valX = 0, adc_valY = 0;
		WG12864A_posXY(cur, 2);
		WG12864A_printf("#", Arial8x6, NEGRO);
		cur += 12;
		norecarga=0;
		num = '#';
	}

	return (num);									// La funcion devuelve el numero seleccionado.
}

//--------------------------------------------------------------------------
/****************************  Submenues 1  ********************************/
//--------------------------------------------------------------------------

//						  1.1 Consulta de estados
//--------------------------------------------------------------------------
void func_consultaestados(void)
{
	int fila=6,z=0;
	while(!((0xD0 < adc_valX) &&(adc_valX < 0xEA) &&
			(0x2A < adc_valY) && (adc_valY < 0x5A))) // Mientras no se presione back
	{
		switch(toca)											// Case para definir el menu actual en la recepción
		{
		case 0:
			menuactual=fyh;
			break;
		case 1:
			menuactual=ests;
			break;
		case 2:
			menuactual=det;
			break;
		case 3:
			menuactual=modp;
			break;
		}
		toca++;
		if(toca==4)
			toca=0;
//--------------------------------------------------------------------------
		WG12864A_posXY(1,1);
		WG12864A_printf("  Autotrol - CT800c  ", Arial8x6, BLANCO);
		muestra_fecha_y_hora (2);
//		muestra_movs (3);													// Muestra mov por programa
		muestra_estados (3);
		muestra_modpot();
//--------------------------------------------------------------------------
		WG12864A_posXY(1,7);
		WG12864A_printf("1", Arial8x6, BLANCO);
		WG12864A_posXY(27,7);
		WG12864A_printf("2", Arial8x6, BLANCO);
		WG12864A_posXY(53,7);
		WG12864A_printf("3", Arial8x6, BLANCO);
		WG12864A_posXY(79,7);
		WG12864A_printf("4", Arial8x6, BLANCO);
		WG12864A_posXY(1,8);
		WG12864A_printf("5", Arial8x6, BLANCO);
		WG12864A_posXY(27,8);
		WG12864A_printf("6", Arial8x6, BLANCO);
		WG12864A_posXY(53,8);
		WG12864A_printf("7", Arial8x6, BLANCO);
		WG12864A_posXY(79,8);
		WG12864A_printf("8", Arial8x6, BLANCO);
		WG12864A_posXY(86, 5);							// Muestra Flecha Izq.
		WG12864A_print_symbol(LeftArrow16x16, BLANCO);
		WG12864A_posXY(110, 5);							// Muestra Flecha Der.
		WG12864A_print_symbol(RightArrow16x16, BLANCO);

		WG12864A_posXY(110,7);
		WG12864A_print_symbol(BACK16x16, BLANCO);
//--------------------------------------------------------------------------
/*		if(acti)															// Si se activo algun conflico lo muestra
		{
			Muestra_causa(modu, cau,fila);
		}
		if(CT800C_Read_Pin_IO(LL_TIT))										// Borra causa y los flags al activar llave tit
		{
			WG12864A_posXY(1,6);
			WG12864A_printf("               ", Arial8x6, NEGRO);
			modu=10;
			cau=0;
			acti=0;
			conflrv=0;
		}
*/

// Se Presionó Flecha Izquerda?
if( (0xA0 < adc_valX) && (adc_valX < 0xC0) &&
	(0x60 < adc_valY) && (adc_valY < 0x8A) )
{
	WG12864A_posXY(86, 5);						// Animacion del dibujo.
	WG12864A_print_symbol(LeftArrow16x16, NEGRO);
	delay32Ms(0, TIMMER_FONDO_BLANCO_BOTONES);
	adc_valX = 0, adc_valY = 0;					// Reseteo el valor de X, Y del ADC.
	muefunc--;
	if(muefunc<0)
		muefunc=3;
}

// Se Presionó Flecha Dererecha?
if( (0xC5 < adc_valX) && (adc_valX < 0xE5) &&
	(0x60 < adc_valY) && (adc_valY < 0x8A) )
{

	WG12864A_posXY(110, 5);						// Animacion del dibujo.
	WG12864A_print_symbol(RightArrow16x16, NEGRO);
	delay32Ms(0, TIMMER_FONDO_BLANCO_BOTONES);
	adc_valX = 0, adc_valY = 0;					// Reseteo el valor de X, Y del ADC.
	muefunc++;
	if(muefunc>3)
		muefunc=0;
}
	switch (muefunc)
	{
	case 0:
		muestra_comunic();
		break;
	case 1:
		muestra_det();
		break;
	case 2:
		muestra_dem();
		break;
	case 3:
		muestra_tens();
		break;
	default:
		break;
	}
		Func_Sleep (flagirq, sleepmenu);
	}
	if(flag_sleepsubmenu)
	{					// Si la bandera esta arriba salio del menu normal, sino salio por sleep.

		WG12864A_posXY(110, 7);
		WG12864A_print_symbol(BACK16x16, NEGRO);
		delay32Ms(0, TIMMER_FONDO_BLANCO);	// Para demorar su utiliza vTaskDelay()
		adc_valX = 0, adc_valY = 0;			// Reseteo el valor de X, Y del ADC.
		WG12864A_Limpiar(NEGRO);
		menu1 = 0;
	}

	flag_sleepsubmenu = 1;						// Restablesco el valor original de la bandera.

}
//						  1.2 Consulta de Parametros
//--------------------------------------------------------------------------
void func_consultaptros()
{
	char t,colu;

	while(!((0xD0 < adc_valX) &&(adc_valX < 0xEA) && (0x2A < adc_valY) && (adc_valY < 0x5A))) // Mientras no se presione back
	{
		menuactual=consp;
		WG12864A_posXY(1, 1);
		WG12864A_printf("*Consulta Parametros*", Arial8x6, BLANCO);
		WG12864A_posXY(1,3);
		WG12864A_printf(" Autotrol  CT800c", Arial8x6, NEGRO);
		WG12864A_posXY(1,5);
		WG12864A_printf(" fw: ", Arial8x6, NEGRO);
		colu=31;
		for(t=0;t<cantver;t++)
		{
			WG12864A_posXY(colu,5);
			WG12864A_printf(&version[t], Arial8x6, NEGRO);
			colu+=6;
		}
		WG12864A_posXY(1,6);
		WG12864A_printf(" sw:", Arial8x6, NEGRO);
		colu=31;
		for(t=0;t<cantprt;t++)
		{
			WG12864A_posXY(colu,6);
			WG12864A_printf(&version_prt[t], Arial8x6, NEGRO);
			colu+=6;
		}
		WG12864A_posXY(1,7);
		WG12864A_printf(" pr:", Arial8x6, NEGRO);
		switch (prs)
		{
		case 1:
				WG12864A_posXY(31,7);
				WG12864A_printf("Flash ext", Arial8x6, NEGRO);
				break;
		case 2:
				WG12864A_posXY(31,7);
				WG12864A_printf("SD-MMC PC", Arial8x6, NEGRO);
				break;
		case 3:
				WG12864A_posXY(31,7);
				WG12864A_printf("SD-MMC CT", Arial8x6, NEGRO);
				break;
		case 4:
				WG12864A_posXY(31,7);
				WG12864A_printf("Sin Prtrs", Arial8x6, NEGRO);
				break;

		default:
//			WG12864A_posXY(31,7);
//			WG12864A_printf("SD-MMC PC", Arial8x6, NEGRO);
			break;
		}

		WG12864A_posXY(110, 7);
		WG12864A_print_symbol(BACK16x16, BLANCO);

		Func_Sleep (flagirq, sleepmenu);
	}
	if(flag_sleepsubmenu)
	{					// Si la bandera esta arriba salio del menu normal, sino salio por sleep.

		WG12864A_posXY(110, 7);
		WG12864A_print_symbol(BACK16x16, NEGRO);
		delay32Ms(0, TIMMER_FONDO_BLANCO);	// Para demorar su utiliza vTaskDelay()
		adc_valX = 0, adc_valY = 0;			// Reseteo el valor de X, Y del ADC.
		WG12864A_Limpiar(NEGRO);
		menu1 = 0;
	}

	flag_sleepsubmenu = 1;						// Restablesco el valor original de la bandera.
}

//						  1.3 Consulta de errores
//--------------------------------------------------------------------------
void func_conserr()
{
	int fila=7;
	sumaerr=0;
	causaerr[0]=sinerr;														//Primer ingreso hasta que lea los datos recibidos
	while(!((0xD0 < adc_valX) &&(adc_valX < 0xEA) && (0x2A < adc_valY) && (adc_valY < 0x5A))) // Mientras no se presione back
	{
		switch (toca1)
		{
		case 0:
			menuactual=errs;
			break;
		case 1:
			queenv=flecha;
			no_recibo=1;
			//menuactual=cauerrs;
			break;
		}
		toca1++;
		if(toca1==2)
			toca1=0;
		if(causaerr[0]!=sinerr)
		{
			if(causaerr[0]!=97)												//if(sumaerr<lec)// Si recorre el buffer con err almacenados o ya los leyó todos
			{
				WG12864A_posXY(1, 1);										// Muestra los errores en Display
				WG12864A_printf(" *Consulta errores*  ", Arial8x6, BLANCO);
				muestra_fyh_err (3);
				WG12864A_posXY(1,5);
				WG12864A_printf("Error:", Arial8x6, NEGRO);
				WG12864A_posXY(1,6);
				WG12864A_printf("Cod:", Arial8x6, NEGRO);
				causaerror[sumaerr]=causaerr[11];
				moduloerr[sumaerr]=causaerr[12];
				if(causaerror[sumaerr]==0)
				{
					moduloerr[sumaerr]=10;
				}
				Muestra_causa(moduloerr[sumaerr], causaerror[sumaerr],fila);

			}
			else
			{
				WG12864A_posXY(1, 1);										// Se leyeron todos los err del buffer
				WG12864A_printf(" *Consulta errores*  ", Arial8x6, BLANCO);
				WG12864A_posXY(1,3);
				WG12864A_printf("                     ", Arial8x6, NEGRO);
				WG12864A_posXY(1,5);
				WG12864A_printf("Ultimo error", Arial8x6, NEGRO);
				WG12864A_posXY(1,6);
				WG12864A_printf("            ", Arial8x6, NEGRO);
				WG12864A_posXY(1,7);
				WG12864A_printf("                ", Arial8x6, NEGRO);
				WG12864A_posXY(110, 7);
				WG12864A_print_symbol(BACK16x16, BLANCO);
			}
			WG12864A_posXY(80, 5);
			WG12864A_print_symbol(LeftArrow16x16, BLANCO);
			WG12864A_posXY(110, 5);
			WG12864A_print_symbol(RightArrow16x16, BLANCO);
			WG12864A_posXY(110, 7);
			WG12864A_print_symbol(BACK16x16, BLANCO);
			if((0x9A < adc_valX) &&(adc_valX < 0xBA) && (0x60 < adc_valY) && (adc_valY < 0x80))	// Se presionó LeftArrow?
			{
				WG12864A_posXY(80, 5);
				WG12864A_print_symbol(LeftArrow16x16, NEGRO);
				delay32Ms(0, TIMMER_FONDO_BLANCO);	// Para demorar su utiliza vTaskDelay()
				adc_valX = 0;												// Reseteo el valor de X, Y del ADC
				adc_valY = 0;
				sumaerr--;
				//				WG12864A_Limpiar(NEGRO);
				if(sumaerr<0)
				{
					sumaerr=8;//9;
				}
			}
			if((0xCA < adc_valX) &&(adc_valX < 0xEA) && (0x60 < adc_valY) && (adc_valY < 0x80))	// Se presionó RightArrow?
			{
				WG12864A_posXY(110, 5);
				WG12864A_print_symbol(RightArrow16x16, NEGRO);
				delay32Ms(0, TIMMER_FONDO_BLANCO);	// Para demorar su utiliza vTaskDelay()
				adc_valX = 0;												// Reseteo el valor de X, Y del ADC
				adc_valY = 0;
				sumaerr++;
				//				WG12864A_Limpiar(NEGRO);
				if(sumaerr>8)//9)											// Solo 8 pq sino muestra sin error por (berr=9)(Ruido?)
				{															// Corregir eso antes de agrandar cant de err
					sumaerr=0;
				}
			}
		}
		else
		{
			WG12864A_posXY(1, 1);											// No hay err almacenados
			WG12864A_printf(" *Consulta errores*  ", Arial8x6, BLANCO);
			WG12864A_posXY(25,3);
			WG12864A_printf(" Sin errores  ", Arial8x6, NEGRO);
			WG12864A_posXY(110, 7);
			WG12864A_print_symbol(BACK16x16, BLANCO);
		}
		Func_Sleep (flagirq, sleepmenu);
	}
	if(flag_sleepsubmenu)
	{					// Si la bandera esta arriba salio del menu normal, sino salio por sleep.

		WG12864A_posXY(110, 7);
		WG12864A_print_symbol(BACK16x16, NEGRO);
		delay32Ms(0, TIMMER_FONDO_BLANCO);	// Para demorar su utiliza vTaskDelay()
		adc_valX = 0, adc_valY = 0;			// Reseteo el valor de X, Y del ADC.
		WG12864A_Limpiar(NEGRO);
		menu1 = 0;
	}

	flag_sleepsubmenu = 1;						// Restablesco el valor original de la bandera.
}

//						  1.4 Borrar errores
//--------------------------------------------------------------------------
void func_borrarerr()
{
	while((!((0xD0 < adc_valX) &&(adc_valX < 0xEA) && (0x2A < adc_valY) && (adc_valY < 0x5A)))&& (pressok==0)) // Mientras no se presione back
	{
		WG12864A_posXY(1, 1);
		WG12864A_printf("  *Borrar errores*   ", Arial8x6, BLANCO);
		WG12864A_posXY(1, 5);
		WG12864A_printf("Confirma borrar      errores?", Arial8x6, NEGRO);
		WG12864A_posXY(80, 7);
		WG12864A_print_symbol(OK16x16, BLANCO);
		if((0xA0 < adc_valX) &&(adc_valX < 0xB9) && (0x2A < adc_valY) && (adc_valY < 0x5A))// Se presionó OK?
		{
			WG12864A_posXY(80, 7);
			WG12864A_print_symbol(OK16x16, NEGRO);
			delay32Ms(0, TIMMER_FONDO_BLANCO);	// Para demorar su utiliza vTaskDelay()
			adc_valX = 0;													// Reseteo el valor de X, Y del ADC
			adc_valY = 0;
			menuactual=berr;											// Envio menu actual por Recibe datos para borrar err en flash
			causaerr[0]=sinerr;	// provisorio, sacar!
			WG12864A_Limpiar(NEGRO);
			pressok=1;														// Activo la señal para salir por ok
			menu1=0;
		}
		WG12864A_posXY(110, 7);
		WG12864A_print_symbol(BACK16x16, BLANCO);
		Func_Sleep (flagirq, sleepmenu);
	}
if(flag_sleepsubmenu)
{					// Si la bandera esta arriba salio del menu normal, sino salio por sleep.
	if(pressok==0)															// Salgo sin borrar err
	{
		WG12864A_posXY(110, 7);
		WG12864A_print_symbol(BACK16x16, NEGRO);
		delay32Ms(0, TIMMER_FONDO_BLANCO);	// Para demorar su utiliza vTaskDelay()
		adc_valX = 0, adc_valY = 0;			// Reseteo el valor de X, Y del ADC.
		WG12864A_Limpiar(NEGRO);
		menu1 = 0;
	}
	pressok=0;

}

flag_sleepsubmenu = 1;						// Restablesco el valor original de la bandera.

}

//--------------------------------------------------------------------------
/****************************  Submenues 2  ********************************/
//--------------------------------------------------------------------------

//						  2.1 Parametrizar Atributos
//--------------------------------------------------------------------------
void Func_Ingreso (void)
{
	// Mientras no se presione Back.
	while(! ((0xD0 < adc_valX) && (adc_valX < 0xEA) &&
			 (0x2A < adc_valY) && (adc_valY < 0x5A)))
	{
		// Titulo del menu.
		WG12864A_posXY(1, 1);
		WG12864A_printf("  *Datos Paciente*   ", Arial8x6, BLANCO);
		WG12864A_posXY(110, 7);
		WG12864A_print_symbol(BACK16x16, BLANCO);
		WG12864A_posXY(1, 2);
		WG12864A_printf("Nombre:", Arial8x6, NEGRO);
		WG12864A_posXY(1, 3);
		WG12864A_printf("Edad:", Arial8x6, NEGRO);
		WG12864A_posXY(1, 5);
		WG12864A_printf(" 1 2 3 4 5 6 7 8 9 0", Arial8x6, NEGRO);
		WG12864A_posXY(1, 6);
		WG12864A_printf("Q W E R T Y U I O P ", Arial8x6, NEGRO);
		WG12864A_posXY(1, 7);
		WG12864A_printf(" A S D F G H J K L", Arial8x6, NEGRO);
		WG12864A_posXY(1, 8);
		WG12864A_printf("  Z X C V B N M ", Arial8x6, NEGRO);
		WG12864A_posXY(90, 3);
		WG12864A_print_symbol(LeftArrow16x16, BLANCO);
		WG12864A_posXY(110, 3);
		WG12864A_print_symbol(OK16x16, BLANCO);
		// Se Presionó Flecha Izquerda?
		if( (0x95 < adc_valX) && (adc_valX < 0xB5) &&
			(0x70 < adc_valY) && (adc_valY < 0xA9) ){

			WG12864A_posXY(90, 3);						// Animacion del dibujo.
			WG12864A_print_symbol(LeftArrow16x16, NEGRO);
			delay32Ms(0, TIMMER_FONDO_BLANCO_BOTONES);
			adc_valX = 0, adc_valY = 0;					// Reseteo el valor de X, Y del ADC.
			WG12864A_posXY(cur, 2);					// Presento el cursor donde corresponde.
			WG12864A_printf(" ", Arial8x6, NEGRO);
			cur -= 12;								// Corro el cursor 1 lugar.
			norecarga=0;
			indice --;									// Decremento el Indice del vector que guarda la clave ingresada.

			// Acomodo el cursor dentro de los 4 casilleros de la clave a ingresar y el indice del vector de la clave.
			if( cur < 76 ){

				cur = 112;
				indice = 3;
			}
		}
		// Se Presionó OK?
		if( (0xD0 < adc_valX) && (adc_valX < 0xEA) &&
			(0x70 < adc_valY) && (adc_valY < 0xA9) ){

			WG12864A_posXY(110, 3);						// Animacion del dibujo.
			WG12864A_print_symbol(OK16x16, NEGRO);
			delay32Ms(0, TIMMER_FONDO_BLANCO_BOTONES);
			adc_valX = 0, adc_valY = 0;					// Reseteo el valor de X, Y del ADC.
			WG12864A_posXY(cur, 2);					// Presento el cursor donde corresponde.
			WG12864A_printf(" ", Arial8x6, NEGRO);
			cur -= 12;								// Corro el cursor 1 lugar.
			norecarga=0;
			indice --;									// Decremento el Indice del vector que guarda la clave ingresada.

			// Acomodo el cursor dentro de los 4 casilleros de la clave a ingresar y el indice del vector de la clave.
			if( cur < 76 ){

				cur = 112;
				indice = 3;
			}
		}

	   func_teclado();
		// Funcion que maneja el Sleep de la pantalla y la IRQ del TOUCH.
		Func_Sleep (flagirq, sleepmenu);
	}

	if(flag_sleepsubmenu){					// Si la bandera esta arriba salio del menu normal, sino salio por sleep.

		WG12864A_posXY(110, 7);
		WG12864A_print_symbol(BACK16x16, NEGRO);
		delay32Ms(0, TIMMER_FONDO_BLANCO);	// Para demorar su utiliza vTaskDelay()
		adc_valX = 0, adc_valY = 0;			// Reseteo el valor de X, Y del ADC.
		WG12864A_Limpiar(NEGRO);
		menu1 = 0;
	}

	flag_sleepsubmenu = 1;						// Restablesco el valor original de la bandera.

}

//						  2.1 Parametrizar semaforos
//--------------------------------------------------------------------------
void Func_Monitoreo (void)
{
	char m=0,n=0;
	// Mientras no se presione Back.
	while(! ((0xD0 < adc_valX) && (adc_valX < 0xEA) &&
			 (0x2A < adc_valY) && (adc_valY < 0x5A))){

		// Titulo del menu.
		WG12864A_posXY(1, 1);
		WG12864A_printf(" *  Monitor Fetal  * ", Arial8x6, BLANCO);
		WG12864A_posXY(110, 7);
		WG12864A_print_symbol(BACK16x16, BLANCO);
		WG12864A_posXY(1, 2);
		WG12864A_printf("Max#----------------%", Arial8x6, NEGRO);
		WG12864A_posXY(1, 3);
		WG12864A_printf("   |", Arial8x6, NEGRO);
		WG12864A_posXY(121, 3);
		WG12864A_printf("|", Arial8x6, NEGRO);
		WG12864A_posXY(1, 4);
		WG12864A_printf("med|", Arial8x6, NEGRO);
		WG12864A_posXY(121, 4);
		WG12864A_printf("|", Arial8x6, NEGRO);
		WG12864A_posXY(1, 5);
		WG12864A_printf("   |", Arial8x6, NEGRO);
		WG12864A_posXY(121, 5);
		WG12864A_printf("|", Arial8x6, NEGRO);
		WG12864A_posXY(1, 6);
		WG12864A_printf("Min$________________&", Arial8x6, NEGRO);
		WG12864A_posXY(1, 7);
		WG12864A_printf("Pulso:", Arial8x6, NEGRO);
		WG12864A_posXY(40, 7);
		WG12864A_printf(buffer_pulso, Arial8x6, NEGRO);

		if((0xA9 < adc_valX) &&(adc_valX < 0xC0) && (0x2A < adc_valY) && (adc_valY < 0x5A))
		{
			adc_valX=0;
			adc_valY=0;
			delay32Ms(0, TIMMER_FONDO_BLANCO);	// Para demorar su utiliza vTaskDelay()
			if(FILTRO_DIG)
				FILTRO_DIG=0;
			else
				FILTRO_DIG=1;
		}
		if(FILTRO_DIG)
		{
			WG12864A_posXY(90, 7);
			WG12864A_print_symbol(FD16x16, BLANCO);
		}
		else
		{
			WG12864A_posXY(90, 7);
			WG12864A_print_symbol(FD16x16, NEGRO);
		}

//----------------------------------------------------------------
		if(hab_corazon)
		{
			hab_corazon=0;
		if(v<10)//SACAR
		{
			WG12864A_posXY(70, 7);
			WG12864A_print_symbol(HEART16x16, BLANCO);
		}
		else
		{
			WG12864A_posXY(70, 7);
			WG12864A_print_symbol(HEART16x16, NEGRO);
		}
		v++;
		if(v>=20)
			v=0;
		}
		else
		{
			WG12864A_posXY(70, 7);
			WG12864A_printf("   ", Arial8x6, NEGRO);
			WG12864A_posXY(70, 8);
			WG12864A_printf("   ", Arial8x6, NEGRO);

		}
//PUNTO Viene de un valor del ADC
//Escalonado entre fila 2 y 6 Columna 25 y 125
		func_temp();
//-----------------------------------------------
		for(m=1;m<5;m++)
		{
			for(n=2;n<7;n++)
			{
				WG12864A_posXY(pos_x+m,n);
				GLCD_Output_High(RS);   // Modo datos
				switch (n)
				{
				case 2:
					BORRA=0x04;
					break;
				case 6:
					BORRA=0x40;
					break;
				default:
					BORRA=0x00;
					break;
				}
				if(pos_x+m<65)
					GLCD_enviaBYTE(IZQ, (BORRA));  // enciende byte
				else
					GLCD_enviaBYTE(DER, (BORRA));  // enciende byte
			}
		}
//---------------------------------------------------------
		WG12864A_posXY(pos_x,pos_y);
		GLCD_Output_High(RS);   // Modo datos
		if(pos_x<65)
			GLCD_enviaBYTE(IZQ, (PUNTO));  // enciende byte
		else
			GLCD_enviaBYTE(DER, (PUNTO));  // enciende byte
//----------------------------------------------------------------
		// Funcionalidad del Menú.
/*		WG12864A_posXY(70, 7);
		WG12864A_putchar((((adc_valX >> 4) & 0x0F)+0x20), Hexa8x6, NEGRO);
		WG12864A_putchar(((adc_valX & 0x0F)+0x20), Hexa8x6, NEGRO);
		WG12864A_posXY(90, 7);
		WG12864A_putchar((((adc_valY >> 4) & 0x0F)+0x20), Hexa8x6, NEGRO);
		WG12864A_putchar(((adc_valY & 0x0F)+0x20), Hexa8x6, NEGRO);
*/
		// Funcion que maneja el Sleep de la pantalla y la IRQ del TOUCH.
		Func_Sleep (flagirq, sleepmenu);
	}

	if(flag_sleepsubmenu){					// Si la bandera esta arriba salio del menu normal, sino salio por sleep.

		WG12864A_posXY(110, 7);
		WG12864A_print_symbol(BACK16x16, NEGRO);
		delay32Ms(0, TIMMER_FONDO_BLANCO);	// Para demorar su utiliza vTaskDelay()
		adc_valX = 0, adc_valY = 0;			// Reseteo el valor de X, Y del ADC.
		WG12864A_Limpiar(NEGRO);
		menu2 = 0;
	}

	flag_sleepsubmenu = 1;						// Restablesco el valor original de la bandera.
}

//						  2.3 Parametrizar dirección
//--------------------------------------------------------------------------
void Func_Config (void){

	// Mientras no se presione Back.
	while(! ((0xD0 < adc_valX) && (adc_valX < 0xEA) &&
			 (0x2A < adc_valY) && (adc_valY < 0x5A))){

		// Titulo del menu.
		WG12864A_posXY(1, 1);
		WG12864A_printf(" *Setea parametros*  ", Arial8x6, BLANCO);
		WG12864A_posXY(110, 7);
		WG12864A_print_symbol(BACK16x16, BLANCO);
		menuactual=comu;
		// Funcionalidad del Menú.
		switch(velc)
		{
			case comu_1200:
				WG12864A_posXY(1, 5);
				WG12864A_printf("UART 1 MODEM 1200           dir:  ", Arial8x6, NEGRO);
				break;
			case comu_9600:
				WG12864A_posXY(1, 5);
				WG12864A_printf("UART 1 MODEM 9600           dir:  ", Arial8x6, NEGRO);
				break;
			case sin_comu:
				WG12864A_posXY(1, 5);
				WG12864A_printf("UART 1 Sin comm. por        MODEM", Arial8x6, NEGRO);
				break;
			default:
				WG12864A_posXY(1, 5);
				WG12864A_printf("UART 1", Arial8x6, NEGRO);

				break;
		}
		switch (dir_comu)															// Muestra el módulo en conflicto
		{
		case 1:
				WG12864A_posXY(79,6);
				WG12864A_printf("1", Arial8x6, NEGRO);
				break;
		case 2:
				WG12864A_posXY(79,6);
				WG12864A_printf("2", Arial8x6, NEGRO);
				break;
		case 3:
				WG12864A_posXY(79,6);
				WG12864A_printf("3", Arial8x6, NEGRO);
				break;
		case 4:
				WG12864A_posXY(79,6);
				WG12864A_printf("4", Arial8x6, NEGRO);
				break;
		case 5:
				WG12864A_posXY(79,6);
				WG12864A_printf("5", Arial8x6, NEGRO);
				break;
		case 6:
				WG12864A_posXY(79,6);
				WG12864A_printf("6", Arial8x6, NEGRO);
				break;
		case 7:
				WG12864A_posXY(79,6);
				WG12864A_printf("7", Arial8x6, NEGRO);
				break;
		case 8:
				WG12864A_posXY(79,6);
				WG12864A_printf("8", Arial8x6, NEGRO);
				break;
		case 0:
		default:
				break;
		}

		WG12864A_posXY(1, 3);
		WG12864A_printf("UART 0 GPS 1200", Arial8x6, NEGRO);
		WG12864A_posXY(1, 7);
		WG12864A_printf("UART 2 Prog", Arial8x6, NEGRO);

		// Funcion que maneja el Sleep de la pantalla y la IRQ del TOUCH.
		Func_Sleep (flagirq, sleepmenu);
	}

	if(flag_sleepsubmenu){					// Si la bandera esta arriba salio del menu normal, sino salio por sleep.

		WG12864A_posXY(110, 7);
		WG12864A_print_symbol(BACK16x16, NEGRO);
		delay32Ms(0, TIMMER_FONDO_BLANCO);	// Para demorar su utiliza vTaskDelay()
		adc_valX = 0, adc_valY = 0;			// Reseteo el valor de X, Y del ADC.
		WG12864A_Limpiar(NEGRO);
		menu2 = 0;
	}

	flag_sleepsubmenu = 1;						// Restablesco el valor original de la bandera.

}

//--------------------------------------------------------------------------
/****************************  Submenues 3  ********************************/
//--------------------------------------------------------------------------
//						  3.1  Muestra fecha y hora
//--------------------------------------------------------------------------
void func_muestrafyh()
{
	while(!((0xD0 < adc_valX) &&(adc_valX < 0xEA) && (0x2A < adc_valY) && (adc_valY < 0x5A))) // Mientras no se presione back
	{
		menuactual=fyh;
		WG12864A_posXY(1, 1);
		WG12864A_printf(" * Consulta reloj *  ", Arial8x6, BLANCO);
		WG12864A_posXY(1, 3);
		WG12864A_printf("  Hora      Fecha ", Arial8x6, NEGRO);
		muestra_fecha_y_hora (5) ;
		muestra_dsemana(ds,80,7);
		WG12864A_posXY(110, 7);
		WG12864A_print_symbol(BACK16x16, BLANCO);
		// Funcion que maneja el Sleep de la pantalla y la IRQ del TOUCH.
		Func_Sleep (flagirq, sleepmenu);
	}
	if(flag_sleepsubmenu){					// Si la bandera esta arriba salio del menu normal, sino salio por sleep.

		WG12864A_posXY(110, 7);
		WG12864A_print_symbol(BACK16x16, NEGRO);
		delay32Ms(0, TIMMER_FONDO_BLANCO);	// Para demorar su utiliza vTaskDelay()
		adc_valX = 0, adc_valY = 0;			// Reseteo el valor de X, Y del ADC.
		WG12864A_Limpiar(NEGRO);
		menu3 = 0;
	}
	flag_sleepsubmenu = 1;						// Restablesco el valor original de la bandera.

}

//						  3.1  Modifica fecha y hora
//--------------------------------------------------------------------------
void func_modfyh()
{
	int i=0;
	while((!((0xD0 < adc_valX) &&(adc_valX < 0xEA) && (0x2A < adc_valY) && (adc_valY < 0x5A))) && flagesp!=1) // Mientras no se presione back
	{
		WG12864A_posXY(1, 1);
		WG12864A_printf(" * Modificar reloj * ", Arial8x6, BLANCO);
		WG12864A_posXY(1, 3);
		WG12864A_printf("Elija para modificar ", Arial8x6, NEGRO);
		WG12864A_posXY(1, 5);
		WG12864A_printf("   Fecha      Hora   ", Arial8x6, NEGRO);
		if((0xA0 < adc_valX) &&(adc_valX < 0xE0) && (0x6A < adc_valY) && (adc_valY < 0x8A))
		{
			menu3=-1;
			menu32=2;
		}
		if((0x2A < adc_valX) &&(adc_valX < 0x70) && (0x6A < adc_valY) && (adc_valY < 0x8A))
		{
			menu3=-1;
			menu32=1;
		}
		switch(menu32)
		{
//------------------------------------------  Modifica Fecha  -------------------------------------------------------------------
				case 1:
					WG12864A_posXY(13, 5);
					WG12864A_printf(" Fecha ", Arial8x6, BLANCO);
					delay32Ms(0, TIMMER_FONDO_BLANCO);	// Para demorar su utiliza vTaskDelay()
					adc_valX = 0;												// Reseteo el valor de X, Y del ADC
					adc_valY = 0;
					for(i=0;i<3;i++)
					{
						tcla[i]=0;
					}
					s=0;
					WG12864A_Limpiar(NEGRO);
					while((!((0xD0 < adc_valX) &&(adc_valX < 0xEA) && (0x2A < adc_valY) && (adc_valY < 0x5A))) && flagesp!=1) // Mientras no se presione back
					{
						switch(mod)
						{
						case 0:
							s=0;
							while((!((0xD0 < adc_valX) &&(adc_valX < 0xEA) && (0x2A < adc_valY) && (adc_valY < 0x5A))) && salir==0) // Mientras no se presione back
							{
								var_aux=Func_MuestraTeclado(2);								// Muestra el teclado numerico
								if( var_aux != -1 ){						// Valida el numero del teclado que se pulso.

									if (indice < 2){						// Si no cargo los 4 Digitos, lo carga sino limpia la clave.

										tcla[indice] = var_aux;				// Carga en el vector los digitos de la clave.
										indice ++;
									}else{

										indice = 0;							// Reseteo el indice del vector de la clave.
										cur = 76;						// Acomodo el cursor para la proxima vez.
										WG12864A_posXY(cur, 2);			// Limpio la clave en pantalla.
										WG12864A_printf(" ", Arial8x6, NEGRO);
										WG12864A_posXY(88, 2);
										WG12864A_printf(" ", Arial8x6, NEGRO);
									}
								}

								if((0xA0 < adc_valX) &&(adc_valX < 0xB9) && (0x2A < adc_valY) && (adc_valY < 0x5A))	// OK?
								{
								varmod=(tcla[0]*10)+(tcla[1]);						// Cargo el numero ingresado
								if((varmod)<32 &&(varmod>0))
								{
									proxf[0]=varmod;					//Cargo el dia
									WG12864A_posXY(80, 7);
									WG12864A_print_symbol(OK16x16, NEGRO);
									delay32Ms(0, TIMMER_FONDO_BLANCO);	// Para demorar su utiliza vTaskDelay()
									adc_valX = 0;									// Reseteo el valor de X, Y del ADC
									adc_valY = 0;
									salir=1;
									mod=1;
									s=0;
									indice = 0;							// Reseteo el indice del vector de la clave.
									cur=76;
									WG12864A_posXY(76, 2);
									WG12864A_printf("       ", Arial8x6, NEGRO);
									for(i=0;i<3;i++)
									{
										tcla[i]=0;
									}
								}
								else												// El valor ingresado no es correcto
								{
									WG12864A_posXY(80, 7);
									WG12864A_print_symbol(Cross16x16, NEGRO);
									delay32Ms(0, TIMMER_FONDO_BLANCO);	// Para demorar su utiliza vTaskDelay()

								}
								}
								Func_Sleep (flagirq, sleepmenu);
							}
							if(salir==0)
							{
								WG12864A_posXY(110, 7);
								WG12864A_print_symbol(BACK16x16, NEGRO);
								delay32Ms(0, TIMMER_FONDO_BLANCO);	// Para demorar su utiliza vTaskDelay()
								cur=76;
								mod=-1;
								menu32=-1;
								menu3= 2;
								adc_valX=0xD5;
								adc_valY=0x3A;
							}
							salir=0;
							break;
						case 1:
							s=0;
							while((!((0xD0 < adc_valX) &&(adc_valX < 0xEA) && (0x2A < adc_valY) && (adc_valY < 0x5A))) && salir==0) // Mientras no se presione back
							{
								var_aux=Func_MuestraTeclado(3);
								if( var_aux != -1 ){						// Valida el numero del teclado que se pulso.

									if (indice < 2){						// Si no cargo los 4 Digitos, lo carga sino limpia la clave.

										tcla[indice] = var_aux;				// Carga en el vector los digitos de la clave.
										indice ++;
									}else{

										indice = 0;							// Reseteo el indice del vector de la clave.
										cur = 76;						// Acomodo el cursor para la proxima vez.
										WG12864A_posXY(cur, 2);			// Limpio la clave en pantalla.
										WG12864A_printf(" ", Arial8x6, NEGRO);
										WG12864A_posXY(88, 2);
										WG12864A_printf(" ", Arial8x6, NEGRO);
									}
								}

								if((0xA0 < adc_valX) &&(adc_valX < 0xB9) && (0x2A < adc_valY) && (adc_valY < 0x5A))
								{
									varmod=(tcla[0]*10)+(tcla[1]);					//Cargo el valor ingresado por teclado
									if((varmod)<13 &&(varmod>0))
									{
										proxf[1]=varmod;				//Cargo el mes
										WG12864A_posXY(80, 7);
										WG12864A_print_symbol(OK16x16, NEGRO);
										delay32Ms(0, TIMMER_FONDO_BLANCO);	// Para demorar su utiliza vTaskDelay()
										adc_valX = 0;									// Reseteo el valor de X, Y del ADC
										adc_valY = 0;
										salir=1;
										mod=2;
										s=0;
										indice = 0;							// Reseteo el indice del vector de la clave.
										cur=76;
										WG12864A_posXY(76, 2);
										WG12864A_printf("       ", Arial8x6, NEGRO);
										for(i=0;i<3;i++)
										{
											tcla[i]=0;
										}
									}
									else
									{
										WG12864A_posXY(80, 7);
										WG12864A_print_symbol(Cross16x16, NEGRO);
										delay32Ms(0, TIMMER_FONDO_BLANCO);	// Para demorar su utiliza vTaskDelay()
									}
								}
								Func_Sleep (flagirq, sleepmenu);
							}
							if(salir==0)
							{
								WG12864A_posXY(110, 7);
								WG12864A_print_symbol(BACK16x16, NEGRO);
								delay32Ms(0, TIMMER_FONDO_BLANCO);	// Para demorar su utiliza vTaskDelay()
								cur=76;
								mod=-1;
								menu32=-1;
								menu3= 2;
								adc_valX=0xD5;
								adc_valY=0x3A;
							}
							salir=0;
							break;
						case 2:
							s=0;
							while((!((0xD0 < adc_valX) &&(adc_valX < 0xEA) && (0x2A < adc_valY) && (adc_valY < 0x5A))) && salir==0) // Mientras no se presione back
							{
								var_aux=Func_MuestraTeclado(4);

								if( var_aux != -1 ){						// Valida el numero del teclado que se pulso.

									if (indice < 4){						// Si no cargo los 4 Digitos, lo carga sino limpia la clave.

										tcla[indice] = var_aux;				// Carga en el vector los digitos de la clave.
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

								if((0xA0 < adc_valX) &&(adc_valX < 0xB9) && (0x2A < adc_valY) && (adc_valY < 0x5A))
								{
									varmod=(tcla[0]*1000)+(tcla[1]*100)+(tcla[2]*10)+(tcla[3]);// Cargo el valor ingresado por teclado
									if(1)//(varmod)<2100 &&(varmod>1979))
									{
										proxf[2]=varmod;				//Cargo el año
										WG12864A_posXY(80, 7);
										WG12864A_print_symbol(OK16x16, NEGRO);
										delay32Ms(0, TIMMER_FONDO_BLANCO);	// Para demorar su utiliza vTaskDelay()
										adc_valX = 0;									// Reseteo el valor de X, Y del ADC
										adc_valY = 0;
										salir=1;
										mod=4;
										s=0;
										indice = 0;							// Reseteo el indice del vector de la clave.
										cur=76;
										WG12864A_posXY(76, 2);
										WG12864A_printf("       ", Arial8x6, NEGRO);
										for(i=0;i<3;i++)
										{
											tcla[i]=0;
										}
									}
									else
									{
										WG12864A_posXY(80, 7);
										WG12864A_print_symbol(Cross16x16, NEGRO);
										delay32Ms(0, TIMMER_FONDO_BLANCO);	// Para demorar su utiliza vTaskDelay()
									}
							}
								Func_Sleep (flagirq, sleepmenu);
							}
							if(salir==0)
							{
								WG12864A_posXY(110, 7);
								WG12864A_print_symbol(BACK16x16, NEGRO);
								delay32Ms(0, TIMMER_FONDO_BLANCO);	// Para demorar su utiliza vTaskDelay()
								cur=76;
								mod=-1;
								menu32=-1;
								menu3= 2;
								adc_valX=0xD5;
								adc_valY=0x3A;
							}
							salir=0;
							break;
						case 4:
							WG12864A_Limpiar(NEGRO);
							while((!((0xD0 < adc_valX) &&(adc_valX < 0xEA) && (0x2A < adc_valY) && (adc_valY < 0x5A))) && salir==0) // Mientras no se presione back
							{
								WG12864A_posXY(1, 1);
								WG12864A_printf(" * Modificar fecha * ", Arial8x6, BLANCO);
								WG12864A_posXY(1, 3);
								WG12864A_printf("Fecha  Mes  A;o", Arial8x6, NEGRO);	// Muestra que los datos ingresados sean correctos
								prox_fecha (5);								// Muestra la prox fecha a cargar
								WG12864A_posXY(1,5);
								WG12864A_posXY(80, 7);													// Muestra ok
								WG12864A_print_symbol(OK16x16, BLANCO);
								WG12864A_posXY(110, 7);													// Muestra back
								WG12864A_print_symbol(BACK16x16, BLANCO);
								if((0xA0 < adc_valX) &&(adc_valX < 0xB9) && (0x2A < adc_valY) && (adc_valY < 0x5A)) //OK?
								{
									WG12864A_posXY(80, 7);
									WG12864A_print_symbol(OK16x16, NEGRO);
									delay32Ms(0, TIMMER_FONDO_BLANCO);	// Para demorar su utiliza vTaskDelay()
									adc_valX=0xD5;
									adc_valY=0x3A;
									mod=-1;
									menu32=-1;
									menu3= 2;
									salir=1;
									queenv=proxfecha;
									no_recibo=1;
								}
								Func_Sleep (flagirq, sleepmenu);
							}
							if(salir==0)
							{
								WG12864A_posXY(110, 7);
								WG12864A_print_symbol(BACK16x16, NEGRO);
								delay32Ms(0, TIMMER_FONDO_BLANCO);	// Para demorar su utiliza vTaskDelay()
								cur=76;
								mod=-1;
								menu32=-1;
								menu3= 2;
								adc_valX=0xD5;
								adc_valY=0x3A;
							}
							salir=0;
							break;
						default:
							break;
						}
						if(norecarga==0)													// Si se presionó un num incrementa el vector de clave
						{
							norecarga=1;
							//tcla[s]=num;
							s++;
						}
					}
					mod=0;
					s=0;
					adc_valX=0x00;
					adc_valY=0x00;
					WG12864A_Limpiar(NEGRO);
					break;

//------------------------------------------  Modifica Hora  --------------------------------------------------------------------
		case 2:
				WG12864A_posXY(78, 5);
				WG12864A_printf(" Hora ", Arial8x6, BLANCO);
				delay32Ms(0, TIMMER_FONDO_BLANCO);	// Para demorar su utiliza vTaskDelay()
				adc_valX = 0;												// Reseteo el valor de X, Y del ADC
				adc_valY = 0;
				for(i=0;i<3;i++)
				{
					tcla[i]=0;
				}
				s=0;
				WG12864A_Limpiar(NEGRO);
				while((!((0xD0 < adc_valX) &&(adc_valX < 0xEA) && (0x2A < adc_valY) && (adc_valY < 0x5A))) && flagesp!=1) // Mientras no se presione back
				{
					switch(mod)
					{
					case 0:
							s=0;
							while((!((0xD0 < adc_valX) &&(adc_valX < 0xEA) && (0x2A < adc_valY) && (adc_valY < 0x5A))) && salir==0) // Mientras no se presione back
							{
								var_aux=Func_MuestraTeclado(5);
								if( var_aux != -1 ){						// Valida el numero del teclado que se pulso.

									if (indice < 2){						// Si no cargo los 4 Digitos, lo carga sino limpia la clave.

										tcla[indice] = var_aux;				// Carga en el vector los digitos de la clave.
										indice ++;
									}else{

										indice = 0;							// Reseteo el indice del vector de la clave.
										cur = 76;						// Acomodo el cursor para la proxima vez.
										WG12864A_posXY(cur, 2);			// Limpio la clave en pantalla.
										WG12864A_printf(" ", Arial8x6, NEGRO);
										WG12864A_posXY(88, 2);
										WG12864A_printf(" ", Arial8x6, NEGRO);
									}
								}

								if((0xA0 < adc_valX) &&(adc_valX < 0xB9) && (0x2A < adc_valY) && (adc_valY < 0x5A))
								{
									varmod=(tcla[0]*10)+(tcla[1]);			// Carga los numeros ingresados por teclado
									if((varmod)<25 &&(varmod>=0))
									{
										proxh[0]=varmod;		//Carga la hora
										WG12864A_posXY(80, 7);
										WG12864A_print_symbol(OK16x16, NEGRO);
										delay32Ms(0, TIMMER_FONDO_BLANCO);	// Para demorar su utiliza vTaskDelay()
										adc_valX = 0;									// Reseteo el valor de X, Y del ADC
										adc_valY = 0;
										salir=1;
										mod=1;
										s=0;
										indice = 0;							// Reseteo el indice del vector de la clave.
										cur=76;
										WG12864A_posXY(76, 2);
										WG12864A_printf("       ", Arial8x6, NEGRO);
										for(i=0;i<3;i++)
										{
											tcla[i]=0;
										}
									}
									else
									{
										WG12864A_posXY(80, 7);
										WG12864A_print_symbol(Cross16x16, NEGRO);
										delay32Ms(0, TIMMER_FONDO_BLANCO);	// Para demorar su utiliza vTaskDelay()
									}
								}
								Func_Sleep (flagirq, sleepmenu);
							}
							if(salir==0)
							{
								WG12864A_posXY(110, 7);
								WG12864A_print_symbol(BACK16x16, NEGRO);
								delay32Ms(0, TIMMER_FONDO_BLANCO);	// Para demorar su utiliza vTaskDelay()
								cur=76;
								mod=-1;
								menu32=-1;
								menu3= 2;
								adc_valX=0xD5;
								adc_valY=0x3A;
							}
							salir=0;
							break;
					case 1:
							s=0;
							while((!((0xD0 < adc_valX) &&(adc_valX < 0xEA) && (0x2A < adc_valY) && (adc_valY < 0x5A))) && salir==0) // Mientras no se presione back
							{
								var_aux=Func_MuestraTeclado(6);
								if( var_aux != -1 ){						// Valida el numero del teclado que se pulso.

									if (indice < 2){						// Si no cargo los 4 Digitos, lo carga sino limpia la clave.

										tcla[indice] = var_aux;				// Carga en el vector los digitos de la clave.
										indice ++;
									}else{

										indice = 0;							// Reseteo el indice del vector de la clave.
										cur = 76;						// Acomodo el cursor para la proxima vez.
										WG12864A_posXY(cur, 2);			// Limpio la clave en pantalla.
										WG12864A_printf(" ", Arial8x6, NEGRO);
										WG12864A_posXY(88, 2);
										WG12864A_printf(" ", Arial8x6, NEGRO);
									}
								}

								if((0xA0 < adc_valX) &&(adc_valX < 0xB9) && (0x2A < adc_valY) && (adc_valY < 0x5A))
								{
									varmod=(tcla[0]*10)+(tcla[1]);			// Carga los numeros ingresados por teclado
									if((varmod)<60 &&(varmod>=0))
									{
										proxh[1]=varmod;		//Cargo los minutos
										WG12864A_posXY(80, 7);
										WG12864A_print_symbol(OK16x16, NEGRO);
										delay32Ms(0, TIMMER_FONDO_BLANCO);	// Para demorar su utiliza vTaskDelay()
										adc_valX = 0;									// Reseteo el valor de X, Y del ADC
										adc_valY = 0;
										salir=1;
										mod=2;
										s=0;
										indice = 0;							// Reseteo el indice del vector de la clave.
										cur=76;
										WG12864A_posXY(76, 2);
										WG12864A_printf("       ", Arial8x6, NEGRO);
										for(i=0;i<3;i++)
										{
											tcla[i]=0;
										}
									}
									else
									{
										WG12864A_posXY(80, 7);
										WG12864A_print_symbol(Cross16x16, NEGRO);
										delay32Ms(0, TIMMER_FONDO_BLANCO);	// Para demorar su utiliza vTaskDelay()
									}
								}
								Func_Sleep (flagirq, sleepmenu);
							}
							if(salir==0)
							{
								WG12864A_posXY(110, 7);
								WG12864A_print_symbol(BACK16x16, NEGRO);
								delay32Ms(0, TIMMER_FONDO_BLANCO);	// Para demorar su utiliza vTaskDelay()
								cur=76;
								mod=-1;
								menu32=-1;
								menu3= 2;
								adc_valX=0xD5;
								adc_valY=0x3A;
							}
							salir=0;
							break;

					case 2:
							WG12864A_Limpiar(NEGRO);
							while((!((0xD0 < adc_valX) &&(adc_valX < 0xEA) && (0x2A < adc_valY) && (adc_valY < 0x5A))) && salir==0) // Mientras no se presione back
							{
								WG12864A_posXY(1, 1);
								WG12864A_printf(" * Modificar Hora * ", Arial8x6, BLANCO);
								WG12864A_posXY(1, 3);
								WG12864A_printf("  HH   MM   SS", Arial8x6, NEGRO);	// Muestra que la hora ingresada sea correcta
								proxh[2]=0;
								prox_hora(5);								// Muestra la proxima hora a cargar
								WG12864A_posXY(80, 7);						// Muestra ok
								WG12864A_print_symbol(OK16x16, BLANCO);
								WG12864A_posXY(110, 7);						// Muestra back
								WG12864A_print_symbol(BACK16x16, BLANCO);
								if((0xA0 < adc_valX) &&(adc_valX < 0xB9) && (0x2A < adc_valY) && (adc_valY < 0x5A))
								{
									WG12864A_posXY(80, 7);
									WG12864A_print_symbol(OK16x16, NEGRO);
									delay32Ms(0, TIMMER_FONDO_BLANCO);	// Para demorar su utiliza vTaskDelay()
									adc_valX=0xD5;
									adc_valY=0x3A;
									mod=-1;
									menu32=-1;
									menu3= 2;
									salir=1;
									queenv=proxhora;
									no_recibo=1;
								}
								Func_Sleep (flagirq, sleepmenu);
							}
							if(salir==0)
							{
								WG12864A_posXY(110, 7);
								WG12864A_print_symbol(BACK16x16, NEGRO);
								delay32Ms(0, TIMMER_FONDO_BLANCO);	// Para demorar su utiliza vTaskDelay()
								cur=76;
								mod=-1;
								menu32=-1;
								menu3= 2;
								adc_valX=0xD5;
								adc_valY=0x3A;
							}
							salir=0;
							break;
					default:
							break;
					}
				}
				mod=0;
				s=0;
				adc_valX=0x00;
				adc_valY=0x00;
				WG12864A_Limpiar(NEGRO);
				break;
		default:
				break;
		}
		WG12864A_posXY(110, 7);
		WG12864A_print_symbol(BACK16x16, BLANCO);
		Func_Sleep (flagirq, sleepmenu);
	}

	if(flag_sleepsubmenu){					// Si la bandera esta arriba salio del menu normal, sino salio por sleep.

		WG12864A_posXY(110, 7);
		WG12864A_print_symbol(BACK16x16, NEGRO);
		delay32Ms(0, TIMMER_FONDO_BLANCO);	// Para demorar su utiliza vTaskDelay()
		adc_valX = 0, adc_valY = 0;			// Reseteo el valor de X, Y del ADC.
		WG12864A_Limpiar(NEGRO);
		menu3 = 0;
	}

	flag_sleepsubmenu = 1;						// Restablesco el valor original de la bandera.

}

//--------------------------------------------------------------------------
/****************************  Submenues 4  ********************************/
//--------------------------------------------------------------------------
//						  4.1  Actualiza FW del CT800C
//--------------------------------------------------------------------------
void func_actualiza_fw(void)
{
//	while(!((0xD0 < adc_valX) &&(adc_valX < 0xEA) && (0x2A < adc_valY) && (adc_valY < 0x5A))) // Mientras no se presione back
//	{
		menuactual=afw;
		actualiza_fw=1;
		WG12864A_posXY(1, 1);
		WG12864A_printf("*Actualizar Firmware*", Arial8x6, BLANCO);
		WG12864A_posXY(1, 3);
		WG12864A_printf(" Espere... ", Arial8x6, NEGRO);
		WG12864A_posXY(1, 5);
		WG12864A_printf(" Actualizando fw", Arial8x6, NEGRO);
		WG12864A_posXY(1, 6);
		WG12864A_printf(" por USB...", Arial8x6, NEGRO);
		delay32Ms(0, TIMMER_ACTUALIZA_FW);	// Para demorar su utiliza vTaskDelay()
		menu41_1 = -1;								// Desabilito que entre al SubMenú.
		menu41 = -1;									// Habilito que entre al Menu Seleccionado.
		menu4 = -1;
		menu = 0;
		menuactual=consp;					// Necesario para sacarlo de la comunicación SPI modo afw
//		WG12864A_posXY(110, 7);
//		WG12864A_print_symbol(BACK16x16, BLANCO);
		// Funcion que maneja el Sleep de la pantalla y la IRQ del TOUCH.
//		Func_Sleep (flagirq, sleepmenu);
//	}
/*	if(flag_sleepsubmenu){					// Si la bandera esta arriba salio del menu normal, sino salio por sleep.

		WG12864A_posXY(110, 7);
		WG12864A_print_symbol(BACK16x16, NEGRO);
		delay32Ms(0, TIMMER_FONDO_BLANCO);	// Para demorar su utiliza vTaskDelay()
		adc_valX = 0, adc_valY = 0;			// Reseteo el valor de X, Y del ADC.
		WG12864A_Limpiar(NEGRO);
		menu41_1 = -1;								// Desabilito que entre al SubMenú.
		menu41 = 0;									// Habilito que entre al Menu Seleccionado.

	}
*/	flag_sleepsubmenu = 1;						// Restablesco el valor original de la bandera.

}
void func_temp(void)
{
	char *		renglon = buffer_pulso ;
	pos_x++;
	if(pos_x>=120)
		pos_x=25;
	// Consulta que menu se presionó.
	if( (0xAA < adc_valY) && (adc_valY < 0xC0) )
	{
		pos_y=2;
		hab_corazon=1;
		num_pulso=140;
		func_punto();
		*renglon++ = ((num_pulso/100) % 10) + '0' ;
		*renglon++ = ((num_pulso/10)  % 10) + '0' ;
		*renglon++ = ( num_pulso      % 10) + '0' ;
	}
	if( (0xA0 < adc_valY) && (adc_valY < 0xAA) )
	{
		pos_y=3;
		hab_corazon=1;
		num_pulso=130;
		func_punto();
		*renglon++ = ((num_pulso/100) % 10) + '0' ;
		*renglon++ = ((num_pulso/10)  % 10) + '0' ;
		*renglon++ = ( num_pulso      % 10) + '0' ;
	}
	if(	(0x85 < adc_valY) && (adc_valY < 0xA0) )
	{
		pos_y=4;
		hab_corazon=1;
		num_pulso=120;
		func_punto();
		*renglon++ = ((num_pulso/100) % 10) + '0' ;
		*renglon++ = ((num_pulso/10)  % 10) + '0' ;
		*renglon++ = ( num_pulso      % 10) + '0' ;
	}
	if( (0x7A < adc_valY) && (adc_valY < 0x85) )
	{
		pos_y=5;
		hab_corazon=1;
		num_pulso=110;
		func_punto();
		*renglon++ = ((num_pulso/100) % 10) + '0' ;
		*renglon++ = ((num_pulso/10)  % 10) + '0' ;
		*renglon++ = ( num_pulso      % 10) + '0' ;
	}
	if(	(0x5A < adc_valY) && (adc_valY < 0x7A) )
	{
		pos_y=6;
		hab_corazon=1;
		num_pulso=100;
		func_punto();
		*renglon++ = ((num_pulso/100) % 10) + '0' ;
		*renglon++ = ((num_pulso/10)  % 10) + '0' ;
		*renglon++ = ( num_pulso      % 10) + '0' ;
	}
}
void func_punto (void)
{
	char val_adc;
	PUNTO=0x10;
	val_adc=adc_valY&0x0f;
	switch (val_adc)
	{
	case 0x01:
		PUNTO=0x10;
		num_pulso+=1;
		break;
	case 0x02:
		PUNTO=0x20;
		num_pulso+=2;
		break;
	case 0x04:
		PUNTO=0x40;
		num_pulso+=3;
		break;
	case 0x08:
		PUNTO=0x80;
		num_pulso+=4;
		break;
	case 0x0A:
		PUNTO=0x01;
		num_pulso+=5;
		break;
	case 0x0C:
		PUNTO=0x02;
		num_pulso+=6;
		break;
	case 0x0D:
		PUNTO=0x04;
		num_pulso+=7;
		break;
	case 0x0F:
		PUNTO=0x08;
		num_pulso+=8;
		break;
	default:
		break;
	}
}
// *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
