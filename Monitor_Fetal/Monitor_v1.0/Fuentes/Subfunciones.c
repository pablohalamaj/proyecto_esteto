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
#include <stdio.h>
#include "gpio.h"
#include "Subfunciones.h"
#include "WG12864A.h"
#include "Definiciones.h"
#include "Comunicaciones.h"

//--------------------------------------------------------------------------
//***********************  Variables Propias  ******************************
char			colu=7,ledcoo;
int 			h,mi,seg,d,m,a,aa,aaa,aaaa,anio,ds,
				ef,pl,e,ee,eee,eeee,dets,esta,estf,
				proxf[8],proxh[4];
unsigned short 	aux=0, aux3=0,err_mod=0, err_mod2=0;

//--------------------------------------------------------------------------
//***********************  Variables Externas  *****************************
extern char 	cantmp,cantver,cantprt,cint,menuactual,flagmm,sumaerr,
				Rx[],btens[],movmp[],causaerr[],causaerror[];
extern char		reclq,reclq1,reclq2,reclq3,mtok;
//--------------------------------------------------------------------------
/****************************  Subfunciones 1  ****************************/
//--------------------------------------------------------------------------

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
//*************************************************	Prueba SACAR
/*	*renglo++ = ((reclq/10)  % 10) + '0' ;
	*renglo++ = ( reclq      % 10) + '0' ;
	*renglo++ = ' ' ;
	*renglo++ = ' ' ;
	*renglo++ = ((reclq1/10)  % 10) + '0' ;
	*renglo++ = ( reclq1      % 10) + '0' ;
	*renglo++ = ' ' ;
	*renglo++ = ' ' ;
	*renglo++ = ((reclq2/10)  % 10) + '0' ;
	*renglo++ = ( reclq2      % 10) + '0' ;
	*renglo++ = ' ' ;
	*renglo++ = ' ' ;
	*renglo++ = ((reclq3/10)  % 10) + '0' ;
	*renglo++ = ( reclq3      % 10) + '0' ;

	WG12864A_posXY(1,6);
	WG12864A_printf(buffe, Arial8x6, NEGRO);
*/
	//*************************************************
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

//						  Muestra comunicaciones
//--------------------------------------------------------------------------
void 	muestra_comunic(void)
{
	int fila=5,col=1;
	switch (ledcoo)
	{
	case 1:
		WG12864A_posXY(col, fila);
		WG12864A_printf("**P:** F:* k:*", Arial8x6, NEGRO);
		break;
	case 2:
		WG12864A_posXY(col, fila);
		WG12864A_printf("OKP:00 F:0 k:0", Arial8x6, NEGRO);
		break;
	default:
		break;

	}
}
//						  Muestra detectores
//--------------------------------------------------------------------------
void 	muestra_det(void)
{
	int x=0,t=0,fila=5,col=1,detsm;

//	dets=0xf<<4;//-----------------------------------------------------------SACAR

	WG12864A_posXY(col, fila);
	WG12864A_printf("Det:", Arial8x6, NEGRO);
	x=0;
	while(x<2)
	{
		if(x==0)
		{
			t=25;
			detsm=(dets>>4)&0xf;
		}
		if(x==1)
		{
			t=55;
			detsm=(dets&0xf);
		}
		x++;
		switch (detsm)
		{
/*			case 0x0:
				WG12864A_posXY(t,fila);
				WG12864A_printf(" 1111", Arial8x6, NEGRO);
				break;
*/			case 0x1:
				WG12864A_posXY(t,fila);
				WG12864A_printf(" 111_", Arial8x6, NEGRO);
				break;
			case 0x2:
				WG12864A_posXY(t,fila);
				WG12864A_printf(" 11_1", Arial8x6, NEGRO);
				break;
			case 0x3:
				WG12864A_posXY(t,fila);
				WG12864A_printf(" 11__", Arial8x6, NEGRO);
				break;
			case 0x4:
				WG12864A_posXY(t,fila);
				WG12864A_printf(" 1_11", Arial8x6, NEGRO);
				break;
			case 0x5:
				WG12864A_posXY(t,fila);
				WG12864A_printf(" 1_1_", Arial8x6, NEGRO);
				break;
			case 0x6:
				WG12864A_posXY(t,fila);
				WG12864A_printf(" 1__1", Arial8x6, NEGRO);
				break;
			case 0x7:
				WG12864A_posXY(t,fila);
				WG12864A_printf(" 1___", Arial8x6, NEGRO);
				break;
			case 0x8:
				WG12864A_posXY(t,fila);
				WG12864A_printf(" _111", Arial8x6, NEGRO);
				break;
			case 0x9:
				WG12864A_posXY(t,fila);
				WG12864A_printf(" _11_", Arial8x6, NEGRO);
				break;
			case 0xa:
				WG12864A_posXY(t,fila);
				WG12864A_printf(" _1_1", Arial8x6, NEGRO);
				break;
			case 0xb:
				WG12864A_posXY(t,fila);
				WG12864A_printf(" _1__", Arial8x6, NEGRO);
				break;
			case 0xc:
				WG12864A_posXY(t,fila);
				WG12864A_printf(" __11", Arial8x6, NEGRO);
				break;
			case 0xd:
				WG12864A_posXY(t,fila);
				WG12864A_printf(" __1_", Arial8x6, NEGRO);
				break;
			case 0xe:
				WG12864A_posXY(t,fila);
				WG12864A_printf(" ___1", Arial8x6, NEGRO);
				break;
			case 0xf:
				WG12864A_posXY(t,fila);
				WG12864A_printf(" ____", Arial8x6, NEGRO);
				break;
			default:
				break;
		}
	}
}
//						  Muestra demandas
//--------------------------------------------------------------------------
void 	muestra_dem(void)
{
	int fila=5,col=1;

	WG12864A_posXY(col, fila);
	WG12864A_printf("DEM: ---- ----", Arial8x6, NEGRO);
}

