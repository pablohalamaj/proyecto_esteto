/***************************************************************************
 *  Proyecto Final-UTN.BA
 *  Proyecto: Monitor Fetal
 *  Versión: v1.2
 *  Fecha: 15-02-2018
 *  Autor: Sebastian Sisevich
****************************************************************************/
/***************************************************************************
 *	Comentarios:
 *
 * Funciones de manejo de SD_MMC
****************************************************************************/

#include "ssp.h"
#include "gpio.h"
#include "Definiciones.h"
#include "sd_mmc_spi.h"

/*_____ D E F I N I T I O N ________________________________________________*/
#define  SPI_ERROR_TIMEOUT  1
#define true	1
#define false	0

static uint32_t  gl_ptr_mem;                       // Memory data pointer
static uint8_t   sector_buf[MMC_SECTOR_SIZE];  // Sector buffer
//static spi_options_t sd_mmc_opt;
static unsigned int sd_mmc_pba_hz;

char  sd_mmc_spi_init_done = false;
uint8_t   r1;
uint16_t  r2;

uint8_t   csd[16];                        // stores the Card Specific Data
volatile  uint64_t  capacity;                       // stores the capacity in bytes
volatile  uint64_t  capacity_mult;                  // stores the HighCapacity in bytes
volatile  uint32_t  sd_mmc_spi_last_block_address;  // stores the address of the last block (sector)
uint16_t  erase_group_size;               // stores the number of blocks concerned by an erase command
uint8_t   card_type;                      // stores SD_CARD or MMC_CARD type card
uint16_t						sec_cal;
uint32_t	 						*offset_cal,*offset,*OFFSET_SD_V;
static uint32_t					ptr_memsd;
uint32_t							sd_sector=7500,SECTOR_SD_V;
char POS_SDHC=4,wXP=1;
unsigned int SAL_SD;
uint8_t   data_mem[513]; // data buffer
#if      (defined SD_MMC_READ_CID) && (SD_MMC_READ_CID == true)
          uint8_t   cid[16];
#endif
unsigned char	SD_HC;
char			checkvr;


#ifdef SD_MMC

