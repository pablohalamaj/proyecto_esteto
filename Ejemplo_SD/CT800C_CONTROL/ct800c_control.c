/**********************************************************
 *	Name        : ct800c_control.c
 *  Created on	: 05/10/2011
 *  Author		: Sebastian Sisevich
 * 	Copyright   : SS2011
 **********************************************************/


#include "msjs.h"
#include "sd_mmc_spi.h"
#include "ct800c_control.h"
#include "sd_mmc_spi_mem.h"
#define MMC_READ_SINGLE_BLOCK             17   								// read a block
//-------------------------------------------------------------------------------------------------------------------------------
//******************************************  Definición FreeRTOS Modpot  ******************************************************/
xTaskHandle xTaskmodpotHandle;												//-----------------
#define normal_TASK_PRIORITY   	(tskIDLE_PRIORITY + 3)						//-----   2   -----																		//-----------------
extern void	task_modpot (void * param) ;									//-----------------


//_______________________________________________________________________________________________________________________________
//_______________________________________________________________________________________________________________________________
char far					version[] = "CT800C 9.07" ;
//_______________________________________________________________________________________________________________________________
//_______________________________________________________________________________________________________________________________

//-------------------------------------------------------------------------------------------------------------------------------
//******************************************  Variables propias  ***************************************************************/
U16							sec_cal;
U32	 						*offset_cal,*offset;
static U32					ptr_memsd;
U32							sd_sector=7500;
//unsigned long				sd_sector=7500;
int 						leoflash,leoflashint,auxti,anio , mes,reset_s=0,cont_conf_TO=0,conf_TO=0,cuent_desact=0;
int 						t=0,lec=0,acterr=0,led_tit=0,led_apa=0,lec_tension=0,refresh=0;
char 						taux1=0,taux2=0,taux3=0,taux4=0,renglon1[16],checkvr=0,esta_normal=1 ;
char						copia_guia_param_F[TAM_GUIA] = "guia F" ;	// los dos primeros bytes indican origen
char						copia_guia_param_V[TAM_GUIA] = "guia V" ;	// los dos primeros bytes indican origen
BYTE						sinpar=0,ledconf=0,muememdis=0,val_adc,tens=0,VADC_OK=0;
BYTE						ADC_filtrado[2],apagdis=0,noentra=0,diase,tension = 0 ;
DWORD 						*ptr_ini,HOT_RESET;
BOOLEAN 					plaq_cfl=SI,estado_apa=NO,estado_titi=NO ;
BYTE far 					espacio_de_memoria_128k[0x2000]	;
UNIDAD_LONG					dets = 0x0FFFF ;
static BYTE					BT_SALTO_MAX=0x10,valor_ant=0 ;
unsigned char 				*RAM_DOWNLOAD = (struct DATOS_FLASH far *) 0x0D0000000L;//(unsigned char *)SDRAM;
unsigned char 				*RAM_DOWNLOADF = (struct DATOS_FLASH far *) 0x0D0000000L;//( (unsigned char *)SDRAM+0x10000 ) ;
unsigned char 				*RAM_DOWNLOAD_ULT = (struct DATOS_FLASH far *) 0x0D0020000L;//( (unsigned char *)SDRAM+0x20000 ) ;
unsigned char 				*FLASH_FIRMWARE = (struct DATOS_FLASH far *) 0x0D0000000L;//(unsigned char *)SDRAM ;
unsigned char 				*SDA_FLASH_LOADER = (struct DATOS_FLASH far *) 0x0D0000000L;//(unsigned char *)SDRAM ;
unsigned long				spec_version_fw[17] ;
unsigned long				pos_HOT_RESET = (unsigned long)&HOT_RESET ;
unsigned long int 			regmem=0xF0,mem=1500;//0xFF
struct STRUCT_SLQ 			struct_SLQ ;
struct DATOS_FLASH far *	ptrFLASH_PARAM = NULL ;
struct DATOS_FLASH far *	datos_flashint = NULL;
volatile unsigned char *	ptr_sdram = SDRAM;
struct STRUCT_LOG_ERR 		errores[10];
extern char 				entrq,entvc,func_cicl;
unsigned char				SD_HC=0;
//Todo
//-------------------------------------------------------------------------------------------------------------------------------
//******************************************  Configuración PLL0 a 48MHz  ******************************************************/
//-------------------------------------------------------------------------------------------------------------------------------
void local_start_pll(void)
{
	volatile avr32_pm_t *pm = &AVR32_PM;

	pm_switch_to_osc0(pm, FOSC0, OSC0_STARTUP);								// Switch the main clock to OSC0
/*	// Setup PLL0 on OSC0, mul=7 ,no divisor, lockcount=16, ie. 12Mhzx(7+1) = 96MHz output
	pm_pll_setup(pm,0,7,1,0,16);											//*pm, PLL ,mul,div,osc,lockcount
	//*pm, PLL,freq0=0-80/1=80-180,pll_div2=1=>divpor2;=0=>divpor1,pll_wbwdisable
	pm_pll_set_option(pm,0,1,1,0);											// set PLL options to run @ 48Mhz (Div 2)
	pm_pll_enable(pm,0);													// Enable PLL0
	pm_wait_for_pll0_locked(pm) ;											// Wait for PLL0 locked
	pm_gc_setup(pm,0,1,0,0,0);												// *pm, ,0=Osc/1=PLL,0=PLL0/1=PLL1
	pm_gc_enable(pm, 0);													// Enable Generic clock 0
    pm_cksel(pm,2,0,0,0,0,0);												//*pm,pbadiv,pbasel,pbbdiv,pbbsel,hsbdiv,hsbsel
    flashc_set_wait_state(1);												// one wait state at 48 Mhz
    gpio_enable_module_pin(AVR32_PM_GCLK_0_0_PIN, AVR32_PM_GCLK_0_0_FUNCTION);// Output the clock to a gpio(PA7)
	pm_switch_to_clock(pm, AVR32_PM_MCCTRL_MCSEL_PLL0);						// switch to clock
*/
}

