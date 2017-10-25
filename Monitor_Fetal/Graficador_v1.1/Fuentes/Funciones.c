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
#include "adc.h"
#include "Touch.h"
#include "timer32.h"
#include "WG12864A.h"
#include "Funciones.h"
#include "Definiciones.h"


int 	Umbral_Sup=3000,Umbral_Inf=2000,
		cont_pico_POS=0,cont_pico_NEG=0,PPM=0,
		inc_ind,inc_ind2,i_ind_buff,
		lat_adc,lat_ant,LAT_PROM;
char 	PULSO_OK,flag_SUP=1,flag_INF=1,
		pos_x,pos_y,PUNTO,AUMENTO,DISMINUYO,
		cont_10seg,FILTRO_DIG,hab_corazon,
		PROMEDIAR,NO_ENTR,ACT_DISP,v,
		ind_adc=0,SUBIR,BAJAR,desp_vert,SALTO;
unsigned int valpru[1000],valpru2[800],ind_Muestras,aux_prov;

//extern int valpru[];
extern char flag_1seg,flag_25ms;
uint16_t leo_adc(char Channel);
//_______________________________________________________________________________________________________________________________

// Muestra Pantalla de Monitoreo
//_______________________________________________________________________________________________________________________________
void Func_Monitoreo (void)
{
	char m=0,n=0;
	//unsigned int valpru2[800];
	// Mientras no se presione Back.
//	while(1)
//	{
	// Titulo del menu.
//	WG12864A_posXY(1, 1);
//	WG12864A_printf(" *  Monitor Fetal  * ", Arial8x6, BLANCO);

		if(flag_25ms)
		{
			flag_25ms=0;
			if(inc_ind<1000)
			{
				valpru[inc_ind]=leo_adc(5);
			}
			else
			{
				valpru2[inc_ind2]=leo_adc(5);
				inc_ind2++;
			}
			inc_ind++;
			aux_prov++;

		}
/*		if(inc_ind>=100)
		{
			inc_ind=0;
			PROMEDIAR=1;
			NO_ENTR=1;
			ACT_DISP=1;
		}
*/		if(flag_1seg)
		{
			flag_1seg=0;
			cont_10seg++;
		}
		if(cont_10seg>=10)
		{
			ind_Muestras=aux_prov;
			aux_prov=0;
			inc_ind=0;
			inc_ind2=0;
			PROMEDIAR=1;
			NO_ENTR=1;
			ACT_DISP=1;
			detector_pulsos();
			cont_10seg=0;
		}
//		Graf_PROM();
//	}
}


//--------------------------------------------------------------------------
//	Detecta Picos + y - del corazón
//--------------------------------------------------------------------------
void detector_pulsos(void)
{
	int i=0,j=0;
	while(i<1000)
	{
		if(valpru[i]>Umbral_Sup && flag_INF)
		{
			flag_SUP=1;
			flag_INF=0;
			cont_pico_POS++;
		}
		if(valpru[i]<Umbral_Inf && flag_SUP)
		{
			flag_SUP=0;
			flag_INF=1;
			cont_pico_NEG++;
		}
		i++;
	}
	while(1000<i &&i<1800)
	{
		if(valpru2[j]>Umbral_Sup && flag_INF)
		{
			flag_SUP=1;
			flag_INF=0;
			cont_pico_POS++;
		}
		if(valpru2[j]<Umbral_Inf && flag_SUP)
		{
			flag_SUP=0;
			flag_INF=1;
			cont_pico_NEG++;
		}
		j++;
	}

	if(cont_pico_POS<cont_pico_NEG+2 && cont_pico_POS>cont_pico_NEG-2)
	{
		PPM=cont_pico_POS*6;
		PULSO_OK=1;
	}
	cont_pico_POS=0;
	cont_pico_NEG=0;
}

//--------------------------------------------------------------------------
//	Grafica promedio de 20 valores cada 100 muestras
//--------------------------------------------------------------------------

