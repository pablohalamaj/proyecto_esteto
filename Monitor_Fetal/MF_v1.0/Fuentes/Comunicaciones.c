/**********************************************************
 Name          	: Comunicaciones.c
 Created on		: 15/01/2014
 Author        	: Sebastian Sisevich
 Copyright     	: SS2014
 **********************************************************/

//#include "gpio.h"
#include "ssp.h"
#include "Definiciones.h"
#include "Comunicaciones.h"
//----------------------------------------------------------
//*****************  Variables Propias  ********************
char 	recibo=0,no_recibo=0,prox=0,cuenta=0,org=0,cantcomu,
		cantmp,cantver,cantprt,empenv,queenv=0,indf=0;
int 	auxi,auxh,auxmi,auxs,hands,auxr=0;
char	reclq=0,reclq1=0,reclq2=0,reclq3=0,mtok=0;
//----------------------------------------------------------
//*****************  Variables Externas  *******************

extern char 			Rx[],Rx1[],Rx2[],btens[],toffset[],movmp[],version[],version_prt[],modo_func,
						causaerr[],moduloerr[],icomu[],b_tog[],sumaerr,lec,ledcoo,prs,velc,dir_comu,
						cont,flagmm,flagbll,menuactual,actualiza_fw,sd_disp;
extern unsigned int 	cont_tra;
extern int 				h,mi,seg,d,m,a,aa,aaa,aaaa,ds,
						ef,pl,e,ee,eee,eeee,dets,esta,estf,
						muefunc,proxf[],proxh[];

//----------------------------------------------------------


//-------------------------------------------------------------------------------------------------------------------------------
//******************************************  Recibe datos del AVR32  ***********************************************************
//-------------------------------------------------------------------------------------------------------------------------------
int recibe_datos(void)
{
	NVIC_DisableIRQ(SSP0_IRQn);								// Deshabilito las interrupciones
	if(flagmm)												// Elijo el buffer de recepción
	{														// doble buffer de recpción para no perder datos
		Rx1[cont] = (LPC_SSP0->DR);							// almaceno en Rx1[] el dato recibido
		if(Rx1[cont]==95)									// Espera al comienzo de trama
			LPC_SSP0->DR =menuactual;						// Envio el menu actual del display
	}
	else
	{
		Rx2[cont] = (LPC_SSP0->DR);							// almaceno en Rx2[] el dato recibido
		if(Rx2[cont]==95)									// Espera al comienzo de trama
			LPC_SSP0->DR =menuactual;						//Envio el menu actial del display
	}
	cont++;
	if(cont==28)
	{
		cont=0;
		flagmm=~flagmm;										// Cambio de buffer de recepción
		flagbll=1;											// flag buffer lleno (para leer buffer)
	}
	NVIC_EnableIRQ(SSP0_IRQn);								// Habilito las interrupciones
	return 0;
}

//-------------------------------------------------------------------------------------------------------------------------------
//******************************************  Envia datos a AVR32  **************************************************************
//-------------------------------------------------------------------------------------------------------------------------------
void envia_datos(void)
{
	char recep=0,au=0;
	NVIC_DisableIRQ(SSP0_IRQn);								// Deshabilito las interrupciones
	recep = LPC_SSP0->DR;									// almaceno en una var el dato recibido
	if(recep==95)											// Espera al comienzo de trama
	{
			LPC_SSP0->DR =queenv;							// Envio el menu actual del display
			empenv=1;
	}
	switch (queenv)
	{
	case proxhora:
		if(/*(recep==96)&&*/(empenv==1))
		{
			au=proxh[indf];
			LPC_SSP0->DR =au;								// Envio la nueva hora
			indf++;
		}
		if((recep==98)&&(empenv==1))
		{
			empenv=0;
			no_recibo=0;									// Habilito nuevamente la recepción
			indf=0;
		}
		break;
	case proxfecha:
		proxf[3] = 	((proxf[2]/1000)% 10) ;					// Convierto año a char
		proxf[4] = 	((proxf[2]/100) % 10) ;
		proxf[5] = 	((proxf[2]/10)  % 10) ;
		proxf[6] = 	( proxf[2]      % 10) ;
		if(/*(recep==96)&&*/(empenv==1))
		{
			LPC_SSP0->DR =proxf[indf];						// Envio la nueva fecha
			indf++;
			if(indf==2)										// no envio año (int)
				indf++;
		}
		if((recep==98)&&(empenv==1))
		{
			empenv=0;
			no_recibo=0;									// Habilito nuevamente la recepción
			indf=0;
		}
		break;
	case flecha:
		if(/*(recep==96)&&*/(empenv==1))
		{
			LPC_SSP0->DR =sumaerr;								// Envio la nueva hora
		}
		if((recep==98)&&(empenv==1))
		{
			empenv=0;
			no_recibo=0;									// Habilito nuevamente la recepción
		}
		break;
	case flechamc:
		if(/*(recep==96)&&*/(empenv==1))
		{
			LPC_SSP0->DR =muefunc;								// Envio la nueva hora
		}
		if((recep==98)&&(empenv==1))
		{
			empenv=0;
			no_recibo=0;									// Habilito nuevamente la recepción
		}
		break;
	case MODO_CICLADO:
		if(/*(recep==96)&&*/(empenv==1))
		{
			LPC_SSP0->DR =modo_func;						// Envio modo de ciclado
		}
		if((recep==98)&&(empenv==1))
		{
			empenv=0;
			no_recibo=0;									// Habilito nuevamente la recepción
		}
		break;
	case afw:
		if(/*(recep==96)&&*/(empenv==1))
		{
			LPC_SSP0->DR =actualiza_fw;						// Envio modo de ciclado
		}
		if((recep==98)&&(empenv==1))
		{
			empenv=0;
			no_recibo=0;									// Habilito nuevamente la recepción
		}
		break;

	default:
		break;
	}
	NVIC_EnableIRQ(SSP0_IRQn);								// Habilito las interrupciones
}

