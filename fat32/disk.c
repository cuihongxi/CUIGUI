#include "disk.h"


// 移植SD卡
#include "sdio.h"

// 读扇区
u32	SDDiskReadBlocks(u32 sector,u8* pbuff,u32 numberOfBlocks)
{
	return HAL_SD_ReadBlocks(&hsd,pbuff,sector,numberOfBlocks,1000);		//读扇区
}

// 写扇区
u32	SDDiskWriteBlocks(u32 sector,u8* pbuff,u32 numberOfBlocks)
{
	return HAL_SD_WriteBlocks(&hsd,pbuff,sector,numberOfBlocks,1000);
}

/*******************************************************
*-函数名称	：SDDisk_Init
*-函数作用	：SD卡初始化函数
*-参数		：
*-返回值	：disk_Handle
*-备注		：移植时需要配置好
*******************************************************/
disk_Handle* SDDisk_Init(void)
{
	disk_Handle* hdis  = (disk_Handle*)malloc(sizeof(disk_Handle));
	HAL_SD_Init(&hsd);					// 硬盘初始化函数SD卡初始化函数
	hdis->readBlocks = SDDiskReadBlocks;
	hdis->writeBlocks = SDDiskWriteBlocks;
	hdis->sectorSize = 512;
	return hdis;
}



