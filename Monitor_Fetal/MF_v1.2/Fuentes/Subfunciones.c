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
#include "sd_mmc_spi.h"
//--------------------------------------------------------------------------
//***********************  Variables Propias  ******************************
char			colu=7,ledcoo,pnt,pnt_ant,Fila_ant,mitad_ant,pulso_ant,pos_xh,
				cont_5seg=0,swt_corazon,cont_pulso,act_espera,mon_his,protec_pos_x;
int 			h,mi,seg,d,m,a,aa,aaa,aaaa,anio,ds,
				ef,pl,e,ee,eee,eeee,dets,esta,estf,
				proxf[8],proxh[4];
unsigned short 	aux=0, aux3=0,err_mod=0, err_mod2=0;
char Graf_punt,ale;
static char  		buffer_pulso[44] ;
char 				buf_par[5],repite_parc,Prom_par,i_t,act_t_par,cont_ant,i_ff,
					HAB_PARLANTE,APA_PARLANTE,cont_bajas_PPM,HAB_G,cont_mem,HAB_G_ALE;
int 				Prom_tot,T_Periodo,T_Per,t_par,PPM_ant,PPM_ale,Cont_per_pulso,Per_par;
//unsigned int 		valpru8[1000],i_prue;
//uint32_t			*OFFSET_SD_V;
//uint32_t			SECTOR_SD_V;

extern char val_bufff[];
//--------------------------------------------------------------------------
//***********************  Variables Externas  *****************************
extern char 	cantmp,cantver,cantprt,cint,menuactual,flagmm,sumaerr,pos_x,pos_xg,
				Rx[],btens[],toffset[],movmp[],causaerr[],causaerror[];
extern char		reclq,reclq1,reclq2,reclq3,mtok,i_p,flag_1seg,flag_25ms,cont_5s,
				cont_10seg,cont_5seg_aux,cont_5seg_au,cont_500ms,Hab_cont_500ms,cont_100ms;
extern unsigned int adc_val5,adc_valX, adc_valY;
extern char 		buff_prueba[],buff_prueba2[],HAB_GUARDAR;
extern unsigned int sleep,flagsleep,flagirq,sleepmenu;
extern int PPM;
extern uint32_t	 	*offset;
extern uint32_t		sd_sector;
extern char buff_prueba[],buff_prueba2[],buff_prueba3[];
//--------------------------------------------------------------------------

