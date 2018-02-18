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
 * Funciones del programa, Se llaman desde Menu dependiendo la pantalla en
 * la que se encuentre el display.
****************************************************************************/

#include "Menu.h"
#include "Touch.h"
#include "timer32.h"
#include "WG12864A.h"
#include "Funciones.h"
#include "Subfunciones.h"
#include "Definiciones.h"
//--------------------------------------------------------------------------
//**********************  Variables Externas  ******************************

extern unsigned int cur, 													// Posicion del cursor dentro de la pantalla.
					indice,													// Variable dinamica para moverse por el vector de la clave ingresada.
					sleep,													// Variable que a cierto valor duerme la pantalla.
					flagsleep ,												// Variable que me indica si el Display esta dormido para despertarlo.
					flagirq ;												// Bandera que me indica si habilitar o no la IRQ del TOUCH.

extern char			flagmm,flagbll,cantver,cantprt,version[],version_prt[],
					no_recibo,queenv,causaerr[],causaerror[],moduloerr[],
					b_tog[],sumaerr,lec,SIN_SD;
extern int 			h,mi,seg,d,m,anio,ds,proxf[],proxh[],var_aux;

//--------------------------------------------------------------------------
//***********************  Variables Propias  ******************************

int					norecarga=0,muefunc=0;
char				menuactual,toca,toca1,modo_func,actualiza_fw=0,
					t,pressok,prs,velc,dir_comu,sd_disp,HAB_GUARDAR,
					flagesp=0,salir=0,mod=0;
unsigned int 		cont_tra=0,tcla[4],
					adc_valX, adc_valY,										// Variables GLOBALES con los valores de posicion en X e Y.
					menu, menu1, menu2,										// Variables GLOBALES para posicionamiento en todo el Menú.
					menu3,	menu32, menu4,
					menu41, menu41_1,menu41_2,
					menu41_3, menu41_4,
					adc_val5,
					tit = 0,												// Variables para hacer titilante el cursor al ingresar datos del teclado numerico.
					num = -1,												// Variable para capturar el numero del teclado en pantalla.
					cla[4],													// Variable para capturar el numero del teclado en pantalla.
					sleepmenu,												// Variable que me indica con que menu vuelvo del Sleep en la funcion Sleep.
					flag_sleepsubmenu = 1,									// Bandera que me indica si se durmio en un SubMenu y evita el codigo del boton Back.
					varmod=0,backmod=0,s=0,tcl;
//				alm_latidos[10][100],lat_co[100];
//static char  		buffer_pulso[44] ;


int 				inc_ind=0,aux1,aux2;
unsigned int 		i= 0,i_buff=0,i_disp=0,i_disp_ant=0;
//int 				/*valpru[100],*/mu_pro[100];
char 				PROMEDIAR=0,ACT_DISP=0,NO_ENTR=0,DISMINUYO=0,AUMENTO=0,sd_mostrar;


int 				Umbral_Sup=3000,Umbral_Inf=2000,
					cont_pico_POS=0,cont_pico_NEG=0,
					inc_ind,inc_ind2,i_ind_buff,PARLANTE,
					lat_adc,lat_ant,LAT_PROM,PPM=0;
char 				PULSO_OK,flag_SUP=1,flag_INF=1,
					pos_x,pos_y,PUNTO,AUMENTO,DISMINUYO,
					cont_10seg,cont_5seg_aux,cont_5seg_au,cont_5s,FILTRO_DIG,hab_corazon,
					PROMEDIAR,NO_ENTR,ACT_DISP,v,
					ind_adc=0,SUBIR,BAJAR,desp_vert,SALTO;
unsigned int 		valpru[100],valpru2[80],ind_Muestras,aux_prov;
//unsigned int valpru[1000],valpru2[800],ind_Muestras,aux_prov;

//extern int valpru[];
extern char 		flag_1seg,flag_25ms,cont_1min,cont_1seg,HAB_PARLANTE,APA_PARLANTE,cont_100ms;
extern int 		T_Periodo,t_par,Cont_per_pulso,Per_par;
uint16_t leo_adc(char Channel);
void detector_pulsos(void);
void grafica_PPM(void);

char i_p=0,buff_prueba[]={135,137,134,133,140,142,151,150,155,153,144,142,138,137,135,136,137,
		138,139,137,138,137,135,138,141,145,141,138,139,142,143,140,137,135,138,143,139,142,141,
		145,148,153,155,155,155,151,151,147,145,147,141,138,137,133,135,138,139,135,138,135,133,
		138,139,137,138,137,135,138,141,145,141,138,139,142,143,140,137,135,138,143,139,142,141};
char buff_prueba2[]={72,74,80,76,74,80,84,88,82,78,80,72,72,72,73,71,76,
		74,74,78,82,88,89,91,93,94,92,92,90,90,88,84,82,78,78,78,76,76,74,
		82,84,84,84,83,83,84,81,82,79,74,75,76,72,73,72,70,76,84,83,82,84,
		86,87,78,76,72,74,73,77,79,74,73,71,76,79,84,86,84,87,83,79,76,74};
