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
					b_tog[],sumaerr,lec;
extern int 			h,mi,seg,d,m,anio,ds,proxf[],proxh[],var_aux;

//--------------------------------------------------------------------------
//***********************  Variables Propias  ******************************

int					norecarga=0,muefunc=0;
char				menuactual,toca,toca1,modo_func,actualiza_fw=0,
					t,pressok,prs,velc,dir_comu,sd_disp,
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
static char  		buffer_pulso[44] ;


int 				inc_ind=0,aux1,aux2;
unsigned int 		i= 0,i_buff=0,i_disp=0,i_disp_ant=0;
//int 				/*valpru[100],*/mu_pro[100];
char 				PROMEDIAR=0,ACT_DISP=0,NO_ENTR=0,DISMINUYO=0,AUMENTO=0;


int 				Umbral_Sup=3000,Umbral_Inf=2000,
					cont_pico_POS=0,cont_pico_NEG=0,PPM=0,
					inc_ind,inc_ind2,i_ind_buff,PARLANTE,
					lat_adc,lat_ant,LAT_PROM;
char 				PULSO_OK,flag_SUP=1,flag_INF=1,
					pos_x,pos_y,PUNTO,AUMENTO,DISMINUYO,
					cont_10seg,FILTRO_DIG,hab_corazon,
					PROMEDIAR,NO_ENTR,ACT_DISP,v,
					ind_adc=0,SUBIR,BAJAR,desp_vert,SALTO;
unsigned int 		valpru[100],valpru2[80],ind_Muestras,aux_prov;
//unsigned int valpru[1000],valpru2[800],ind_Muestras,aux_prov;
//extern int valpru[];
extern char 		flag_1seg,flag_25ms,cont_1min,cont_1seg;

uint16_t leo_adc(char Channel);
void detector_pulsos(void);
void grafica_PPM(void);


