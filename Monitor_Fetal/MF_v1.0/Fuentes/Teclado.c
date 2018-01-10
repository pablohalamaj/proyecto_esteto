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
 * Programa que lee el touch y toma la tecla precionada
****************************************************************************/
#include "Funciones.h"
#include "WG12864A.h"
#include "Definiciones.h"

extern unsigned int 	cur,indice,tit,num;
extern char pos_x,pos_y,PUNTO,BORRA,num_pulso,hab_corazon;
extern unsigned int adc_valX, adc_valY,tcl;
extern int norecarga;

//--------------------------------------------------------------------------
void func_teclado(void)
{
// Se presionó Q?
	if( (0x10 < adc_valX) && (adc_valX < 0x35) &&
		(0x5A < adc_valY) && (adc_valY < 0x7A) )
	{
		WG12864A_posXY(1, 6);
		WG12864A_printf("Q ", Arial8x6, BLANCO);
		delay32Ms(0, TIMMER_FONDO_BLANCO);
		adc_valX = 0, adc_valY = 0;											// Reseteo el valor de X, Y del ADC.
		WG12864A_posXY(cur, 2);
		WG12864A_printf("Q", Arial8x6, NEGRO);
		cur += 6;															// Corro el cursor 1 lugar.
		norecarga=0;
		tcl = 1;															// Guardo el numero seleccionado para que la funcion lo devuelva.
	}
// Se presionó W?
	if( (0x35 < adc_valX) && (adc_valX < 0x45) &&
		(0x5A < adc_valY) && (adc_valY < 0x7A) )
	{
		WG12864A_posXY(7, 6);
		WG12864A_printf(" W ", Arial8x6, BLANCO);
		delay32Ms(0, TIMMER_FONDO_BLANCO);
		adc_valX = 0, adc_valY = 0;											// Reseteo el valor de X, Y del ADC.
		WG12864A_posXY(cur, 2);
		WG12864A_printf("W", Arial8x6, NEGRO);
		cur += 6;															// Corro el cursor 1 lugar.
		norecarga=0;
		tcl = 1;															// Guardo el numero seleccionado para que la funcion lo devuelva.
	}
	// Se presionó E?
	if( (0x45 < adc_valX) && (adc_valX < 0x55) &&
		(0x5A < adc_valY) && (adc_valY < 0x7A) )
	{
		WG12864A_posXY(19, 6);
		WG12864A_printf(" E ", Arial8x6, BLANCO);
		delay32Ms(0, TIMMER_FONDO_BLANCO);
		adc_valX = 0, adc_valY = 0;											// Reseteo el valor de X, Y del ADC.
		WG12864A_posXY(cur, 2);
		WG12864A_printf("E", Arial8x6, NEGRO);
		cur += 6;															// Corro el cursor 1 lugar.
		norecarga=0;
		tcl = 1;															// Guardo el numero seleccionado para que la funcion lo devuelva.
	}
	// Se presionó R?
	if( (0x55 < adc_valX) && (adc_valX < 0x65) &&
		(0x5A < adc_valY) && (adc_valY < 0x7A) )
	{
		WG12864A_posXY(31, 6);
		WG12864A_printf(" R ", Arial8x6, BLANCO);
		delay32Ms(0, TIMMER_FONDO_BLANCO);
		adc_valX = 0, adc_valY = 0;											// Reseteo el valor de X, Y del ADC.
		WG12864A_posXY(cur, 2);
		WG12864A_printf("R", Arial8x6, NEGRO);
		cur += 6;															// Corro el cursor 1 lugar.
		norecarga=0;
		tcl = 1;															// Guardo el numero seleccionado para que la funcion lo devuelva.
	}
	// Se presionó T?
	if( (0x65 < adc_valX) && (adc_valX < 0x75) &&
		(0x5A < adc_valY) && (adc_valY < 0x7A) )
	{
		WG12864A_posXY(43, 6);
		WG12864A_printf(" T ", Arial8x6, BLANCO);
		delay32Ms(0, TIMMER_FONDO_BLANCO);
		adc_valX = 0, adc_valY = 0;											// Reseteo el valor de X, Y del ADC.
		WG12864A_posXY(cur, 2);
		WG12864A_printf("T", Arial8x6, NEGRO);
		cur += 6;															// Corro el cursor 1 lugar.
		norecarga=0;
		tcl = 1;															// Guardo el numero seleccionado para que la funcion lo devuelva.
	}
	// Se presionó Y?
	if( (0x75 < adc_valX) && (adc_valX < 0x85) &&
		(0x5A < adc_valY) && (adc_valY < 0x7A) )
	{
		WG12864A_posXY(55, 6);
		WG12864A_printf(" Y ", Arial8x6, BLANCO);
		delay32Ms(0, TIMMER_FONDO_BLANCO);
		adc_valX = 0, adc_valY = 0;											// Reseteo el valor de X, Y del ADC.
		WG12864A_posXY(cur, 2);
		WG12864A_printf("Y", Arial8x6, NEGRO);
		cur += 6;															// Corro el cursor 1 lugar.
		norecarga=0;
		tcl = 1;															// Guardo el numero seleccionado para que la funcion lo devuelva.
	}
	// Se presionó U?
	if( (0x85 < adc_valX) && (adc_valX < 0x95) &&
		(0x5A < adc_valY) && (adc_valY < 0x7A) )
	{
		WG12864A_posXY(67, 6);
		WG12864A_printf(" U ", Arial8x6, BLANCO);
		delay32Ms(0, TIMMER_FONDO_BLANCO);
		adc_valX = 0, adc_valY = 0;											// Reseteo el valor de X, Y del ADC.
		WG12864A_posXY(cur, 2);
		WG12864A_printf("U", Arial8x6, NEGRO);
		cur += 6;															// Corro el cursor 1 lugar.
		norecarga=0;
		tcl = 1;															// Guardo el numero seleccionado para que la funcion lo devuelva.
	}
	// Se presionó I?
	if( (0x95 < adc_valX) && (adc_valX < 0xA5) &&
		(0x5A < adc_valY) && (adc_valY < 0x7A) )
	{
		WG12864A_posXY(79, 6);
		WG12864A_printf(" I ", Arial8x6, BLANCO);
		delay32Ms(0, TIMMER_FONDO_BLANCO);
		adc_valX = 0, adc_valY = 0;											// Reseteo el valor de X, Y del ADC.
		WG12864A_posXY(cur, 2);
		WG12864A_printf("I", Arial8x6, NEGRO);
		cur += 6;															// Corro el cursor 1 lugar.
		norecarga=0;
		tcl = 1;															// Guardo el numero seleccionado para que la funcion lo devuelva.
	}
	// Se presionó O?
	if( (0xA5 < adc_valX) && (adc_valX < 0xB5) &&
		(0x5A < adc_valY) && (adc_valY < 0x7A) )
	{
		WG12864A_posXY(91, 6);
		WG12864A_printf(" O ", Arial8x6, BLANCO);
		delay32Ms(0, TIMMER_FONDO_BLANCO);
		adc_valX = 0, adc_valY = 0;											// Reseteo el valor de X, Y del ADC.
		WG12864A_posXY(cur, 2);
		WG12864A_printf("O", Arial8x6, NEGRO);
		cur += 6;															// Corro el cursor 1 lugar.
		norecarga=0;
		tcl = 1;															// Guardo el numero seleccionado para que la funcion lo devuelva.
	}
	// Se presionó P?
	if( (0xB5 < adc_valX) && (adc_valX < 0xC5) &&
		(0x5A < adc_valY) && (adc_valY < 0x7A) )
	{
		WG12864A_posXY(103, 6);
		WG12864A_printf(" P ", Arial8x6, BLANCO);
		delay32Ms(0, TIMMER_FONDO_BLANCO);
		adc_valX = 0, adc_valY = 0;											// Reseteo el valor de X, Y del ADC.
		WG12864A_posXY(cur, 2);
		WG12864A_printf("P", Arial8x6, NEGRO);
		cur += 6;															// Corro el cursor 1 lugar.
		norecarga=0;
		tcl = 1;															// Guardo el numero seleccionado para que la funcion lo devuelva.
	}
	// Se presionó A?
	if( (0x25 < adc_valX) && (adc_valX < 0x35) &&
		(0x55 < adc_valY) && (adc_valY < 0x60) )
	{
		WG12864A_posXY(1, 7);
		WG12864A_printf(" A ", Arial8x6, BLANCO);
		delay32Ms(0, TIMMER_FONDO_BLANCO);
		adc_valX = 0, adc_valY = 0;											// Reseteo el valor de X, Y del ADC.
		WG12864A_posXY(cur, 2);
		WG12864A_printf("A", Arial8x6, NEGRO);
		cur += 6;															// Corro el cursor 1 lugar.
		norecarga=0;
		tcl = 1;															// Guardo el numero seleccionado para que la funcion lo devuelva.
	}
	// Se presionó S?
	if( (0x35 < adc_valX) && (adc_valX < 0x45) &&
		(0x55 < adc_valY) && (adc_valY < 0x60) )
	{
		WG12864A_posXY(13, 7);
		WG12864A_printf(" S ", Arial8x6, BLANCO);
		delay32Ms(0, TIMMER_FONDO_BLANCO);
		adc_valX = 0, adc_valY = 0;											// Reseteo el valor de X, Y del ADC.
		WG12864A_posXY(cur, 2);
		WG12864A_printf("S", Arial8x6, NEGRO);
		cur += 6;															// Corro el cursor 1 lugar.
		norecarga=0;
		tcl = 1;															// Guardo el numero seleccionado para que la funcion lo devuelva.
	}
	// Se presionó D?
	if( (0x45 < adc_valX) && (adc_valX < 0x55) &&
		(0x55 < adc_valY) && (adc_valY < 0x60) )
	{
		WG12864A_posXY(25, 7);
		WG12864A_printf(" D ", Arial8x6, BLANCO);
		delay32Ms(0, TIMMER_FONDO_BLANCO);
		adc_valX = 0, adc_valY = 0;											// Reseteo el valor de X, Y del ADC.
		WG12864A_posXY(cur, 2);
		WG12864A_printf("D", Arial8x6, NEGRO);
		cur += 6;															// Corro el cursor 1 lugar.
		norecarga=0;
		tcl = 1;															// Guardo el numero seleccionado para que la funcion lo devuelva.
	}
	// Se presionó F?
	if( (0x55 < adc_valX) && (adc_valX < 0x65) &&
		(0x55 < adc_valY) && (adc_valY < 0x60) )
	{
		WG12864A_posXY(37, 7);
		WG12864A_printf(" F ", Arial8x6, BLANCO);
		delay32Ms(0, TIMMER_FONDO_BLANCO);
		adc_valX = 0, adc_valY = 0;											// Reseteo el valor de X, Y del ADC.
		WG12864A_posXY(cur, 2);
		WG12864A_printf("F", Arial8x6, NEGRO);
		cur += 6;															// Corro el cursor 1 lugar.
		norecarga=0;
		tcl = 1;															// Guardo el numero seleccionado para que la funcion lo devuelva.
	}
	// Se presionó G?
	if( (0x65 < adc_valX) && (adc_valX < 0x75) &&
		(0x55 < adc_valY) && (adc_valY < 0x60) )
	{
		WG12864A_posXY(49, 7);
		WG12864A_printf(" G ", Arial8x6, BLANCO);
		delay32Ms(0, TIMMER_FONDO_BLANCO);
		adc_valX = 0, adc_valY = 0;											// Reseteo el valor de X, Y del ADC.
		WG12864A_posXY(cur, 2);
		WG12864A_printf("G", Arial8x6, NEGRO);
		cur += 6;															// Corro el cursor 1 lugar.
		norecarga=0;
		tcl = 1;															// Guardo el numero seleccionado para que la funcion lo devuelva.
	}
	// Se presionó H?
	if( (0x75 < adc_valX) && (adc_valX < 0x90) &&
		(0x55 < adc_valY) && (adc_valY < 0x60) )
	{
		WG12864A_posXY(61, 7);
		WG12864A_printf(" H ", Arial8x6, BLANCO);
		delay32Ms(0, TIMMER_FONDO_BLANCO);
		adc_valX = 0, adc_valY = 0;											// Reseteo el valor de X, Y del ADC.
		WG12864A_posXY(cur, 2);
		WG12864A_printf("H", Arial8x6, NEGRO);
		cur += 6;															// Corro el cursor 1 lugar.
		norecarga=0;
		tcl = 1;															// Guardo el numero seleccionado para que la funcion lo devuelva.
	}
	// Se presionó J?
	if( (0x90 < adc_valX) && (adc_valX < 0xA0) &&
		(0x55 < adc_valY) && (adc_valY < 0x60) )
	{
		WG12864A_posXY(73, 7);
		WG12864A_printf(" J ", Arial8x6, BLANCO);
		delay32Ms(0, TIMMER_FONDO_BLANCO);
		adc_valX = 0, adc_valY = 0;											// Reseteo el valor de X, Y del ADC.
		WG12864A_posXY(cur, 2);
		WG12864A_printf("J", Arial8x6, NEGRO);
		cur += 6;															// Corro el cursor 1 lugar.
		norecarga=0;
		tcl = 1;															// Guardo el numero seleccionado para que la funcion lo devuelva.
	}
	// Se presionó K?
	if( (0xA0 < adc_valX) && (adc_valX < 0xB0) &&
		(0x55 < adc_valY) && (adc_valY < 0x60) )
	{
		WG12864A_posXY(85, 7);
		WG12864A_printf(" K ", Arial8x6, BLANCO);
		delay32Ms(0, TIMMER_FONDO_BLANCO);
		adc_valX = 0, adc_valY = 0;											// Reseteo el valor de X, Y del ADC.
		WG12864A_posXY(cur, 2);
		WG12864A_printf("K", Arial8x6, NEGRO);
		cur += 6;															// Corro el cursor 1 lugar.
		norecarga=0;
		tcl = 1;															// Guardo el numero seleccionado para que la funcion lo devuelva.
	}
	// Se presionó L?
	if( (0xB0 < adc_valX) && (adc_valX < 0xCA) &&
		(0x55 < adc_valY) && (adc_valY < 0x60) )
	{
		WG12864A_posXY(97, 7);
		WG12864A_printf(" L", Arial8x6, BLANCO);
		delay32Ms(0, TIMMER_FONDO_BLANCO);
		adc_valX = 0, adc_valY = 0;											// Reseteo el valor de X, Y del ADC.
		WG12864A_posXY(cur, 2);
		WG12864A_printf("L", Arial8x6, NEGRO);
		cur += 6;															// Corro el cursor 1 lugar.
		norecarga=0;
		tcl = 1;															// Guardo el numero seleccionado para que la funcion lo devuelva.
	}
	// Se presionó Z?
	if( (0x35 < adc_valX) && (adc_valX < 0x45) &&
		(0x10 < adc_valY) && (adc_valY < 0x50) )
	{
		WG12864A_posXY(7, 8);
		WG12864A_printf(" Z ", Arial8x6, BLANCO);
		delay32Ms(0, TIMMER_FONDO_BLANCO);
		adc_valX = 0, adc_valY = 0;											// Reseteo el valor de X, Y del ADC.
		WG12864A_posXY(cur, 2);
		WG12864A_printf("Z", Arial8x6, NEGRO);
		cur += 6;															// Corro el cursor 1 lugar.
		norecarga=0;
		tcl = 1;															// Guardo el numero seleccionado para que la funcion lo devuelva.
	}
	// Se presionó X?
	if( (0x45 < adc_valX) && (adc_valX < 0x55) &&
		(0x10 < adc_valY) && (adc_valY < 0x50) )
	{
		WG12864A_posXY(19, 8);
		WG12864A_printf(" X ", Arial8x6, BLANCO);
		delay32Ms(0, TIMMER_FONDO_BLANCO);
		adc_valX = 0, adc_valY = 0;											// Reseteo el valor de X, Y del ADC.
		WG12864A_posXY(cur, 2);
		WG12864A_printf("X", Arial8x6, NEGRO);
		cur += 6;															// Corro el cursor 1 lugar.
		norecarga=0;
		tcl = 1;															// Guardo el numero seleccionado para que la funcion lo devuelva.
	}
	// Se presionó C?
	if( (0x55 < adc_valX) && (adc_valX < 0x65) &&
		(0x10 < adc_valY) && (adc_valY < 0x50) )
	{
		WG12864A_posXY(31, 8);
		WG12864A_printf(" C ", Arial8x6, BLANCO);
		delay32Ms(0, TIMMER_FONDO_BLANCO);
		adc_valX = 0, adc_valY = 0;											// Reseteo el valor de X, Y del ADC.
		WG12864A_posXY(cur, 2);
		WG12864A_printf("C", Arial8x6, NEGRO);
		cur += 6;															// Corro el cursor 1 lugar.
		norecarga=0;
		tcl = 1;															// Guardo el numero seleccionado para que la funcion lo devuelva.
	}
	// Se presionó V?
	if( (0x65 < adc_valX) && (adc_valX < 0x75) &&
		(0x10 < adc_valY) && (adc_valY < 0x50) )
	{
		WG12864A_posXY(43, 8);
		WG12864A_printf(" V ", Arial8x6, BLANCO);
		delay32Ms(0, TIMMER_FONDO_BLANCO);
		adc_valX = 0, adc_valY = 0;											// Reseteo el valor de X, Y del ADC.
		WG12864A_posXY(cur, 2);
		WG12864A_printf("V", Arial8x6, NEGRO);
		cur += 6;															// Corro el cursor 1 lugar.
		norecarga=0;
		tcl = 1;															// Guardo el numero seleccionado para que la funcion lo devuelva.
	}
	// Se presionó B?
	if( (0x75 < adc_valX) && (adc_valX < 0x85) &&
		(0x10 < adc_valY) && (adc_valY < 0x50) )
	{
		WG12864A_posXY(55, 8);
		WG12864A_printf(" B ", Arial8x6, BLANCO);
		delay32Ms(0, TIMMER_FONDO_BLANCO);
		adc_valX = 0, adc_valY = 0;											// Reseteo el valor de X, Y del ADC.
		WG12864A_posXY(cur, 2);
		WG12864A_printf("B", Arial8x6, NEGRO);
		cur += 6;															// Corro el cursor 1 lugar.
		norecarga=0;
		tcl = 1;															// Guardo el numero seleccionado para que la funcion lo devuelva.
	}
	// Se presionó N?
	if( (0x85 < adc_valX) && (adc_valX < 0x95) &&
		(0x10 < adc_valY) && (adc_valY < 0x50) )
	{
		WG12864A_posXY(67, 8);
		WG12864A_printf(" N ", Arial8x6, BLANCO);
		delay32Ms(0, TIMMER_FONDO_BLANCO);
		adc_valX = 0, adc_valY = 0;											// Reseteo el valor de X, Y del ADC.
		WG12864A_posXY(cur, 2);
		WG12864A_printf("N", Arial8x6, NEGRO);
		cur += 6;															// Corro el cursor 1 lugar.
		norecarga=0;
		tcl = 1;															// Guardo el numero seleccionado para que la funcion lo devuelva.
	}
	// Se presionó M?
	if( (0x95 < adc_valX) && (adc_valX < 0xA5) &&
		(0x10 < adc_valY) && (adc_valY < 0x50) )
	{
		WG12864A_posXY(79, 8);
		WG12864A_printf(" M ", Arial8x6, BLANCO);
		delay32Ms(0, TIMMER_FONDO_BLANCO);
		adc_valX = 0, adc_valY = 0;											// Reseteo el valor de X, Y del ADC.
		WG12864A_posXY(cur, 2);
		WG12864A_printf("M", Arial8x6, NEGRO);
		cur += 6;															// Corro el cursor 1 lugar.
		norecarga=0;
		tcl = 1;															// Guardo el numero seleccionado para que la funcion lo devuelva.
	}
}