//						  Detecta los pulsos del corazón
//--------------------------------------------------------------------------
void Det_corazon(void)
{
	char *		renglon = buffer_pulso,i,j ;
	int Per_Temp;
	GPIOSetValue( 2, 9, 0 );												// Habilito Señal DIGITAL
	WG12864A_posXY(37, 7);													// Si no detecta Pulsos borra las PPM y el corazón
	WG12864A_printf("         ", Arial8x6, NEGRO);
	WG12864A_posXY(1, 8);
	WG12864A_printf("               ", Arial8x6, NEGRO);
	i_ff=2;
//------------------------------------------------------------------------
	while(!((0xA0 < adc_valX) &&(adc_valX < 0xB9) &&						// Mientras este activo el FD
		 (0x2A < adc_valY) && (adc_valY < 0x5A)))
	{
		adc_val5=0xFF;
		while((adc_val5>PICO_POSIT) && cont_5seg_au<5)						// Tomo valores del ADC hasta encontrar un pico
		{																	// o salgo a los 5 seg por protección
			if(!act_espera)
				Leo_ADC5();
			else
			{
				if(cont_100ms>2)
				{
					if(APA_PARLANTE)
					{
						WG12864A_posXY(70, 7);
						WG12864A_print_symbol(HEART16x16, NEGRO);
						GPIOSetValue( 2, 10, 0 );								// Habilito salida de latido
						APA_PARLANTE=0;
						Cont_per_pulso=0;
					}
					act_espera=0;
				}
			}
			i++;
			for(i=0;i<11;i++);
			if(i>=10)
				i=0;
		}
		if(cont_5seg_au<5)													// Si encontré un pico
		{
			if(!act_espera)
			{
				cont_pulso++;													// Cuento el pico
			}
			act_espera=1;
			cont_100ms=0;
			if(cont_pulso==1)
				T_Periodo=0;
		}
		else
		{
//			cont_pulso=0;													// Debo empezar a contar nuevamente
//			T_Periodo=0;
			t_par=0;
			Cont_per_pulso=0;
			Per_par=0;
			Per_Temp=0;
			Per_par=0;
			HAB_PARLANTE=0;
//			cont_100ms=0;
			WG12864A_posXY(37, 7);											// Si no detecta Pulsos borra las PPM y el corazón
			WG12864A_printf("         ", Arial8x6, NEGRO);
			WG12864A_posXY(37, 8);
			WG12864A_printf("         ", Arial8x6, NEGRO);
		}
		cont_5seg_au=0;
		if(T_Periodo!=0&& cont_pulso==4)
		{
			//Prom_tot=((Prom_par*4)*25);
			if(T_Periodo<20)
			{
				T_Per=T_Periodo*25;											// T_Per------ 4 p
				PPM=24000/T_Per;///Prom_tot;///1.75;									// 60seg------xx p
				Per_Temp=T_Periodo*3;
			}
/*			else
			{
			if(T_Periodo>60)
			{
				T_Per=T_Periodo*4;//*25											// T_Per------ 4 p
				PPM=24000/T_Per;///Prom_tot;///1.75;									// 60seg------xx p
				Per_Temp=T_Periodo/2;
			}
			}
*/			T_Periodo=0;												// xx p=60*4/T_per
			T_Per=0;
			Prom_par=0;
			cont_pulso=0;
			if(PPM_ant==0&&PPM<100&&PPM>60)
				PPM_ant=PPM;
			if(PPM<=160&&PPM>=60)
			{
				HAB_PARLANTE=1;
				GPIOSetValue( 3, 3, 1 );												// Apago Led ALERTA
				cont_bajas_PPM=0;														// Reseteo cont led
				if((PPM<PPM_ant+15)&&(PPM>PPM_ant-15))
				{
					PPM_ant=PPM;
					Per_par=Per_Temp;
/*					val_bufff[i_ff]=PPM;
					i_ff++;
					if(i_ff>90)
						i_ff=2;
*/					WG12864A_posXY(1, 7);
					WG12864A_printf("Pulso:", Arial8x6, NEGRO);
					renglon = buffer_pulso ;
					*renglon++ = ((PPM/100) % 10) + '0' ;
					*renglon++ = ((PPM/10)  % 10) + '0' ;
					*renglon++ = ( PPM      % 10) + '0' ;
					WG12864A_posXY(40, 7);
					WG12864A_printf(buffer_pulso, Arial8x6, NEGRO);
					if(HAB_G>2)
					{
						HAB_G=0;
						val_bufff[i_ff]=PPM;
						i_ff++;
						if(i_ff>90)
							i_ff=2;
						Grafica_monitoreo(PPM);										// Grafico PPM cada 5 seg
						protec_pos_x=1;
					}
					else
					{
						HAB_G++;
					}
					cont_ant=0;
					HAB_GUARDAR=1;
				}
				else
				{
					if(cont_ant<4)
					{

						if(PPM_ant!=0)
						{
							if(ale)
							{
								PPM_ale=PPM_ant-1;
								ale=0;
							}
							else
							{
								PPM_ale=PPM_ant+1;
								ale=1;
							}
						}
						renglon = buffer_pulso ;
						*renglon++ = (((PPM_ale)/100) % 10) + '0' ;
						*renglon++ = (((PPM_ale)/10)  % 10) + '0' ;
						*renglon++ = ( (PPM_ale)      % 10) + '0' ;
						WG12864A_posXY(40, 7);
						WG12864A_printf(buffer_pulso, Arial8x6, NEGRO);
						if(HAB_G_ALE>2)
						{
							HAB_G_ALE=0;
							cont_ant++;
							val_bufff[i_ff]=PPM_ale;
							i_ff++;
							if(i_ff>90)
								i_ff=2;
							Grafica_monitoreo((PPM_ant+1));										// Grafico PPM cada 5 seg
							protec_pos_x=1;
						}
						else
						{
							HAB_G_ALE++;
						}
					}
					else
					{
						if(PPM<100&&PPM>60)
						{
							PPM_ant=PPM;
							cont_ant=0;
						}
					}
				}
			}
			else
			{
				if(PPM<60&&cont_bajas_PPM>5)
					GPIOSetValue( 3, 3, 0 );												// Enciendo Led ALERTA
				else
					cont_bajas_PPM++;
			}

		}
		sleep=1;
		Func_Sleep (flagirq, sleepmenu);									// Funcion que maneja el Sleep de la pantalla y la IRQ del TOUCH.

	}
	HAB_PARLANTE=0;
	WG12864A_posXY(91, 7);
	WG12864A_print_symbol(FD16x16, NEGRO);									// FD Negro
	delay32Ms(0, TIMMER_FONDO_BLANCO);										// Para demorar su utiliza vTaskDelay()
	adc_valX = 0, adc_valY = 0;												// Reseteo el valor de X, Y del ADC.
	GPIOSetValue( 2, 9, 1 );												// Habilito Señal ANALOGICA
	T_Periodo=0;															// Limpio variables
	PPM_ant=0;
	PPM=0;
	WG12864A_posXY(37, 7);													// Si no detecta Pulsos borra las PPM y el corazón
	WG12864A_printf("         ", Arial8x6, NEGRO);
	WG12864A_posXY(37, 8);
	WG12864A_printf("         ", Arial8x6, NEGRO);
}
//						Almacena monitoreo en sd
//--------------------------------------------------------------------------
void almacena_sd(void)
{
	char 		SD_Protect,re;
	char				res=0;
	int  		i;
	uint32_t	envsd,punt=0,kons=512;
	char		SD_val,m;

	re=sd_mmc_spi_internal_init();        								// Restart Init of SD/MMC card after previous first init
	if(re!=KO)
	{
		SD_Protect=GPIOGetValue(3,5);
		if(!SD_Protect)
		{
			WG12864A_posXY(1, 8);
			WG12864A_printf("Alm en SD...", Arial8x6, NEGRO);					// Titulo del menu.
			delay32Ms(0, TIMMER_LOGO_AUTOTROL);
//---------		SECTOR_SD_V y OFFSET_SD_V		-------- Contienen el inicio del programa en la SD
			switch(cont_mem)
			{
			case 1:
				sd_sector=0x2008;
				offset=0x401000;
				cont_mem=2;
				for(m=0;m<100;m++)
					buff_prueba[m]=val_bufff[m];
				break;
			case 2:
				sd_sector=0x2009;
				offset=0x401200;
				cont_mem=1;
				for(m=0;m<100;m++)
					buff_prueba2[m]=val_bufff[m];
				break;
			default:
				sd_sector=0x2008;
				offset=0x401000;
				cont_mem=2;
				for(m=0;m<100;m++)
					buff_prueba3[m]=val_bufff[m];
				break;
			}
			envsd=(offset+((punt*kons)>>2));							// Incrementa de a 512b
			res=sd_mmc_spi_search_sector_to_ram(envsd);					// lee el sector de programa de la SD-MMC OK=ppio del prog

			for(punt=0;punt<85;punt++)									// Graba 96 sectores de 512 = 48k
			{
//				envsd=(offset+((punt*kons)>>2));							// Incrementa de a 512b
				if (sd_mmc_spi_write_open (envsd) == OK)			// Abre en forma de escritura
				{
					SD_val=(val_bufff[punt]);//+(punt*kons);
					sd_mmc_spi_write_sector_from_ram(&SD_val);	// Graba en SD-MMC
				}
				sd_mmc_spi_write_close ();
				envsd++;
				//offset++;//SECTOR_SD_V++;
			}
		}
		else
		{
			WG12864A_posXY(1, 8);
			WG12864A_printf("SD Protegida", Arial8x6, NEGRO);					// Titulo del menu.
			delay32Ms(0, TIMMER_LOGO_AUTOTROL);
		}
	}
}
//						  Muestra fecha y hora
//--------------------------------------------------------------------------
void Historial(char His)
{
	char i=0;
	int buff_His[100];
	switch (His)
	{
	case 1:
		for(i=0;i<100;i++)
			buff_His[i]=buff_prueba[i];
		break;
	case 2:
		for(i=0;i<100;i++)
			buff_His[i]=buff_prueba2[i];
		break;
	case 3:
		for(i=0;i<100;i++)
			buff_His[i]=buff_prueba3[i];
		break;
	default:
		break;
	}
	Graf_datos_est();
	i=0;
	pos_xg=25;
	mon_his=1;
	while(pos_xg<119)
	{
		if(!buff_His[i])
			pos_xg++;
		Grafica_monitoreo(buff_His[i]);										// Grafico PPM cada 5 seg
		i++;
	}
	// Mientras no se presione Back.
	while(! ((0xD0 < adc_valX) && (adc_valX < 0xEA) &&
			 (0x2A < adc_valY) && (adc_valY < 0x5A)))
	{
		mon_his=0;
		pos_xg=25;
		sleep=1;
		Func_Sleep (flagirq, sleepmenu);									// Funcion que maneja el Sleep de la pantalla y la IRQ del TOUCH.
	}
	WG12864A_posXY(110, 7);
	WG12864A_print_symbol(BACK16x16, NEGRO);
	delay32Ms(0, TIMMER_FONDO_BLANCO);									// Para demorar su utiliza vTaskDelay()
	adc_valX = 0, adc_valY = 0;											// Reseteo el valor de X, Y del ADC.
	WG12864A_Limpiar(NEGRO);
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