char sd_mmc_spi_internal_init(void)
{
	uint16_t retry;
  int i;
  int if_cond;

/*  // Start at low frequency
  sd_mmc_opt.baudrate = 400000;
  spi_setupChipReg(SD_MMC_SPI, &sd_mmc_opt, sd_mmc_pba_hz);
*/
  /* card needs 74 cycles minimum to start up */
 // spi_selectChip(SD_MMC_SPI, SD_MMC_SPI_NPCS);    // select SD_MMC_SPI
  for(i = 0; i < 10; ++i)
  {
	  SAL_SD=0xFF;
	  SSP_Send(SPI_0, &SAL_SD, 1);
  }
  for(i = 0; i < 10; ++i)
  {
	  SAL_SD=0xFF;
	  SSP_Send(SPI_0, &SAL_SD, 1);
  }
//  spi_unselectChip(SD_MMC_SPI, SD_MMC_SPI_NPCS);  // unselect SD_MMC_SPI
  // RESET THE MEMORY CARD
  sd_mmc_spi_init_done = false;
  card_type = MMC_CARD;
  retry = 0;
  do
  {
    // reset card and go to SPI mode
    r1 = sd_mmc_spi_send_command(MMC_GO_IDLE_STATE, 0);
    SAL_SD=0xFF;
    SSP_Send(SPI_0, &SAL_SD, 1);
    // do retry counter
    retry++;
    if(retry > 100)
      return false;
 }
  while(r1 != 0x01);   // check memory enters idle_state

  if_cond = sd_mmc_spi_get_if();
  if(if_cond == -1) {
      return false; // card is bad
  } else if (if_cond == 1) {
      card_type = SD_CARD_2;
  } else {
    // IDENTIFICATION OF THE CARD TYPE (SD or MMC)
    // Both cards will accept CMD55 command but only the SD card will respond to ACMD41
    r1 = sd_mmc_spi_send_command(SD_APP_CMD55,0);
    SSP_Send(SPI_0, &SAL_SD, 1);

    r1 = sd_mmc_spi_send_command(SD_SEND_OP_COND_ACMD, 0);
    SSP_Send(SPI_0, &SAL_SD, 1);

    if ((r1&0xFE) == 0) {   // ignore "in_idle_state" flag bit
      card_type = SD_CARD;    // card has accepted the command, this is a SD card
    } else {
      card_type = MMC_CARD;   // card has not responded, this is a MMC card
      // reset card again
      retry = 0;
      do {
        // reset card again
        r1 = sd_mmc_spi_send_command(MMC_GO_IDLE_STATE, 0);
        SSP_Send(SPI_0, &SAL_SD, 1);
        // do retry counter
        retry++;
        if(retry > 100)
          return false;
      }
      while(r1 != 0x01);   // check memory enters idle_state
    }
  }

  // CONTINUE INTERNAL INITIALIZATION OF THE CARD
  // Continue sending CMD1 while memory card is in idle state
  retry = 0;
  do {
    switch(card_type) {
    case MMC_CARD:
      r1 = sd_mmc_spi_send_command(MMC_SEND_OP_COND, 0);
      SSP_Send(SPI_0, &SAL_SD, 1);
      break;
    case SD_CARD:
      sd_mmc_spi_send_command(SD_APP_CMD55,0);
      r1 = sd_mmc_spi_send_command(SD_SEND_OP_COND_ACMD, 0);
      SSP_Send(SPI_0, &SAL_SD, 1);
      break;
    case SD_CARD_2:
      // set high capacity bit mask
      sd_mmc_spi_send_command(SD_APP_CMD55,0);
      r1 = sd_mmc_spi_send_command(SD_SEND_OP_COND_ACMD, 0x40000000);
      SSP_Send(SPI_0, &SAL_SD, 1);
      break;
    }
     // do retry counter
     retry++;
     if(retry == 50000)    // measured approx. 500 on several cards
        return false;
  } while (r1);

  // CHECK FOR SDHC
  if(card_type == SD_CARD_2) {
    if_cond = sd_mmc_spi_check_hc();
    if (if_cond == -1) {
        SD_HC=0;
      return false;
    } else if (if_cond == 1){
          card_type = SD_CARD_2_SDHC;
          SD_HC=1;
      }
  }

  // DISABLE CRC TO SIMPLIFY AND SPEED UP COMMUNICATIONS
  r1 = sd_mmc_spi_send_command(MMC_CRC_ON_OFF, 0);  // disable CRC (should be already initialized on SPI init)
  SSP_Send(SPI_0, &SAL_SD, 1);

  // SET BLOCK LENGTH TO 512 BYTES
  r1 = sd_mmc_spi_send_command(MMC_SET_BLOCKLEN, 512);
  SSP_Send(SPI_0, &SAL_SD, 1);
  if (r1 != 0x00)
    return false;    // card unsupported if block length of 512b is not accepted

  // GET CARD SPECIFIC DATA
  if (false ==  sd_mmc_spi_get_csd(csd))
    return false;

  // GET CARD CAPACITY and NUMBER OF SECTORS
  sd_mmc_spi_get_capacity();

  // GET CARD IDENTIFICATION DATA IF REQUIRED
#if (defined SD_MMC_READ_CID) && (SD_MMC_READ_CID == true)
  if (false ==  sd_mmc_spi_get_cid(cid))
    return false;
#endif

  sd_mmc_spi_init_done = true;

  // Set SPI Speed to MAX
/*  sd_mmc_opt.baudrate = SD_MMC_SPI_MASTER_SPEED;
  spi_setupChipReg(SD_MMC_SPI, &sd_mmc_opt, sd_mmc_pba_hz);
 */
  return true;
}
//--------------------------------------------------------------------------
uint8_t sd_mmc_spi_send_command(uint8_t command, uint32_t arg)
{
//	spi_status_t a,b;
	uint8_t a,b, aux_command;
	uint32_t aux_arg;
//  a=spi_selectChip(SD_MMC_SPI, SD_MMC_SPI_NPCS);    // select SD_MMC_SPI
	a=a+b;
	aux_command=command;
	aux_arg=arg;
  r1 = sd_mmc_spi_command(aux_command, aux_arg);
//  b=spi_unselectChip(SD_MMC_SPI, SD_MMC_SPI_NPCS);  // unselect SD_MMC_SPI
  b=b-a;
  return r1;

}
//--------------------------------------------------------------------------