//Todo
//-------------------------------------------------------------------------------------------------------------------------------
//******************************************  carga parametros al inicializar desde memoria flash  ******************************
//-------------------------------------------------------------------------------------------------------------------------------
int carga_parametros ()
{
	int 				i;
	BYTE 				*t;
	Bool				re;
	U32 				punt=0,kons=512;
	unsigned long int	sector=601;											// Sector de comienzo para el grabado de parametros
//	volatile unsigned char *ptr_sdram = SDRAM;								// No iniciar no funciona SD con esto!!!
//	ptrFLASH_PARAM=ptr_sdram+0x10000;
	datos_flashint=(struct DATOS_FLASH far *) 0x080067000;					// Puntero a flash interna
	sdramc_init(CP_CPU_SPEED);												// Inicializacion de la SDRAM externa
	t=datos_flashint;
	ptr_sdram = SDRAM;														// Puntero a sdram externa
	ptr_sdram=ptr_sdram+0x10000;
	t=datos_flashint;
/*//------------------------------------------------------------------------// -->  Graba a SDRAM desde flash interna
	for(i=0; i < 49152; i++)												// Se habilita solo si hay prog en flash interna
	{
		*ptr_sdram++ = *t++;												// Copia los 48Kbytes de la flash a la sdram
	}
*///------------------------------------------------------------------------// -->  Graba a flash externa desde SDRAM
/*	ptr_sdram = SDRAM;														// Habilitar solo para cambiar el prog de
	ptr_sdram=ptr_sdram+0x10000;											// flash interna al iniciar...
	for(punt=0;punt<48;punt++)												// Graba 48 sectores de 1k
	{
		if (at45dbx_write_open(sector) == OK)
		{
			for(i=0; i < 1024; i++)											// copio los 48kbyte a flash externa
			{
				at45dbx_write_byte(*ptr_sdram++);
			}
			at45dbx_write_close();
			sector=sector+10;												// Incrementa un sector despues de 1k
		}
	}
*/
//--------------------------------------------------------------------------
//	compara_flash();														// Se usa para ver los resultados en debug
//--------------------------------------------------------------------------// -->  Graba a SDRAM desde flash externa
	sector=601;
	ptr_sdram = SDRAM;
	ptr_sdram=ptr_sdram+0x10000;
	for(punt=0;punt<48;punt++)
	{
		if(at45dbx_read_open(sector) == OK)
		{
			for(i=0; i < 1024; i++)
			{
				*ptr_sdram++ = at45dbx_read_byte();							//Copia los 48Kbytes de la flash a la sdram
			}
			at45dbx_read_close();
			sector=sector+10;
		}
	}

//--------------------------------------------------------------------------// Inicia SD-MMC

	if (gpio_get_pin_value(AVR32_PIN_PC00)==0)								// SD Card detec
	{
		ptr_sdram = SDRAM+0x10000;
		re=sd_mmc_spi_internal_init();        								// Restart Init of SD/MMC card after previous first init
		if(re==KO)
		{
			leds (CONF) ;
		}
		else
		{
//--------------------------------------------------------------------------// Graba a SD-MMC desde SDRAM
			if (gpio_get_pin_value(AVR32_PIN_PC01)==0)						// SD write protec
			{
				ptr_sdram = SDRAM+0x10000;
				leds (SDMMC) ;
				buscaensd(0);												// Se posiciona en ROM externo (grabado desde el CT)
				for(punt=0;punt<96;punt++)									// Graba 96 sectores de 512 = 48k
				{
					if (sd_mmc_spi_write_open (sd_sector) == OK)			// Abre en forma de escritura
					{
						sd_mmc_spi_write_sector_from_ram((ptr_sdram+(punt*kons)));	// Graba en SD-MMC
						leds (SDWR) ;
					}
					sd_mmc_spi_write_close ();
					sd_sector++;
				}
			}
//--------------------------------------------------------------------------// Graba a SDRAM desde SD-MMC
			if((ENTRADAS_IO &  JUMPER7) != 0)								// Segun jumper 7...
			{
				buscaensd(1);												// Se posiciona en ROM externo (grabado desde la PC)
			}
			else
			{
				buscaensd(0);												// Se posiciona en ROM externo (grabado desde el CT)
			}
			ptr_sdram = SDRAM+0x10000;										// Posición SDRAM
			leds (SDMMC) ;
			for(punt=0;punt<96;punt++)										// Graba 96 sectores de 512 = 48k
			{
				if (sd_mmc_spi_read_open (sd_sector) == OK)					//
				{
					leds (SDWR) ;
					sd_mmc_spi_read_sector_to_ram (offset+(punt*kons));		// lee la SD-MMC
				}
				sd_mmc_spi_read_close ();
				if(!checkvr)
					sd_sector++;
				else
					punt--;

			}
			leds (SDMMC) ;
		}
	}// Fin de inicializar
	else
	{
		muememdis=FLASH_EXT;
	}
//--------------------------------------------------------------------------// Inicia el puntero de parametros
	ptr_sdram = SDRAM;
//	ptrFLASH_PARAM = (ptr_sdram+0x10000);
	ptrFLASH_PARAM = (struct DATOS_FLASH far *) 0x0D0010000L;				// Apunta a SDRAM externa

	if((ptrFLASH_PARAM->jbf_final_final[0])==0xaa)							// Compara si los param son correctos
	{
		ptrFLASH_PARAM = (struct DATOS_FLASH far *) 0x0D0010000L;			// Posición en sdram de parametros
		sinpar=0;
		cicsinp=0;
		cicled=NORMAL;															// Con parametros ciclado normal

	}
	else
	{
//--------------------------------------------------------------------------//Rellena la memoria vacia
		sector = 601;														// (Primer uso)
		for(punt=0;punt<48;punt++)											// Graba 48 sectores de 1k
		{
			if (at45dbx_write_open(sector) == OK)
			{
				for(i=0; i < 1024; i++)										// copio los 48kbyte a flash externa
				{
					at45dbx_write_byte(0x00);
				}
				at45dbx_write_close();
				sector=sector+10;											// Incrementa un sector despues de 1k
			}
		}

//--------------------------------------------------------------------------
		ptrFLASH_PARAM = (struct DATOS_FLASH far *) 0x0D0010000L;			// Posición en sdram de parametros
		//ptrFLASH_PARAM=(struct DATOS_FLASH far *)0x080067000;				// Posición de mem sin parametros
		sinpar=1;
		cicsinp=5;															// Sin parametros ciclado en conflicto
		muememdis=SIN_PA;
//		noentra=1;
	}
	return 0 ;
}