//--------------------------------------------------------------------------
unsigned int Func_MuestraTeclado(int opcion)
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

	WG12864A_posXY(80, 4);													// Muestra Flecha Izq.
	WG12864A_print_symbol(LeftArrow16x16, BLANCO);
	WG12864A_posXY(110, 4);													// Muestra Flecha Der.
	WG12864A_print_symbol(RightArrow16x16, BLANCO);
	WG12864A_posXY(80, 7);													// Muestra Boton Ok.
	WG12864A_print_symbol(OK16x16, BLANCO);
	WG12864A_posXY(110, 7);													// Muestra Boton Back.
	WG12864A_print_symbol(BACK16x16, BLANCO);

	// Se Presionó Flecha Izquerda?
	if( (0x95 < adc_valX) && (adc_valX < 0xB5) &&
		(0x70 < adc_valY) && (adc_valY < 0x9A) ){

		WG12864A_posXY(80, 4);												// Animacion del dibujo.
		WG12864A_print_symbol(LeftArrow16x16, NEGRO);
		delay32Ms(0, TIMMER_FONDO_BLANCO_BOTONES);
		adc_valX = 0, adc_valY = 0;											// Reseteo el valor de X, Y del ADC.
		WG12864A_posXY(cur, 2);												// Presento el cursor donde corresponde.
		WG12864A_printf(" ", Arial8x6, NEGRO);
		cur -= 12;															// Corro el cursor 1 lugar.
		norecarga=0;
		indice --;															// Decremento el Indice del vector que guarda la clave ingresada.

		// Acomodo el cursor dentro de los 4 casilleros de la clave a ingresar y el indice del vector de la clave.
		if( cur < 48 ){

			cur = 112;
			indice = 3;
		}
	}

	// Se Presionó Flecha Dererecha?
	if( (0xC5 < adc_valX) && (adc_valX < 0xE5) &&
		(0x70 < adc_valY) && (adc_valY < 0x9A) ){

		WG12864A_posXY(110, 4);												// Animacion del dibujo.
		WG12864A_print_symbol(RightArrow16x16, NEGRO);
		delay32Ms(0, TIMMER_FONDO_BLANCO_BOTONES);
		adc_valX = 0, adc_valY = 0;											// Reseteo el valor de X, Y del ADC.
		WG12864A_posXY(cur, 2);												// Presento el cursor donde corresponde.
		WG12864A_printf(" ", Arial8x6, NEGRO);
		cur += 12;															// Corro el cursor 1 lugar.
		norecarga=0;
		indice ++;															// Incremento el Indice del vector que guarda la clave ingresada.

		// Acomodo el cursor dentro de los 4 casilleros de la clave a ingresar y el indice del vector de la clave.
		if( cur > 112 ){

			cur = 48;
			norecarga=0;
			indice = 0;
		}
	}

	delay32Ms(0, TIMMER_TIT_CURSOR);										// Demora para que titile el cursor.
	WG12864A_posXY(cur, 2);

	if( tit )	{															// Cursor titilante.

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
		adc_valX = 0, adc_valY = 0;											// Reseteo el valor de X, Y del ADC.
		WG12864A_posXY(cur, 2);
		WG12864A_printf("1", Arial8x6, NEGRO);
		cur += 12;															// Corro el cursor 1 lugar.
		norecarga=0;
		num = 1;															// Guardo el numero seleccionado para que la funcion lo devuelva.
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

	return (num);															// La funcion devuelve el numero seleccionado.
}

