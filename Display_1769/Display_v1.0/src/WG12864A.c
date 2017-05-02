/********************************************************
 Name          	: WG12864A.c
 Created on		: 16/02/2011
 Author        	: Natan Kohon
 Copyright     	: NK2011
 **********************************************************/

//#include "ssp.h"
#include "WG12864A.h"
#include "Definiciones.h"
#include "lpc17xx_ssp.h"
// *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
// Variables del modulo
unsigned int salida;				// buffer de salida
unsigned int posX = 1, posY = 1;	// posicion actual en pantalla

// *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

const char Arial8x6[] = {
0x00,0x00,0x00,0x00,0x00,0x00,   // espacio)
0x00,0x00,0x5E,0x00,0x00,0x00,   // !
0x00,0x00,0x0E,0x00,0x0E,0x00,   // "
0x00,0x14,0x7F,0x14,0x7F,0x14,   // #
0x00,0x24,0x2A,0x7F,0x2A,0x12,   // $
0x00,0x23,0x13,0x08,0x64,0x62,   // %
0x00,0x36,0x49,0x55,0x22,0x50,   // &
0x00,0x00,0x05,0x03,0x00,0x00,   // '
0x00,0x1C,0x22,0x41,0x00,0x00,   // (
0x00,0x00,0x41,0x22,0x1C,0x00,   // )
0x00,0x14,0x08,0x3E,0x08,0x14,   // *
0x00,0x08,0x08,0x3E,0x08,0x08,   // +
0x00,0x00,0x50,0x30,0x00,0x00,   // ,
0x00,0x08,0x08,0x08,0x08,0x08,   // -
0x00,0x00,0x60,0x60,0x00,0x00,   // .
0x00,0x20,0x10,0x08,0x04,0x02,   // /
0x00,0x3E,0x51,0x49,0x45,0x3E,   // 0
0x00,0x00,0x42,0x7F,0x40,0x00,   // 1
0x00,0x42,0x61,0x51,0x49,0x46,   // 2
0x00,0x21,0x41,0x45,0x4B,0x31,   // 3
0x00,0x18,0x14,0x12,0x7F,0x10,   // 4
0x00,0x27,0x45,0x45,0x45,0x39,   // 5
0x00,0x3C,0x4A,0x49,0x49,0x30,   // 6
0x00,0x01,0x71,0x09,0x05,0x03,   // 7
0x00,0x36,0x49,0x49,0x49,0x36,   // 8
0x00,0x06,0x49,0x49,0x29,0x1E,   // 9
0x00,0x00,0x36,0x36,0x00,0x00,   // :
0x00,0x00,0x56,0x36,0x00,0x00,   // ;
0x00,0x08,0x14,0x22,0x41,0x00,   // <
0x00,0x14,0x14,0x14,0x14,0x00,   // =
0x00,0x41,0x22,0x14,0x08,0x00,   // >
0x00,0x02,0x01,0x51,0x09,0x06,   // ?
0x00,0x32,0x49,0x79,0x41,0x3E,   // @
0x00,0x7E,0x09,0x09,0x09,0x7E,   // A
0x00,0x7F,0x49,0x49,0x49,0x36,   // B
0x00,0x3E,0x41,0x41,0x41,0x22,   // C
0x00,0x7F,0x41,0x41,0x41,0x3E,   // D
0x00,0x7F,0x49,0x49,0x41,0x41,   // E
0x00,0x7F,0x09,0x09,0x01,0x01,   // F
0x00,0x3E,0x41,0x41,0x49,0x79,   // G
0x00,0x7F,0x08,0x08,0x08,0x7F,   // H
0x00,0x41,0x7F,0x41,0x00,0x00,   // I
0x00,0x20,0x40,0x40,0x40,0x3F,   // J
0x00,0x7F,0x08,0x14,0x22,0x41,   // K
0x00,0x7F,0x40,0x40,0x40,0x40,   // L
0x00,0x7F,0x02,0x04,0x02,0x7F,   // M
0x00,0x7F,0x04,0x08,0x10,0x7F,   // N
0x00,0x3E,0x41,0x41,0x41,0x3E,   // O
0x00,0x7F,0x09,0x09,0x09,0x06,   // P
0x00,0x3E,0x41,0x51,0x21,0x5E,   // Q
0x00,0x7F,0x09,0x19,0x29,0x46,   // R
0x00,0x46,0x49,0x49,0x49,0x31,   // S
0x00,0x01,0x01,0x7F,0x01,0x01,   // T
0x00,0x3F,0x40,0x40,0x40,0x3F,   // U
0x00,0x1F,0x20,0x40,0x20,0x1F,   // V
0x00,0x3F,0x40,0x70,0x40,0x3F,   // W
0x00,0x63,0x14,0x08,0x14,0x63,   // X
0x00,0x03,0x04,0x78,0x04,0x03,   // Y
0x00,0x61,0x51,0x49,0x45,0x43,   // Z
0x00,0x00,0x7F,0x41,0x41,0x00,   // [
0x00,0x02,0x04,0x08,0x10,0x20,   /* \ */
0x00,0x41,0x41,0x7F,0x00,0x00,   // ]
0x00,0x04,0x02,0x01,0x02,0x04,   // ^
0x00,0x40,0x40,0x40,0x40,0x40,   // _
0x00,0x01,0x02,0x04,0x00,0x00,   // `
0x00,0x20,0x54,0x54,0x54,0x78,   // a
0x00,0x7F,0x44,0x44,0x44,0x38,   // b
0x00,0x38,0x44,0x44,0x44,0x44,   // c
0x00,0x38,0x44,0x44,0x44,0x7F,   // d
0x00,0x38,0x54,0x54,0x54,0x18,   // e
0x00,0x08,0x7E,0x09,0x01,0x02,   // f
0x00,0x0C,0x52,0x52,0x52,0x3E,   // g
0x00,0x7F,0x08,0x04,0x04,0x78,   // h
0x00,0x00,0x44,0x7D,0x40,0x00,   // i
0x00,0x20,0x40,0x44,0x3D,0x00,   // j
0x00,0x7F,0x10,0x28,0x44,0x00,   // k
0x00,0x00,0x41,0x7F,0x40,0x00,   // l
0x00,0x7C,0x04,0x18,0x04,0x78,   // m
0x00,0x7C,0x08,0x04,0x04,0x78,   // n
0x00,0x38,0x44,0x44,0x44,0x38,   // o
0x00,0x7C,0x14,0x14,0x14,0x08,   // p
0x00,0x08,0x14,0x14,0x18,0x7C,   // q
0x00,0x7C,0x08,0x04,0x04,0x08,   // r
0x00,0x48,0x54,0x54,0x54,0x24,   // s
0x00,0x04,0x3F,0x44,0x40,0x20,   // t
0x00,0x3C,0x40,0x40,0x20,0x7C,   // u
0x00,0x1C,0x20,0x40,0x20,0x1C,   // v
0x00,0x3C,0x40,0x30,0x40,0x3C,   // w
0x00,0x44,0x28,0x10,0x28,0x44,   // x
0x00,0x0C,0x50,0x50,0x50,0x3C,   // y
0x00,0x44,0x64,0x54,0x4C,0x44,   // z
0x00,0x00,0x08,0x36,0x41,0x00,   // {
0x00,0x00,0x00,0x7F,0x00,0x00,   // |
0x00,0x41,0x36,0x08,0x00,0x00,   // }
};