//Todo
//-------------------------------------------------------------------------------------------------------------------------------
//******************************************  Carga parametros de visual a flash  **********************************************/
//-------------------------------------------------------------------------------------------------------------------------------
int FLASH_cargar_param ()
{
	int 				i,punt=0;
	unsigned long int	sector=601;											// Sector de comienzo para el grabado de parametros
	BYTE env='!',env1='.',env2='*';
	apagdis=1;
	while(!noentra);
	ptr_sdram=(struct DATOS_FLASH far *) 0x0D0000000L;//ptr_sdram;
	for(i=0;i<512;i++);														// Espera que el puerto este listo
	CT800C_USART2_tx_byte(env );											// Envia "!"
	for(punt=0;punt<48;punt++)												// Graba 48 sectores de 1k
	{
		if (at45dbx_write_open(sector) == OK)
		{
			for(i=0; i < 1024; i++)											// copio los 48kbyte a flash externa
			{
				spi_selectChip(AT45DBX_SPI, AT45DBX_SPI_NPCS);
				at45dbx_write_byte(*ptr_sdram++);							// Escribe en flash lo que esta en sdram
				if(i==512)
				{
					CT800C_USART2_tx_byte (env1 );							// Envia un "." cada medio k
				}
			}
			at45dbx_write_close();
			sector=sector+10;												// Incrementa un sector despues de 1k
			CT800C_USART2_tx_byte (env1 );									// Envia un "." cada medio k
		}
	}
	for(i=0;i<512;i++);														// Espera que el puerto este listo
	CT800C_USART2_tx_byte(env2);											// Envia "*" fin de transmisión
	for(i=0;i<512;i++);														// Espera que el puerto este listo
	for(i=0;i<512;i++);														// Espera que el puerto este listo
//	gpio_set_gpio_pin(AVR32_PIN_PB19);										// Activo WDT externo
//	apagdis=0;
	reset_s=1;																// Desborda el wdt se resetea el micro!
	return 0 ;

}


//-------------------------------------------------------------------------------------------------------------------------------
//******************************************  Carga parametros de visual a flash  **********************************************/
//-------------------------------------------------------------------------------------------------------------------------------
int FLASH_cargar_vparam (BOOLEAN 	desplazado )
{
	int 				i,punt=0;
	unsigned long int	sector=601;											// Sector de comienzo para el grabado de parametros
	BYTE env='!',env1='.',env2='*';
	apagdis=1;
	while(!noentra);
	ptr_sdram=(struct DATOS_FLASH far *) 0x0D0000000L;//ptr_sdram;
	for(i=0;i<512;i++);														// Espera que el puerto este listo
	CT800C_USART2_tx_byte (env );											// Envia "!"
	for(punt=0;punt<48;punt++)												// Graba 48 sectores de 1k
	{
		if (at45dbx_write_open(sector) == OK)
		{
			for(i=0; i < 1024; i++)											// copio los 48kbyte a flash externa
			{
				spi_selectChip(AT45DBX_SPI, AT45DBX_SPI_NPCS);
				at45dbx_write_byte(*ptr_sdram++);							// Escribe en flash lo que esta en sdram
				if(i==512)
				{
					CT800C_USART2_tx_byte (env1 );									// Envia un "." cada medio k
				}
			}
			at45dbx_write_close();
			sector=sector+10;												// Incrementa un sector despues de 1k
			CT800C_USART2_tx_byte(env1 );											// Envia un "." cada medio k
		}
	}
	for(i=0;i<512;i++);														// Espera que el puerto este listo
	CT800C_USART2_tx_byte (env2);													// Envia "*" fin de transmisión
	for(i=0;i<512;i++);														// Espera que el puerto este listo
	for(i=0;i<512;i++);														// Espera que el puerto este listo
//	gpio_set_gpio_pin(AVR32_PIN_PB19);										// Activo WDT externo
//	apagdis=0;
	reset_s=1;																// Desborda el wdt se resetea el micro!
	return 0 ;

}


//Todo
//-------------------------------------------------------------------------------------------------------------------------------
//******************************************  Funciones que tratan con el HW  **************************************************/
//-------------------------------------------------------------------------------------------------------------------------------
extern BOOLEAN estado_titi ;
extern BOOLEAN estado_apa  ;

void lee_llaves_de_hw (BOOLEAN *	bit_tit ,BOOLEAN *	bit_apa )
{
	BOOLEAN 	estado_tit ;
	BOOLEAN		estado_apa ;

	if ( CT800C_Read_Pin_IO(LL_TIT) )
	{
		estado_tit = SI ;
		led_tit=1;
	}
	else
	{
		estado_tit = NO ;
		led_tit=0;
	}
	if ( CT800C_Read_Pin_IO(LL_APAG) )
	{
		estado_apa = SI ;
		led_apa=1;
	}
	else
	{
		estado_apa = NO ;
		led_apa=0;
	}

	*bit_tit = estado_tit ;
	*bit_apa = estado_apa ;
}

BOOLEAN hw_conflicto_anulado ()
{
	if ( CT800C_Read_Pin_IO(JUMPER1) && (!conf_TO))							// Anula conflictos segun J1
	{
		cuent_desact=1;
		return SI;
	}
	else
	{
		cuent_desact=0;
		cont_conf_TO=0;
		return NO;
	}
	if(!CT800C_Read_Pin_IO(JUMPER1))										// Si saco J1 borro conflicto por time out
	{
		conf_TO=0;
		cuent_desact=0;
		cont_conf_TO=0;
	}
}

