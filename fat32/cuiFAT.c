/*
* 大部分函数应参考UNIX的文件操作，但是UNIX的文件有时候会比较繁琐，所以加以简化
*/


#include "cuiFAT.h"
#define	CuiFat_Valu(parray,type)	*((type*)parray)					// FAT获得小端的值

// 定义结构体存储MBR主启动记录,
// 如果0扇区的第一个字节是0XEB则说明该扇区是DBR，该磁盘没有MBR的信息
typedef struct{
	u8  SDguide[446];						//引导码
	
	/*DPT磁盘分区表1*/
	u8  zone1_activ;						//0x00：分区不可引导标志；0x80可引导
	u8  zone1_CHSstart[3];					//分区起始CHS的地址
	u8  zone1_type;							//分区类型，0x0b 是FAT32
	u8  zone1_CHSend[3];					//分区结束CHS的地址
	u8  zone1_DBRSector[4]; 				//分区的第一个扇区,DBR的开始位置
	u8  zone1_DPTsizeof[4]; 				//分区的大小
	/*DPT磁盘分区表2*/
	u8  zone2_activ;						//0x00：分区不可引导标志；0x80可引导
	u8  zone2_CHSstart[3];					//分区起始CHS的地址
	u8  zone2_type;							//分区类型
	u8  zone2_CHSend[3];					//分区结束CHS的地址
	u8  zone2_DBRSector[4]; 				//分区的第一个扇区
	u8  zone2_DPTsizeof[4]; 				//分区的大小
	/*DPT磁盘分区表3*/
	u8  zone3_activ;						//0x00：分区不可引导标志；0x80可引导
	u8  zone3_CHSstart[3];					//分区起始CHS的地址
	u8  zone3_type;							//分区类型
	u8  zone3_CHSend[3];					//分区结束CHS的地址
	u8  zone3_DBRSector[4]; 				//分区的第一个扇区
	u8  zone3_DPTsizeof[4]; 				//分区的大小
	/*DPT磁盘分区表4*/
	u8  zone4_activ;						//0x00：分区不可引导标志；0x80可引导
	u8  zone4_CHSstart[3];					//分区起始CHS的地址
	u8  zone4_type;							//分区类型
	u8  zone4_CHSend[3];					//分区结束CHS的地址
	u8  zone4_DBRSector[4]; 				//分区的第一个扇区
	u8  zone4_DPTsizeof[4]; 				//分区的大小
	u8  signature[2];						//签名；应该是55，AA
}SDMBR_Struct;

//定义结构体存储DBR区数据
typedef struct{

	u8 SDguide[3];				//跳转指令BS_jmpBoot
	u8 BS_OEMName[8];			//文件系统和版本号
	u8 bytsPerSec[2];			//每扇区的字节数
	u8 secPerClus;				//每个簇的扇区数
	u8 rsvdSecCnt[2];			//保留扇区数
	
	u8 numFATs;					//FAT表个数 =2
	u8 rootEntCnt[2];			//根目录最多容纳的目录项数，FAT32置0
	u8 totSec16[2];				//FAT16扇区总数
	u8 media;					//介质；硬盘为F8，F0为可移动
	u8 FATSz16[2];				//FAT16的一个FAT表扇区数，FAT32置0
	u8 secPerTrk[2];			//磁道扇区数，FAT32置0
	u8 numHeads[2];				//磁头数
	u8 hiddSec[4];				//分区前已经使用的扇区数（包含MBR）
	
	u8 totalSectors[4];			//总扇区数
/*************************以下FAT12/FAT16区别于FAT32*************************************/
	u8 listFATSize[4];			//一个FAT32表占的扇区数
	u8 BPB_ExtFlags[2];			//标记
	u8 BPB_FSVer[2];			//版本号
	u8 rootClus[4];				//根目录所在的簇号
	
	u8 BPB_FSInfo[2];			//文件系统扇区信息:1 数据区的第一个扇区
	u8 BPB_BkBootSec[2];		//备份引导扇区，通常为6
	u8 BPB_Reserved[12];		//备用
	
	u8 BS_DrvNum;				//驱动器号
	u8 BS_Reserved1;			//未用
	u8 BS_BootSig;				//扩展引导标签
	u8 BS_VolID[4];				//卷序列号
	u8 BS_FilSysType[11];		//卷标
	u8 BS_FilSysID[8];			//系统ID
	u8 remain[420];				//空
	u8 signer[2];				//签名；应该是55，AA 
}SDDBR_Struct;

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