/*char buff_prueba3[]={115,117,114,113,120,122,131,130,135,133,124,122,128,127,115,116,117,
		118,119,117,118,117,115,118,111,115,111,118,119,122,123,120,127,115,118,123,119,122,121,
		115,118,123,125,125,125,121,111,117,115,117,111,118,117,113,115,118,119,115,118,115,113,
		108,109,107,108,107,105,108,101,115,111,118,119,112,113,110,117,105,108,103,109,102,101};
*/
extern int val_bufff[],buff_prueba3[];
/*
char buff_PPM[]={60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,
				 90,91,92,93,94,95,96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,
				 115,116,117,118,119,120,121,122,123,124,125,126,127,128,129,130,131,132,133,134,135,136,137,
				 138,139,140,141,142,143,144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,160};
*/
//--------------------------------------------------------------------------
void TIMER32_1_IRQHandler(void)
{
																					// La interrupcion la genero el MRO??
	if( LPC_TMR32B1->IR & 0x01 )
	{

		LPC_TMR32B1->IR = 1;														// Reseteo el Flag de IRQ.
		flag_25ms=1;																// Flag 25ms
		T_Periodo++;																// Tiempo para PPM (Tiempo de 4 pulsaciones)
		t_par++;																	// Tiempo parcial entre pulsos
		cont_100ms++;
		Cont_per_pulso++;
		if((Cont_per_pulso>Per_par) && HAB_PARLANTE && Per_par!=0)
		{
			GPIOSetValue( 2, 10, 1 );										// Habilito salida de latido
			WG12864A_posXY(70, 7);
			WG12864A_posXY(70, 7);
			WG12864A_printf("   ", Arial8x6, NEGRO);
			WG12864A_posXY(70, 8);
			WG12864A_posXY(70, 8);
			WG12864A_printf("   ", Arial8x6, NEGRO);

//			WG12864A_print_symbol(HEART16x16, BLANCO);
			Cont_per_pulso=0;
			APA_PARLANTE=1;
		}
		if(T_Periodo>1000)													// cada 1seg borro variables si no estoy en 2.1
		{
			T_Periodo=0;														// Tiempo para PPM (Tiempo de 4 pulsaciones)
			t_par=0;															// Tiempo parcial entre pulsos
			cont_100ms=0;
			Cont_per_pulso=0;
		}
		cont_1seg++;																// Incremento cont 1seg
		if(cont_1seg>=40)
		{
			cont_1seg=0;
			flag_1seg=1;															// Flag 1seg
			cont_5seg_au++;
//			cont_5s++;
			cont_1min++;															// Incremento cont 1min
			if(cont_1min>=60)
			{
				cont_1min=0;
				if( sleep == 0 )
				{
					flagsleep = 1;													// Activo la Bandera de que el Display duerme para en la IRQ despertarlo.
					GLCD_Output_Low(BACKLIGHT);										// Apaga el display
					TOUCH_Standby();												// Standby, en espera de si activacion.
					flagirq = 0;													// Bandera que me indica si habilitar o no la IRQ del TOUCH.
				}
				if( sleep == MULTIPLICADOR_TIMMER_SLEEP )
				{
					flagirq = 0;													// Bandera que me indica si habilitar o no la IRQ del TOUCH.
					disable_timer32(1); 											// Desabilito el timmer porque ya se durmio el Display.
					cla[0] = 0, cla[1] = 0,	cla[2] = 0, cla[3] = 0;					// Limpio la clave del ingreso al menu especial.
					indice = 0;														// Acomodo el indicel del vector de clave al inicio.
					cur = 48;														// Preparo el cursor para la proxima vez.
				}
				sleep ++;															// Incremento la variable que duerme la pantalla.
			}
		}
	}
	if (LPC_TMR32B1->IR & 0x02)														//Int MR1
	{
		LPC_TMR32B1->IR = 2;														// Reset Int
//		flag_1seg=1;
	}
	if (LPC_TMR32B1->IR & 0x04)														//Int MR2
	{
	   LPC_TMR32B1->IR = 4;															//Reseteo interrupción
	   flag_25ms=1;
	}
	if ( LPC_TMR32B1->IR & (0x1<<4) )
	{
		LPC_TMR32B1->IR = 0x1<<4;													// clear interrupt flag
	//	timer32_1_capture++;
	}
	return;
}

//--------------------------------------------------------------------------
void Func_Sleep(int opcion_1, int opcion_2 )
{
	int i;
	switch( opcion_1 ){

		case 0:

			GPIOIntClear(PORT1, 2);											// Limpio la Interrupcion.
			for(i=0; i == 10; i ++){;}										// Pequeña demora (Ciclos de Clock para que baje la Bandera).
			GPIOIntEnable(PORT1, 2);										// Habilito la Interrupcion.
			flagirq = -1;													// Bandera que me indica si habilitar o no la IRQ del TOUCH.
			menu = menu, menu1 = menu1, menu2 = menu2, menu3 = menu3,
			menu32 = menu32, menu4 = menu4, menu41 = menu41, menu41_1 = menu41_1,
			menu41_2 = menu41_2, menu41_3 = menu41_3, menu41_4 = menu41_4;
			break;

		default:
			break;
	}

	switch( opcion_2 ){														// Si es Mayor a 5 reinicio por completo, sino solo prendo la pantalla.

		case 0:

			WG12864A_Limpiar(NEGRO); 										// Limpio la pantalla de lo que tenia.
			Menu_Logo();													// Presento el Logo de la Empresa.
			WG12864A_Limpiar(NEGRO); 										// Limpio el logo de la pantalla.
			sleepmenu = -1;													// Seleccion con que modalidad vuelve del Sleep.
			adc_valX = 0, adc_valY = 0;										// Reseteo el valor de X, Y del ADC.
			break;

		case 1:

			WG12864A_Limpiar(NEGRO); 										// Limpio la pantalla de lo que tenia.
			Menu_Logo();													// Presento el Logo
			WG12864A_Limpiar(NEGRO); 										// Limpio el logo de la pantalla.
			sleepmenu = -1;													// Seleccion con que modalidad vuelve del Sleep.
			flag_sleepsubmenu = 0;											// Aviso por medio de esta bandera que vuelve al menu principal por medio de Sleep.

			// Si me encuentro en un SubMenu le paso a X e Y las coordenadas del BACK y acomodo el menu.
			if( (menu2 >= 1 || menu2 <= 3) ||
				(menu3 >= 1 || menu3 <= 2) ){

				adc_valX = 0xDF, adc_valY = 0x40;							// Selecciono las coordenadas del BACK.
				menu = 0, menu1 = -1, menu2 = -1, menu3 = -1,
				menu32 = -1, menu4 = -1, menu41 = -1, menu41_1 = -1,
				menu41_2 = -1, menu41_3 = -1, menu41_4 = -1;
			}else{

				adc_valX = 0, adc_valY = 0;									// Reseteo el valor de X, Y del ADC.
				menu = 0, menu1 = -1, menu2 = -1, menu3 = -1,
				menu32 = -1, menu4 = -1, menu41 = -1, menu41_1 = -1,
				menu41_2 = -1, menu41_3 = -1, menu41_4 = -1;
			}
			break;
	}
	if(flagbll)																// Flag buffer lleno
	{
		flagbll=0;
		leobuffer(flagmm);													// Leo buffer de recepción
	}
}
//--------------------------------------------------------------------------
/****************************  Submenues 1  ********************************/
//--------------------------------------------------------------------------