uint8_t sd_mmc_spi_command(uint8_t command, uint32_t arg)
{
	uint8_t retry;
	SAL_SD=0xFF;
	SSP_Send(SPI_0, &SAL_SD, 1);
	SAL_SD=(command | 0x40);
	SSP_Send(SPI_0, &SAL_SD, 1);
	SAL_SD=(arg>>24);
	SSP_Send(SPI_0, &SAL_SD, 1);
	SAL_SD=(arg>>16);
	SSP_Send(SPI_0, &SAL_SD, 1);
	SAL_SD=(arg>>8);
	SSP_Send(SPI_0, &SAL_SD, 1);
	SAL_SD=(arg>>0);
	SSP_Send(SPI_0, &SAL_SD, 1);
	switch(command)
	{
      case MMC_GO_IDLE_STATE:
    	  SAL_SD=0x95;
         SSP_Send(SPI_0, &SAL_SD, 1);
         break;
      case MMC_SEND_IF_COND:
    	  SAL_SD=0x87;
         SSP_Send(SPI_0, &SAL_SD, 1);
         break;
      default:
    	  SAL_SD=0xFF;
         SSP_Send(SPI_0, &SAL_SD, 1);
         break;
  }

  // end command
  // wait for response
  // if more than 8 retries, card has timed-out and return the received 0xFF
  retry = 0;
  r1    = 0xFF;
  SAL_SD=0xFF;
  while((r1) == 0xFF)
  {
	r1 = sd_mmc_spi_send_and_read(SAL_SD);
    retry++;
    if(retry > 100) break;
  }
  return r1;
}
//--------------------------------------------------------------------------

uint8_t sd_mmc_spi_send_and_read(uint8_t data_to_send)
{
   unsigned short data_read;
   SAL_SD=data_to_send;
   SSP_Send(SPI_0, &SAL_SD, 1);
 /*  if( SPI_ERROR_TIMEOUT == SSP_Receive( SPI_0, &data_read, 1 ) )
     return 0xFF;
 */  SSP_Receive( SPI_0, &data_read, 1 );
   return data_read;
}