//-------------------------------------------------------------------------------------------------------------------------------
//******************************************  Funcion que lee el buffer de recepción  *******************************************
//-------------------------------------------------------------------------------------------------------------------------------
void leobuffer(char buffer)
{
	char i=0,z;
	if(buffer)												// Leo el buffer que no esta recibiendo
	{
		while(Rx2[i]!=95) 									// busco el inicio de una trama
		{
			i++;
			org=i;
		}
		for(i=0;i<30;i++)
		{
			Rx[i]=Rx2[org];									// Cargo la trama en un buffer aux
			org++;
		}
	}
	else
	{
		while(Rx1[i]!=95)									// Busco el inicio de una trama
		{
			i++;
			org=i;
		}
		for(i=0;i<30;i++)
		{
			Rx[i]=Rx1[org];									// Cargo la trama en un buffer aux
			org++;
		}
	}
	org=0;
	switch (menuactual)										// Segun el munu donde me encuentro
	{
	case fyh:
		i=0;
		if(!(Rx[0]==95))
			return;
		while(Rx[i]!=98)									// Cuento la cantidad de datos
			i++;
		if(!i==12)											// Si no es trama valida retorno
			return;
		h=	Rx[2];											// Cargo fyh
		mi=	Rx[3];
		seg=Rx[4];
		d=	Rx[5];
		m=	Rx[6];
		a=	Rx[7];
		aa=	Rx[8];
		aaa=Rx[9];
		aaaa=Rx[10];
		ds=	Rx[11];
		break;
	case ests:
		i=0;
		if(!(Rx[0]==95))
			return;
		while(Rx[i]!=98)
			i++;
		if(!i==10)//if(!i==12)//if(!i==8)
			return;
		e=	 Rx[2];											// Cargo estados
		ee=	 Rx[3];
		eee= Rx[4];
		eeee=Rx[5];
		ef=	 Rx[6];
		pl=	 Rx[7];
		esta=Rx[8];
		estf=Rx[9];
/*		reclq= Rx[8];//Prueba!!!!! provisorio
		reclq1=Rx[9];
		reclq2=Rx[10];
		reclq3=Rx[11];//---------------------
*/		break;
	case det:
		i=0;
		z=0;
		if(!(Rx[0]==95))
			return;
		while(Rx[i]!=98)									// Cuento la cantidad de datos
			i++;
		if(!i==18)											// Si no es trama valida retorno
			return;
		dets=Rx[2];
		ledcoo=Rx[3];
		if(muefunc==3)														// Tiempo de sistema y offset
		{
			if((Rx[4]!='P') || (Rx[11]!=':'))				// Verifica trama correcta
				return;
			i=4;
			while((Rx[i]!=98)&&(z<17))
			{
				toffset[z]=Rx[i];											// Cargo valor de tensión
				i++;
				z++;
			}

		}
		if(muefunc==4)														// Tensión de linea
		{
			if((Rx[7]!='=') || (Rx[11]!=',')||(Rx[14]!='V'))				// Verifica trama correcta
				return;
			i=4;
			while((Rx[i]!=98)&&(z<17))
			{
				btens[z]=Rx[i];												// Cargo valor de tensión
				i++;
				z++;
			}
		}
		break;
	case modp:
		i=2;
		z=0;
		if(!(Rx[0]==95))
			return;
		while((Rx[i]!=98)&&(z<16))
		{
			movmp[z]=Rx[i];									// Cargo movimientos
			i++;
			z++;
		}
		cantmp=z;
		break;
	case consp:
		i=2;
		z=0;
		if(!(Rx[0]==95))
			return;
		if(Rx[7]!='.')
			return;
		if(Rx[15]!='/')
			return;
		while((Rx[i]!=98)&&(z<16))
		{
			version[z]=Rx[i];								// Cargo versión
			i++;
			z++;
		}
		cantver=z-1;
		i=18;
		z=0;
		while((Rx[i]!=98)&&(z<4))
		{
			version_prt[z]=Rx[i];								// Cargo versión
			i++;
			z++;
		}
		cantprt=z-1;
		prs=Rx[22];
		sd_disp=Rx[23];

		break;
	case errs:
		i=2;
		z=0;
		if(!(Rx[0]==95))
			return;
		while((Rx[i]!=98)&&(z<14))
		{
			causaerr[z]=Rx[i];
			i++;
			z++;
		}
		lec=z;
		break;
	case cauerrs:
/*		i=2;
		z=0;
		if(!(Rx[0]==95))
			return;
		while((Rx[0]!=98)&&(z<10))
		{
			moduloerr[z]=Rx[i];
			i++;
			z++;
		}
*/		break;
	case ED_PARAM:
		i=0;
		if(!(Rx[0]==95))
			return;
/*		while(Rx[i]!=98)									// Cuento la cantidad de datos
			i++;
		if(!i==10)											// Si no es trama valida retorno
			return;
*/		for(i=0;i<8;i++)
			b_tog[i]=Rx[i+2];
		break;
	case comu:
		i=2;
		if(!(Rx[0]==95))
			return;
/*		while(Rx[i]!=98)									// Cuento la cantidad de datos
			i++;
		if(!i==12)											// Si no es trama valida retorno
			return;
*/		velc=Rx[2];									// Cargo movimientos
		dir_comu=Rx[3];
		break;

	default:
		break;
	}
}

//-------------------------------------------------------------------------------------------------------------------------------


