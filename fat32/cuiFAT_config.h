/**
	cuiFAT的配置
	2019年5月3日12:40:22
*/

#ifndef	__CUIFAT_CONFIG_H
#define	__CUIFAT_CONFIG_H

#define	CuiFat_molloc(x)											CUI_Mallco(x)												//动态内存分配
#define	CuiFat_Free(p)												FreeCUI_Mallco(p)										//释放内存

#define	CuiFat_EndianValu(parray,type)				*((type*)parray)										//获得小端的值

#define	CuiFat_ReadSingleBlock(sector,pbuff)	SD_ReadBlock(pbuff,sector,512);			//读一个扇区
#define	CuiFat_WriteSingleBlock(sector,pbuff)	SD_WriteBlock(pbuff,sector,512);		//写一个扇区
    
//分配盘符


#endif
