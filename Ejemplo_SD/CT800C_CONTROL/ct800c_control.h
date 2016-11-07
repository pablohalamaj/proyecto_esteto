/**********************************************************
 *	Name        : ct800c_control.h
 *  Created on	: 05/10/2011
 *  Author		: Sebastian Sisevich
 * 	Copyright   : SS2011
 **********************************************************/

#ifndef CT800C_CONTROL_H_
#define CT800C_CONTROL_H_

//-------------------------------------------------------------------------------------------------------------------------------
//******************************************  Includes  ************************************************************************/

#include "adc.h"
#include "gpio.h"
#include "intc.h"
#include "board.h"
#include "_utils.h"
#include "sdramc.h"
#include "at45dbx.h"
#include "xCTtodo.h"
#include "xlogerrs.h"
#include "ct800c_io.h"
#include "ct800c_mp.h"
#include "ct800C_adc.h"
#include "ct800c_usart.h"
//****** Scheduler header files.
#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"

//-------------------------------
#define NORMAL					0
#define CONF					1
#define SDMMC					2
#define SDWR					3
//-------------------------------
#define SIN_PA					4
#define FLASH_EXT				1
#define SD_PC					2
#define SD_CT					3
//-------------------------------

//-------------------------------------------------------------------------------------------------------------------------------
//******************************************  Funciones propias  ***************************************************************/
void 				local_start_pll(void);
int					carga_parametros (void) ;
int					inv_bits(int);
Bool 				sd_mmc_spi_search_sector_to_ram(U32 *psd);
void				inicia_errores(void);
void				espera_entre_resets (void);
void				reset_conflictos_hardware (BOOLEAN y_rst_hw);
void 				buscaensd(BYTE prosd);
void				muestra_adcvsen (void) ;
void 				SENS_Init(void);
void				AJUSTE_tiempo_soft (int ini) ;
void 				(far * func_resetini) (void) ;
BYTE 				esc_reloj_CT800C (struct TPO * tpo,BOOLEAN reinic,BOOLEAN ho,BOOLEAN fe);
//int far			cons_paramavr (struct def_datos_PROT * datos, int dato) ;

//-------------------------------------------------------------------------------------------------------------------------------
//******************************************  Funciones externas  **************************************************************/

//****** funciones que tratan con el hw ------
extern void 		leds (BYTE);
extern void			hw_lee_conflictos_X (void) ;
extern void			comienza_hw_lectura_tension (void) ;
extern void			condiciones_set_port (void) ;
extern void			lee_llaves_de_hw (BOOLEAN * bit_tit, BOOLEAN * bit_apa) ;
extern BOOLEAN		hw_conflicto_anulado (void) ;
extern BOOLEAN		hw_reset_x_conflicto_anulado (void) ;
extern BOOLEAN		hw_conflicto_tipo_X (void) ;
extern BOOLEAN		hw_plaq_conflictos_presente_X (void) ;
extern BOOLEAN		hw_conflicto_detectado_X (void) ;
extern BOOLEAN		hw_falla_pulso_tit (void) ;
extern BOOLEAN		hw_lectura_tension (BYTE * valor) ;
extern BOOLEAN		condiciones_deshabilitadas (void) ;
extern UNIDAD_LONG	lee_hardware_detectores (void) ;
extern UNIDAD_LONG	lee_hardware_entradas (void) ;

//****** funciones de modulos de potencia ------
extern void			ini_control_modpot (void) ;
extern void			reset_conflictos_modpot (void) ;
extern void			nueva_combinac_luces (void) ;
extern BYTE 		jbf_final,cicled,cicsinp;

//****** otras funciones                ------
extern int			espera_inic_hw (void) ;
extern int			calculo_DDS (struct TPO * tpo) ;
extern BYTE			ini_softRTC (void) ;
extern BYTE			inc_softRTC (void) ;
extern BYTE			registra_err (enum LISTA_BUFFER	qbuffer, LOGERR err, LOGPARAM param, BOOLEAN evento);
extern void 		DS1390_Get_Hora(unsigned int *HH, unsigned int *MM, unsigned int *SS);
extern void 		DS1390_Get_Dia(unsigned int *DD, unsigned int *FF, unsigned int *MM, unsigned int *AA);
extern void 		DS1390_Set_Hora(unsigned int HH, unsigned int MM, unsigned int SS);
extern void 		DS1390_Set_Dia(unsigned int DD, unsigned int FF, unsigned int MM, unsigned int AA);
extern void			reset_cfl (BOOLEAN y_rst_hw) ;
extern void			doble_reset_cfl (void) ;
extern void 		AJUSTE_tiempo_soft (int ini) ;
extern void 		apagadisp(void);
extern void			interroga_modocicla(void);

//-------------------------------------------------------------------------------------------------------------------------------
//******************************************  Variables Externas  **************************************************************/
extern U8          				r1;
extern int 						reset_s;
extern int						cauerr,moduerr,leof,led_cicla;
extern char 					muestra_extension_de_verdes ;
extern PLAN						plan ;
extern PLAN						plan_FUTURO ;
extern PLAN						plan_COMU ;
extern ESTADO					estado ;
extern ESTADO					estado_FUTURO ;
extern TIEMPOL					base_i ;
extern TIEMPOL					timer_gen_reintentos ;
extern BOOLEAN					interv ;
extern CTFLAGS					flags_COMU ;
extern FLAGS_32					kes_COMU ;
extern INTERVALO				intervalo ;
extern struct TPO				tiempo_soft ;
extern UNIDAD_LONG				DEMANDAS ;
extern UNIDAD_LONG				DEMANDAS_i ;
extern UNIDAD_LONG				estado_no_demandado ;
extern unsigned int 			timeout_en_baja_tension ;
extern unsigned short 			reciboRTC[8];
extern volatile unsigned int	ENTRADAS_IO;
extern volatile unsigned char   *ptr_sdram;// = SDRAM;
extern struct ESTADO_LUCES far 	*ptrLUCES ;
extern struct DATOS_FLASH far 	*ptrFLASH_PARAM;


#endif /* CT800C_CONTROL_H_ */