//Todo
//-------------------------------------------------------------------------------------------------------------------------------
//******************************************  Interrupción de la lectura de tensión  *******************************************/
//-------------------------------------------------------------------------------------------------------------------------------
extern BYTE	tension;
void comienza_hw_lectura_tension ()
{
	adc_enable(TOUCH_ADC, TOUCH_ADC_SENS_CHANNEL);							// Habilito el canal de ADC correspondiente
	adc_start(TOUCH_ADC);													// Comienzo conversion
	val_adc = adc_get_value(TOUCH_ADC, TOUCH_ADC_SENS_CHANNEL);				// Tomo el valor sensado
	adc_disable(TOUCH_ADC, TOUCH_ADC_SENS_CHANNEL);
	VADC_OK=1;
}
/*
__attribute__((__interrupt__)) static void 	tension_int_handler (void) ;

__attribute__((__interrupt__)) static void tension_int_handler(void)
{
	lec_tension=0;															// limpio el flag del timer de 5ms
	gpio_clear_pin_interrupt_flag(lec_tension);								// limpio la interrupcion del pin
	gpio_disable_pin_interrupt(lec_tension);								// deshabilito la interrupcion de BT
	comienza_hw_lectura_tension ();											// Salta a la función para que el ADC convierta
	gpio_enable_pin_interrupt(lec_tension, GPIO_RISING_EDGE);				// habilito la interrupcion de BT
}
*/
void SENS_Init(void)
{
//	INTC_register_interrupt(&tension_int_handler, SENS_IRQ + (lec_tension/8), AVR32_INTC_INT3);	// registro la interrupcion
//	gpio_disable_pin_interrupt(lec_tension);								// deshabilito la interrupcion de BT
//	TOUCH_ADC->ier =AVR32_GPIO_IER;
//	gpio_enable_pin_interrupt(lec_tension, GPIO_RISING_EDGE);				// habilito la interrupcion de BT
}


//-------------------------------------------------------------------------------------------------------------------------------
//******************************************  Lectura de tension de alimentación  **********************************************/
//-------------------------------------------------------------------------------------------------------------------------------
BOOLEAN hw_lectura_tension (BYTE *	valor)
{
	BYTE aux;
	if(!VADC_OK)															// Espera que el ADC obtenga el resultado
		return NO;

	aux=val_adc;
	if(!aux)																// Si el ADC no convirtió no sensa
		return NO;

	if ( valor_ant == 0 )
	{
    	valor_ant = aux ;
    }
	else
	{
		if ( valor_ant > aux )
		{
			if ( (valor_ant - aux) > BT_SALTO_MAX )
				valor_ant =valor_ant;//-= BT_SALTO_MAX ;
			else
				valor_ant = aux ;
		}
		else
		{
			if ( (aux - valor_ant) > BT_SALTO_MAX )
				valor_ant =valor_ant;//+= BT_SALTO_MAX ;
			else
				valor_ant = aux ;
		}
	}
//	tension=valor_ant;

	if(tens<valor_ant)
	{
		tension=valor_ant;//aux;
		tens=valor_ant;//aux;
		VADC_OK=0;
//		*valor = tension;
	}
	if(refresh>2)
	{
		refresh=0;
		tens=0;
	}
	return NO ;	// Hay que modificar el programa para poner SI
//	return SI ;
}
//-------------------------------------------------------------------------------------------------------------------------------
UNIDAD_LONG lee_hardware_detectores ()
{
//	UNIDAD_LONG	dets = 0x0FFFF ;
	dets = 0x000FF ;

	dets = CT800C_Read_Port_IO();											// leo el puerto
	dets = (dets >> 8) & 0x0FFFF;											// desplazo y enmascaro para obtener las señales de detectores

	return dets ;															// retorno el valor de los detectores

}



//Todo
//-------------------------------------------------------------------------------------------------------------------------------
//******************************************  Funciones de almacenamiento y manejo de errores  **********************************
//-------------------------------------------------------------------------------------------------------------------------------

void inicia_errores(void)
{
	char taux1;
	if(at45dbx_read_open(regmem) == OK)										// Leo el valor de t de la mem al iniciar
	{
		taux1=at45dbx_read_byte();
		at45dbx_read_close();
		lec=taux1;
		t=taux1;
	}
}