const char Hexa8x6[] = {
0x00,0x3E,0x51,0x49,0x45,0x3E,   // 0
0x00,0x00,0x42,0x7F,0x40,0x00,   // 1
0x00,0x42,0x61,0x51,0x49,0x46,   // 2
0x00,0x21,0x41,0x45,0x4B,0x31,   // 3
0x00,0x18,0x14,0x12,0x7F,0x10,   // 4
0x00,0x27,0x45,0x45,0x45,0x39,   // 5
0x00,0x3C,0x4A,0x49,0x49,0x30,   // 6
0x00,0x01,0x71,0x09,0x05,0x03,   // 7
0x00,0x36,0x49,0x49,0x49,0x36,   // 8
0x00,0x06,0x49,0x49,0x29,0x1E,   // 9
0x00,0x7E,0x09,0x09,0x09,0x7E,   // A
0x00,0x7F,0x49,0x49,0x49,0x36,   // B
0x00,0x3E,0x41,0x41,0x41,0x22,   // C
0x00,0x7F,0x41,0x41,0x41,0x3E,   // D
0x00,0x7F,0x49,0x49,0x41,0x41,   // E
0x00,0x7F,0x09,0x09,0x01,0x01,   // F
};

const char RightArrow16x16[] = {
0x00,0xFE,0x02,0x02,0xE2,0xE2,0xE2,0xE2,	//
0xFA,0xF2,0xE2,0xC2,0x82,0x02,0xFE,0x00,	//
0x00,0x7F,0x40,0x40,0x47,0x47,0x47,0x47,	//
0x5F,0x4F,0x47,0x43,0x41,0x40,0x7F,0x00,	//
};

