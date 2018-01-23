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
 * Funciones particulares de cada pantalla del display
****************************************************************************/

#include <stdio.h>
#include "gpio.h"
#include "Subfunciones.h"
#include "WG12864A.h"
#include "Definiciones.h"
#include "Comunicaciones.h"

//--------------------------------------------------------------------------
//***********************  Variables Propias  ******************************
char			colu=7,ledcoo,pos_x,pnt,pnt_ant,Fila_ant,mitad_ant,pulso_ant;
int 			h,mi,seg,d,m,a,aa,aaa,aaaa,anio,ds,
				ef,pl,e,ee,eee,eeee,dets,esta,estf,
				proxf[8],proxh[4];
unsigned short 	aux=0, aux3=0,err_mod=0, err_mod2=0;
char Graf_punt;
//--------------------------------------------------------------------------
//***********************  Variables Externas  *****************************
extern char 	cantmp,cantver,cantprt,cint,menuactual,flagmm,sumaerr,
				Rx[],btens[],toffset[],movmp[],causaerr[],causaerror[];
extern char		reclq,reclq1,reclq2,reclq3,mtok;
//--------------------------------------------------------------------------

// Toma el PPM obtenido, cada fila tiene 20 valores, calcula fila a usar
//--------------------------------------------------------------------------
void Grafica_monitoreo(char pulso)
{
	char m,n,Fila_mon,pulso_env,mitad_f,NO_GRAF;
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
		pos_x++;																// incrementa una posición
		if(pos_x>=120)															// Si llega al tope del display, vuelve
			pos_x=25;
	}
//--------------------------------------------------------------------------
	for(m=1;m<5;m++)														// Borra el display a medida que actualiza la pantalla
	{
		for(n=2;n<7;n++)
		{
			WG12864A_posXY(pos_x+m,n);
			GLCD_Output_High(RS);   										// Modo datos
			if(pos_x+m<65)
				GLCD_enviaBYTE(IZQ, 0x00);  								// enciende byte
			else
				GLCD_enviaBYTE(DER, 0x00);  								// enciende byte
		}
	}
