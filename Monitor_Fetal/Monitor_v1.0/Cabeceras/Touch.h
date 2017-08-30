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

#ifndef TOUCH_H_
#define TOUCH_H_
// *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

// Include Files
#include "gpio.h"
#include "adc.h"

// *-*-*-*-*-*-*-*-*-*-*-*-*- FUNCIONES -*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

// Funcion que inicializa el TOUCH PANEL
void PIOINT1_IRQHandler(void);

// Funcion que inicializa el TOUCH PANEL
void TOUCH_Init(void);

// Funcion que setea en modo STANDBY al TOUCH PANEL
void TOUCH_Standby(void);

// Funcion que obtiene el valor de la coordenada en X de presion en TOUCH PANEL
void TOUCH_GetX(void);

// Funcion que obtiene el valor de la coordenada en Y de presion en TOUCH PANEL
void TOUCH_GetY(void);

#endif /* TOUCH_H_ */