//--------------------------------------------------------------------------
int sd_mmc_spi_get_if(void)
{
  // wait for MMC not busy
  if (false == sd_mmc_spi_wait_not_busy())
    return SD_FAILURE;

//  spi_selectChip(SD_MMC_SPI, SD_MMC_SPI_NPCS);    // select SD_MMC_SPI
  r1 = sd_mmc_spi_command(MMC_SEND_IF_COND, 0x000001AA);
  // check for valid response
  if((r1 & MMC_R1_ILLEGAL_COM) != 0) {
//    spi_unselectChip(SD_MMC_SPI, SD_MMC_SPI_NPCS);  // unselect SD_MMC_SPI
    return SD_MMC;
  }
  r1 = sd_mmc_spi_send_and_read(0xFF);
  r1 = sd_mmc_spi_send_and_read(0xFF);
  r1 = sd_mmc_spi_send_and_read(0xFF);
  if((r1 & 0x01) == 0) {
//    spi_unselectChip(SD_MMC_SPI, SD_MMC_SPI_NPCS);  // unselect SD_MMC_SPI
    return SD_FAILURE;
  }
  r1 = sd_mmc_spi_send_and_read(0xFF);
//  r1 = sd_mmc_spi_send_and_read(0xFF);
//  r1 = sd_mmc_spi_send_and_read(0xFF);
//   if(r1 != 0xaa) {
//    spi_unselectChip(SD_MMC_SPI, SD_MMC_SPI_NPCS);  // unselect SD_MMC_SPI
//    return SD_FAILURE; /* wrong test pattern */
//  }
//  spi_unselectChip(SD_MMC_SPI, SD_MMC_SPI_NPCS);  // unselect SD_MMC_SPI
  return true;
}
//--------------------------------------------------------------------------
char sd_mmc_spi_get_csd(uint8_t *buffer)
{
	uint8_t retry;
unsigned short data_read;
  // wait for MMC not busy
  if (false == sd_mmc_spi_wait_not_busy())
    return false;

//  spi_selectChip(SD_MMC_SPI, SD_MMC_SPI_NPCS);    // select SD_MMC_SPI
  // issue command
  r1 = sd_mmc_spi_command(MMC_SEND_CSD, 0);
  // check for valid response
  if(r1 != 0x00)
  {
//    spi_unselectChip(SD_MMC_SPI, SD_MMC_SPI_NPCS);  // unselect SD_MMC_SPI
    sd_mmc_spi_init_done = false;
    return false;
  }
  // wait for block start
  retry = 0;
  while((r1 = sd_mmc_spi_send_and_read(0xFF)) != MMC_STARTBLOCK_READ)
  {
    if (retry > 8)
    {
//      spi_unselectChip(SD_MMC_SPI, SD_MMC_SPI_NPCS);  // unselect SD_MMC_SPI
      return false;
    }
    retry++;
  }
  // store valid data block
  for (retry = 0; retry <16; retry++)
  {
   SSP_Send(SPI_0, &SAL_SD, 1);
   SSP_Receive( SPI_0, &data_read, 1 );
    buffer[retry] = data_read;
  }
  SSP_Send(SPI_0, &SAL_SD, 1);
  SSP_Send(SPI_0, &SAL_SD, 1);
  SSP_Send(SPI_0, &SAL_SD, 1);
  return true;
}

//--------------------------------------------------------------------------
char sd_mmc_spi_get_cid(uint8_t *buffer)
{
	uint8_t retry;
unsigned short data_read;
  // wait for MMC not busy
  if (false == sd_mmc_spi_wait_not_busy())
    return false;

//  spi_selectChip(SD_MMC_SPI, SD_MMC_SPI_NPCS);  // select SD_MMC_SPI
  // issue command
  r1 = sd_mmc_spi_command(MMC_SEND_CID, 0);
  // check for valid response
  if(r1 != 0x00)
  {
//    spi_unselectChip(SD_MMC_SPI, SD_MMC_SPI_NPCS);  // unselect SD_MMC_SPI
    sd_mmc_spi_init_done = false;
    return false;
  }
  // wait for data block start
  retry = 0;
  while((r2 = sd_mmc_spi_send_and_read(0xFF)) != MMC_STARTBLOCK_READ)
  {
    if (retry > 8)
    {
//      spi_unselectChip(SD_MMC_SPI, SD_MMC_SPI_NPCS);  // unselect SD_MMC_SPI
      return false;
    }
    retry++;
  }
  // store valid data block
  for (retry = 0; retry <16; retry++)
  {
	  SSP_Send(SPI_0, &SAL_SD, 1);
   SSP_Receive( SPI_0, &data_read, 1 );
   buffer[retry] = data_read;
  }
  SSP_Send(SPI_0, &SAL_SD, 1);
  SSP_Send(SPI_0, &SAL_SD, 1);
  SSP_Send(SPI_0, &SAL_SD, 1);
  return true;
}