//						  Muestra tensión
//--------------------------------------------------------------------------
void 	muestra_tens(void)
{
	int i=0,fila=5,colut=1;
	for(i=0;i<14;i++)
	{
		WG12864A_posXY(colut, fila);
		WG12864A_printf(&btens[i], Arial8x6, NEGRO);
		colut+=6;

	}
}

//						  Muestra estados
//--------------------------------------------------------------------------
void muestra_estados (int		fila )
{
	static char	buffer[100] ;
	char *		renglon = buffer ;

	*renglon++ = 'T' ;
	*renglon++ = ':' ;
	*renglon++ = e ;
	*renglon++ = ee ;
	*renglon++ = eee ;
	*renglon++ = '.' ;
	*renglon++ = eeee ;
	*renglon++ = ' ' ;
	*renglon++ = 'E' ;
	*renglon++ = 's' ;
	*renglon++ = 't' ;
	*renglon++ = ':' ;
	if(esta==0)
		*renglon++ = '-' ;
	else
		*renglon++ = esta + '0' ;
	if(estf==0)
		*renglon++ = '-' ;
	else
		*renglon++ = estf + '0' ;
	*renglon++ = ' ' ;
	*renglon++ = 'P' ;
	*renglon++ = 'l' ;
	*renglon++ = 'a' ;
	*renglon++ = 'n' ;
	*renglon++ = ':' ;
	*renglon++ = pl + '0' ;
	*renglon++ = '\0' ;

	WG12864A_posXY(1,fila);
	WG12864A_printf(buffer, Arial8x6, NEGRO);
	switch(ef)
	{
	case 1:
		WG12864A_posXY(12,6);
		WG12864A_printf("MODO INICIAL", Arial8x6, NEGRO);
		break;
	case 2:
		WG12864A_posXY(12,6);
		WG12864A_printf("            ", Arial8x6, NEGRO); 					// MODO NORMAL (No lo muestra)
		break;
	case 3:
		WG12864A_posXY(12,6);
		WG12864A_printf("TITILANTE HW", Arial8x6, NEGRO);
		break;
	case 4:
		WG12864A_posXY(12,6);
		WG12864A_printf("TITILANTE SW", Arial8x6, NEGRO);
		break;
	case 5:
		WG12864A_posXY(12,6);
		WG12864A_printf("CONFLICTO   ", Arial8x6, NEGRO);
		break;
	case 6:
		WG12864A_posXY(12,6);
		WG12864A_printf("APAGADO SW  ", Arial8x6, NEGRO);
		break;
	case 7:
		WG12864A_posXY(12,6);
		WG12864A_printf("BAJA TENSION", Arial8x6, NEGRO);
		break;
	case 8:
		WG12864A_posXY(12,6);
		WG12864A_printf("EMERGENCIA  ", Arial8x6, NEGRO);
		break;
	case 9:
		WG12864A_posXY(12,6);
		WG12864A_printf("ERR	     ", Arial8x6, NEGRO);
		break;

	default:
		WG12864A_posXY(12,6);
		WG12864A_printf(" N C ...    ", Arial8x6, NEGRO);
		break;
	}
}