BYTE registra_err (enum LISTA_BUFFER qbuffer ,LOGERR err,LOGPARAM param,BOOLEAN evento )	// Agregado fecha10/09/12
{
	char auxt1,auxt2,auxt3,auxt4,auxt;
//____________convierte int a char__________
	auxt1=tiempo_soft.tpo.tm_year%10;
	auxt=tiempo_soft.tpo.tm_year/10;
	auxt2=auxt%10;
	auxt=auxt/10;
	auxt3=auxt%10;
	auxt=auxt/10;
	auxt4=auxt%10;
//_______________Envia a flash_________________

	if (at45dbx_write_open(mem+t) == OK)									// Guardo el error en flash externa
	{
		t++;
		if(lec<10)
		{
			lec++;
		}
 		at45dbx_write_byte(t);	//lec
	   	at45dbx_write_byte(auxt1);
	   	at45dbx_write_byte(auxt2);
	   	at45dbx_write_byte(auxt3);
	   	at45dbx_write_byte(auxt4);
		at45dbx_write_byte(tiempo_soft.tpo.tm_mon);
		at45dbx_write_byte(tiempo_soft.tpo.tm_mday);
		at45dbx_write_byte(tiempo_soft.tpo.tm_hour);						// tiempo_soft.tpo.tm_hour
		at45dbx_write_byte(tiempo_soft.tpo.tm_min);
		at45dbx_write_byte(tiempo_soft.tpo.tm_sec);
		at45dbx_write_byte(err);
		at45dbx_write_byte(param);
		at45dbx_write_byte(cauerr);
		at45dbx_write_byte(moduerr);
		at45dbx_write_close();
		if(t>10)
		{
			mem=1500;//0xFF
			t=1;
		}
		leof=1;
	}
//____________convierte int a char__________
	regmem=0xF0;
	if (at45dbx_write_open(regmem) == OK)									// copio el numero de errores en flash externa
	{
		at45dbx_write_byte(t);
		at45dbx_write_close();

	}
	return 0 ;
}
//Todo
//-------------------------------------------------------------------------------------------------------------------------------
//******************************************  Cambio de modos por teclado  *****************************************************/
//-------------------------------------------------------------------------------------------------------------------------------
void interroga_modocicla(void)
{
	switch (func_cicl)
	{
	case CICLA_NORMAL:
		if(!esta_normal)
		{
			ejec_estado_inic ();
			esta_normal=1;
		}
		break;
	case CICLA_TITILANTE:
		ejec_estado_tit ();
		esta_normal=0;
		break;
	case CICLA_APAGADO:
		ejec_estado_apag ();
		esta_normal=0;
		break;
	case CICLA_EMERGENCIA:
		ejec_emergencia ();
		esta_normal=0;
		break;
	default:
		break;
	}
}
//Todo
//-------------------------------------------------------------------------------------------------------------------------------
//******************************************  Funciones de los Módulos de Potencia  ********************************************/
//-------------------------------------------------------------------------------------------------------------------------------
void ini_control_modpot ()
{
	CT800C_Inic_MP();														// Inicializacion de MODULOS DE POTENCIA
																			// creacion de la task de manejo de modulos
	xTaskCreate (task_modpot, (const signed portCHAR *)"MODPOT",
								configMINIMAL_STACK_SIZE, NULL, normal_TASK_PRIORITY,&xTaskmodpotHandle) ;		//normal
}

void reset_conflictos_modpot ()
{

}

BOOLEAN		nueva_combinacion = NO;

void nueva_combinac_luces ()
{

	nueva_combinacion = SI;													// nueva combinacion de salida de lamparas
	if ( CT_estado_func == ESTF_CFL )
	{
		modpot.PASAR_A_CONFLICTO = SI ;
		ledconf=1;
		cicled=CONF;
	}
	else
	{
		modpot.PASAR_A_CONFLICTO = NO ;
		ledconf=0;
		cicled=NORMAL;
	}
}

void espera_entre_resets ()
{
}

void reset_conflictos_hardware (BOOLEAN	y_rst_hw )
{
}

//Todo
//-------------------------------------------------------------------------------------------------------------------------------
//******************************************  Otras funciones  *****************************************************************/
//-------------------------------------------------------------------------------------------------------------------------------

void AJUSTE_tiempo_soft (int ini )
{
    if ( ini )
    {
    	unsigned int HH,MM,SS,DD,FF,M,AA;
    	DS1390_Get_Hora(&HH,&MM,&SS);
    	DS1390_Get_Dia(&DD, &FF, &M, &AA);
   		tiempo_soft.tpo.tm_year = AA;
   		tiempo_soft.tpo.tm_mon   = M;
   		tiempo_soft.tpo.tm_mday  = FF;
   		tiempo_soft.tpo.tm_wday = DD;
   		tiempo_soft.tpo.tm_hour  = HH;
   		tiempo_soft.tpo.tm_min   = MM;
   		tiempo_soft.tpo.tm_sec   = SS;
   		ini_softRTC () ;
    }
    else
    {
       	inc_softRTC () ;
    }
}

//-------------------------------------------------------------------------------------------------------------------------------
//******************************************  Busca posición de comienzo en SD-MMC  *********************************************
//-------------------------------------------------------------------------------------------------------------------------------
void buscaensd(BYTE prosd)
{
	Bool				res=0;
	U32 				envsd,punt=0,kons=512;
	BYTE				busct,flags=0,noencu=0;
	punt=0;
	busct=0;
	sd_sector=0x59d8;//0x3B4C;//15180//0x3B50;//15184;//15182;				// Sector base para busqueda del programa en SD-MMC
	offset=0xb3b000;//0x769800;//0x3A9800;//0x76A000;//0x769C00;			// Offset base para busqueda del programa en SD-MMC

	if(SD_HC)
	{
		sd_sector=0x59d8;													// Sector base para busqueda del programa en SD-MMC
		offset=0xb3b000;													// Offset base para busqueda del programa en SD-MMC
	}
	else
	{
		sd_sector=0x1D4C;													// Sector base para busqueda del programa en SD-MMC
		offset=0x3A9800;													// Offset base para busqueda del programa en SD-MMC
	}
	leds (SDMMC) ;
	while(flags!=2)
	{
		switch (busct)
		{
		case 0:																// Busca el inicio del programa
			while(res!=1&&punt<1000)										// Mientras no encuentre el ppio del prog o recorra 1000 sec
			{
				envsd=(offset+((punt*kons)>>2));							// Incrementa de a 512b
				res=sd_mmc_spi_search_sector_to_ram(envsd);					// lee el sector de programa de la SD-MMC OK=ppio del prog
				punt++;
			}
			if(prosd)														// Si esta el jumper levanta el programa de la PC
			{
				busct=2;
				break;
			}
			if(flags)														// Salta al 2do programa de la memoria SD-MMC
				busct=2;
			else
				busct=1;
			break;
		case 1:
			if(res)
			{
				punt--;
				sd_sector=sd_sector+punt+96;								// Pasa al segundo ROM en SD-MMC
				offset=offset_cal+(0xc000>>2);								// Carga el offset del 2do ROM en SD-MMC
				punt=0;
				flags=1;
				busct=0;
				res=0;
			}
			else
			{
				if(SD_HC)
				{
				sd_sector=0x59d8;													// Sector base para busqueda del programa en SD-MMC
				offset=0xb3b000;													// Offset base para busqueda del programa en SD-MMC
				punt=0;
				busct=0;
				flags=0;
				if(noencu++==3)												// Protección para salir por error al buscar
					flags=2;

				}
				else
				{
				sd_sector=0x1D4C;//7500;												// Sector base para busqueda del programa en SD-MMC
				offset=0x3A9800;											// Offset base para busqueda del programa en SD-MMC
				punt=0;
				busct=0;
				flags=0;
				if(noencu++==3)												// Protección para salir por error al buscar
					flags=2;
				}
			}
			break;
		case 2:
			if(res)
			{
				punt--;
				sd_sector=sd_sector+punt;									// Carga el sector de inicio
				offset=offset_cal;											// Carga el offset de inicio
				flags=2;
				if(prosd)
					muememdis=SD_PC;
				else
					muememdis=SD_CT;
			}
			else
			{
				if(SD_HC)
				{
				sd_sector=0x59d8;													// Sector base para busqueda del programa en SD-MMC
				offset=0xb3b000;													// Offset base para busqueda del programa en SD-MMC
				punt=0;
				busct=0;
				flags=0;
				if(noencu++==3)												// Protección para salir por error al buscar
					flags=2;

				}
				else
				{
				sd_sector=0x1D4C;//7500;												// Sector base para busqueda del programa en SD-MMC
				offset=0x3A9800;											// Offset base para busqueda del programa en SD-MMC
				punt=0;
				busct=0;
				flags=0;
				if(noencu++==3)												// Protección para salir por error al buscar
					flags=2;
				}
			}
			break;
		}
	}
}
//-------------------------------------------------------------------------------------------------------------------------------

