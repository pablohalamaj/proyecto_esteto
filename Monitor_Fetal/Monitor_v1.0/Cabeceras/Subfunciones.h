
/**********************************************************
 Name          	: Subfunciones.h
 Created on		: 14/01/2014
 Author        	: Sebastian Sisevich
 Copyright     	: SS2014
 **********************************************************/

#ifndef SUBFUNCIONES_H_
#define SUBFUNCIONES_H_

//---  Subfunciones 1  ---
void muestra_fecha_y_hora 	(int fila );
void muestra_comunic		(void);
void muestra_det			(void);
void muestra_dem			(void);
void muestra_tens			(void);
void muestra_estados 		(int fila );
void Muestra_causa			(int modu, int cau, int fila);
void muestra_modpot			(void);
void muestra_fyh_err 		(int fila);
//---  Subfunciones 3  ---
void prox_fecha (int 	fila );
void prox_hora (int 	fila );
void muestra_dsemana(unsigned int FF,int columna,int fila);

#endif /* SUBFUNCIONES_H_ */
