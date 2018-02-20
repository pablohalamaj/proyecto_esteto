/***************************************************************************
 *  Proyecto Final-UTN.BA
 *  Proyecto: Monitor Fetal
 *  Versión: v1.0
 *  Fecha: 11-02-2018
 *  Autor: Sebastian Sisevich
****************************************************************************/
/***************************************************************************
 *	Comentarios:
 *
 * Funciones que se ocupan de graficar en pantalla
****************************************************************************/
#include <stdio.h>
#include "gpio.h"
#include "Subfunciones.h"
#include "WG12864A.h"
#include "Definiciones.h"

char		pos_xg,alm_pos_x;
extern char pos_x,pnt,pnt_ant,Fila_ant,mitad_ant,Graf_punt,
			pos_xh,mon_his,protec_pos_x;


//						  Grafica Datos Estaticos del Monitoreo
//--------------------------------------------------------------------------

void Graf_datos_est	(void)
{
	WG12864A_posXY(1, 1);
	WG12864A_printf(" *  Monitor Fetal  * ", Arial8x6, BLANCO);				// Titulo del menu.
//-------------------------------------------------------------------------- Grafica recuadro
	WG12864A_posXY(110, 7);
	WG12864A_print_symbol(BACK16x16, BLANCO);
	WG12864A_posXY(91, 7);
	WG12864A_print_symbol(FD16x16, NEGRO);									// FD Negro
	WG12864A_posXY(1, 2);
	WG12864A_printf("160|", Arial8x6, NEGRO);
	WG12864A_posXY(121, 2);
	WG12864A_printf("|", Arial8x6, NEGRO);
	WG12864A_posXY(1, 3);
	WG12864A_printf("   |", Arial8x6, NEGRO);
	WG12864A_posXY(121, 3);
	WG12864A_printf("|", Arial8x6, NEGRO);
	WG12864A_posXY(1, 4);
	WG12864A_printf("110|", Arial8x6, NEGRO);
	WG12864A_posXY(121, 4);
	WG12864A_printf("|", Arial8x6, NEGRO);
	WG12864A_posXY(1, 5);
	WG12864A_printf("   |", Arial8x6, NEGRO);
	WG12864A_posXY(121, 5);
	WG12864A_printf("|", Arial8x6, NEGRO);
	WG12864A_posXY(1, 6);
	WG12864A_printf(" 60|", Arial8x6, NEGRO);
	WG12864A_posXY(121, 6);
	WG12864A_printf("|", Arial8x6, NEGRO);
	WG12864A_posXY(1, 7);
	WG12864A_printf("Pulso:", Arial8x6, NEGRO);
}


