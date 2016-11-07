/**********************************************************
 *	Name        : msjs.h
 *  Created on	: 20/05/2014
 *  Author		: Sebastian Sisevich
 * 	Copyright   : SS2014
 **********************************************************/

#ifndef MSJS_H_
#define MSJS_H_

#include "xCTtodo.h"
#include "xlogerrs.h"



char 					TXT_LISTO[]      				= "....listo !!...." ;
char 					TXT_BLANCO[]     				= "                " ;
char 					PROT02_TXT_ESTADO_BASE[] 		= "inicializ  ....." ;
char 					PROT02_TXT_COMUNICACIONES[] 	= "**  P:** F:* K:*" ;
char 					PROT02_TXT_TABLA_HORARIA[] 		= "** [*] BL:* PG:*" ;
char 					PROT02_TXT_DETECTORES[] 		= "det. : .... ...." ;
char 					PROT02_TXT_DEMANDAS[] 			= "DEM. : .... ...." ;
char 					PROT02_TXT_PLANES[] 			= "P...: ...... ..." ;
char 					PROT02_TXT_NORMAL[]     	   	= "P:** E:**  ***.*" ;
char 					PROT02_TXT_EMERGENCIA[] 	   	= " emerg. #       " ;
char 					PROT02_TXT_BLOQUES_RAM[] 	   	= " bloques de RAM " ;
char 					PROT02_TXT_BLOQUES_COMUNIC0[]  	= "com:            " ;
char 					PROT02_TXT_BLOQUES_DOWNLOAD[]  	= " transferencia  " ;
char					PROT02_TXT_BLOQUES_CONFLICTO[] 	= " conflicto      " ;
char 					PROT02_TXT_ADCV[]               = "Vin=***,* V    ." ;
char 					PROT02_TXT_TxFER[]              = ". txfer: ****  ." ;
char 					PROT02_TXT_EST_ERROR[] 			= "*********" ;
char 					PROT02_TXT_EMERG[]     			= "emerg. " ;
char 					PROT02_TXT_INICIALIZ[] 			= "initializ" ;
char 					PROT02_TXT_LLAVE_TIT[] 			= "llave tit" ;
char 					PROT02_TXT_BAJA_TENS[] 			= "bja tensi" ;
char 					PROT02_TXT_CONFLICTO[] 			= "conflicto" ;
char 					PROT02_TXT_APAGADO[]   			= "lamps apa" ;
char					PROT02_TXT_TITILANTE[] 			= "titilante" ;
char					PROT02_TXT_ERR_INT_1[] 			= "err.int.1" ;
char					PROT02_TXT_ERR_INT_2[] 			= "err.int.2" ;
char					PROT02_TXT_NO_HABILITADO[]  	= " NO HABILITADO  " ;
char					PROT02_TXT_NO_ESTE_MODELO[] 	= "PARA ESTE MODELO" ;
char					PROT02_TXT_SIN_ERRORES[] 		= " sin errores    " ;
char					PROT02_TXT_CONSULTA_FEHORA_1[] 	= "***   **-**-****" ;
char					PROT02_TXT_CONSULTA_FEHORA_2[] 	= "      **:**:**.*" ;
char 					PROT02_TXT_CONFIG_FW[] 			= ".. .. .. .. .. ." ;
char 					PROT02_TXT_PROTOCOLO[] 			= "sin protoc.comm." ;
char 					PROT02_TXT_DIRECCION[] 			= "direccion = ... " ;
char 					PROT02_TXT_SELECCION[] 			= "seleccion ......" ;
char 					PROT02_TXT_SEL[] 	   			= ".NADA."
														  " PLAN "
														  " TIT. "
											        	  " APAG."
												          "EMERG." ;
char 					PROT02_TXT_SELECCION_DURACION[] = "durac.  infinita" ;
														//"durac.  xx:xx:xx" ;
char 					PROT02_TXT_texto_error[] =
													"................" 	STR_EOT
													C_ERR_RESET_COLD	STR_EOT
													C_ERR_SIN_PARAM     STR_EOT
													C_ERR_LAMP_QUEM     STR_EOT
													C_ERR_LAMP_OKEY     STR_EOT
													C_ERR_LOW_LINE      STR_EOT
													C_ERR_CFL_HW        STR_EOT
													C_ERR_CFL_FALTA_PLQ STR_EOT
													C_ERR_CFL_SW        STR_EOT
													C_ERR_CFL_INI_HW    STR_EOT
													C_ERR_BORR_ERR      STR_EOT
													C_ERR_CFL_COMP      STR_EOT
													C_ERR_CFL_VMAX      STR_EOT
													C_ERR_CFL_BOLU      STR_EOT
													C_ERR_COMM_uP       STR_EOT
													C_ERR_CFL_SW_AR     STR_EOT
													C_ERR_CFL_SW_CV     STR_EOT
													C_ERR_P_TIT         STR_EOT
													C_LOGERR_J_ANULAC 	STR_EOT
													C_ERR_REINIC_HOT    STR_EOT
													C_ERR_REINIC_SW   	STR_EOT
													C_ERR_RTC_REINIC    STR_EOT
													C_ERR_FALLA_RAM2_ERR STR_EOT
													C_ERR_FALLA_RAM2_OVR STR_EOT
													C_ERR_RTC_FALTA     STR_EOT
													STR_EOT ;

#endif /* MSJS_H_ */