// 根据文件目录项生成文件的名字，需要free
u8* MallocFileName(directoryStruct* dir)
{
	u8* filename = 0;
	u8 i = 0;
	u8 j = 0;
	if(dir->dirName[0] == 0xe5) return 0;	
	for(i=0;i<8;i++)
	{
		if(dir->dirName[i] == 0x20) break;		
	}
	filename = (u8*)malloc(i+1);
	for(j=0;j<i;j++)
	{
		filename[j] = dir->dirName[j];
	}	
	j++;
//	filename[j] = '.';
//	j++;
//	for(i=0;i<3;i++)	//添加扩展名
//	{
//		filename[j + i] = dir->fileType[i];
//	}
	filename[j] = 0;
	return filename;
}

// 获得目录项创建时间和最后访问时间
void Fat_GetTimeDate(directoryStruct* dir,dirDateTime* datbuf)
{
	datbuf->sec_creat = (dir->crtTime[0]&0x1f)<<1;
	datbuf->min_creat = ((dir->crtTime[1]&0x07)<<3)|((dir->crtTime[0]&0xe0)>>5);
	datbuf->hour_creat = dir->crtTime[1]>>3;
	datbuf->day_creat = dir->crtDate[0]&0x1f;
	datbuf->mon_creat = (dir->crtDate[0]>>5)|((dir->crtDate[1]&0x01)<<3);
	datbuf->year_creat = (dir->crtDate[1]>>1) + 1980;
	
	datbuf->sec_write = (dir->writeTime[0]&0x1f)<<1;
	datbuf->min_write = ((dir->writeTime[1]&0x07)<<3)|((dir->writeTime[0]&0xe0)>>5);
	datbuf->hour_write = dir->writeTime[1]>>3;
	datbuf->day_write = dir->writeDate[0]&0x1f;
	datbuf->mon_write = (dir->writeDate[0]>>5)|((dir->writeDate[1]&0x01)<<3);
	datbuf->year_write = (dir->writeDate[1]>>1) + 1980;
	
	datbuf->day_visit = dir->visitDate[0]&0x1f;
	datbuf->mon_visit = (dir->visitDate[0]>>5)|((dir->visitDate[1]&0x01)<<3);
	datbuf->year_visit = (dir->visitDate[1]>>1) + 1980;	
}

// 读文件目录项，获得文件信息
void ReadDirMessage(fileMessage* filemess,directoryStruct* dir)
{
	filemess->filename = CreatFileName(dir);
	filemess->fileSize = CuiFat_Valu(dir->fileSize,u32);
	filemess->fileStartCube = (CuiFat_Valu(dir->dirClusHigh,u16) << 16)|(CuiFat_Valu(dir->dirClusLow,u16));
	Fat_GetTimeDate(dir,&filemess->time);
	filemess->nature = (fileNature)dir->fileNature;
}

//比较字符串disk是否包含在path字符串内
BOOL  CampareStringIn(const char* path,const char* disk)
{
	u16 i = 0;
	for(i=0;disk[i]!=0;i++)
	{
		if(disk[i]^path[i])return FALSE;
	}
	return TRUE;
}