//--------------------------------------------------------------------------
	if(!NO_GRAF)
	{
		WG12864A_posXY(pos_x,Fila_mon);											// Escribe el punto en pantalla
		GLCD_Output_High(RS);   												// Modo datos
		if(pos_x<65)
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
	char pnt_rell,pnt_aux,pnt_cast,entro_rell,z,x;
	if(Fila_act==Fila_ant)													// Misma fila
	{
		if(mitad_act!=mitad_ant)											// Distinta mitad
		{
		if(mitad_act>mitad_ant)												// Completa hacia arriba
		{
			pnt_rell=0x80;													// Posición mas baja
			z=0;
			while(pnt_ant!=pnt_rell && z<8)									// Busca el punto anterior para tener de inicio
			{
				pnt_rell=pnt_rell>>1;
				z++;
			}
			entro_rell=0;
			if(pnt_ant==pnt_rell)											// Si lo encontró...
			{
				pnt_rell=pnt_rell>>1;										// Se desplaza 1
				pos_x++;
				entro_rell=1;												// Hab relleno
			}
			x=0;
			pnt_aux=pnt_rell;												// Valor origen
			while((pnt!=pnt_aux )&& x<8 && entro_rell)						// Mientras no llegue al punto actual...
			{
				WG12864A_posXY(pos_x,Fila_ant);								// Escribe el punto en pantalla
				GLCD_Output_High(RS);   									// Modo datos
				if(pos_x<65)
					GLCD_enviaBYTE(IZQ, (pnt_rell));  						// enciende byte
				else
					GLCD_enviaBYTE(DER, (pnt_rell));  						// enciende byte
				pnt_aux=pnt_aux>>1;											// Desplaza para ver si llega
				pnt_rell=pnt_rell|(pnt_rell>>1);							// Rellena un nuevo punto en la misma columna
				x++;
			}
		}
		else																// Completa hacia abajo
		{
			pnt_rell=0x01;													// Posición mas alta
			z=0;
			while(pnt_ant!=pnt_rell && z<8)									// Busca el punto anterior para tener de inicio
			{
				pnt_rell=pnt_rell<<1;
				z++;
			}
			entro_rell=0;
			if(pnt_ant==pnt_rell)											// Si lo encontró
			{
				pnt_rell=pnt_rell<<1;										// Se desplaza 1
				pos_x++;
				entro_rell=1;												// Hab relleno
			}
			x=0;
			pnt_aux=pnt_rell;												// Valor origen
			while((pnt!=pnt_aux ) && x<8 && entro_rell)						// Mientras no llegue al punto actual...
			{
				WG12864A_posXY(pos_x,Fila_ant);								// Escribe el punto en pantalla
				GLCD_Output_High(RS);   									// Modo datos
				if(pos_x<65)
					GLCD_enviaBYTE(IZQ, (pnt_rell));  						// enciende byte
				else
					GLCD_enviaBYTE(DER, (pnt_rell));  						// enciende byte
				pnt_aux=pnt_aux<<1;											// Desplaza para ver si llega
				pnt_rell=pnt_rell|(pnt_rell<<1);							// Rellena un mismo punto en la misma columna
				x++;
			}

		}

		}
	}
	else																	// Si estan en distinta fila
	{
		if(Fila_act<Fila_ant)
		{
			pnt_rell=0x80;													// Posición mas baja
			z=0;
			while(pnt_ant!=pnt_rell && z<8)									// Busca el punto anterior para tener de inicio
			{
				pnt_rell=pnt_rell>>1;
				z++;
			}
			entro_rell=0;
			if(pnt_ant==pnt_rell)											// Si lo encontró...
			{
				pnt_rell=pnt_rell>>1;										// Se desplaza 1
				pos_x++;
				entro_rell=1;												// Hab relleno
			}
			x=0;
			pnt_aux=pnt_rell;												// Valor origen
			while((pnt!=pnt_aux )&& x<8 && entro_rell)						// Mientras no llegue al punto actual...
			{

				WG12864A_posXY(pos_x,Fila_ant);								// Escribe el punto en pantalla
				GLCD_Output_High(RS);   									// Modo datos
				if(pos_x<65)
					GLCD_enviaBYTE(IZQ, (pnt_rell));  						// enciende byte
				else
					GLCD_enviaBYTE(DER, (pnt_rell));  						// enciende byte
				pnt_cast=pnt_rell&0x01;
				if(pnt_cast==0x01)
				{
					Fila_ant--;
					pnt_aux=0x80;
					pnt_rell=0x80;
				}
				else
				{
					pnt_aux=pnt_aux>>1;										// Desplaza para ver si llega
					pnt_rell=pnt_rell|(pnt_rell>>1);						// Rellena un nuevo punto en la misma columna
					x++;
				}
			}
		}
		if(Fila_act>Fila_ant)
		{
			pnt_rell=0x01;													// Posición mas alta
			z=0;
			while(pnt_ant!=pnt_rell && z<8)									// Busca el punto anterior para tener de inicio
			{
				pnt_rell=pnt_rell<<1;
				z++;
			}
			entro_rell=0;
			if(pnt_ant==pnt_rell)											// Si lo encontró
			{
				pnt_rell=pnt_rell<<1;										// Se desplaza 1
				pos_x++;
				entro_rell=1;												// Hab relleno
			}
			x=0;
			pnt_aux=pnt_rell;												// Valor origen
			while((pnt!=pnt_aux ) && x<8 && entro_rell)						// Mientras no llegue al punto actual...
			{
				WG12864A_posXY(pos_x,Fila_ant);								// Escribe el punto en pantalla
				GLCD_Output_High(RS);   									// Modo datos
				if(pos_x<65)
					GLCD_enviaBYTE(IZQ, (pnt_rell));  						// enciende byte
				else
					GLCD_enviaBYTE(DER, (pnt_rell));  						// enciende byte
				pnt_cast=pnt_rell&0x80;
				if(pnt_cast==0x80)
				{
					Fila_ant++;
					pnt_aux=0x01;
					pnt_rell=0x01;
				}
				else
				{
					pnt_aux=pnt_aux<<1;											// Desplaza para ver si llega
					pnt_rell=pnt_rell|(pnt_rell<<1);							// Rellena un mismo punto en la misma columna
					x++;
				}
			}

		}
	}
}
/*
void analizo_salto(char fila_act,char mitad_act)
{
	char w,result,May_act,func,pnt_rell,No_rell;
	No_rell=0;
	if(fila_act==Fila_ant)													// Si esta en la misma fila
	{
		if(mitad_act==mitad_ant)											// Si esta en la misma mitad
		{
			if(pnt>pnt_ant)													// Si el pnt act es mayor al anterior
			{
				result=pnt-pnt_ant;
				May_act=1;
			}
			else
			{
				result=pnt_ant-pnt;
				May_act=0;
			}
			switch(result)
			{
			case 2:
				if(May_act)
					func=pnt_ant;
				else
					func=pnt;
				switch(func)
				{
				case 1:
				case 2:
					pnt_rell=0xC0;
					break;
				case 3:
				case 4:
					pnt_rell=0x60;
					break;
				case 5:
				case 6:
					pnt_rell=0x30;
					break;
				default:
					No_rell=1;
					break;
				}
				break;
			case 3:
				break;
			case 4:
				break;
			case 5:
				break;
			case 6:
				break;
			case 8:
				break;
			case 9:
				break;
			case 0:
			case 1:
			default:
				break;
			}
		}
		else
		{
			if(mitad_act<mitad_ant)
			{

			}
			else
			{

			}
		}
	}
	else
	{
		if(fila_act>Fila_ant)
		{
			w=0;
			while(Fila_ant+w<fila_act)
			{
				w++;
			}
		}
		else
		{
			w=0;
			while(Fila_ant-w>fila_act)
			{
				w++;
			}

		}
	}
	if(!No_rell)
	{
		WG12864A_posXY(pos_x,Fila_ant);										// Escribe el punto en pantalla
		GLCD_Output_High(RS);   											// Modo datos
		if(pos_x<65)
			GLCD_enviaBYTE(IZQ, (pnt_rell));  								// enciende byte
		else
			GLCD_enviaBYTE(DER, (pnt_rell));  								// enciende byte
	}
}
*/
//						  Muestra fecha y hora
//--------------------------------------------------------------------------
void muestra_fecha_y_hora (int 	fila )
{
	static char  	buffer[44] ;
	static char  	buffe[44] ;
	char *		renglon = buffer ;
	char *		renglo = buffe ;

	*renglon++ = ((h/10)  % 10) + '0' ;
	*renglon++ = ( h      % 10) + '0' ;
	*renglon++ = ':' ;
	*renglon++ = ((mi/10)  % 10) + '0' ;
	*renglon++ = ( mi      % 10) + '0' ;
	*renglon++ = ':' ;
	*renglon++ = ((seg/10)  % 10) + '0' ;
	*renglon++ = ( seg      % 10) + '0' ;
	*renglon++ = ' ' ;
	*renglon++ = ' ' ;
	*renglon++ = ((d/10)  % 10) + '0' ;
	*renglon++ = ( d      % 10) + '0' ;
	*renglon++ = '/' ;
	*renglon++ = ((m/10)  % 10) + '0' ;
	*renglon++ = ( m      % 10) + '0' ;
	*renglon++ = '/' ;
	*renglon++ = a ;
	*renglon++ = aa ;
	*renglon++ = aaa ;
	*renglon++ = aaaa ;

	WG12864A_posXY(1,fila);
	WG12864A_printf(buffer, Arial8x6, NEGRO);
}
//						  Muestra día de la semana
//--------------------------------------------------------------------------
void muestra_dsemana(unsigned int FF,int columna,int fila)
{
	static char  	buffer[44] ;

	switch (FF)
	{
	case 1:
			WG12864A_posXY(columna,fila);
			WG12864A_printf("Dom", Arial8x6, NEGRO);
			break;
	case 2:
			WG12864A_posXY(columna,fila);
			WG12864A_printf("Lun", Arial8x6, NEGRO);
			break;
	case 3:
			WG12864A_posXY(columna,fila);
			WG12864A_printf("Mar", Arial8x6, NEGRO);
			break;
	case 4:
			WG12864A_posXY(columna,fila);
			WG12864A_printf("Mie", Arial8x6, NEGRO);
			break;
	case 5:
			WG12864A_posXY(columna,fila);
			WG12864A_printf("Jue", Arial8x6, NEGRO);
			break;
	case 6:
			WG12864A_posXY(columna,fila);
			WG12864A_printf("Vie", Arial8x6, NEGRO);
			break;
	case 7:
			WG12864A_posXY(columna,fila);
			WG12864A_printf("Sab", Arial8x6, NEGRO);
			break;
	default:
//			sprintf (buffer, "%02d",FF); // No se usa sprintf para ahorrar espacio de compilación//Si no  es un día valido muestra el hexa guardado
			WG12864A_posXY(columna,fila);
			WG12864A_printf(buffer, Arial8x6, NEGRO);
			break;
	}
}
//						  Muestra en menu 3.2 la nueva fecha
//--------------------------------------------------------------------------

