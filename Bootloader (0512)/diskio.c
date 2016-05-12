/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2016        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "diskio.h"

#include "SD_Card.h"

#define SD_CARD 0

/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
	return 0;
}

/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
	int result;
	switch (pdrv) {
		case SD_CARD :
			result=SD_Init();
			break;
		default :  
			result=1;
			break;	
	}
	
	if(result==1)
	  return STA_NOINIT;
	else
		return 0;
}

/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Sector address in LBA */
	UINT count		/* Number of sectors to read */
)
{
	int result;
  if(!count)  return RES_PARERR;
	switch (pdrv) 
	{
		case SD_CARD:
			result=SD_ReadDisk(buff,sector,count);	 
			while(result)//������
			{
				SD_Init();	//���³�ʼ��SD��
				result=SD_ReadDisk(buff,sector,count);	
			}
			break;
		default :
			result=1;
			break;
	}
	
	if(result==0x00)
		return RES_OK;	 
	else 
		return RES_ERROR;	
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Sector address in LBA */
	UINT count			/* Number of sectors to write */
)
{
	
	int result;	
	if (!count)
		return RES_PARERR;
	switch(pdrv)
	{
		case SD_CARD://SD��
			result=SD_WriteDisk((uint8_t*)buff,sector,count);
			while(result)//д����
			{
				SD_Init();	//���³�ʼ��SD��
				result=SD_WriteDisk((uint8_t*)buff,sector,count);
				//printf("sd wr error:%d\r\n",res);
			}
			break;
	
		default:
			result=1; 
		  break;
	}
    
    if(result == 0x00)
			return RES_OK;	 
    else 
			return RES_ERROR;	
}



/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/


DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	uint8_t result;

	if(pdrv==SD_CARD)//SD��
		{
				switch(cmd)
				{
					case CTRL_SYNC:
					result = RES_OK; 
							break;	 
					case GET_SECTOR_SIZE:
					*(DWORD*)buff = 512; 
							result = RES_OK;
							break;	 
					case GET_BLOCK_SIZE:
					*(WORD*)buff = SDCardInfo.CardBlockSize;
							result = RES_OK;
							break;	 
					case GET_SECTOR_COUNT:
							*(DWORD*)buff = SDCardInfo.CardCapacity/512;
							result = RES_OK;
							break;
					default:
							result = RES_PARERR;
							break;
				}
		}
	else result=RES_ERROR;
			
	if(result == 0x00)
			return RES_OK;	 
    else 
			return RES_ERROR;	
}


DWORD get_fattime (void)
{				 
	return 0;
}	
