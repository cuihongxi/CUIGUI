/*
注：没有盘符的概念，直接操作hfat句柄
	
	应该可以格式化一个盘符
*/

/*
*	配置硬件接口在disk文件中
*	使用步骤：
	① 根据硬件不同，进行配置，填充获得硬件句柄 disk_Handle
	② 使用	hfat = CuiFat_BindingDisk(disk_Handle);	绑定硬件，获得hfat句柄
	③ CuiFatOpenBootDir(hfat); 打开根目录
	④ CuiFatOpenDirByName(hfat,"BOOT"); 打开该目录下的文件夹
	⑤ CuiFatBack(hfat); 文件夹回退，返回上一级菜单
	
*/

#ifndef __CUIFAT__H
#define __CUIFAT__H

#include "uhead.h"
#include "disk.h"
#include "SingleList.h"

#if (DEBUG_LEVEL > 0)
#include "stdio.h"
#define  DEBUG_Log(...) printf(__VA_ARGS__);
#else
#define DEBUG_Log(...)   
#endif 

//根目录区定义文件的属性
typedef enum{
	ATTR_RW = 0,								// 读写
	ATTR_READ_ONLY = 1,							// 只读
	ATTR_HIDDEN = 2,							// 隐藏
	ATTR_SYSTEM	= 4,							// 系统
	ATTR_VOLUME_ID = 8,							// 卷标
	ATTR_DIRECTORY = 0x10,						// 文件夹
	ATTR_ARCHIVE = 0x20,						// 归档，文件
	ATTR_LONG_NAME = 0x0f,						// 此文件名为长文件名
}fileNature;

#define FAT_END 				0xfffffff		// FAT表的最后一个值

/*fat32数据的宏*/
#define	DPT_ACTIVE				0x80			// 该分区是否有效
#define	SIGNATURE				0xAA55			// 扇区签名
#define	DBR_IS					0XEB			// DBR扇区标志
#define	FAT32_ID				0x0B			// FAT32文件系统标识



//#define  get_valu(x) getvalu(x,sizeof(x))		//x为数组指针

// FSINFO扇区：文件系统扇区信息，提供空簇总数和下一个簇的信息,保留扇区数
typedef struct{

	u8 SDguide[4];				//扩展引导标识52526141 FSI_LeadSig
	u8 undef0[480];				//未定义区
	u8 FSINFOsig[4];			//签名72724161，表明该扇区已经被占用
	u8 FSI_Free_Count[4];		//空闲簇数
	u8 FSI_Nxt_free[4];			//下个可以用的簇号，如果此值为0XFFFFFFFF，要从第2簇开始查找
	u8 undef1[14];				//未定义区
	u8 signer[2];				//签名；应该是0xAA55 
}SDFSINFO_Struct;



//FAT表数据结构
typedef struct{

    u8 FATListArray[128][4];
}FATListStruct;
//文件信息
typedef struct{
    u32 CuID;
    u32 FileSize;
    u32 i;							//记录第几条记录
}FileInforStruct;


typedef enum{
	FatOK = 0,
	ERRO_DBTNotActive	= 0x01, 		// 分区无效
	ERRO_NotFat32 		= 0x02,			// 不是Fat32文件系统
	
}FatErro;

// 文件的创建和访问时间
typedef struct{
	u16 year_creat;
	u8 	mon_creat;
	u8	day_creat;
	u8	hour_creat;
	u8	min_creat;
	u8	sec_creat;

	u16 year_write;
	u8 	mon_write;
	u8	day_write;
	u8	hour_write;
	u8	min_write;
	u8	sec_write;	
	
	u16 year_visit;
	u8 	mon_visit;
	u8	day_visit;
	
}dirDateTime;

typedef struct{
	u32 dirSector;		// 目录项所在的扇区	
	u8 	num;			// 所在的条目
	
}dirOneMes;

