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
char			colu=7,ledcoo,pos_x,pnt;
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

void Grafica_monitoreo(char pulso)
{
	char pnt_p,m,n,Fila_mon,pulso_env,mitad_f;
	if(pulso>=60 && pulso<70)
	{
		Fila_mon=6;
		mitad_f=0;
	}
	if(pulso>=70 && pulso<80)
	{
		Fila_mon=6;
		mitad_f=1;
	}
	if(pulso>=80 && pulso<90)
	{
		Fila_mon=5;
		mitad_f=0;
	}
	if(pulso>=90 && pulso<100)
	{
		Fila_mon=5;
		mitad_f=1;
	}
	if(pulso>=100 && pulso<110)
	{
		Fila_mon=4;
		mitad_f=0;
	}
	if(pulso>=110 && pulso<120)
	{
		Fila_mon=4;
		mitad_f=1;
	}
	if(pulso>=120 && pulso<130)
	{
		Fila_mon=3;
		mitad_f=0;
	}
	if(pulso>=130 && pulso<140)
	{
		Fila_mon=3;
		mitad_f=1;
	}
	if(pulso>=140 && pulso<=150)
	{
		Fila_mon=2;
		mitad_f=0;
	}
	if(pulso>=150 && pulso<=160)
	{
		Fila_mon=2;
		mitad_f=1;
	}
	pulso_env=pulso;
	pnt_p=func_punto (pulso_env,mitad_f);														// Posiciona el punto en pantalla

	pos_x++;															// incrementa una posición
	if(pos_x>=120)
	{	pos_x=25;
//		pos_y=4;
	}

//---------------------------------------------------------
	for(m=1;m<5;m++)													// Borra el display a medida que actualiza la pantalla
	{
		for(n=2;n<7;n++)
		{
			WG12864A_posXY(pos_x+m,n);
			GLCD_Output_High(RS);   // Modo datos
			if(pos_x+m<65)
				GLCD_enviaBYTE(IZQ, 0x00);  // enciende byte
			else
				GLCD_enviaBYTE(DER, 0x00);  // enciende byte
		}
	}
//---------------------------------------------------------
	WG12864A_posXY(pos_x,Fila_mon);										// Escribe el punto en pantalla
	GLCD_Output_High(RS);   // Modo datos
	if(pos_x<65)
		GLCD_enviaBYTE(IZQ, (pnt));  // enciende byte
	else
		GLCD_enviaBYTE(DER, (pnt));  // enciende byte

}
char func_punto (char punto,char mitad)
{

//	Graf_punt=punto&0x1;
	Graf_punt = ((punto)  % 10);// + '0' ;
	switch(Graf_punt)
	{
	case 0:
	case 1:
	case 2:
		pnt=0x8;
		break;
	case 3:
	case 4:
		pnt=0x4;
		break;
	case 5:
	case 6:
		pnt=0x2;
		break;
	case 7:
	case 8:
	case 9:
		pnt=0x1;
		break;
	default:
		break;
	}
	if(!mitad)
		pnt=pnt<<1;
	return pnt;
}

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
