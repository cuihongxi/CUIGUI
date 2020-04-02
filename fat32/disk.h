/*
*	硬盘驱动接口
*	该文件的作用：不同的硬件，获得disk_Handle，不能有缺省。将来作为参数，会被CuiFat_BindingDisk调用.
*/

#ifndef __DUSK_H
#define __DUSK_H

#include "uhead.h"



// 硬盘信息
typedef struct
{ 				  				  
	u32 		blockNbr;                   							// 扇区总数
	u32 		blockSize;                  							// 扇区大小
	void		(*DiskInit)(void);										// 硬盘初始化函数
	u32			(*readBlocks)(u32 sector,u8* pbuff,u32 numberOfBlocks);	// 读扇区函数
	u32			(*writeBlocks)(u32 sector,u8* pbuff,u32 numberOfBlocks);// 写扇区函数
	
}disk_Handle;


/* 函数 */

disk_Handle* SDDisk_Init(void);			// 硬盘初始化函数


#endif // !__DUSK_H