// 文件信息表
typedef struct{
//	dirOneMes 	mes;			
	u32 		fileStartClus;		// 文件对应的起始簇
	u32 		fileSize;			// 文件大小
	u8 			filename[13];		// 文件名
	fileNature 	nature;				// 文件属性
	dirDateTime	time;				// 文件的时间
	
//	u32 		preClus;			// 当前簇
//	u32 		preSector;			// 当前扇区
//	u32			haseread;			// 已读的文件数量
//	u16 		preByte;			// 扇区中的偏移	
}fileMessage;


typedef struct{
	u32 clus;		// FAT表项
	u16	filnum;		// 目录项
	u8 	secnum;		// 一个簇内的扇区编号
}dirlislog;

#define	dirSecList		SingleList*		// 保存目录信息链表，该链表保存目录的dirOneMes
//目录项结构体
typedef struct{						
	u8 dirName[8];					//文件名的第一个ASCII码。如果没有被使用则为00，如果使用过了0xE5
	u8 fileType[3];					//文件名类型
	u8 fileNature;					//文件属性
	u8 reserve;						//保留为0
	u8 DIR_CrtTimeTeenth[1];		//文件建立的十分之一秒
	u8 crtTime[2];					//文件建立的时间：111 (22222)秒的一半 (33333)时 444，分为444111
	u8 crtDate[2];					//文件建立日期：111 (22222)日 (3333333)年+1980 4，月4111
	u8 visitDate[2];				//文件最后访问日期：111 (22222)日 (3333333)年+1980 4，月4111
	u8 dirClusHigh[2];				//该目录项簇号的高字节
	u8 writeTime[2];				//文件写的时间
	u8 writeDate[2];				//文件改写日期
	u8 dirClusLow[2];				//该目录项簇号的低字节
	u8 fileSize[4];					//文件大小
}directoryStruct;

// fat文件系统的句柄
typedef struct
{ 
	FatErro  	ErrorCode;					  	
	void		(*DiskInit)(void);										// 硬盘初始化函数
	u32			(*readBlocks)(u32 sector,u8* pbuff,u32 numberOfBlocks);	// 读扇区函数
	u32			(*writeBlocks)(u32 sector,u8* pbuff,u32 numberOfBlocks);// 写扇区函数
	
	u8*			buf;					// 数据缓存区指针	
	u32			DBRSector;				// DBR扇区号
	u32 		totalSector;			// 总扇区数
	u32			listFATSize;			// 一个FAT表占的扇区数
	u32			totalSize;				// 单位，M字节
	u32    		firstFATSector;			// 第一个FAT表的扇区号：DBRSector + BPB_RsvdSecCnt
	u32    		SecondFATSector;		// 第2个FAT表的扇区号
	u32 		rootClus;				// 根目录所在的簇号
	u32    		rootSector;				// 根目录扇区号：FirstFATSector +dbr->numFATs*BPB_FATSz32.
	u16			dirperSector;			// 一个扇区可以保存的最大目录项数
	u8   		secPerClus;				// 每个簇的扇区数
	u16 		sectorSize;             // 扇区大小	
	u16			FSInfo;					// FSINFO的扇区号（文件系统信息扇区）
	u16			numClusperSector;		// FAT表一个扇区能保存几个簇
	dirlislog	dirlog;					// 保存当前读到的目录项
	directoryStruct* dir;				// 指向当前的目录项地址
	
	dirSecList 	diskMesList;			// 目录结构链表，保存 dirlog.clus
}fat_Handle;

//值节点
typedef struct 
{	
	listType* next;
	u32 value;

}valueNode;

/*函数*/

fat_Handle* CuiFat_BindingDisk(disk_Handle* dishandle);			// 将盘符跟硬件进行绑定

void CuiFatOpenBootDir(fat_Handle* hfat);						// 打开根目录
void CuiFatOpenDirByName(fat_Handle* hfat,const char* dirname);	// 按名称打开文件夹
void CuiFatBack(fat_Handle* hfat);								// 回退

/*数据结构相关函数*/
#define		SingeListGetValue(nodehead)		(((valueNode*)nodehead)->value)
void  		ValuPush(SingleList* list,u32 value);	// 值的入栈的push操作
valueNode* 	ValuPop(SingleList* list);				// 值的出栈pop操作

#endif