//						  1.1 Ingreso de Pacientes
//--------------------------------------------------------------------------

void Func_Ingreso (void)
{
	// Mientras no se presione Back.
	while(! ((0xD0 < adc_valX) && (adc_valX < 0xEA) &&
			 (0x2A < adc_valY) && (adc_valY < 0x5A)))
	{
		// Titulo del menu.
		WG12864A_posXY(1, 1);
		WG12864A_printf(" * Datos Paciente *  ", Arial8x6, BLANCO);
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
		WG12864A_print_symbol(BK8x16, NEGRO);
 		WG12864A_posXY(110, 3);
		WG12864A_print_symbol(OKS8x16, NEGRO);
		// Se Presionó Flecha Izquerda?
		if( (0x95 < adc_valX) && (adc_valX < 0xB5) &&
			(0x70 < adc_valY) && (adc_valY < 0xA9) ){

			WG12864A_posXY(90, 3);											// Animacion del dibujo.
			WG12864A_print_symbol(BK8x16, BLANCO);
			delay32Ms(0, TIMMER_FONDO_BLANCO_BOTONES);
			adc_valX = 0, adc_valY = 0;										// Reseteo el valor de X, Y del ADC.
			WG12864A_posXY(cur, 2);											// Presento el cursor donde corresponde.
			WG12864A_printf(" ", Arial8x6, NEGRO);
			cur -= 12;														// Corro el cursor 1 lugar.
			norecarga=0;
			indice --;														// Decremento el Indice del vector que guarda la clave ingresada.

			// Acomodo el cursor dentro de los 4 casilleros de la clave a ingresar y el indice del vector de la clave.
			if( cur < 48 ){

				cur = 48;
				indice = 3;
			}
		}
		// Se Presionó OK?
		if( (0xD0 < adc_valX) && (adc_valX < 0xEA) &&
			(0x70 < adc_valY) && (adc_valY < 0xA9) ){

			WG12864A_posXY(110, 3);											// Animacion del dibujo.
			WG12864A_print_symbol(OKS8x16, BLANCO);
			delay32Ms(0, TIMMER_FONDO_BLANCO_BOTONES);
			adc_valX = 0, adc_valY = 0;										// Reseteo el valor de X, Y del ADC.
			WG12864A_posXY(48, 3);											// Presento el cursor donde corresponde.
			WG12864A_printf(" ", Arial8x6, NEGRO);

			// Acomodo el cursor dentro de los 4 casilleros de la clave a ingresar y el indice del vector de la clave.
			if( cur < 48 ){

				cur = 48;
				indice = 3;
			}
		}

	   func_teclado();
		// Funcion que maneja el Sleep de la pantalla y la IRQ del TOUCH.
		Func_Sleep (flagirq, sleepmenu);
	}

	if(flag_sleepsubmenu){													// Si la bandera esta arriba salio del menu normal, sino salio por sleep.

		WG12864A_posXY(110, 7);
		WG12864A_print_symbol(BACK16x16, NEGRO);
		delay32Ms(0, TIMMER_FONDO_BLANCO);									// Para demorar su utiliza vTaskDelay()
		adc_valX = 0, adc_valY = 0;											// Reseteo el valor de X, Y del ADC.
		WG12864A_Limpiar(NEGRO);
		menu1 = 0;
	}

	flag_sleepsubmenu = 1;													// Restablesco el valor original de la bandera.

}


//						  1.2 Consulta de Pacientes
//--------------------------------------------------------------------------
void func_consultapaciente()
{
	char t,colu;

	while(!((0xD0 < adc_valX) &&(adc_valX < 0xEA) &&
			(0x2A < adc_valY) && (adc_valY < 0x5A))) 						// Mientras no se presione back
	{
		menuactual=consp;
		WG12864A_posXY(1, 1);
		WG12864A_printf(" *Consulta Paciente* ", Arial8x6, BLANCO);
		WG12864A_posXY(1,3);
		WG12864A_printf(" Nombre:", Arial8x6, NEGRO);
		WG12864A_posXY(110, 7);
		WG12864A_print_symbol(BACK16x16, BLANCO);
		WG12864A_posXY(1, 5);
		WG12864A_printf(" 1 2 3 4 5 6 7 8 9 0", Arial8x6, NEGRO);
		WG12864A_posXY(1, 6);
		WG12864A_printf("Q W E R T Y U I O P ", Arial8x6, NEGRO);
		WG12864A_posXY(1, 7);
		WG12864A_printf(" A S D F G H J K L", Arial8x6, NEGRO);
		WG12864A_posXY(1, 8);
		WG12864A_printf("  Z X C V B N M ", Arial8x6, NEGRO);
		WG12864A_posXY(90, 3);
		WG12864A_print_symbol(BK8x16, NEGRO);
 		WG12864A_posXY(110, 3);
		WG12864A_print_symbol(OKS8x16, NEGRO);
		// Se Presionó Flecha Izquerda?
		if( (0x95 < adc_valX) && (adc_valX < 0xB5) &&
			(0x70 < adc_valY) && (adc_valY < 0xA9) ){

			WG12864A_posXY(90, 3);											// Animacion del dibujo.
			WG12864A_print_symbol(BK8x16, BLANCO);
			delay32Ms(0, TIMMER_FONDO_BLANCO_BOTONES);
			adc_valX = 0, adc_valY = 0;										// Reseteo el valor de X, Y del ADC.
			WG12864A_posXY(cur, 2);											// Presento el cursor donde corresponde.
			WG12864A_printf(" ", Arial8x6, NEGRO);
			cur -= 12;														// Corro el cursor 1 lugar.
			norecarga=0;
			indice --;														// Decremento el Indice del vector que guarda la clave ingresada.

			// Acomodo el cursor dentro de los 4 casilleros de la clave a ingresar y el indice del vector de la clave.
			if( cur < 48 ){

				cur = 48;
				indice = 3;
			}
		}
		// Se Presionó OK?
		if( (0xD0 < adc_valX) && (adc_valX < 0xEA) &&
			(0x70 < adc_valY) && (adc_valY < 0xA9) ){

			WG12864A_posXY(110, 3);											// Animacion del dibujo.
			WG12864A_print_symbol(OKS8x16, BLANCO);
			delay32Ms(0, TIMMER_FONDO_BLANCO_BOTONES);
			adc_valX = 0, adc_valY = 0;										// Reseteo el valor de X, Y del ADC.
			WG12864A_posXY(48, 3);											// Presento el cursor donde corresponde.
			WG12864A_printf(" ", Arial8x6, NEGRO);

			// Acomodo el cursor dentro de los 4 casilleros de la clave a ingresar y el indice del vector de la clave.
			if( cur < 48 ){

				cur = 48;
				indice = 3;
			}
		}

	   func_teclado();

		Func_Sleep (flagirq, sleepmenu);
	}
	if(flag_sleepsubmenu)
	{

		WG12864A_posXY(110, 7);
		WG12864A_print_symbol(BACK16x16, NEGRO);
		delay32Ms(0, TIMMER_FONDO_BLANCO);									// Para demorar su utiliza vTaskDelay()
		adc_valX = 0, adc_valY = 0;											// Reseteo el valor de X, Y del ADC.
		WG12864A_Limpiar(NEGRO);
		menu1 = 0;
	}

	flag_sleepsubmenu = 1;													// Restablesco el valor original de la bandera.
}