// Toma el PPM obtenido, cada fila tiene 20 valores, calcula fila a usar
//--------------------------------------------------------------------------
void Grafica_monitoreo(char pulso)
{
	char m,n,Fila_mon,pulso_env,mitad_f,NO_GRAF;
/*	if(mon_his)
	{
		if(protec_pos_x)
		{
			alm_pos_x=pos_xg;
			protec_pos_x=0;
		}
		pos_xg=pos_xh;
	}
	if(mon_his==0&&protec_pos_x==0)
		pos_xg=alm_pos_x;
*/
	if(pulso>=60 && pulso<70)
	{
		Fila_mon=6;
		mitad_f=0;
		NO_GRAF=0;
	}
	if(pulso>=70 && pulso<80)
	{
		Fila_mon=6;
		mitad_f=1;
		NO_GRAF=0;
	}
	if(pulso>=80 && pulso<90)
	{
		Fila_mon=5;
		mitad_f=0;
		NO_GRAF=0;
	}
	if(pulso>=90 && pulso<100)
	{
		Fila_mon=5;
		mitad_f=1;
		NO_GRAF=0;
	}
	if(pulso>=100 && pulso<110)
	{
		Fila_mon=4;
		mitad_f=0;
		NO_GRAF=0;
	}
	if(pulso>=110 && pulso<120)
	{
		Fila_mon=4;
		mitad_f=1;
		NO_GRAF=0;
	}
	if(pulso>=120 && pulso<130)
	{
		Fila_mon=3;
		mitad_f=0;
		NO_GRAF=0;
	}
	if(pulso>=130 && pulso<140)
	{
		Fila_mon=3;
		mitad_f=1;
		NO_GRAF=0;
	}
	if(pulso>=140 && pulso<150)
	{
		Fila_mon=2;
		mitad_f=0;
		NO_GRAF=0;
	}
	if(pulso>=150 && pulso<160)
	{
		Fila_mon=2;
		mitad_f=1;
		NO_GRAF=0;
	}
	if(pulso>160||pulso<60)
		NO_GRAF=1;
	if(!NO_GRAF)
	{
		pulso_env=pulso;
		func_punto (pulso_env,mitad_f);											// Posiciona el punto en pantalla
		analizo_salto(Fila_mon,mitad_f);										// Función que completa en caso de salto entre puntos
		pos_xg++;																// incrementa una posición
		if(pos_xg>=120)															// Si llega al tope del display, vuelve
			pos_xg=25;
	}
//--------------------------------------------------------------------------
	for(m=1;m<5;m++)														// Borra el display a medida que actualiza la pantalla
	{
		for(n=2;n<7;n++)
		{
			WG12864A_posXY(pos_xg+m,n);
			GLCD_Output_High(RS);   										// Modo datos
			if(pos_xg+m<65)
				GLCD_enviaBYTE(IZQ, 0x00);  								// enciende byte
			else
				GLCD_enviaBYTE(DER, 0x00);  								// enciende byte
		}
	}
//--------------------------------------------------------------------------
	if(!NO_GRAF)
	{
		WG12864A_posXY(pos_xg,Fila_mon);											// Escribe el punto en pantalla
		GLCD_Output_High(RS);   												// Modo datos
		if(pos_xg<65)
			GLCD_enviaBYTE(IZQ, (pnt));  										// enciende byte
		else
			GLCD_enviaBYTE(DER, (pnt));  										// enciende byte
	pnt_ant=pnt;
	Fila_ant=Fila_mon;
	mitad_ant=mitad_f;
	}
}
// Posiciona el punto dentro de la fila segun parte alta o baja
//--------------------------------------------------------------------------
void func_punto (char punto,char mitad)
{
	Graf_punt = ((punto)  % 10);											// Tomo la unidad
	switch(Graf_punt)														// Elijo el punto a representar
	{
	case 0:
	case 1:
	case 2:
		pnt=0x80;
		break;
	case 3:
	case 4:
		pnt=0x40;
		break;
	case 5:
	case 6:
		pnt=0x20;
		break;
	case 7:
	case 8:
	case 9:
		pnt=0x10;
		break;
	default:
		break;
	}
	if(mitad)																// Si es la mitad alta de la fila desplazo
		pnt=pnt>>4;
}
/*
* 	Fila
*
*	x	1
*	x	2
*	x	4	BYTE 0
*	x	8___________
*	x	1
*	x	2
*	x	4	BYTE 1
*	x	8
**/

