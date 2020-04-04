#include "disk.h"


// ��ֲSD��
#include "sdio.h"

// ������
u32	SDDiskReadBlocks(u32 sector,u8* pbuff,u32 numberOfBlocks)
{
	return HAL_SD_ReadBlocks(&hsd,pbuff,sector,numberOfBlocks,1000);		//������
}

// д����
u32	SDDiskWriteBlocks(u32 sector,u8* pbuff,u32 numberOfBlocks)
{
	return HAL_SD_WriteBlocks(&hsd,pbuff,sector,numberOfBlocks,1000);
}

/*******************************************************
*-��������	��SDDisk_Init
*-��������	��SD����ʼ������
*-����		��
*-����ֵ	��disk_Handle
*-��ע		����ֲʱ��Ҫ���ú�
*******************************************************/
disk_Handle* SDDisk_Init(void)
{
	disk_Handle* hdis  = (disk_Handle*)malloc(sizeof(disk_Handle));
	HAL_SD_Init(&hsd);					// Ӳ�̳�ʼ������SD����ʼ������
	hdis->readBlocks = SDDiskReadBlocks;
	hdis->writeBlocks = SDDiskWriteBlocks;
	return hdis;
}