//						  1.3 Editar Paciente
//--------------------------------------------------------------------------
void func_editapaciente()
{
	char t,colu;

	while(!((0xD0 < adc_valX) &&(adc_valX < 0xEA) &&
			(0x2A < adc_valY) && (adc_valY < 0x5A))) 						// Mientras no se presione back
	{
		menuactual=consp;
		WG12864A_posXY(1, 1);
		WG12864A_printf(" * Editar Paciente*  ", Arial8x6, BLANCO);
		WG12864A_posXY(1,3);
		WG12864A_printf(" N:Maria      E:26", Arial8x6, NEGRO);
		WG12864A_posXY(1,5);
		WG12864A_printf(" Ed Nombre:", Arial8x6, NEGRO);
		WG12864A_posXY(1,7);
		WG12864A_printf(" Ed Edad:", Arial8x6, NEGRO);
		WG12864A_posXY(110, 7);
		WG12864A_print_symbol(BACK16x16, BLANCO);

		Func_Sleep (flagirq, sleepmenu);
	}
	if(flag_sleepsubmenu)
	{
		WG12864A_posXY(110, 7);
		WG12864A_print_symbol(BACK16x16, NEGRO);
		delay32Ms(0, TIMMER_FONDO_BLANCO);									// Para demorar su utiliza vTaskDelay()
		adc_valX = 0, adc_valY = 0;											// Reseteo el valor de X, Y del ADC.
		WG12864A_Limpiar(NEGRO);
		menu1 = 0;
	}

	flag_sleepsubmenu = 1;													// Restablesco el valor original de la bandera.
}

//						  1.4 Borrar Paciente
//--------------------------------------------------------------------------
void func_borrarpaciente()
{
	char m;
	while((!((0xD0 < adc_valX) &&(adc_valX < 0xEA) &&
			(0x2A < adc_valY) && (adc_valY < 0x5A)))&& (pressok==0)) 		// Mientras no se presione back
	{
		WG12864A_posXY(1, 1);
		WG12864A_printf("  *Borrar Paciente*  ", Arial8x6, BLANCO);
		WG12864A_posXY(1, 3);
		WG12864A_printf(" N:Natalia      E:30 ", Arial8x6, NEGRO);
		WG12864A_posXY(1, 5);
		WG12864A_printf("Confirma borrar      Paciente?", Arial8x6, NEGRO);
		WG12864A_posXY(80, 7);
		WG12864A_print_symbol(OK16x16, BLANCO);
		if((0xA0 < adc_valX) &&(adc_valX < 0xB9) && (0x2A < adc_valY) && (adc_valY < 0x5A))// Se presionó OK?
		{
			WG12864A_posXY(80, 7);
			WG12864A_print_symbol(OK16x16, NEGRO);
			delay32Ms(0, TIMMER_FONDO_BLANCO);								// Para demorar su utiliza vTaskDelay()
			adc_valX = 0;													// Reseteo el valor de X, Y del ADC
			adc_valY = 0;
			menuactual=berr;												// Envio menu actual por Recibe datos para borrar err en flash
			causaerr[0]=sinerr;	// provisorio, sacar!
			for(m=0;m<100;m++)
				buff_prueba3[m]=0;
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
		delay32Ms(0, TIMMER_FONDO_BLANCO);									// Para demorar su utiliza vTaskDelay()
		adc_valX = 0, adc_valY = 0;											// Reseteo el valor de X, Y del ADC.
		WG12864A_Limpiar(NEGRO);
		menu1 = 0;
	}
	pressok=0;

}

flag_sleepsubmenu = 1;														// Restablesco el valor original de la bandera.

}

//--------------------------------------------------------------------------
/****************************  Submenues 2  ********************************/
//--------------------------------------------------------------------------