//						  Muestra estados
//--------------------------------------------------------------------------
void muestra_modpot(void)													// Muestra el estado real de cada módulo de potencia
{
	char t,aux,fila=7;
	colu=7;
	for(t=0;t<cantmp;t++)
	{
		if(t<4)																// Si tiene mas de 4 vov muestra en la fila 8
			fila=7;
		else
			fila=8;

		WG12864A_posXY(colu,fila);
		aux=movmp[t]&0xFF;
		switch (aux)
		{
		case 0x20:
				WG12864A_printf("R-T", Arial8x6, NEGRO);
				break;
		case 0x30:
				WG12864A_printf("RTI", Arial8x6, NEGRO);
				break;
		case 0x08:
				WG12864A_printf("A-T", Arial8x6, NEGRO);
				break;
		case 0x80:
				WG12864A_printf("RQ ", Arial8x6, NEGRO);
				break;
		case 0x90:
				WG12864A_printf("RQ ", Arial8x6, NEGRO);
				break;
		case 0xD0:
				WG12864A_printf("RQ ", Arial8x6, NEGRO);
				break;
		case 0x40:
				WG12864A_printf("VC ", Arial8x6, NEGRO);
				break;
		case 0x41:
				WG12864A_printf("VC ", Arial8x6, NEGRO);
				break;
		case 0xC0:
				WG12864A_printf("RVQ", Arial8x6, NEGRO);
				break;
		case 0xD1:
				WG12864A_printf("RVQ", Arial8x6, BLANCO);
				break;
		case 0x10:
				WG12864A_printf("ROJ", Arial8x6, NEGRO);
				break;
		case 0x14:
				WG12864A_printf("R-A", Arial8x6, NEGRO);
				break;
		case 0x04:
				WG12864A_printf("AMA", Arial8x6, NEGRO);
				break;
		case 0x01:
				WG12864A_printf("VRD", Arial8x6, NEGRO);
				break;
		case 0x05:
				WG12864A_printf("V-A", Arial8x6, NEGRO);
				break;
		case 0x02:
				WG12864A_printf("V-T", Arial8x6, NEGRO);
				break;
		case 0x0A:
				WG12864A_printf("VAT", Arial8x6, NEGRO);
				break;
		case 0x00:
//				WG12864A_printf("   ", Arial8x6, NEGRO);
				break;
		default:
//				WG12864A_printf("ERR", Arial8x6, NEGRO);
//				err_mod=1;													// Si hay error activo los flag para volver a leer el estado
//				err_mod2=1;													// y salvar el desfasaje que aparece en la lectura despues de err
				break;
		}
		colu+=26;															// Incrementa 30 col (5posx6pixel cada una)
		if(colu>90)															// Si incrementa mas del 4to mov vuelve a la col inicial
			colu=7;
	}
}