const char LeftArrow16x16[] = {
0x00,0xFE,0x02,0x82,0xC2,0xE2,0xF2,0xFA,	//
0xE2,0xE2,0xE2,0xE2,0x02,0x02,0xFE,0x00,	//
0x00,0x7F,0x40,0x41,0x43,0x47,0x4F,0x5F,	//
0x47,0x47,0x47,0x47,0x40,0x40,0x7F,0x00,	//
};

const char UpArrow16x16[] = {
0x00,0xFE,0x02,0x82,0xC2,0xE2,0xF2,0xFA,	//
0xFA,0xF2,0xE2,0xC2,0x82,0x02,0xFE,0x00,	//
0x00,0x7F,0x40,0x40,0x40,0x4F,0x4F,0x4F,	//
0x4F,0x4F,0x4F,0x40,0x40,0x40,0x7F,0x00,	//
};

const char DownArrow16x16[] = {
0x00,0xFE,0x02,0x02,0x02,0xF2,0xF2,0xF2,	//
0xF2,0xF2,0xF2,0x02,0x02,0x02,0xFE,0x00,	//
0x00,0x7F,0x40,0x41,0x43,0x47,0x4F,0x5F,	//
0x5F,0x4F,0x47,0x43,0x41,0x40,0x7F,0x00,	//
};

const char BACK16x16[] = {
0x00,0xFE,0xC2,0x42,0x42,0x02,0xC2,0x42,	//
0x42,0x02,0xC2,0x42,0x42,0x02,0xFE,0x00,	//
0x00,0x7F,0x47,0x45,0x44,0x40,0x45,0x45,	//
0x47,0x40,0x47,0x44,0x44,0x40,0x7F,0x00,	//
};
/*
const char BACK16x16[] = {
0x00,0xFE,0x02,0x02,0x02,0x82,0x02,0x02,	//
0x02,0x06,0x1E,0xFA,0xF2,0xE2,0xFE,0x00,	//
0x00,0x7F,0x48,0x4C,0x4E,0x4F,0x5C,0x78,	//
0x58,0x4C,0x46,0x43,0x41,0x40,0x7F,0x00,	//
};
*/
const char Cross16x16[] = {
0x00,0xFE,0x0A,0x1E,0x3A,0x72,0xE2,0xC2,	//
0xC2,0xE2,0x72,0x3A,0x1E,0x0A,0xFE,0x00,	//
0x00,0x7F,0x50,0x78,0x5C,0x4E,0x47,0x43,	//
0x43,0x47,0x4E,0x5C,0x78,0x50,0x7F,0x00,	//
};
//--------------------------------------------------------------------------
const char OK16x16[] = {									// Agregado 2012
0x00,0xfe,0x02,0x02,0x02,0x02,0x02,0x02,	//
0x02,0x02,0xfa,0xfa,0xfa,0x02,0xfe,0x00,	//
0x00,0x7f,0x40,0x44,0x4e,0x5f,0x4e,0x4e,	//
0x4e,0x4e,0x4f,0x4f,0x4f,0x40,0x7f,0x00,	//
};