//						  2.1 Iniciar Monitoreo
//--------------------------------------------------------------------------
void Func_Monitoreo (void)
{
	char m=0,n=0,SALIR_MF,SD_Detect,HAB_GUA_SD;
	HAB_GUARDAR=0;
	SALIR_MF=0;
	Graf_datos_est();
	GPIOSetValue( 2, 8, 0 );												// Habilito el MUX
	GPIOSetValue( 2, 9, 1 );												// Habilito Señal ANALOGICA
	if(SIN_SD)
	{
		WG12864A_posXY(1, 8);
		WG12864A_printf("SD NO detectada", Arial8x6, NEGRO);
	}
	else
	{
		WG12864A_posXY(1, 8);
		WG12864A_printf("SD OK", Arial8x6, NEGRO);
	}
	delay32Ms(0, TIMMER_LOGO_AUTOTROL);
	WG12864A_posXY(1, 8);
	WG12864A_printf("               ", Arial8x6, NEGRO);
	sd_mostrar=1;
	// Mientras no se presione Back.
	while((! ((0xD0 < adc_valX) && (adc_valX < 0xEA) &&
			 (0x2A < adc_valY) && (adc_valY < 0x5A)))&& SALIR_MF==0)
	{
		if((0xA0 < adc_valX) &&(adc_valX < 0xB9) &&
				(0x2A < adc_valY) && (adc_valY < 0x5A))						// Se presionó FD?
		{
			WG12864A_posXY(91, 7);
			WG12864A_print_symbol(FD16x16, BLANCO);							// FD Blanco
			delay32Ms(0, TIMMER_FONDO_BLANCO);								// Para demorar su utiliza vTaskDelay()
			adc_valX = 0, adc_valY = 0;										// Reseteo el valor de X, Y del ADC.
			WG12864A_posXY(110, 7);
			WG12864A_printf("   ", Arial8x6, NEGRO);
			WG12864A_posXY(110, 8);
			WG12864A_printf("   ", Arial8x6, NEGRO);
			Det_corazon();													// Función que detecta las PPM
		}
		WG12864A_posXY(110, 7);
		WG12864A_print_symbol(BACK16x16, BLANCO);
		if(HAB_GUARDAR)
		{
			if(sd_mostrar)
			{
				SD_Detect=GPIOGetValue(3,4);
				if(SD_Detect)
				{
					sd_mostrar=0;
					WG12864A_posXY(1, 8);
					WG12864A_printf("SD Extraida", Arial8x6, NEGRO);					// Titulo del menu.
					delay32Ms(0, TIMMER_LOGO_AUTOTROL);
					HAB_GUA_SD=0;
				}
				else
				{
					sd_mostrar=0;
					WG12864A_posXY(70, 7);
					WG12864A_print_symbol(OK16x16, BLANCO);							// OK Blanco
					WG12864A_posXY(1, 8);
					WG12864A_printf("SD OK      ", Arial8x6, NEGRO);					// Titulo del menu.
					delay32Ms(0, TIMMER_LOGO_AUTOTROL);
					HAB_GUA_SD=1;
				}
			}
			if(HAB_GUA_SD)
			{
				WG12864A_posXY(1, 8);
				WG12864A_printf("Almacenar?", Arial8x6, NEGRO);					// Titulo del menu.
				if(((0x7A < adc_valX) && (adc_valX < 0x9A) &&
						(0x2A < adc_valY) && (adc_valY < 0x5A)))
				{
					WG12864A_posXY(70, 7);
					WG12864A_print_symbol(OK16x16, NEGRO);						// OK NEGRO
					delay32Ms(0, TIMMER_FONDO_BLANCO);							// Para demorar su utiliza vTaskDelay()
					adc_valX = 0, adc_valY = 0;									// Reseteo el valor de X, Y del ADC.
					WG12864A_posXY(37, 7);										// Si no detecta Pulsos borra las PPM y el corazón
					WG12864A_printf("         ", Arial8x6, NEGRO);
					WG12864A_posXY(1, 8);
					WG12864A_printf("               ", Arial8x6, NEGRO);
					HAB_GUARDAR=0;
					SALIR_MF=1;
					for(m=0;m<100;m++)
						buff_prueba3[m]=val_bufff[m];
					almacena_sd();												// Almacena monitoreo en sd
				}
			}
		}
		sleep=1;
		Func_Sleep (flagirq, sleepmenu);		// Funcion que maneja el Sleep de la pantalla y la IRQ del TOUCH.
	}
	if(flag_sleepsubmenu)
	{
		if(!SALIR_MF)
		{
			WG12864A_posXY(110, 7);
			WG12864A_print_symbol(BACK16x16, NEGRO);
			delay32Ms(0, TIMMER_FONDO_BLANCO);									// Para demorar su utiliza vTaskDelay()
		}
		adc_valX = 0, adc_valY = 0;											// Reseteo el valor de X, Y del ADC.
		WG12864A_Limpiar(NEGRO);
		menu2 = 0;
	}

	flag_sleepsubmenu = 1;													// Restablesco el valor original de la bandera.
	GPIOSetValue( 2, 8, 1 );												// Deshabilito el MUX
}

