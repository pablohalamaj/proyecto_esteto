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

#ifndef SUBFUNCIONES_H_
#define SUBFUNCIONES_H_

//---  Subfunciones 1  ---
void muestra_fecha_y_hora 	(int fila );
void muestra_comunic		(void);
void muestra_det			(void);
void muestra_dem			(void);
void muestra_tin_offset		(void);
void muestra_tens			(void);
void muestra_estados 		(int fila );
void Muestra_causa			(int modu, int cau, int fila);
void muestra_modpot			(void);
void muestra_fyh_err 		(int fila);
//---  Subfunciones 2  ---
void func_punto 			(char,char);
void analizo_salto			(char,char);
void Graf_datos_est			(void);
//---  Subfunciones 3  ---
void prox_fecha 			(int 	fila );
void prox_hora 				(int 	fila );
void muestra_dsemana		(unsigned int FF,int columna,int fila);

#endif /* SUBFUNCIONES_H_ */