const char a16x16[] = {
0xff,0x5f,0x4f,0x47,0x43,0x41,0x41,0x47,	//
0x59,0x61,0x01,0x01,0x01,0x01,0x01,0x01,	//
0xff,0xc4,0x34,0x0c,0x1c,0xe4,0x04,0x04,	//
0x04,0x04,0x1c,0xe0,0x00,0x00,0x00,0x80,	//
};

const char b16x16[] = {
0x01,0x01,0xc1,0xe1,0xe1,0xe1,0xc1,0x01,	//
0x01,0x01,0xe1,0xe1,0xe1,0x01,0x01,0x01,	//
0xf0,0xfe,0x7e,0x0e,0x02,0x1e,0x7e,0xfe,	//
0xf0,0x80,0x7e,0xfe,0xfe,0x80,0x00,0x00,	//
};

const char c16x16[] = {
0xe1,0xe1,0xe1,0x01,0xe1,0xe1,0xe1,0xe1,	//
0xe1,0xe1,0xe1,0xe1,0x01,0x81,0xc1,0xc1,	//
0xfe,0xfe,0xfe,0x00,0x00,0x00,0xfe,0xfe,	//
0xfe,0x00,0x00,0x00,0x7c,0xfe,0xfe,0x82,	//
};

const char d16x16[] = {
0xe1,0xe1,0xe1,0xc1,0xc1,0x81,0x01,0xe1,	//
0xe1,0xe1,0xe1,0xe1,0xe1,0xe1,0xe1,0x01,	//
0x00,0x00,0x00,0x82,0xfe,0xfe,0x7c,0x00,	//
0x00,0x00,0xfe,0xfe,0xfe,0x00,0x00,0x00,	//
};

const char e16x16[] = {
0xe1,0xe1,0xe1,0xe1,0xe1,0xe1,0xe1,0xc1,	//
0x81,0x01,0x01,0x81,0xc1,0xc1,0xe1,0xe1,	//
0xfe,0xfe,0xfe,0x1e,0x7e,0xfe,0xfe,0xce,	//
0x86,0x00,0x7c,0xfe,0xfe,0x82,0x00,0x00,	//
};

const char f16x16[] = {
0xe1,0xc1,0xc1,0x81,0x01,0x01,0xe1,0xe1,	//
0xe1,0x01,0x01,0x01,0x01,0x01,0x01,0xff,	//
0x00,0x82,0xfe,0xfe,0x7c,0x00,0xfe,0xfe,	//
0xfe,0x00,0x00,0x00,0x00,0x00,0x00,0xff,	//
};

const char g16x16[] = {
0xff,0x81,0x80,0x80,0x80,0x80,0x87,0xb8,	//
0xc0,0x80,0x80,0x80,0x80,0x87,0xbc,0xc7,	//
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,	//
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,	//
};

const char h16x16[] = {
0x87,0x83,0x80,0x80,0x80,0x80,0x80,0x83,	//
0x87,0x87,0x84,0x81,0x83,0x87,0x87,0x87,	//
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,	//
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,	//
};

const char i16x16[] = {
0x87,0x87,0x87,0x80,0x80,0x80,0x87,0x87,	//
0x87,0x80,0x80,0x80,0x80,0x81,0x83,0x83,	//
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,	//
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,	//
};

const char j16x16[] = {
0x87,0x87,0x87,0x83,0x83,0x81,0x80,0x80,	//
0x80,0x80,0x87,0x87,0x87,0x80,0x80,0x80,	//
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,	//
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,	//
};

const char k16x16[] = {
0x87,0x87,0x87,0x80,0x80,0x81,0x87,0x87,	//
0x87,0x86,0x80,0x81,0x83,0x83,0x87,0x87,	//
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,	//
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,	//
};

const char l16x16[] = {
0x87,0x83,0x83,0x81,0x80,0x80,0x87,0x87,	//
0x87,0x87,0x87,0xc7,0xc7,0xe0,0xf0,0xff,	//
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,	//
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,	//
};