//						  2.2 Configuraciones
//--------------------------------------------------------------------------
void Func_Configuraciones (void)
{
	int i=0,fila=8,colut=1;
	menuactual=ED_PARAM;
	// Mientras no se presione Back.
	while(! ((0xD0 < adc_valX) && (adc_valX < 0xEA) &&
			 (0x2A < adc_valY) && (adc_valY < 0x5A))){

		// Titulo del menu.
		WG12864A_posXY(1, 1);
		WG12864A_printf(" * Configuraciones * ", Arial8x6, BLANCO);
		WG12864A_posXY(110, 7);
		WG12864A_print_symbol(BACK16x16, BLANCO);

		// Funcionalidad del Menú.
		WG12864A_posXY(7, 3);
		WG12864A_printf("Pantalla de Prueba", Arial8x6, NEGRO);
		WG12864A_posXY(1, 6);
		WG12864A_printf("ADC Corazon", Arial8x6, NEGRO);
		WG12864A_posXY(1, 7);
		WG12864A_printf("ADC Touch", Arial8x6, NEGRO);

		for(i=0;i<8;i++)
		{
			WG12864A_posXY(colut, fila);
			WG12864A_printf(&b_tog[i], Arial8x6, NEGRO);
			colut+=6;

		}
		colut=1;
		WG12864A_posXY(70, 7);
		WG12864A_putchar((((adc_valX >> 4) & 0x0F)+0x20), Hexa8x6, NEGRO);
		WG12864A_putchar(((adc_valX & 0x0F)+0x20), Hexa8x6, NEGRO);
		WG12864A_posXY(90, 7);
		WG12864A_putchar((((adc_valY >> 4) & 0x0F)+0x20), Hexa8x6, NEGRO);
		WG12864A_putchar(((adc_valY & 0x0F)+0x20), Hexa8x6, NEGRO);
		Leo_ADC5();
		WG12864A_posXY(70, 6);
		WG12864A_putchar((((adc_val5 >> 4) & 0x0F)+0x20), Hexa8x6, NEGRO);
		WG12864A_putchar(((adc_val5 & 0x0F)+0x20), Hexa8x6, NEGRO);
		// Funcion que maneja el Sleep de la pantalla y la IRQ del TOUCH.
		Func_Sleep (flagirq, sleepmenu);
	}
	if(flag_sleepsubmenu)
	{
		WG12864A_posXY(110, 7);
		WG12864A_print_symbol(BACK16x16, NEGRO);
		delay32Ms(0, TIMMER_FONDO_BLANCO);									// Para demorar su utiliza vTaskDelay()
		adc_valX = 0, adc_valY = 0;											// Reseteo el valor de X, Y del ADC.
		WG12864A_Limpiar(NEGRO);
		menu2 = 0;
	}
	flag_sleepsubmenu = 1;													// Restablesco el valor original de la bandera.
}
void Func_Historial()
{
	menuactual=comu;
	// Mientras no se presione Back.
	while(! ((0xD0 < adc_valX) && (adc_valX < 0xEA) &&
			 (0x2A < adc_valY) && (adc_valY < 0x5A))){

		// Titulo del menu.
		WG12864A_posXY(1, 1);
		WG12864A_printf("*Historial Monitoreo*", Arial8x6, BLANCO);
		WG12864A_posXY(1, 2);
		WG12864A_printf("N: Maria   E:26      ", Arial8x6, NEGRO);
		WG12864A_posXY(1, 4);
		WG12864A_printf("N: Paula   E:33      ", Arial8x6, NEGRO);
		WG12864A_posXY(1, 6);
		WG12864A_printf("N: Natalia E:30      ", Arial8x6, NEGRO);
		WG12864A_posXY(110, 7);
		WG12864A_print_symbol(BACK16x16, BLANCO);
		if( (0x1A < adc_valX) && (adc_valX < 0xEA) &&
			(0xAA < adc_valY) && (adc_valY < 0xC0) )
		{
			WG12864A_posXY(1, 2);
			WG12864A_printf("N: Maria  E:26       ", Arial8x6, BLANCO);
			delay32Ms(0, TIMMER_FONDO_BLANCO);								// Para demorar su utiliza vTaskDelay()
			adc_valX = 0, adc_valY = 0;										// Reseteo el valor de X, Y del ADC.
			WG12864A_Limpiar(NEGRO);
			Historial(1);													// Función que detecta las PPM
		}
		if( (0x1A < adc_valX) && (adc_valX < 0xEA) &&
			(0x85 < adc_valY) && (adc_valY < 0xA0) )
		{
			WG12864A_posXY(1, 4);
			WG12864A_printf("N: Paula   E:33      ", Arial8x6, BLANCO);
			delay32Ms(0, TIMMER_FONDO_BLANCO);								// Para demorar su utiliza vTaskDelay()
			adc_valX = 0, adc_valY = 0;										// Reseteo el valor de X, Y del ADC.
			WG12864A_Limpiar(NEGRO);
			Historial(2);													// Función que detecta las PPM
		}
		if( (0x1A < adc_valX) && (adc_valX < 0xC0) &&
			(0x5A < adc_valY) && (adc_valY < 0x7A) )
		{
			WG12864A_posXY(1, 6);
			WG12864A_printf("N: Natalia E:30     ", Arial8x6, BLANCO);
			delay32Ms(0, TIMMER_FONDO_BLANCO);								// Para demorar su utiliza vTaskDelay()
			adc_valX = 0, adc_valY = 0;										// Reseteo el valor de X, Y del ADC.
			WG12864A_Limpiar(NEGRO);
			Historial(3);													// Función que detecta las PPM
		}
		// Funcion que maneja el Sleep de la pantalla y la IRQ del TOUCH.
		Func_Sleep (flagirq, sleepmenu);
	}
	if(flag_sleepsubmenu)
	{
		WG12864A_posXY(110, 7);
		WG12864A_print_symbol(BACK16x16, NEGRO);
		delay32Ms(0, TIMMER_FONDO_BLANCO);									// Para demorar su utiliza vTaskDelay()
		adc_valX = 0, adc_valY = 0;											// Reseteo el valor de X, Y del ADC.
		WG12864A_Limpiar(NEGRO);
		menu2 = 0;
	}
	flag_sleepsubmenu = 1;													// Restablesco el valor original de la bandera.
}
//--------------------------------------------------------------------------
/****************************  Submenues 3  ********************************/
//--------------------------------------------------------------------------
//						  3.1  Muestra fecha y hora
//--------------------------------------------------------------------------
void func_muestrafyh()
{
	while(!((0xD0 < adc_valX) &&(adc_valX < 0xEA) &&
			(0x2A < adc_valY) && (adc_valY < 0x5A))) 						// Mientras no se presione back
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
	if(flag_sleepsubmenu)
	{
		WG12864A_posXY(110, 7);
		WG12864A_print_symbol(BACK16x16, NEGRO);
		delay32Ms(0, TIMMER_FONDO_BLANCO);									// Para demorar su utiliza vTaskDelay()
		adc_valX = 0, adc_valY = 0;											// Reseteo el valor de X, Y del ADC.
		WG12864A_Limpiar(NEGRO);
		menu3 = 0;
	}
	flag_sleepsubmenu = 1;													// Restablesco el valor original de la bandera.
}

