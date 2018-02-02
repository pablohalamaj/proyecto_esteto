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

#ifndef FUNCIONES_H_
#define FUNCIONES_H_
// *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

// Include Files


// *-*-*-*-*-*-*-*-*-*-*-*-*- FUNCIONES -*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

// Interrupcion del timmer para el ahorro de energia del display.
void TIMER32_1_IRQHandler(void);
// Funcion Sleep del Display.
void Func_Sleep (int, int);
// Funcion Teclado Numerico.
//unsigned int Func_MuestraTeclado (int);

//--------------------------------------------------------------------------
/****************************  Submenues 1  ********************************/
//--------------------------------------------------------------------------

// Funcion Ingreso Pacientes ------ 1.1
void Func_Ingreso (void);
void func_teclado(void);
unsigned int Func_MuestraTeclado (int);
// Funcion consulta pacientes ----- 1.2
void func_consultapaciente(void);
// Funcion edita pacientes -------- 1.3
void func_editapaciente(void);
// Funcion borrar pacientes ------- 1.4
void func_borrarpaciente(void);
//--------------------------------------------------------------------------
/****************************  Submenues 2  ********************************/
//--------------------------------------------------------------------------

// Funcion iniciar monitoreo ------ 2.1
void Func_Monitoreo(void);
void Det_corazon(void);
// Funcion configuraciones -------- 2.2
void Func_Configuraciones (void);
// Funcion Historial monitoreos --- 2.3
void Func_Historial (void);

//--------------------------------------------------------------------------
/****************************  Submenues 3  ********************************/
//--------------------------------------------------------------------------
// Funcion muestra fecha y hora --- 2.3
void func_muestrafyh(void);
// Funcion modifica fecha y hora -- 2.3
void func_modfyh(void);
//--------------------------------------------------------------------------
/****************************  Submenues 4  ********************************/
//--------------------------------------------------------------------------


#endif /* FUNCIONES_H_ */
