/*
	应该设置一个函数将盘符跟硬件进行绑定，操作盘符就相当于对绑定的硬件进行文件操作
	应该使用一个函数打开盘符，该函数获得盘的基本信息，比如SD卡的基本信息
	应该可以格式化一个盘符
*/

/*
*	配置硬件接口在disk文件中
*
*/

#ifndef __CUIFAT__H
#define __CUIFAT__H

#include "uhead.h"
#include "disk.h"


#if (DEBUG_LEVEL > 0)
#include "stdio.h"
#define  DEBUG_Log(...) printf(__VA_ARGS__);
#else
#define DEBUG_Log(...)   
#endif 

//根目录区定义文件的属性
#define ATTR_RW 				0				//读写
#define ATTR_READ_ONLY			1				//只读
#define ATTR_HIDDEN				2				//隐藏
#define ATTR_SYSTEM				4				//系统
#define ATTR_VOLUME_ID			8				//卷标
#define ATTR_DIRECTORY			0x10			//子目录
#define ATTR_ARCHIVE			0x20			//归档
#define	ATTR_LONG_NAME			0x0f			//此文件名为长文件名


#define FAT_END 				0xfffffff		//FAT表的最后一个值


//#define  get_valu(x) getvalu(x,sizeof(x))		//x为数组指针


// 定义结构体存储MBR主启动记录,
// 如果0扇区的第一个字节是0XEB则说明该扇区是DBR，该磁盘没有MBR的信息
typedef struct{
	u8  SDguide[446];						//引导码
	
	/*DPT磁盘分区表1*/
	u8  zone1_bootbit[1];					//0x00：分区不可引导标志；0x80可引导
	u8  zone1_CHSstart[3];					//分区起始CHS的地址
	u8  zone1_type[1];						//分区类型，0x0b 是FAT32
	u8  zone1_CHSend[3];					//分区结束CHS的地址
	u8  zone1_DPTFirstSector[4]; 			//分区的第一个扇区,DBR的开始位置
	u8  zone1_DPTsizeof[4]; 				//分区的大小
	/*DPT磁盘分区表2*/
	u8  zone2_bootbit[1];					//0x00：分区不可引导标志；0x80可引导
	u8  zone2_CHSstart[3];					//分区起始CHS的地址
	u8  zone2_type[1];						//分区类型
	u8  zone2_CHSend[3];					//分区结束CHS的地址
	u8  zone2_DPTFirstSector[4]; 			//分区的第一个扇区
	u8  zone2_DPTsizeof[4]; 				//分区的大小
	/*DPT磁盘分区表3*/
	u8  zone3_bootbit[1];					//0x00：分区不可引导标志；0x80可引导
	u8  zone3_CHSstart[3];					//分区起始CHS的地址
	u8  zone3_type[1];						//分区类型
	u8  zone3_CHSend[3];					//分区结束CHS的地址
	u8  zone3_DPTFirstSector[4]; 			//分区的第一个扇区
	u8  zone3_DPTsizeof[4]; 				//分区的大小
	/*DPT磁盘分区表4*/
	u8  zone4_bootbit[1];					//0x00：分区不可引导标志；0x80可引导
	u8  zone4_CHSstart[3];					//分区起始CHS的地址
	u8  zone4_type[1];						//分区类型
	u8  zone4_CHSend[3];					//分区结束CHS的地址
	u8  zone4_DPTFirstSector[4]; 			//分区的第一个扇区
	u8  zone4_DPTsizeof[4]; 				//分区的大小
	u8  signer[2];							//签名；应该是55，AA
}SDMBR_Struct;

//定义结构体存储DBR区数据
typedef struct{

	u8 SDguide[3];				//跳转指令BS_jmpBoot
	u8 BS_OEMName[8];			//文件系统和版本号
	u8 BPB_BytsPerSec[2];		//每扇区的字节数
	u8 BPB_SecPerClus[1];		//每个簇的扇区数
	u8 BPB_RsvdSecCnt[2];		//保留扇区数
	
	u8 BPB_NumFATs[1];			//FAT表个数 =2
	u8 BPB_RootEntCnt[2];		//根目录最多容纳的目录项数，FAT32置0
	u8 BPB_TotSec16[2];			//FAT16扇区总数
	u8 BPB_Media[1];				//介质；硬盘为F8，F0为可移动
	u8 BPB_FATSz16[2];			//FAT16的一个FAT表扇区数，FAT32置0
	u8 BPB_SecPerTrk[2];		//磁道扇区数，FAT32置0
	u8 BPB_NumHeads[2];			//磁头数
	u8 BPB_HiddSec[4];			//分区前已经使用的扇区数（包含MBR）
	
	u8 BPB_TotSec32[4];			//FAT32总扇区数
/*************************以下FAT12/FAT16区别于FAT32*************************************/
	u8 BPB_FATSz32[4];			//一个FAT32表占的扇区数
	u8 BPB_ExtFlags[2];			//标记
	u8 BPB_FSVer[2];				//版本号
	u8 BPB_RootClus[4];			//根目录所在的簇号
	
	u8 BPB_FSInfo[2];			//文件系统扇区信息:1 数据区的第一个扇区
	u8 BPB_BkBootSec[2];		//备份引导扇区，通常为6
	u8 BPB_Reserved[12];		//备用
	
	u8 BS_DrvNum[1];			//驱动器号
	u8 BS_Reserved1[1];			//未用
	u8 BS_BootSig[1];			//扩展引导标签
	u8 BS_VolID[4];				//卷序列号
	u8 BS_FilSysType[11];		//卷标
	u8 BS_FilSysID[8];			//系统ID
	u8 remain[420];				//空
	u8 signer[2];				//签名；应该是55，AA 
}SDDBR_Struct;