//--------------------------------------------------------------------------
char sd_mmc_spi_wait_not_busy(void)
{
	uint32_t retry;

  // Select the SD_MMC memory gl_ptr_mem points to//
//  spi_selectChip(SD_MMC_SPI, SD_MMC_SPI_NPCS);
  retry = 0;
  while((r1 = sd_mmc_spi_send_and_read(0xFF)) != 0xFF)
  {
    retry++;
    if (retry == 50000)
    {
//      spi_unselectChip(SD_MMC_SPI, SD_MMC_SPI_NPCS);
      return false;
    }
  }
//  spi_unselectChip(SD_MMC_SPI, SD_MMC_SPI_NPCS);
  return true;
}
//--------------------------------------------------------------------------
void sd_mmc_spi_get_capacity(void)
{
	uint32_t c_size;
    uint8_t  c_size_mult;
    uint8_t  read_bl_len;
    uint8_t  erase_grp_size;
    uint8_t  erase_grp_mult;

  // extract variables from CSD array
  read_bl_len = csd[5] & 0x0F;
  if (card_type == SD_CARD_2_SDHC) {
    c_size = ((csd[7] & 0x3F) << 16) | (csd[8] << 8) | csd[9];
    ++c_size;
    capacity = (uint64_t)c_size << 19;
    capacity_mult = (c_size >> 13) & 0x01FF;
    sd_mmc_spi_last_block_address = (capacity >> 9) + (capacity_mult << 23) - 1;
  } else {
    c_size      = ((csd[6] & 0x03) << 10) + (csd[7] << 2) + ((csd[8] & 0xC0) >> 6);
    c_size_mult = ((csd[9] & 0x03) << 1) + ((csd[10] & 0x80) >> 7);
    sd_mmc_spi_last_block_address = ((uint32_t)(c_size + 1) * (uint32_t)((1 << (c_size_mult + 2)))) - 1;
    capacity = (1 << read_bl_len) * (sd_mmc_spi_last_block_address + 1);
    capacity_mult = 0;
    if (read_bl_len > 9) {  // 9 means 2^9 = 512b
      sd_mmc_spi_last_block_address <<= (read_bl_len - 9);
    }
  }
  if (card_type == MMC_CARD)
  {
    erase_grp_size = ((csd[10] & 0x7C) >> 2);
    erase_grp_mult = ((csd[10] & 0x03) << 3) | ((csd[11] & 0xE0) >> 5);
  }
  else
  {
    erase_grp_size = ((csd[10] & 0x3F) << 1) + ((csd[11] & 0x80) >> 7);
    erase_grp_mult = 0;
  }
  erase_group_size = (erase_grp_size + 1) * (erase_grp_mult + 1);
}
//--------------------------------------------------------------------------
int sd_mmc_spi_check_hc(void)
{
  unsigned char hc_bit;
  // wait for MMC not busy
  if (false == sd_mmc_spi_wait_not_busy())
    return SD_FAILURE;

//  spi_selectChip(SD_MMC_SPI, SD_MMC_SPI_NPCS);    // select SD_MMC_SPI
  r1 = sd_mmc_spi_command(SD_READ_OCR, 0);
  // check for valid response
  if(r1 != 0) {
//    spi_unselectChip(SD_MMC_SPI, SD_MMC_SPI_NPCS);  // unselect SD_MMC_SPI
    return SD_FAILURE;
  }
  hc_bit = sd_mmc_spi_send_and_read(0xFF);
  r1 = sd_mmc_spi_send_and_read(0xFF);
  r1 = sd_mmc_spi_send_and_read(0xFF);
  r1 = sd_mmc_spi_send_and_read(0xFF);
//  spi_unselectChip(SD_MMC_SPI, SD_MMC_SPI_NPCS);  // unselect SD_MMC_SPI
  if(hc_bit & 0x40) {
      return SDHC_CARD;
  }
  return 0;
}
//--------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------
//******************************************  Busca posición de comienzo en SD-MMC  *********************************************
//-------------------------------------------------------------------------------------------------------------------------------
void buscaensd(char prosd)
{
	char				res=0;
	uint32_t 			envsd,punt=0,kons=512;
	char				busct,flags=0,noencu=0;
	punt=0;
	busct=0;
//	sd_sector=0x1D4C;					//Hasta 2Gb (NO SD_HC)				// Sector base para busqueda del programa en SD-MMC
//	offset=0x3A9800;														// Offset base para busqueda del programa en SD-MMC
	sd_sector=0x59d8;					//4Gb	SD_HC
	offset=0xb3b000;
//	sd_sector=0x84d0;					//8Gb	SD_HC
//	offset=0xb3b000;
//	sd_sector=0x11d28;					//16Gb	SD_HC w7 POS=73000
//	offset=0x23a5000;

	if(SD_HC)
	{
		sd_sector=0x84d0;				//8Gb w7 POS=34000					// Sector base para busqueda del programa en SD-MMC
		offset=0x109a000;													// Offset base para busqueda del programa en SD-MMC
		POS_SDHC=8;
//		DISP_SD=8;
	}
	else
	{
		sd_sector=0x1D4C;				//2Gb no HC							// Sector base para busqueda del programa en SD-MMC
		offset=0x3A9800;													// Offset base para busqueda del programa en SD-MMC
//		DISP_SD=2;
	}
//	leds (SDMMC) ;
	while(flags!=2)
	{
		switch (busct)
		{
		case 0:																// Busca el inicio del programa
			while(res!=1&&punt<1000)										// Mientras no encuentre el ppio del prog o recorra 1000 sec
			{																// Se amplio a 2000 porque el format entre XP y W7 ubica el prog en distintos sectores
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
					switch (POS_SDHC)
					{
					case 4:													// Posición para 4Gb
						if(wXP)
						{
							sd_sector=0x59d8;	//POS=23000 wXP				// Sector base para busqueda del programa en SD-MMC
							offset=0xb3b000;								// Offset base para busqueda del programa en SD-MMC
							wXP=0;
//							DISP_SD=4;
						}
						else
						{													// Posición para 4Gb
							sd_sector=0x3E80;	//POS=16000	w7
							offset=0x7D0000;
							wXP=1;
//							DISP_SD=5;
						}
						punt=0;
						busct=0;
						flags=0;
						POS_SDHC=8;
						if(noencu++==6)										// Protección para salir por error al buscar
							flags=2;
						break;
					case 8:													// Posición para 8Gb
						sd_sector=0x84d0;		//8Gb w7 POS=34000			// Sector base para busqueda del programa en SD-MMC
						offset=0x109a000;									// Offset base para busqueda del programa en SD-MMC
						punt=0;
						busct=0;
						flags=0;
						POS_SDHC=16;
//						DISP_SD=8;
						if(noencu++==6)										// Protección para salir por error al buscar
							flags=2;
						break;
					case 16:												// Posición para 16Gb
						sd_sector=0x11d28;		//16Gb w7 POS=73000
						offset=0x23a5000;
						punt=0;
						busct=0;
						flags=0;
						POS_SDHC=4;
//						DISP_SD=9;
						if(noencu++==6)										// Protección para salir por error al buscar
						flags=2;
						break;
					default:
						break;
					}

				}
				else
				{
				sd_sector=0x1D4C;//7500;		//2Gb no HC					// Sector base para busqueda del programa en SD-MMC
				offset=0x3A9800;											// Offset base para busqueda del programa en SD-MMC
				punt=0;
				busct=0;
				flags=0;
				//DISP_SD=2;
				if(noencu++==6)												// Protección para salir por error al buscar
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
				SECTOR_SD_V=sd_sector;
				OFFSET_SD_V=offset;
				flags=2;
/*				if(prosd)
					muememdis=SD_PC;
				else
					muememdis=SD_CT;
*/			}
			else
			{
				if(SD_HC)
				{
					switch (POS_SDHC)
					{
					case 4:													// Posición para 4Gb
						if(wXP)
						{
							sd_sector=0x59d8;	//POS=23000 wXP				// Sector base para busqueda del programa en SD-MMC
							offset=0xb3b000;								// Offset base para busqueda del programa en SD-MMC
							wXP=0;
//							DISP_SD=4;
						}
						else
						{													// Posición para 4Gb
							sd_sector=0x3E80;	//POS=16000	w7
							offset=0x7D0000;
							wXP=1;
//							DISP_SD=5;
						}
						punt=0;
						busct=0;
						flags=0;
						POS_SDHC=8;
						if(noencu++==6)										// Protección para salir por error al buscar
							flags=2;
						break;
					case 8:													// Posición para 8Gb
						sd_sector=0x84d0;		//POS=34000 w7				// Sector base para busqueda del programa en SD-MMC
						offset=0x109a000;									// Offset base para busqueda del programa en SD-MMC
						punt=0;
						busct=0;
						flags=0;
						POS_SDHC=16;//4;
//						DISP_SD=8;
						if(noencu++==6)										// Protección para salir por error al buscar
							flags=2;
						break;
					case 16:												// Posición para 16Gb
					sd_sector=0x11d28;		//16Gb w7 POS=73000
					offset=0x23a5000;
					punt=0;
					busct=0;
					flags=0;
					POS_SDHC=4;
//					DISP_SD=9;
					if(noencu++==6)										// Protección para salir por error al buscar
					flags=2;
					break;
					default:
						break;
					}
				}
				else
				{
				sd_sector=0x1D4C;//7500;		//2Gb no HC					// Sector base para busqueda del programa en SD-MMC
				offset=0x3A9800;											// Offset base para busqueda del programa en SD-MMC
				punt=0;
				busct=0;
				flags=0;
//				DISP_SD=2;
				if(noencu++==6)												// Protección para salir por error al buscar
					flags=2;
				}
			}
			break;
		}
	}
}
//-------------------------------------------------------------------------------------------------------------------------------