//						  Muestra errores
//--------------------------------------------------------------------------
void Muestra_causa(int modu, int cau, int fila)
{
	static char  	buffer[10] ,buff[10];
	char *		renglon = buffer ;
	char *		rengl = buff ;

	*renglon++ = (((sumaerr+1)/10)  % 10) + '0' ;							// Hora
	*renglon++ = ( (sumaerr+1)      % 10) + '0' ;
	*renglon++ = ' ';
	*renglon++ = ' ';
	*renglon++ = ' ';
	*renglon++ = ' ';
	WG12864A_posXY(42,5);
	WG12864A_printf(buffer, Arial8x6, NEGRO);

	if(!(causaerr[9]==0))
	{
		*rengl++ = ((causaerr[9]/10)  % 10) + '0' ;							// Hora
		*rengl++ = ( causaerr[9]      % 10) + '0' ;
		*rengl++ = ' ';
		*rengl++ = ' ';
		*rengl++ = ' ';
		*rengl++ = ' ';
		WG12864A_posXY(42,6);
		WG12864A_printf(buff, Arial8x6, NEGRO);
	}
//	*renglon++ = causaerr[10];
//	WG12864A_posXY(30,6);

//	envia_submenu(modc);
	if(!(causaerr[9]==0))													// Si no hay causa no muestra modulo
	{

	switch (modu)															// Muestra el módulo en conflicto
	{
	case 1:
			WG12864A_posXY(80,fila);
			WG12864A_printf("1", Arial8x6, NEGRO);
			break;
	case 2:
			WG12864A_posXY(80,fila);
			WG12864A_printf("2", Arial8x6, NEGRO);
			break;
	case 3:
			WG12864A_posXY(80,fila);
			WG12864A_printf("3", Arial8x6, NEGRO);
			break;
	case 4:
			WG12864A_posXY(80,fila);
			WG12864A_printf("4", Arial8x6, NEGRO);
			break;
	case 5:
			WG12864A_posXY(80,fila);
			WG12864A_printf("5", Arial8x6, NEGRO);
			break;
	case 6:
			WG12864A_posXY(80,fila);
			WG12864A_printf("6", Arial8x6, NEGRO);
			break;
	case 7:
			WG12864A_posXY(80,fila);
			WG12864A_printf("7", Arial8x6, NEGRO);
			break;
	case 8:
			WG12864A_posXY(80,fila);
			WG12864A_printf("8", Arial8x6, NEGRO);
			break;
	case 0:
	default:
			break;
	}
	switch (causaerr[9])//(cau)															// Muestra la causa del conflicto
	{
	case 1:
			WG12864A_posXY(1,fila);
			WG12864A_printf("Reinicio Cold ", Arial8x6, NEGRO);
			break;
	case 2:
			WG12864A_posXY(1,fila);
			WG12864A_printf("Sin Parametros", Arial8x6, NEGRO);
			break;
	case 5:
			WG12864A_posXY(1,fila);
			WG12864A_printf("Baja Tension  ", Arial8x6, NEGRO);
			break;
	case 6:
			WG12864A_posXY(1,fila);
			WG12864A_printf("Conflicto HW  ", Arial8x6, NEGRO);
			break;
	case 8:
			WG12864A_posXY(1,fila);
			WG12864A_printf("Conflicto SW  ", Arial8x6, NEGRO);
			break;
	case 11:
			WG12864A_posXY(1,fila);
			WG12864A_printf("Conflicto UC  ", Arial8x6, NEGRO);
			break;
	case 12:
			WG12864A_posXY(1,fila);
			WG12864A_printf("Confl ver max ", Arial8x6, NEGRO);
			break;
	case 13:
			WG12864A_posXY(1,fila);
			WG12864A_printf("Confl prog    ", Arial8x6, NEGRO);
			break;
	case 14:
			WG12864A_posXY(1,fila);
			WG12864A_printf("Conflicto UP  ", Arial8x6, NEGRO);
			break;

	case 15://5:
			WG12864A_posXY(1,fila);
			WG12864A_printf("Confl RQ mod:", Arial8x6, NEGRO);
			break;
	case 16://1:
			WG12864A_posXY(1,fila);
			WG12864A_printf("Confl VC mod:", Arial8x6, NEGRO);
			break;
	case 19:
			WG12864A_posXY(1,fila);
			WG12864A_printf("Reinicio HOT  ", Arial8x6, NEGRO);
			break;
	default:
			break;
	}
	}
}
//						  Muestra en menu 1.3 la fyh del error
//--------------------------------------------------------------------------

void muestra_fyh_err 		(int fila)
{
	static char  	buffer[44] ;
	char *			renglon = buffer ;
	int 			anioint=0;
	*renglon++ = ((causaerr[0]/10)  % 10) + '0' ;							// Hora
	*renglon++ = ( causaerr[0]      % 10) + '0' ;
	*renglon++ = ':' ;
	*renglon++ = ((causaerr[1]/10)  % 10) + '0' ;							// Minutos
	*renglon++ = ( causaerr[1]      % 10) + '0' ;
	*renglon++ = ':' ;
	*renglon++ = ((causaerr[2]/10)  % 10) + '0' ;							// Segundos
	*renglon++ = ( causaerr[2]      % 10) + '0' ;
	*renglon++ = ' ' ;
	*renglon++ = ' ' ;
	*renglon++ = ((causaerr[3]/10)  % 10) + '0' ;							// Día
	*renglon++ = ( causaerr[3]      % 10) + '0' ;
	*renglon++ = '/' ;
	*renglon++ = ((causaerr[4]/10)  % 10) + '0' ;							// Mes
	*renglon++ = ( causaerr[4]      % 10) + '0' ;
	*renglon++ = '/' ;
	anioint=((causaerr[5]*1000)+(causaerr[6]*100)+(causaerr[7]*10)+causaerr[8]);
	*renglon++ = ((anioint/1000)% 10) + '0' ;
	*renglon++ = ((anioint/100) % 10) + '0' ;
	*renglon++ = ((anioint/10)  % 10) + '0' ;
	*renglon++ = ( anioint      % 10) + '0' ;
	if(buffer[16]!='0' && buffer[18]!='0')									// Evita ruido en display
	{
		WG12864A_posXY(1,fila);
		WG12864A_printf(buffer, Arial8x6, NEGRO);
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