//--------------------------------------------------------------------
// *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
void GLCD_Data_Out(unsigned int data){

	unsigned int aux;

	aux = (data << 8) & DATA_MASK;	// Roto 8 bits a la Izquierda el dato y lo enmascaro para dejar el dato limpio.
	salida &= CTRL_MASK;			// Enmascaro en el buffer de salida los bits de control.
	salida |= aux;					// Cargo en buffer de salida los datos a enviar.

	SSP_SendData(SPI_1, &salida);
}

// *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
void GLCD_Output_Low(unsigned int signal){

	salida &= ~signal;				// Cargo en SALIDA la Instruccion a enviar.
	SSP_SendData(SPI_1, &salida);
}

// *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
void GLCD_Output_High(unsigned int signal){

	salida |= signal;				// cargo en SALIDA la señal a enviar en alto
	SSP_SendData(SPI_1, &salida);
}

// *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
void GLCD_enviaBYTE(unsigned int lado, unsigned int valor){

	if(lado == IZQ){				// Selecciono el lado correspondiente, y los Habilito.

		GLCD_Output_High(CS1);
	} else{

		GLCD_Output_High(CS2);
	}

	GLCD_Data_Out(valor);			// Coloco el dato en el puerto.
	//delay_ms(1);					// espero.
	GLCD_Output_High(E);			// Coloco el bit de ENABLE en alto, para que entre el dato.
	//delay_ms(2);					// espero.
	GLCD_Output_Low(E);				// Coloco el bit de ENABLE en bajo.

	if(lado == IZQ){				// selecciono el lado correspondiente y lo Deshabilito.

		GLCD_Output_Low(CS1);
	} else{

		GLCD_Output_Low(CS2);
	}
}

// *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
void WG12864A_Init(void){

	salida = 0;						// Reseteo el buffer de salida.

	// Seteo los pines de control para el inico.
	GLCD_Output_Low(E);				// Bajo la señal de Enable (Desabilito del Display).
	GLCD_Output_Low(CS1);			// Bajo la señal de CS1 (Desabilito Columna 1-64).
	GLCD_Output_Low(CS2);			// Bajo la señal de CS2 (Desabilito Columna 65-128).
	GLCD_Output_Low(RS);			// Bajo la señal de RS (Modo Cargar Instrucciones).

	// Envio de datos de inicializacion.
	GLCD_enviaBYTE(IZQ, START_LINE);
	GLCD_enviaBYTE(DER, START_LINE);
	GLCD_enviaBYTE(IZQ, Y_ADDRESS);
	GLCD_enviaBYTE(DER, Y_ADDRESS);
	GLCD_enviaBYTE(IZQ, X_ADDRESS);
	GLCD_enviaBYTE(DER, X_ADDRESS);

	GLCD_enviaBYTE(IZQ, DISPLAY_ON);
	GLCD_enviaBYTE(DER, DISPLAY_ON);

	WG12864A_Limpiar(NEGRO);
}

// *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
void WG12864A_Limpiar(unsigned int color)
{
   unsigned int i, j;

   // Recorre las 8 paginas (Vertical).
   for(i = 0; i < 8; ++i){

      GLCD_Output_Low(RS);   		// Bajo la señal de RS (Modo Cargar Instrucciones).

      //Comienzo, en cada pagina, desde la direccion 0
      GLCD_enviaBYTE(IZQ, Y_ADDRESS);
      GLCD_enviaBYTE(DER, Y_ADDRESS);

      //Selecciono la direccion dentro de la pagina
      GLCD_enviaBYTE(IZQ, i | X_ADDRESS);
      GLCD_enviaBYTE(DER, i | X_ADDRESS);

      GLCD_Output_High(RS);   		// Levanto la señal de RS (Modo Cargar Datos).

      // Recorre las dos mitades (Horizontales).
      for(j = 0; j < 64; ++j){

    	 GLCD_enviaBYTE(IZQ, color);  // Enciende/Apaga pixeles.
         GLCD_enviaBYTE(DER, color);  // Enciende/Apaga pixeles.
      }
   }
}