Bool sd_mmc_spi_search_sector_to_ram(U32 *psd)
{
	U32 /**ptr_memsd,*/*offset;                       // Memory data pointer
	U8 *_ram = psd;
//  volatile unsigned char offset;
	U16  i,sector;
	U16  read_time_out;
	Bool op,ini;
	unsigned short data_read,dato;
	// wait for MMC not busy
	if (KO == sd_mmc_spi_wait_not_busy())
		return KO;
	ptr_memsd=psd;
	spi_selectChip(SD_MMC_SPI, SD_MMC_SPI_NPCS);    // select SD_MMC_SPI
	  // issue command
	  if(card_type == SD_CARD_2_SDHC) {
	    r1 = sd_mmc_spi_command(MMC_READ_SINGLE_BLOCK, ptr_memsd>>9);
	  } else {
	    r1 = sd_mmc_spi_command(MMC_READ_SINGLE_BLOCK, ptr_memsd);
	  }

	// check for valid response
	if (r1 != 0x00)
	{
		spi_unselectChip(SD_MMC_SPI, SD_MMC_SPI_NPCS);  // unselect SD_MMC_SPI
		return KO;
	}

	// wait for token (may be a datablock start token OR a data error token !)
	read_time_out = 30000;
	while((r1 = sd_mmc_spi_send_and_read(0xFF)) == 0xFF)
	{
		read_time_out--;
		if (read_time_out == 0)   // TIME-OUT
		{
			spi_unselectChip(SD_MMC_SPI, SD_MMC_SPI_NPCS); // unselect SD_MMC_SPI
			return KO;
		}
	}

	// check token
	if (r1 != MMC_STARTBLOCK_READ)
	{
		spi_write(SD_MMC_SPI,0xFF);
		spi_unselectChip(SD_MMC_SPI, SD_MMC_SPI_NPCS);  // unselect SD_MMC_SPI
		return KO;
	}

	// store datablock
	for(i=0;i<MMC_SECTOR_SIZE;i++)
	{
		spi_write(SD_MMC_SPI,0xFF);
		spi_read(SD_MMC_SPI,&data_read);
		dato=data_read;
//  	*ptr_sd_mmc++=data_read;
		switch (dato)															// Busca el inicio del programa
		{
		case 0xAA:
			ini=1;
			sector=i;
			offset=_ram;
			break;
		case 0x55:
			if(ini==1)
			{
				sec_cal=sector;													// Si cumple con AA 55 guarda los valores
				offset_cal=offset;
				i=600;															// Para salir del for
				op=OK;
			}
			else
			{
/*				sector=0;
				offset=0;
				op=KO;
*/			}
			break;
		default:
			ini=0;
			op=KO;
			break;
		}
		_ram++;
	}
//  ptr_memsd += 512;     // Update the memory pointer.
// 	load 16-bit CRC (ignored)
	spi_write(SD_MMC_SPI,0xFF);
	spi_write(SD_MMC_SPI,0xFF);
//	continue delivering some clock cycles
	spi_write(SD_MMC_SPI,0xFF);
	spi_write(SD_MMC_SPI,0xFF);
//	release chip select
	spi_unselectChip(SD_MMC_SPI, SD_MMC_SPI_NPCS);  // unselect SD_MMC_SPI
	return op;   // Read done.
}
//-------------------------------------------------------------------------------------------------------------------------------

#define UMBRAL_MIN_Vmains				125
extern unsigned long convierte_a_valores_ing (WORD 	dato_ADC );
void muestra_adcvsen ()
{
	char 			PROT02_TXT_ADCV[] = "Vin=***,* V   ." ;
	char far *		ptr ;
	unsigned long	valor ;

	ptr = PROT02_TXT_ADCV ;
    FarStrCpy (renglon1, ptr) ;
    valor=tension;
	format_hexa (renglon1+12, (WORD)valor, 2) ;
    valor *= (250 - UMBRAL_MIN_Vmains) * 100L ;
    valor /= 256 ;
    valor += UMBRAL_MIN_Vmains * 100L ;
    valor=tension;
    valor *= 5L * 100L ;
    valor /= 256L ;
    valor += 250L ;
    valor *= 33L ;
    valor = convierte_a_valores_ing (tension) ;
	format_decimal (renglon1+4, (WORD)(valor/100), 3) ;
	format_decimal (renglon1+8, (WORD)(valor%100), 2) ;
	return ;
}


//-------------------------------------------------------------------------------------------------------------------------------