char sd_mmc_spi_search_sector_to_ram(uint32_t *psd)
{
	uint32_t /**ptr_memsd,*/*offset;												// Memory data pointer
	uint8_t *_ram = psd;
//  volatile unsigned char offset;
	uint16_t  i,sector;
	uint16_t  read_time_out;
	char op,ini;
	unsigned short data_read,dato;
	// wait for MMC not busy
	if (KO == sd_mmc_spi_wait_not_busy())
		return KO;
	ptr_memsd=psd;
//	spi_selectChip(SD_MMC_SPI, SD_MMC_SPI_NPCS);							// select SD_MMC_SPI
	  // issue command
	  if(card_type == SD_CARD_2_SDHC) {
	    r1 = sd_mmc_spi_command(MMC_READ_SINGLE_BLOCK, ptr_memsd>>9);
	  } else {
	    r1 = sd_mmc_spi_command(MMC_READ_SINGLE_BLOCK, ptr_memsd);
	  }

	// check for valid response
	if (r1 != 0x00)
	{
//		spi_unselectChip(SD_MMC_SPI, SD_MMC_SPI_NPCS);						// unselect SD_MMC_SPI
		return KO;
	}

	// wait for token (may be a datablock start token OR a data error token !)
	read_time_out = 30000;
	while((r1 = sd_mmc_spi_send_and_read(0xFF)) == 0xFF)
	{
		read_time_out--;
		if (read_time_out == 0)												// TIME-OUT
		{
//			spi_unselectChip(SD_MMC_SPI, SD_MMC_SPI_NPCS);					// unselect SD_MMC_SPI
			return KO;
		}
	}

	// check token
	if (r1 != MMC_STARTBLOCK_READ)
	{
		SAL_SD=0xFF;
		 SSP_Send(SPI_0, &SAL_SD, 1);
//		spi_unselectChip(SD_MMC_SPI, SD_MMC_SPI_NPCS);						// unselect SD_MMC_SPI
		return KO;
	}

	// store datablock
	for(i=0;i<MMC_SECTOR_SIZE;i++)
	{
		SAL_SD=0xFF;
		SSP_Send(SPI_0, &SAL_SD, 1);
		SSP_Receive( SPI_0, &data_read, 1 );
		dato=data_read;
//  	*ptr_sd_mmc++=data_read;
		switch (dato)														// Busca el inicio del programa
		{
		case 0xAA:
			ini=1;
			sector=i;
			offset=_ram;
			break;
		case 0x55:
			if(ini==1)
			{
				sec_cal=sector;												// Si cumple con AA 55 guarda los valores
				offset_cal=offset;
				i=600;														// Para salir del for
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
	SAL_SD=0xFF;
	 SSP_Send(SPI_0, &SAL_SD, 1);
	 SSP_Send(SPI_0, &SAL_SD, 1);
	 SSP_Send(SPI_0, &SAL_SD, 1);
	 SSP_Send(SPI_0, &SAL_SD, 1);
	return op;   // Read done.
}
//--------------------------------------------------------------------------

#endif	//SD_MMC