void prox_fecha (int 	fila )
{
	static char  	buffer[44] ;
	char *		renglon = buffer ;

	*renglon++ = ' ' ;
	*renglon++ = ' ' ;
	*renglon++ = ((proxf[0]/10)  % 10) + '0' ;
	*renglon++ = ( proxf[0]      % 10) + '0' ;
	*renglon++ = ' ' ;
	*renglon++ = '/' ;
	*renglon++ = ' ' ;
	*renglon++ = ((proxf[1]/10)  % 10) + '0' ;
	*renglon++ = ( proxf[1]      % 10) + '0' ;
	*renglon++ = ' ' ;
	*renglon++ = '/' ;
	*renglon++ = ' ' ;
	*renglon++ = ((proxf[2]/1000)% 10) + '0' ;
	*renglon++ = ((proxf[2]/100) % 10) + '0' ;
	*renglon++ = ((proxf[2]/10)  % 10) + '0' ;
	*renglon++ = ( proxf[2]      % 10) + '0' ;

	WG12864A_posXY(1,fila);
	WG12864A_printf(buffer, Arial8x6, NEGRO);
}
//						  Muestra en menu 3.2 la nueva hora
//--------------------------------------------------------------------------

void prox_hora (int 	fila )
{
	static char  	buffer[44] ;
	char *		renglon = buffer ;

	*renglon++ = ' ' ;
	*renglon++ = ' ' ;
	*renglon++ = ((proxh[0]/10)  % 10) + '0' ;
	*renglon++ = ( proxh[0]      % 10) + '0' ;
	*renglon++ = ' ' ;
	*renglon++ = ':' ;
	*renglon++ = ' ' ;
	*renglon++ = ((proxh[1]/10)  % 10) + '0' ;
	*renglon++ = ( proxh[1]      % 10) + '0' ;
	*renglon++ = ' ' ;
	*renglon++ = ':' ;
	*renglon++ = ' ' ;
	*renglon++ = ((proxh[2]/10)  % 10) + '0' ;
	*renglon++ = ( proxh[2]      % 10) + '0' ;

	WG12864A_posXY(1,fila);
	WG12864A_printf(buffer, Arial8x6, NEGRO);
}