//						  3.1  Modifica fecha y hora
//--------------------------------------------------------------------------
void func_modfyh()
{
	int i=0;
	while((!((0xD0 < adc_valX) &&(adc_valX < 0xEA) &&
			(0x2A < adc_valY) && (adc_valY < 0x5A))) && flagesp!=1) 		// Mientras no se presione back
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
					delay32Ms(0, TIMMER_FONDO_BLANCO);						// Para demorar su utiliza vTaskDelay()
					adc_valX = 0;											// Reseteo el valor de X, Y del ADC
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
								var_aux=Func_MuestraTeclado(2);				// Muestra el teclado numerico
								if( var_aux != -1 ){						// Valida el numero del teclado que se pulso.

									if (indice < 2){						// Si no cargo los 4 Digitos, lo carga sino limpia la clave.

										tcla[indice] = var_aux;				// Carga en el vector los digitos de la clave.
										indice ++;
									}else{

										indice = 0;							// Reseteo el indice del vector de la clave.
										cur = 48;							// Acomodo el cursor para la proxima vez.
										WG12864A_posXY(cur, 2);				// Limpio la clave en pantalla.
										WG12864A_printf(" ", Arial8x6, NEGRO);
										WG12864A_posXY(88, 2);
										WG12864A_printf(" ", Arial8x6, NEGRO);
									}
								}

								if((0xA0 < adc_valX) &&(adc_valX < 0xB9) && (0x2A < adc_valY) && (adc_valY < 0x5A))	// OK?
								{
								varmod=(tcla[0]*10)+(tcla[1]);				// Cargo el numero ingresado
								if((varmod)<32 &&(varmod>0))
								{
									proxf[0]=varmod;						//Cargo el dia
									WG12864A_posXY(80, 7);
									WG12864A_print_symbol(OK16x16, NEGRO);
									delay32Ms(0, TIMMER_FONDO_BLANCO);		// Para demorar su utiliza vTaskDelay()
									adc_valX = 0;							// Reseteo el valor de X, Y del ADC
									adc_valY = 0;
									salir=1;
									mod=1;
									s=0;
									indice = 0;								// Reseteo el indice del vector de la clave.
									cur=48;
									WG12864A_posXY(76, 2);
									WG12864A_printf("       ", Arial8x6, NEGRO);
									for(i=0;i<3;i++)
									{
										tcla[i]=0;
									}
								}
								else										// El valor ingresado no es correcto
								{
									WG12864A_posXY(80, 7);
									WG12864A_print_symbol(Cross16x16, NEGRO);
									delay32Ms(0, TIMMER_FONDO_BLANCO);		// Para demorar su utiliza vTaskDelay()

								}
								}
								Func_Sleep (flagirq, sleepmenu);
							}
							if(salir==0)
							{
								WG12864A_posXY(110, 7);
								WG12864A_print_symbol(BACK16x16, NEGRO);
								delay32Ms(0, TIMMER_FONDO_BLANCO);			// Para demorar su utiliza vTaskDelay()
								cur=48;
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
							while((!((0xD0 < adc_valX) &&(adc_valX < 0xEA) &&
									(0x2A < adc_valY) && (adc_valY < 0x5A))) && salir==0) // Mientras no se presione back
							{
								var_aux=Func_MuestraTeclado(3);
								if( var_aux != -1 ){						// Valida el numero del teclado que se pulso.

									if (indice < 2){						// Si no cargo los 4 Digitos, lo carga sino limpia la clave.

										tcla[indice] = var_aux;				// Carga en el vector los digitos de la clave.
										indice ++;
									}else{

										indice = 0;							// Reseteo el indice del vector de la clave.
										cur = 48;							// Acomodo el cursor para la proxima vez.
										WG12864A_posXY(cur, 2);				// Limpio la clave en pantalla.
										WG12864A_printf(" ", Arial8x6, NEGRO);
										WG12864A_posXY(88, 2);
										WG12864A_printf(" ", Arial8x6, NEGRO);
									}
								}

								if((0xA0 < adc_valX) &&(adc_valX < 0xB9) &&
										(0x2A < adc_valY) && (adc_valY < 0x5A))
								{
									varmod=(tcla[0]*10)+(tcla[1]);			//Cargo el valor ingresado por teclado
									if((varmod)<13 &&(varmod>0))
									{
										proxf[1]=varmod;					//Cargo el mes
										WG12864A_posXY(80, 7);
										WG12864A_print_symbol(OK16x16, NEGRO);
										delay32Ms(0, TIMMER_FONDO_BLANCO);	// Para demorar su utiliza vTaskDelay()
										adc_valX = 0;						// Reseteo el valor de X, Y del ADC
										adc_valY = 0;
										salir=1;
										mod=2;
										s=0;
										indice = 0;							// Reseteo el indice del vector de la clave.
										cur=48;
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
								delay32Ms(0, TIMMER_FONDO_BLANCO);			// Para demorar su utiliza vTaskDelay()
								cur=48;
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
							while((!((0xD0 < adc_valX) &&(adc_valX < 0xEA) &&
									(0x2A < adc_valY) && (adc_valY < 0x5A))) && salir==0) // Mientras no se presione back
							{
								var_aux=Func_MuestraTeclado(4);

								if( var_aux != -1 ){						// Valida el numero del teclado que se pulso.

									if (indice < 4){						// Si no cargo los 4 Digitos, lo carga sino limpia la clave.

										tcla[indice] = var_aux;				// Carga en el vector los digitos de la clave.
										indice ++;
									}else{

										indice = 0;							// Reseteo el indice del vector de la clave.
										cur = 48;							// Acomodo el cursor para la proxima vez.
										WG12864A_posXY(cur, 2);				// Limpio la clave en pantalla.
										WG12864A_printf(" ", Arial8x6, NEGRO);
										WG12864A_posXY(88, 2);
										WG12864A_printf(" ", Arial8x6, NEGRO);
										WG12864A_posXY(100, 2);
										WG12864A_printf(" ", Arial8x6, NEGRO);
										WG12864A_posXY(112, 2);
										WG12864A_printf(" ", Arial8x6, NEGRO);
									}
								}

								if((0xA0 < adc_valX) &&(adc_valX < 0xB9) &&
										(0x2A < adc_valY) && (adc_valY < 0x5A))
								{
									varmod=(tcla[0]*1000)+(tcla[1]*100)+(tcla[2]*10)+(tcla[3]);// Cargo el valor ingresado por teclado
									if(1)//(varmod)<2100 &&(varmod>1979))
									{
										proxf[2]=varmod;					//Cargo el año
										WG12864A_posXY(80, 7);
										WG12864A_print_symbol(OK16x16, NEGRO);
										delay32Ms(0, TIMMER_FONDO_BLANCO);	// Para demorar su utiliza vTaskDelay()
										adc_valX = 0;						// Reseteo el valor de X, Y del ADC
										adc_valY = 0;
										salir=1;
										mod=4;
										s=0;
										indice = 0;							// Reseteo el indice del vector de la clave.
										cur=48;
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
								delay32Ms(0, TIMMER_FONDO_BLANCO);			// Para demorar su utiliza vTaskDelay()
								cur=48;
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
							while((!((0xD0 < adc_valX) &&(adc_valX < 0xEA) &&
									(0x2A < adc_valY) && (adc_valY < 0x5A))) && salir==0) // Mientras no se presione back
							{
								WG12864A_posXY(1, 1);
								WG12864A_printf(" * Modificar fecha * ", Arial8x6, BLANCO);
								WG12864A_posXY(1, 3);
								WG12864A_printf("Fecha  Mes  A;o", Arial8x6, NEGRO);	// Muestra que los datos ingresados sean correctos
								prox_fecha (5);								// Muestra la prox fecha a cargar
								WG12864A_posXY(1,5);
								WG12864A_posXY(80, 7);						// Muestra ok
								WG12864A_print_symbol(OK16x16, BLANCO);
								WG12864A_posXY(110, 7);						// Muestra back
								WG12864A_print_symbol(BACK16x16, BLANCO);
								if((0xA0 < adc_valX) &&(adc_valX < 0xB9) &&
										(0x2A < adc_valY) && (adc_valY < 0x5A)) //OK?
								{
									WG12864A_posXY(80, 7);
									WG12864A_print_symbol(OK16x16, NEGRO);
									delay32Ms(0, TIMMER_FONDO_BLANCO);		// Para demorar su utiliza vTaskDelay()
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
								delay32Ms(0, TIMMER_FONDO_BLANCO);			// Para demorar su utiliza vTaskDelay()
								cur=48;
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
						if(norecarga==0)									// Si se presionó un num incrementa el vector de clave
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
				delay32Ms(0, TIMMER_FONDO_BLANCO);							// Para demorar su utiliza vTaskDelay()
				adc_valX = 0;												// Reseteo el valor de X, Y del ADC
				adc_valY = 0;
				for(i=0;i<3;i++)
				{
					tcla[i]=0;
				}
				s=0;
				WG12864A_Limpiar(NEGRO);
				while((!((0xD0 < adc_valX) &&(adc_valX < 0xEA) &&
						(0x2A < adc_valY) && (adc_valY < 0x5A))) && flagesp!=1) // Mientras no se presione back
				{
					switch(mod)
					{
					case 0:
							s=0;
							while((!((0xD0 < adc_valX) &&(adc_valX < 0xEA) &&
									(0x2A < adc_valY) && (adc_valY < 0x5A))) && salir==0) // Mientras no se presione back
							{
								var_aux=Func_MuestraTeclado(5);
								if( var_aux != -1 ){						// Valida el numero del teclado que se pulso.

									if (indice < 2){						// Si no cargo los 4 Digitos, lo carga sino limpia la clave.

										tcla[indice] = var_aux;				// Carga en el vector los digitos de la clave.
										indice ++;
									}else{

										indice = 0;							// Reseteo el indice del vector de la clave.
										cur = 48;							// Acomodo el cursor para la proxima vez.
										WG12864A_posXY(cur, 2);				// Limpio la clave en pantalla.
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
										proxh[0]=varmod;					//Carga la hora
										WG12864A_posXY(80, 7);
										WG12864A_print_symbol(OK16x16, NEGRO);
										delay32Ms(0, TIMMER_FONDO_BLANCO);	// Para demorar su utiliza vTaskDelay()
										adc_valX = 0;						// Reseteo el valor de X, Y del ADC
										adc_valY = 0;
										salir=1;
										mod=1;
										s=0;
										indice = 0;							// Reseteo el indice del vector de la clave.
										cur=48;
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
								delay32Ms(0, TIMMER_FONDO_BLANCO);			// Para demorar su utiliza vTaskDelay()
								cur=48;
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
										cur = 48;							// Acomodo el cursor para la proxima vez.
										WG12864A_posXY(cur, 2);				// Limpio la clave en pantalla.
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
										proxh[1]=varmod;					//Cargo los minutos
										WG12864A_posXY(80, 7);
										WG12864A_print_symbol(OK16x16, NEGRO);
										delay32Ms(0, TIMMER_FONDO_BLANCO);	// Para demorar su utiliza vTaskDelay()
										adc_valX = 0;						// Reseteo el valor de X, Y del ADC
										adc_valY = 0;
										salir=1;
										mod=2;
										s=0;
										indice = 0;							// Reseteo el indice del vector de la clave.
										cur=48;
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
								delay32Ms(0, TIMMER_FONDO_BLANCO);			// Para demorar su utiliza vTaskDelay()
								cur=48;
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
							while((!((0xD0 < adc_valX) &&(adc_valX < 0xEA) &&
									(0x2A < adc_valY) && (adc_valY < 0x5A))) && salir==0) // Mientras no se presione back
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
									delay32Ms(0, TIMMER_FONDO_BLANCO);		// Para demorar su utiliza vTaskDelay()
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
								delay32Ms(0, TIMMER_FONDO_BLANCO);			// Para demorar su utiliza vTaskDelay()
								cur=48;
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
	if(flag_sleepsubmenu)
	{
		WG12864A_posXY(110, 7);
		WG12864A_print_symbol(BACK16x16, NEGRO);
		delay32Ms(0, TIMMER_FONDO_BLANCO);									// Para demorar su utiliza vTaskDelay()
		adc_valX = 0, adc_valY = 0;											// Reseteo el valor de X, Y del ADC.
		WG12864A_Limpiar(NEGRO);
		menu3 = 0;
	}
	flag_sleepsubmenu = 1;													// Restablesco el valor original de la bandera.
}

//--------------------------------------------------------------------------
/****************************  Submenues 4  ********************************/
//--------------------------------------------------------------------------
//						  4.1  Actualiza FW del MF
//--------------------------------------------------------------------------
void func_actualiza_fw(void)
{
	menuactual=afw;
	actualiza_fw=1;
	WG12864A_posXY(1, 1);
	WG12864A_printf("*Actualizar Firmware*", Arial8x6, BLANCO);
	WG12864A_posXY(1, 3);
	WG12864A_printf(" Espere... ", Arial8x6, NEGRO);
	WG12864A_posXY(1, 5);
	WG12864A_printf(" Actualizando fw", Arial8x6, NEGRO);
	WG12864A_posXY(1, 6);
	WG12864A_printf(" ...", Arial8x6, NEGRO);
	delay32Ms(0, TIMMER_ACTUALIZA_FW);									// Para demorar su utiliza vTaskDelay()
	menu41_1 = -1;														// Desabilito que entre al SubMenú.
	menu41 = -1;														// Habilito que entre al Menu Seleccionado.
	menu4 = -1;
	menu = 0;
	menuactual=consp;													// Necesario para sacarlo de la comunicación SPI modo afw
	flag_sleepsubmenu = 1;												// Restablesco el valor original de la bandera.

}
//--------------------------------------------------------------------------
//ss	2017