//	Completa grafica entre puntos
//--------------------------------------------------------------------------
void analizo_salto(char Fila_act,char mitad_act)
{
	char pnt_rell,pnt_aux,pnt_cast,entro_rell,z_salto,x_salto;
	if(Fila_act==Fila_ant)													// Misma fila
	{
		if(mitad_act!=mitad_ant)											// Distinta mitad
		{
			if(mitad_act>mitad_ant)											// Completa hacia arriba
			{
				pnt_rell=0x80;												// Posición mas baja
				z_salto=0;
				while(pnt_ant!=pnt_rell && z_salto<8)								// Busca el punto anterior para tener de inicio
				{
					pnt_rell=pnt_rell>>1;
					z_salto++;
				}
				entro_rell=0;
				if(pnt_ant==pnt_rell)										// Si lo encontró...
				{
					pnt_rell=pnt_rell>>1;									// Se desplaza 1
					pos_xg++;
					entro_rell=1;											// Hab relleno
				}
				x_salto=0;
				pnt_aux=pnt_rell;											// Valor origen
				while((pnt!=pnt_aux )&& x_salto<8 && entro_rell)					// Mientras no llegue al punto actual...
				{
					WG12864A_posXY(pos_xg,Fila_ant);							// Escribe el punto en pantalla
					GLCD_Output_High(RS);   								// Modo datos
					if(pos_xg<65)
						GLCD_enviaBYTE(IZQ, (pnt_rell));  					// enciende byte
					else
						GLCD_enviaBYTE(DER, (pnt_rell));  					// enciende byte
					pnt_aux=pnt_aux>>1;										// Desplaza para ver si llega
					pnt_rell=pnt_rell|(pnt_rell>>1);						// Rellena un nuevo punto en la misma columna
					x_salto++;
				}
			}
			else															// Completa hacia abajo
			{
				pnt_rell=0x01;												// Posición mas alta
				z_salto=0;
				while(pnt_ant!=pnt_rell && z_salto<8)								// Busca el punto anterior para tener de inicio
				{
					pnt_rell=pnt_rell<<1;
					z_salto++;
				}
				entro_rell=0;
				if(pnt_ant==pnt_rell)										// Si lo encontró
				{
					pnt_rell=pnt_rell<<1;									// Se desplaza 1
					pos_xg++;
					entro_rell=1;											// Hab relleno
				}
				x_salto=0;
				pnt_aux=pnt_rell;											// Valor origen
				while((pnt!=pnt_aux ) && x_salto<8 && entro_rell)					// Mientras no llegue al punto actual...
				{
					WG12864A_posXY(pos_xg,Fila_ant);							// Escribe el punto en pantalla
					GLCD_Output_High(RS);   								// Modo datos
					if(pos_xg<65)
						GLCD_enviaBYTE(IZQ, (pnt_rell));  					// enciende byte
					else
						GLCD_enviaBYTE(DER, (pnt_rell));  					// enciende byte
					pnt_aux=pnt_aux<<1;										// Desplaza para ver si llega
					pnt_rell=pnt_rell|(pnt_rell<<1);						// Rellena un mismo punto en la misma columna
					x_salto++;
				}
			}
		}
		else																// Misma fila misma mitad
		{

		}
	}
	else																	// Si estan en distinta fila (SOLO SIRVE PARA DIF DE 1 FILA)
	{
		if(Fila_act<Fila_ant)
		{
			pnt_rell=0x80;													// Posición mas baja
			z_salto=0;
			while(pnt_ant!=pnt_rell && z_salto<8)									// Busca el punto anterior para tener de inicio
			{
				pnt_rell=pnt_rell>>1;
				z_salto++;
			}
			entro_rell=0;
			if(pnt_ant==pnt_rell)											// Si lo encontró...
			{
				pnt_rell=pnt_rell>>1;										// Se desplaza 1
				pos_xg++;
				entro_rell=1;												// Hab relleno
			}
			x_salto=0;
			pnt_aux=pnt_rell;												// Valor origen
			while((pnt!=pnt_aux )&& x_salto<8 && entro_rell)						// Mientras no llegue al punto actual...
			{

				WG12864A_posXY(pos_xg,Fila_ant);								// Escribe el punto en pantalla
				GLCD_Output_High(RS);   									// Modo datos
				if(pos_xg<65)
					GLCD_enviaBYTE(IZQ, (pnt_rell));  						// enciende byte
				else
					GLCD_enviaBYTE(DER, (pnt_rell));  						// enciende byte
				pnt_cast=pnt_rell&0x01;
				if(pnt_cast==0x01)
				{
					Fila_ant--;
					pnt_aux=0x80;
					pnt_rell=0x80;
//****************
					if(Fila_act<Fila_ant)
					{
						WG12864A_posXY(pos_xg,Fila_ant);						// Escribe el punto en pantalla
						GLCD_Output_High(RS);   							// Modo datos
						if(pos_xg<65)
							GLCD_enviaBYTE(IZQ, (0xFF));  					// enciende byte
						else
							GLCD_enviaBYTE(DER, (0xFF));  					// enciende byte

					}
//****************
				}
				else
				{
					pnt_aux=pnt_aux>>1;										// Desplaza para ver si llega
					pnt_rell=pnt_rell|(pnt_rell>>1);						// Rellena un nuevo punto en la misma columna
					x_salto++;
				}
			}
		}
		if(Fila_act>Fila_ant)
		{
			pnt_rell=0x01;													// Posición mas alta
			z_salto=0;
			while(pnt_ant!=pnt_rell && z_salto<8)									// Busca el punto anterior para tener de inicio
			{
				pnt_rell=pnt_rell<<1;
				z_salto++;
			}
			entro_rell=0;
			if(pnt_ant==pnt_rell)											// Si lo encontró
			{
				pnt_rell=pnt_rell<<1;										// Se desplaza 1
				pos_xg++;
				entro_rell=1;												// Hab relleno
			}
			x_salto=0;
			pnt_aux=pnt_rell;												// Valor origen
			while((pnt!=pnt_aux ) && x_salto<8 && entro_rell)						// Mientras no llegue al punto actual...
			{
				WG12864A_posXY(pos_xg,Fila_ant);								// Escribe el punto en pantalla
				GLCD_Output_High(RS);   									// Modo datos
				if(pos_xg<65)
					GLCD_enviaBYTE(IZQ, (pnt_rell));  						// enciende byte
				else
					GLCD_enviaBYTE(DER, (pnt_rell));  						// enciende byte
				pnt_cast=pnt_rell&0x80;
				if(pnt_cast==0x80)
				{
					Fila_ant++;
					pnt_aux=0x01;
					pnt_rell=0x01;
//****************
					if(Fila_act>Fila_ant)
					{
						WG12864A_posXY(pos_xg,Fila_ant);						// Escribe el punto en pantalla
						GLCD_Output_High(RS);   							// Modo datos
						if(pos_xg<65)
							GLCD_enviaBYTE(IZQ, (0xFF));  					// enciende byte
						else
							GLCD_enviaBYTE(DER, (0xFF));  					// enciende byte

					}
//****************

				}
				else
				{
					pnt_aux=pnt_aux<<1;										// Desplaza para ver si llega
					pnt_rell=pnt_rell|(pnt_rell<<1);						// Rellena un mismo punto en la misma columna
					x_salto++;
				}
			}

		}
	}
}