BOOLEAN obtiene_fehora (struct TPO *tiempo_aux )
{
    tiempo_aux->flag_modif ++ ;												// la actualizacion lo incrementa en 1
	return 0 ;
}
void arma_string_base (char *renglon1 ,BOOLEAN flag_moduloZ )
{
	int						base ;
    int						i ;
	char far *				ptr ;
	enum CT_ESTADO_FUNC 	est_func ;
    ESTADO 					est ;
    ESTADO					est_fut ;
    PLAN 					pln ;
    PLAN					pln_fut ;
    PLAN					plan_aux ;
    plan_aux ;              // evita warning
	flag_moduloZ ;			// evita warning


    ptr = PROT02_TXT_ESTADO_BASE ;

    FarStrCpy (renglon1, ptr) ;

#ifdef PROT_MODULO_Z
    if ( flag_moduloZ ) {
		est_func = struct_Z.CT_estado_func ;
        est = struct_Z.estado ;
        est_fut = 0 ;
        pln      = struct_Z.plan ;
        pln_fut  = 0 ;
    } else
#endif
	{
		est_func = CT_estado_func ;
        est      = estado ;
        est_fut  = estado_FUTURO ;
        //pln      = ptrFLASH_PARAM->def_planes[plan-1].nombre ;
        plan_aux = plan - 1 ;
        pln      = ptrFLASH_PARAM->def_planes[plan_aux].nombre ;
        //pln_fut  = ptrFLASH_PARAM->def_planes[plan_FUTURO-1].nombre ;
        plan_aux = plan_FUTURO - 1 ;
        pln_fut  = ptrFLASH_PARAM->def_planes[plan_aux].nombre ;
    }

	switch ( est_func ) {
		case ESTF_NRM :
		    FarCpy (renglon1, PROT02_TXT_NORMAL) ;
			format_plan (renglon1+2, pln) ;
			format_plan (renglon1+3, pln_fut) ;
			if ( interv ) {
				//disp_wr (DISP_DATO, 'I') ;
				//disp_wr (DISP_DATO, ':') ;
				//disp_word (intervalo, 2) ;
				//base = (int) base_i ;
            	renglon1[5] = 'i' ;
				base_t = base_i ;
				//format_digito (renglon1+7, intervalo/10) ;
				//format_digito (renglon1+8, intervalo%10) ;
				format_decimal (renglon1+7, intervalo, 2) ;
			} else {
				//disp_wr (DISP_DATO, 'E') ;
				//disp_wr (DISP_DATO, ':') ;
				//disp_digito (estado) ;
				//disp_digito (estado_FUTURO) ;
            	renglon1[5] = 'E' ;
				format_digito (renglon1+7, est) ;
				format_digito (renglon1+8, est_fut) ;
			}
			if ( control_MANUAL() )
            	renglon1[0] = 'F' ;
        	break ;

		case ESTF_EMERG :
		    FarCpy (renglon1, PROT02_TXT_EMERGENCIA) ;
			if ( control_MANUAL() )
            	renglon1[0] = 'F' ;
 			renglon1[8] = datos_emergencias.campos.en_curso + '0' ;
			break ;

		case ESTF_INIC :
		    FarCpy (renglon1, PROT02_TXT_INICIALIZ) ;
			break ;

		case ESTF_LLAVE :
		    FarCpy (renglon1, PROT02_TXT_LLAVE_TIT) ;
			break ;

		case ESTF_BAJAT :
		    FarCpy (renglon1, PROT02_TXT_BAJA_TENS) ;
			break ;

		case ESTF_CFL :
		    FarCpy (renglon1, PROT02_TXT_CONFLICTO) ;
			break ;

		case ESTF_APAG :
		    FarCpy (renglon1, PROT02_TXT_APAGADO) ;
			if ( control_MANUAL() )
            	renglon1[0] = 'F' ;
			break ;

		case ESTF_TIT :
		    FarCpy (renglon1, PROT02_TXT_TITILANTE) ;
			if ( control_MANUAL() )
            	renglon1[0] = 'F' ;
			break ;

		case ESTF_ERR_INT_1 :
		    FarCpy (renglon1, PROT02_TXT_ERR_INT_1) ;
			break ;

		case ESTF_ERR_INT_2 :
		    FarCpy (renglon1, PROT02_TXT_ERR_INT_2) ;
			break ;


		default :
		    FarCpy (renglon1, PROT02_TXT_EST_ERROR) ;
			break ;
	}

#ifdef PROT_MODULO_Z
    if ( flag_moduloZ ) {
		format_hexa (renglon1+10, protZ_otra_copia_msj_tx[0]&0x0FF, 2) ;
		format_hexa (renglon1+12, protZ_otra_copia_msj_tx[1]&0x0FF, 2) ;
		format_hexa (renglon1+14, protZ_otra_copia_msj_tx[2]&0x0FF, 2) ;
        return ;
    }
#endif
	base = (int) base_t ;


	if ( CT_estado_func == ESTF_CFL ) {
        if ( timer_gen_reintentos > 9990L )
			format_decimal (renglon1+11, 999, 3) ;
        else {
        	i = (int) (timer_gen_reintentos / 10L) ;
			format_decimal (renglon1+11, i, 3) ;
        }
	} else if ( CT_estado_func == ESTF_BAJAT ) {
		format_decimal (renglon1+11, timeout_en_baja_tension, 3) ;
    } else
		format_decimal (renglon1+11, base/10, 3) ;

	if ( (CT_estado_func == ESTF_NRM) && (estado_policial>1) ) {
		if ( estado_policial == EST_POL_PULSLATCH )
			renglon1[15] = 'P' ;
		else
			renglon1[15] = 'p' ;
	} else {
		if ( CT_flags_int.bit.det_modo_manual )
			renglon1[15] = 'M' ;
        else
			renglon1[15] = (char) ((base%10) + '0') ;
	}

}
BYTE diase;
int anio , mes;
BYTE esc_reloj (struct TPO *tpo ,BOOLEAN reinic )
{
		if(tpo->tpo.tm_year>500)
		{
			tpo->tpo.tm_year=0;
		}
		mes=tpo->tpo.tm_mon+1;
		anio=tpo->tpo.tm_year+1980;
		tpo->tpo.tm_mon=mes;
		tpo->tpo.tm_year=anio+1;												// Prueba 1980 Ver!!!!
		tpo->tpo.tm_wday=calculo_DDS (tpo) ;
		diase=(tpo->tpo.tm_wday)-1;
		DS1390_Set_Dia(diase, tpo->tpo.tm_mday,mes, anio);	//Agregado 2013-07-15 Para el manejo del RTC
		DS1390_Set_Hora(tpo->tpo.tm_hour, tpo->tpo.tm_min, tpo->tpo.tm_sec);

	if(reinic)
	{
		AJUSTE_tiempo_soft (1) ;
		CT_flags_int.bit.ini_thoraria= SI;
		CT_flags_int.bit.ini_planes =SI;
	}

	return 1 ;
}
BYTE esc_reloj_CT800C (struct TPO * tpo,BOOLEAN reinic,BOOLEAN ho,BOOLEAN fe)
{
	if(fe)
	{
		if(tpo->tpo.tm_year>500)
		{
			tpo->tpo.tm_year=0;
		}
		mes=tpo->tpo.tm_mon+1;
		anio=tpo->tpo.tm_year+1980;
		tpo->tpo.tm_mon=mes;
		tpo->tpo.tm_year=anio+1;												// Prueba 1980 Ver!!!!
		tpo->tpo.tm_wday=calculo_DDS (tpo) ;
		diase=(tpo->tpo.tm_wday)-1;
		DS1390_Set_Dia(diase, tpo->tpo.tm_mday,mes, anio);	//Agregado 2013-07-15 Para el manejo del RTC
	}
	if(ho)
	{
		DS1390_Set_Hora(tpo->tpo.tm_hour, tpo->tpo.tm_min, tpo->tpo.tm_sec);
	}
	if(reinic)
	{
		AJUSTE_tiempo_soft (1) ;
		CT_flags_int.bit.ini_thoraria= SI;
		CT_flags_int.bit.ini_planes =SI;
	}

	return 1 ;
}