// *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
void WG12864A_posXY(unsigned int X, unsigned int Y){

	GLCD_Output_Low(RS);   			// Bajo la señal de RS (Modo Cargar Instrucciones).

	if( (0<Y) && (Y<9) ){

		if( (0<X) && (X<65) ){

			posX = X;
			posY = Y;
			GLCD_enviaBYTE(IZQ, (Y_ADDRESS | (posX - 1)));	// Ubico en la columna X.
			GLCD_enviaBYTE(IZQ, (X_ADDRESS | (posY - 1)));	// Ubico en el renglon Y.
		}

		if( (64<X) && (X<129) ){

			posX = X;
			posY = Y;
			GLCD_enviaBYTE(DER, (Y_ADDRESS | (posX - 65)));	// Ubico en la columna X.
			GLCD_enviaBYTE(DER, (X_ADDRESS | (posY - 1)));	// Ubico en el renglon Y.
		}
	}
}

// *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
void WG12864A_putchar(const char c, const char *letra, const char color){

	char i;

    GLCD_Output_High(RS);				// Levanto la señal de RS (Modo Cargar Datos).

	if( (posX>128) || ((posX+5)>128) ){	// Si sobrepasa la ultima columna de escritura.

		posX = 1;						// Reseteo la columna.
		posY++;							// Incremento el renglon.

		if( posY > 8 ){					// Si me paso del ultimo renglon.

			posY = 1;					// Reseteo el renglon.
		}

		WG12864A_posXY(posX, posY);		// Reseteo la posicion en pantalla.

		GLCD_Output_High(RS);   		// Levanto la señal de RS (Modo Cargar Datos).
	}

	for( i = 0; i < 6; i++ ){

		if( posX < 65 ){				// Con 5 mas se completa el ancho de la letra y no entra.

			GLCD_enviaBYTE(IZQ, (letra[((c-32)*6)+i])^color);  	// Enciende byte
		}

		if( posX > 64 ){

			WG12864A_posXY(posX, posY);
			GLCD_Output_High(RS);  								// Levanto la señal de RS (Modo Cargar Datos).
			GLCD_enviaBYTE(DER, (letra[((c-32)*6)+i])^color);  	// Enciende byte.
		}

		posX++;							//incremento posicion en X
	}
}

// *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
void WG12864A_printf(const char *texto, const char *letra, const char color){

	while(*texto != '\0'){				// Mientras no se haya terminado el texto a representar.

		WG12864A_putchar(*texto++, letra, color);
	}
}

// *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
void WG12864A_print_symbol(const char *symbol, const char color){

	char i, j;
	volatile unsigned char X;

	X = posX;

	for( j = 0; j < 2; j++){

		GLCD_Output_High(RS);   		// Modo datos

		if( (posX>128) || ((posX+15)>128) ){	// si sobrepasa la ultima columna de escritura

			posX = 1;					// reseteo la columna
			posY++;						// incremento el renglon
			if( posY > 8 ){				// si me paso del ultimo renglon

				posY = 1;				// reseteo el renglon
			}

			WG12864A_posXY(posX, posY);	// reseteo la posicion en pantalla
			GLCD_Output_High(RS);   	// Modo datos
		}

		for( i = 0; i < 16; i++){

			if( posX < 65 ){			// con 5 mas se completa el ancho de la letra y no entra

				GLCD_enviaBYTE(IZQ, (symbol[i+(16*j)])^color);  // enciende byte
			}

			if( posX > 64){

				WG12864A_posXY(posX, posY);
				GLCD_Output_High(RS);   // Modo datos
				GLCD_enviaBYTE(DER, (symbol[i+(16*j)])^color);  // enciende byte
			}
			posX++;						//incremento posicion en X
		}

		posY++;
		posX = X;
		WG12864A_posXY(posX, posY);		// reseteo la posicion en pantalla
	}

	posX = X;
}