void Graf_PROM(void)
{
	char *		renglon = PPM ;
	char n;
	int aux1,aux2,i=0,m;
//-------------------------------------------------------------
	WG12864A_posXY(110, 7);
	WG12864A_print_symbol(BACK16x16, BLANCO);
	WG12864A_posXY(1, 2);
	WG12864A_printf("Max|", Arial8x6, NEGRO);
	WG12864A_posXY(121, 2);
	WG12864A_printf("|", Arial8x6, NEGRO);
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
	WG12864A_printf("Min|", Arial8x6, NEGRO);
	WG12864A_posXY(121, 6);
	WG12864A_printf("|", Arial8x6, NEGRO);
	WG12864A_posXY(1, 7);
	WG12864A_printf("Pulso:", Arial8x6, NEGRO);
	WG12864A_posXY(40, 7);
	WG12864A_printf(PPM, Arial8x6, NEGRO);

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
//			hab_corazon=0;
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


//-------------------------------------------------------------


	lat_ant=LAT_PROM;
/*	if(lat_ant!=LAT_PROM)
	{
		mu_pro[i_buff]=LAT_PROM;
		i_buff++;
		if(i_buff>99)
			i_buff=0;
	}
*/	if(PROMEDIAR&&NO_ENTR)															// Promedia 20 muestras
	{
		PROMEDIAR=0;
		for(i=0;i<10;i++)
//			lat_adc+=valpru[i_ind_buff+i];
		lat_adc=lat_adc/10;
		LAT_PROM=lat_adc;
		//pos_y=4;
//		hab_corazon=1;
		*renglon++ = ((LAT_PROM/1000) % 10) + '0' ;
		*renglon++ = ((LAT_PROM/100)  % 10) + '0' ;
		*renglon++ = ((LAT_PROM/10)   % 10) + '0' ;
		*renglon++ = ( LAT_PROM       % 10) + '0' ;

	}
	i_ind_buff+=10;															// Incrementa 20 pos del buffer para prox promedio
	PROMEDIAR=1;
	if(i_ind_buff>99)
	{
		i_ind_buff=0;
		NO_ENTR=0;
		hab_corazon=1;
	}

	if(ACT_DISP)															// Cada vez que completa el buffer muestra 1 prom
	{
		ACT_DISP=0;
		pos_x++;															// incrementa una posición
		if(pos_x>=120)
		{	pos_x=25;
			pos_y=4;
		}
		hab_corazon=1;
//		aux1=mu_pro[i_disp];
//		aux2=mu_pro[i_disp_ant];
		if(LAT_PROM<lat_ant)// && (LAT_PROM<(lat_ant+5)))		//if(LAT_PROM>(lat_ant))//+5))// Sube o baja dependiendo del resultado del prom
			desp_vert++;
		if(LAT_PROM>lat_ant)// && (LAT_PROM>(lat_ant-5)))		//if(LAT_PROM<(lat_ant))//-5))
			desp_vert--;
/*		if((LAT_PROM>lat_ant) && (LAT_PROM>(lat_ant+5)))
		{
			AUMENTO=1;
			desp_vert+=4;
		}
		if((LAT_PROM<lat_ant) && (LAT_PROM<(lat_ant-5)))
		{
			DISMINUYO=1;
			desp_vert-=4;
		}
*//*		i_disp_ant=i_disp;
		i_disp++;
		if(i_disp>99)
			i_disp=0;
*/		if(desp_vert>8)														// Mantiene dentro de los margenes
		{
			desp_vert=1;
			pos_y--;
		}
		if(desp_vert<1)
		{
			desp_vert=8;
			pos_y++;
		}
		func_punto ();														// Posiciona el punto en pantalla
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
		if(pos_y>6)
		{
			pos_y=6;
			PUNTO=0x80;
			hab_corazon=0;
			desp_vert=1;
		}
		if(pos_y<2)
		{
			pos_y=2;
			PUNTO=0x01;
			hab_corazon=0;
			desp_vert=8;
		}
		WG12864A_posXY(pos_x,pos_y);										// Escribe el punto en pantalla
		GLCD_Output_High(RS);   // Modo datos
		if(pos_x<65)
			GLCD_enviaBYTE(IZQ, (PUNTO));  // enciende byte
		else
			GLCD_enviaBYTE(DER, (PUNTO));  // enciende byte
		if (SALTO!=0)
		{

		switch (SALTO)
		{
		case 1:
			PUNTO=0x80;
			pos_y--;
			SALTO=0;
			break;
		case 2:
			PUNTO=0xC0;
			pos_y--;
			SALTO=0;
			break;
		case 3:
			PUNTO=0xE0;
			pos_y--;
			SALTO=0;
			break;
		case 4:
			PUNTO=0x01;
			pos_y++;
			SALTO=0;
			break;
		case 5:
			PUNTO=0x03;
			pos_y++;
			SALTO=0;
			break;
		case 6:
			PUNTO=0x07;
			pos_y++;
			SALTO=0;
			break;
		default:
			break;
		}
		if(pos_y>6)
		{
			pos_y=6;
			PUNTO=0x80;
			hab_corazon=0;
			desp_vert=1;
		}
		if(pos_y<2)
		{
			pos_y=2;
			PUNTO=0x01;
			hab_corazon=0;
			desp_vert=8;
		}
		WG12864A_posXY(pos_x,pos_y);										// Escribe el punto en pantalla
		GLCD_Output_High(RS);   // Modo datos
		if(pos_x<65)
			GLCD_enviaBYTE(IZQ, (PUNTO));  // enciende byte
		else
			GLCD_enviaBYTE(DER, (PUNTO));  // enciende byte

	}
	}
}
/*
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
void func_punto (void)
{
	PUNTO=0x10;
	switch (desp_vert)
	{
	case 1:
		PUNTO=0x80;
		break;
	case 2:
		PUNTO=0x40;
		break;
	case 3:
		PUNTO=0x20;
		break;
	case 4:
		PUNTO=0x10;
		break;
	case 5:
		PUNTO=0x08;
		break;
	case 6:
		PUNTO=0x04;
		break;
	case 7:
		PUNTO=0x02;
		break;
	case 8:
		PUNTO=0x01;
		break;
	default:
		break;
	}
	if(AUMENTO)
	{
		AUMENTO=0;
		switch (desp_vert)
		{
		case 1:
			PUNTO=0xF0;
			break;
		case 2:
			PUNTO=0x78;
			break;
		case 3:
			PUNTO=0x3C;
			break;
		case 4:
			PUNTO=0x1E;
			break;
		case 5:
			PUNTO=0x0F;
			break;
		case 6:
			PUNTO=0x07;
			SALTO=1;
			break;
		case 7:
			PUNTO=0x03;
			SALTO=2;
			break;
		case 8:
			PUNTO=0x01;
			SALTO=3;
			break;
		default:
			break;
		}

	}
	if(DISMINUYO)
	{
		DISMINUYO=0;
		switch (desp_vert)
		{
	case 8:
		PUNTO=0x80;
		SALTO=4;
		break;
	case 7:
		PUNTO=0xC0;
		SALTO=5;
		break;
	case 6:
		PUNTO=0xE0;
		SALTO=6;
		break;
	case 5:
		PUNTO=0xF0;
		break;
	case 4:
		PUNTO=0x78;
		break;
	case 3:
		PUNTO=0x3C;
		break;
	case 2:
		PUNTO=0x1E;
		break;
	case 1:
		PUNTO=0x0F;
		break;
	default:
		break;

		}

	}
}

uint16_t leo_adc(char Channel)
{

	uint16_t Data = 0;

	LPC_ADC->CR |= (1<<24);                         //start conversion by setting "Start Conversion Now" bit (sec. 25.5.1)
	while((LPC_ADC->DR[Channel] < 0x7FFFFFFF));//wait for "done" bit to be set (sec. 25.5.4)
	Data = (LPC_ADC->DR[Channel] & 0xFFF0) >> 4;//8;0xFFC0
	return (Data);
}





// *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