// 读DBR扇区数据
void ReadDBRSector(fat_Handle* hfat)
{
	hfat->readBlocks(hfat->DBRSector,hfat->buf,1);
	
	SDDBR_Struct* dbr = (SDDBR_Struct*)hfat->buf;
	hfat->sectorSize = CuiFat_Valu(dbr->bytsPerSec,u16);
	hfat->secPerClus = dbr->secPerClus;
	hfat->totalSize = (CuiFat_Valu(dbr->totalSectors,u32)*hfat->sectorSize)>>20;
	hfat->totalSector = CuiFat_Valu(dbr->totalSectors,u32);
	hfat->listFATSize = CuiFat_Valu(dbr->listFATSize,u32);
	hfat->firstFATSector = hfat->DBRSector + CuiFat_Valu(dbr->rsvdSecCnt,u16);
	hfat->firstDirSector = hfat->firstFATSector + (hfat->listFATSize * dbr->numFATs);
	hfat->rootClus = CuiFat_Valu(dbr->rootClus,u32);
	
	debug("单个扇区大小：%d byts\r\n",hfat->sectorSize);
	debug("磁盘容量：%d M\r\n",hfat->totalSize);
	debug("根目录扇区号：%#X \r\n",hfat->firstDirSector);
	debug("FAT表扇区号：%#X \r\n",hfat->firstFATSector);
	debug("FAT表占的扇区数：%d\r\n",hfat->listFATSize);
	debug("一个簇 %d 个扇区\r\n",hfat->secPerClus);

}

// 开始读0扇区
void StartReadFatMessage(fat_Handle* hfat)
{
	hfat->readBlocks(0,hfat->buf,1);
	if(hfat->buf[0] == DBR_IS) 				
	{
		debug("0扇区是DBR区\r\n");
		hfat->DBRSector = 0;
		ReadDBRSector(hfat);
	}else								
	{
		SDMBR_Struct* mbr = (SDMBR_Struct*)hfat->buf;	
		if(mbr->zone1_activ == DPT_ACTIVE)				// 该分区有效	
		{
			debug("0扇区是MBR区\r\n");
			if(mbr->zone1_type == FAT32_ID)				// FAT32系统
			{
				hfat->DBRSector = CuiFat_Valu(mbr->zone1_DBRSector,u32);	// 保存DBR扇区号
				ReadDBRSector(hfat);					// 读DBR扇区数据
			}else  	hfat->ErrorCode |= ERRO_NotFat32;
		}else hfat->ErrorCode |= ERRO_DBTNotActive;			
	}
}

/*******************************************************
*-函数名称	：CuiFat_BindingDisk
*-函数作用	：将盘符跟硬件进行绑定
*-参数		：dishandle 硬盘的句柄
*-返回值	：hfat 文件信息句柄，用来保存文件信息
*-备注		：调用该函数，将盘符与硬件绑定后压到链表中；当盘符重复则失败
*******************************************************/
fat_Handle* CuiFat_BindingDisk(disk_Handle* dishandle)
{
	fat_Handle* hfat = (fat_Handle*)malloc(sizeof(fat_Handle));
	hfat->ErrorCode = FatOK;
	hfat->readBlocks = dishandle->readBlocks;
	hfat->writeBlocks = dishandle->writeBlocks;
	hfat->diskMesList.next = 0;
	hfat->diskMesList.num = 0;
/*	SingleList* pNode = &diskList;
	debug("所有盘符：\r\n");
	while(SingleList_Iterator(&pNode))
	{
		debug("%s\r\n",SingeListGetnode(fat_Handle,pNode)->driveName);
	}	*/	
	
	StartReadFatMessage(hfat);

	return hfat;
}



/*******************************************************
*-函数名称	：CuiFat_OpenDisk
*-函数作用	：打开一个盘符，返回该硬盘的基本信息
*-参数		：
*-返回值	：打开成功返回句柄，失败返回0
*-备注		：
*******************************************************/
FatErro CuiFat_OpenDisk(const char* disk)
{
	fat_Handle* hfat = 0;
	// 遍历盘符链表，如果盘符重复则退出
	SingleListNode* pNode = &diskList;	
	while(SingleList_Iterator(&pNode))
	{
		if(CampareStringIn(disk,SingeListGetnode(fat_Handle,pNode)->driveName))break;
	}
	if(pNode == 0)	
	{
		debug("盘符无效\r\n");
		return ERRO_DiskNameE;
	}
	hfat = SingeListGetnode(fat_Handle,pNode);
	ReadFatMessage(hfat);
	
	return FatOK;
}

// 打开文件
FatErro CuiFat_OpenFile(const char* file)
{
	return FatOK;
}