void format_plan ( char *ptr ,BYTE 	dato )
{
	if ( dato == 0 )
		*ptr = '-' ;
	else if ( dato < 10 )
		*ptr = dato + '0' ;
	else if ( dato == 10 )
		*ptr = '0' ;
	else if ( dato <= 32 )
		*ptr = dato - 10 + 'A' ;
    else if ( dato <= 127 )
    	*ptr = '+' ;
    else
    	*ptr = '?' ;
	/*
	else
		disp_wr (DISP_DATO, '+') ;
	*/
}

int commA_tx_byte (BYTE	byte )
{
	return 0 ;
}
//-------------------------------------------------------------------------------------------------------------------------------
//******************************************  Invierte bits y bytes de comunicaciones  ******************************************
//-------------------------------------------------------------------------------------------------------------------------------
int inv_bits(int a)
{
	int i=0,aux,aux1,aux2,aux3,aux4,aux5, conv[4];

	for(i=0;i<4;i++)
	{
		aux=((a>>(4*i))&0xf);
		aux1=(aux&0x1)<<3;
		aux2=(aux&0x2)<<1;
		aux3=(aux&0x4)>>1;
		aux4=(aux&0x8)>>3;
		conv[i]=aux1+aux2+aux3+aux4;
	}
	aux5=(conv[0]<<12)+(conv[1]<<8)+(conv[2]<<4)+(conv[3]);
	aux =aux5<<16;
	return aux;
}

//-------------------------------------------------------------------------------------------------------------------------------
//******************************************  Funciones vacias  ****************************************************************/
//-------------------------------------------------------------------------------------------------------------------------------

void reset_cfl (BOOLEAN y_rst_hw )
{
	reset_conflictos_modpot () ;
}

void doble_reset_cfl ()
{
}

int espera_inic_hw ()
{
	return 0 ;
}

BOOLEAN estado_RAM1000_propio (enum RAM1000	cual )
{
	return SI ;
}
BOOLEAN estado_RAM1000_datos (enum RAM1000 	cual )
{
	return SI ;
}
void datos_en_RAM1000 (enum RAM1000 	cual )
{
}
void libera_RAM1000 (enum RAM1000 	cual )
{
}

struct STRUCT_LOG_ERR 	structlogerr_nada = { { 0 }, 0, 0 } ;
struct STRUCT_LOG_ERR *busca_err (BYTE canal ,BOOLEAN primero ,BOOLEAN evento )
{
	return &structlogerr_nada ;
}

void _di_ ()
{
}

int FLASH_cargar_loader ()
{
    return 0 ;
}

BYTE parallel_port (BYTE	port ,BYTE 	init )
{
	return 0 ;
}

BOOLEAN verif_JBF (BYTE far *	ptr )
{
	return SI ;
}

int FLASH_duplicar_flash (BOOLEAN 	bkground )
{
    return 0 ;
}

int FLASH_cargar_firmware ()
{
	return 0 ;
}

int FLASH_cargar_firmwareF ()
{
    return 0 ;
}
UNIDAD_LONG lee_hardware_entradas ()
{
	return 0 ;
}
BOOLEAN condiciones_deshabilitadas ()
{
	return NO ;
}

void condiciones_set_port ()
{
}

void ini_comp_luces_modpot ()
{
}


BOOLEAN hw_reset_x_conflicto_anulado ()
{
	return NO ;
}

BOOLEAN hw_conflicto_tipo_X ()
{
	return SI ;
}

void hw_lee_conflictos_X ()
{
	return ;
}

extern BOOLEAN		plaq_cfl ;
BOOLEAN hw_plaq_conflictos_presente_X ()
{
	return plaq_cfl ;
}

BOOLEAN hw_conflicto_detectado_X ()
{
	return NO ;
}

BOOLEAN hw_falla_pulso_tit ()
{
	return NO ;
}





