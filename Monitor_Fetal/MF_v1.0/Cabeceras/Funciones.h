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
unsigned int Func_MuestraTeclado (int);

//--------------------------------------------------------------------------
/****************************  Submenues 1  ********************************/
//--------------------------------------------------------------------------

// Funcion consulta de estados ---- 1.1
void func_consultaestados(void);
// Funcion consulta parametros ---- 1.2
//void func_consultaptros(void);
void func_consultapaciente(void);
// Funcion consulta errores ------- 1.3
//void func_conserr(void);
void func_editapaciente(void);
// Funcion borrar errores --------- 1.4
//void func_borrarerr(void);
void func_borrarpaciente(void);
//--------------------------------------------------------------------------
/****************************  Submenues 2  ********************************/
//--------------------------------------------------------------------------

// Funcion Parametrizar Atributos - 2.1
void Func_Atributos (void);
// Funcion Parametrizar Semaforos - 2.2
void Func_Semaforos (void);
// Funcion Parametrizar Direccion - 2.3
void Func_Direccion (void);

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