//--------------------------------------------------------------------------
void TIMER32_1_IRQHandler(void)
{
																					// La interrupcion la genero el MRO??
	if( LPC_TMR32B1->IR & 0x01 )
	{

		LPC_TMR32B1->IR = 1;														// Reseteo el Flag de IRQ.
		flag_25ms=1;																// Flag 25ms
		cont_1seg++;																// Incremento cont 1seg
		if(cont_1seg>=40)
		{
			cont_1seg=0;
			flag_1seg=1;															// Flag 1seg
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
		WG12864A_printf(" N:Pepita     E:26", Arial8x6, NEGRO);
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
	while((!((0xD0 < adc_valX) &&(adc_valX < 0xEA) &&
			(0x2A < adc_valY) && (adc_valY < 0x5A)))&& (pressok==0)) 		// Mientras no se presione back
	{
		WG12864A_posXY(1, 1);
		WG12864A_printf("  *Borrar Paciente*  ", Arial8x6, BLANCO);
		WG12864A_posXY(1, 3);
		WG12864A_printf(" N:Pepita     E:26", Arial8x6, NEGRO);
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
	char m=0,n=0;
	// Mientras no se presione Back.
	while(! ((0xD0 < adc_valX) && (adc_valX < 0xEA) &&
			 (0x2A < adc_valY) && (adc_valY < 0x5A)))
	{
		// Titulo del menu.
		WG12864A_posXY(1, 1);
		WG12864A_printf(" *  Monitor Fetal  * ", Arial8x6, BLANCO);
		//SACAR DE ACA
		grafica_PPM();														// Función que detecta los pulsos del corazon
			if(flag_25ms)													// Toma una muestra cada 5ms!
			{																// 1800 muestras en 10seg
				flag_25ms=0;
				if(inc_ind<1000)
				{
//					valpru[inc_ind]=leo_adc(5);								// Toma las primeras 1000
				}
				else
				{
//					valpru2[inc_ind2]=leo_adc(5);							// Toma las ultimas 800
					inc_ind2++;
				}
	/*			if(PARLANTE>20)
					GPIOSetValue( 2, 10, 0 );								// Habilito salida de latido 1 vez por seg PRUEBA
	*/			inc_ind++;
				aux_prov++;
	//			PARLANTE++;
			}
			if(inc_ind>=100)
			{
				inc_ind=0;
				PROMEDIAR=1;
				NO_ENTR=1;
				ACT_DISP=1;
			}
			if(flag_1seg)													// Contador 1 seg
			{
				flag_1seg=0;
				cont_10seg++;
				if(PARLANTE)
				{
					WG12864A_posXY(70, 7);
					WG12864A_print_symbol(HEART16x16, BLANCO);

					GPIOSetValue( 2, 10, 1 );								// Habilito salida de latido 1 vez por seg PRUEBA
					PARLANTE=0;
				}
				else
				{
					WG12864A_posXY(70, 7);
					WG12864A_print_symbol(HEART16x16, NEGRO);
					GPIOSetValue( 2, 10, 0 );								// Habilito salida de latido 1 vez por seg PRUEBA
					PARLANTE=1;
				}
			}
			if(cont_10seg>=10)												// A los 10 seg
			{
				ind_Muestras=aux_prov;										// almacena la cantidad de muestras que se tomaron
				aux_prov=0;													// Resetea variables
				inc_ind=0;
				inc_ind2=0;
				PROMEDIAR=1;												// Habilita el promedio de las muestras
				NO_ENTR=1;
				ACT_DISP=1;
				detector_pulsos();											// Función que detecta los pulsos del corazon
				cont_10seg=0;
			}
	//		Graf_PROM();

		// Funcion que maneja el Sleep de la pantalla y la IRQ del TOUCH.
		sleep=1;
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
//	Detecta Picos + y - del corazón
//--------------------------------------------------------------------------
void detector_pulsos(void)
{
	int i=0,j=0;
	while(i<1000)															// Analiza el primer buffer
	{
		if(valpru[i]>Umbral_Sup && flag_INF)								// Debe superar el Umbral sup y haber pasado un pico -
		{																	// Para no detectar falso pico +
			flag_SUP=1;
			flag_INF=0;
			cont_pico_POS++;
		}
		if(valpru[i]<Umbral_Inf && flag_SUP)								// debe ser menor al Umbral neg y haber pasado un pico +
		{																	// para no detectar un falso pico -
			flag_SUP=0;
			flag_INF=1;
			cont_pico_NEG++;
		}
		i++;
	}
/*	while(1000<i &&i<1800)													// Analiza el 2do buffer mismo procedimiento
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

	if(cont_pico_POS<cont_pico_NEG+2 && cont_pico_POS>cont_pico_NEG-2)		// Si conto la misma cantidad de picos + y -
	{																		// +/-2 de error, entonces
		PPM=cont_pico_POS*6;												// Calcula las pulsaciones (cant en 10seg*6=PPM)
		PULSO_OK=1;
	}
*/	cont_pico_POS=0;
	cont_pico_NEG=0;
	PPM++;
	if(PPM>165)
		PPM=60;
	if(PPM<60)
		PPM=60;
	Grafica_monitoreo(PPM);
}

void grafica_PPM(void)
{
//	char PPM ;
	char *		renglon = buffer_pulso ;
	char n;
	int aux1,aux2,i=0,m;
//-------------------------------------------------------------------------- Grafica recuadro
	WG12864A_posXY(110, 7);
	WG12864A_print_symbol(BACK16x16, BLANCO);
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
	WG12864A_posXY(40, 7);
	WG12864A_printf(buffer_pulso, Arial8x6, NEGRO);
	if((0xA0 < adc_valX) &&(adc_valX < 0xB9) &&
			(0x2A < adc_valY) && (adc_valY < 0x5A))							// Se presionó FD?
	{
		adc_valX = 0, adc_valY = 0;											// Reseteo el valor de X, Y del ADC.
		if(FILTRO_DIG)
			FILTRO_DIG=0;
		else
			FILTRO_DIG=1;
	}
	if(FILTRO_DIG)															// Grafica boton FD
	{
		WG12864A_posXY(90, 7);
		WG12864A_print_symbol(FD16x16, BLANCO);
	//	PPM=121;//SACARRRRR
		*renglon++ = ((PPM/100) % 10) + '0' ;
		*renglon++ = ((PPM/10)  % 10) + '0' ;
		*renglon++ = ( PPM      % 10) + '0' ;

	}
	else
	{
		WG12864A_posXY(90, 7);
		WG12864A_print_symbol(FD16x16, NEGRO);
	//	PPM=60;//SACARRRR
		*renglon++ = ((PPM/100) % 10) + '0' ;
		*renglon++ = ((PPM/10)  % 10) + '0' ;
		*renglon++ = ( PPM      % 10) + '0' ;

	}

//--------------------------------------------------------------------------// Grafica Corazón
/*	if(PPM>70&&PPM<150)
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
	v++;																	// Apaga Corazon
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
*/
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