// FSINFO扇区：文件系统扇区信息，提供空簇总数和下一个簇的信息,保留扇区数

typedef struct{

	u8 SDguide[4];				//扩展引导标识52526141 FSI_LeadSig
	u8 undef0[480];				//未定义区
	u8 FSINFOsig[4];			//签名72724161，表明该扇区已经被占用
	u8 FSI_Free_Count[4];		//空闲簇数
	u8 FSI_Nxt_free[4];			//下个可以用的簇号，如果此值为0XFFFFFFFF，要从第2簇开始查找
	u8 undef1[14];				//未定义区
	u8 signer[2];				//签名；应该是55，AA 
}SDFSINFO_Struct;


//短名目录项结构体
typedef struct{						//小写字母不允许出现在DIRNAME中
	u8 DirName0[1];					//文件名的第一个ASCII码。如果没有被使用则为00，如果使用过了0xE5
	u8 DirName1to7[7];				//文件名的第2~8个ASCII码
	u8 DirName8to10[3];				//文件名类型
	u8 DIR_Attr[1];					//文件属性
	u8 DIR_NTRes[1];				//保留为0
	u8 DIR_CrtTimeTeenth[1];		//文件建立的十分之一秒
	u8 DIR_CrtTime[2];				//文件建立的时间：111 (22222)秒的一半 (33333)时 444，分为444111
	u8 DIR_CrtDate[2];				//文件建立日期：111 (22222)日 (3333333)年+1980 4，月4111
	u8 DIR_LastAccDate[2];			//文件最后访问日期：111 (22222)日 (3333333)年+1980 4，月4111
	u8 DIR_FstClusHI[2];			//该目录项簇号的高字节
	u8 DIR_WrtTime[2];				//文件写的时间
	u8 DIR_WrtDate[2];				//文件改写日期
	u8 DIR_FstClusLO[2];			//该目录项簇号的低字节
	u8 DIR_FileSize[4];				//文件大小
}SD_shortDirName_Struct;

//目录项结构体
typedef struct{
    SD_shortDirName_Struct Menu[16];
}MenuStruct;


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

//有用的参数结构体
typedef struct{
	
	u32    	FirstFATSector;			//第一个FAT表的扇区号：zone1_DPTFirstSector + BPB_RsvdSecCnt
	u32    	FATSz32;				//一个FAT32表占的扇区数
	u32    	FirstDirSector;			//根目录的扇区号：FirstFATSector +2*BPB_FATSz32
	u8   	FSInfo;					//FSINFO的扇区号
  	u32  	FSINFO_secoter;         //FSINFO的记录的最后使用的扇区号
	u8   	SecPerClus;				//每个簇的扇区数
	u32    	ClusBase;				//0簇所在的扇区号
	u8    	FirstDirCuNum;			//根目录所在的簇号
	
}SD_valueINFO_Struct;


typedef enum{
	FatOK = 0,
	ERRO_DiskNameS,	// 已有的盘符
}FatErro;
// fat文件系统的句柄
typedef struct
{ 
	const char* driveName;												// 盘符字符串
	FatErro  	ErrorCode;					  
	u32 		blockNbr;                   							// 扇区总数
	u32 		blockSize;                  							// 扇区大小
	void		(*DiskInit)(void);										// 硬盘初始化函数
	u32			(*readBlocks)(u32 sector,u8* pbuff,u32 numberOfBlocks);	// 读扇区函数
	u32			(*writeBlocks)(u32 sector,u8* pbuff,u32 numberOfBlocks);// 写扇区函数
	
}fat_Handle;



//函数

FatErro CuiFat_BindingDisk(const char* driveName,disk_Handle* dishandle);// 将盘符跟硬件进行绑定


#endif
