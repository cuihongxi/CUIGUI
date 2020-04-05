/*
* 大部分函数应参考UNIX的文件操作，但是UNIX的文件有时候会比较繁琐，所以加以简化
*/


#include "cuiFAT.h"
#define	CuiFat_Valu(parray,type)	*((type*)parray)					// FAT获得小端的值

//根据簇号，转换成起始扇区号
u32 ChangeClus2Sector(fat_Handle* hfat,u32 clus)
{
	return hfat->rootSector + hfat->secPerClus * (clus - hfat->rootClus);
}

// 值的入栈的push操作
void  ValuPush(SingleList* list,u32 value)
{ 
    valueNode* newnode = (valueNode*)malloc(sizeof(valueNode));
	newnode->next =	list->next;
	list->next = newnode;
	newnode->value = value;    
    list->num ++;
}

// 值的出栈pop操作
// 注：出栈之后的节点不需要时，需要free
valueNode* ValuPop(SingleList* list)
{
	valueNode* nod;
	list->num --;
	nod = list->next;
	if(list->next)list->next = nod->next;
	return nod;
}

// 链表中获得栈顶的FAT表地址
u32 GetValueFormList(SingleList* list)
{
	SingleListNode* pNode = list;
	SingleList_Iterator(&pNode);
	return SingeListGetValue(pNode);
}
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
	
	u8 FSInfo[2];			//FSINFO（文件系统信息扇区）扇区号,该扇区为操作系统提供关于空簇总数及下一可用簇的信息
	u8 BPB_BkBootSec[2];		//备份引导扇区，通常为6
	u8 BPB_Reserved[12];		//备用
	
	u8 BS_DrvNum;				//驱动器号
	u8 BS_Reserved1;			//未用
	u8 BS_BootSig;				//扩展引导标签
	u8 BS_VolID[4];				//卷序列号
	u8 BS_FilSysType[11];		//卷标:NO NAME
	u8 BS_FilSysID[8];			//系统ID文件系统格式的 ASCII 码，FAT32
	u8 remain[420];				//空
	u8 signer[2];				//签名；应该是55，AA 
}SDDBR_Struct;



// 根据文件目录项生成文件的名字，需要free
u8* GetFileName(fileMessage* filemess,directoryStruct* dir)
{
	u8 j = 0;
	for(j=0;j<8;j++)
	{
		if(dir->dirName[j] == 0x20)break;
		filemess->filename[j] = dir->dirName[j];
	}
	if(dir->fileNature == ATTR_ARCHIVE)
	{
		filemess->filename[j] = '.';
		j++;
		for(u8 i=0;i<3;i++)	//添加扩展名
		{
			filemess->filename[j] = dir->fileType[i];
			j++;
		}		
	}

	filemess->filename[j] = 0;
	return filemess->filename;
}

// 获得目录项下的FAT表
u32 DirGetFat(directoryStruct* dir)
{
	return (CuiFat_Valu(dir->dirClusHigh,u16) << 16)|(CuiFat_Valu(dir->dirClusLow,u16));
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
	GetFileName(filemess,dir);
	filemess->fileSize = CuiFat_Valu(dir->fileSize,u32);
	filemess->fileStartClus = (CuiFat_Valu(dir->dirClusHigh,u16) << 16)|(CuiFat_Valu(dir->dirClusLow,u16));
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
	if(CampareStringIn((const char*)dbr->BS_FilSysID,"FAT32") == FALSE)
		hfat->ErrorCode |= ERRO_NotFat32;
	hfat->sectorSize = CuiFat_Valu(dbr->bytsPerSec,u16);
	hfat->secPerClus = dbr->secPerClus;
	hfat->totalSize = (CuiFat_Valu(dbr->totalSectors,u32)*hfat->sectorSize)>>20;
	hfat->totalSector = CuiFat_Valu(dbr->totalSectors,u32);
	hfat->listFATSize = CuiFat_Valu(dbr->listFATSize,u32);
	hfat->firstFATSector = hfat->DBRSector + CuiFat_Valu(dbr->rsvdSecCnt,u16);
	hfat->SecondFATSector = hfat->firstFATSector + hfat->listFATSize;
	hfat->rootSector = hfat->firstFATSector + (hfat->listFATSize * dbr->numFATs);
	hfat->rootClus = CuiFat_Valu(dbr->rootClus,u32);
	hfat->FSInfo = CuiFat_Valu(dbr->FSInfo,u16);
	hfat->dirperSector = hfat->sectorSize/32;				// 一个扇区有几个目录项
	hfat->dirlog.clus = hfat->rootClus;
	hfat->dirlog.filnum = 0;
	hfat->dirlog.secnum = 0;
	hfat->numClusperSector = hfat->sectorSize/4;
	hfat->diskMesList = NewSingleList();							// 创建新链表

	
	debug("单个扇区大小：%d byts\r\n",hfat->sectorSize);
	debug("磁盘容量：%d M\r\n",hfat->totalSize);
	debug("根目录扇区号：%#X \r\n",hfat->rootSector);
	debug("FAT1表扇区号：%#X \r\n",hfat->firstFATSector);
	debug("FSInfo扇区号：%#X\r\n",hfat->FSInfo);
	debug("每簇 %d 个扇区\r\n",hfat->secPerClus);
	debug("每个扇区有 %d 个目录项\r\n",hfat->dirperSector);
	debug("签名标志: %#X\r\n",CuiFat_Valu(dbr->signer,u16));
	//根据簇号，转换成扇区号
 
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
	hfat->buf = (u8*)malloc(dishandle->sectorSize);
	hfat->sectorSize = dishandle->sectorSize;
	
/*	SingleListNode* pNode = &diskList;
	debug("所有盘符：\r\n");
	while(SingleList_Iterator(&pNode))
	{
		debug("%s\r\n",SingeListGetnode(fat_Handle,pNode)->driveName);
	}	*/	
	
	StartReadFatMessage(hfat);

	return hfat;
}
// 获得某簇里面的值
u32 GetClusDate(fat_Handle* hfat,u32 clus)
{
	u8* buf = (u8*)malloc(hfat->sectorSize);
	u32 dat = 0;
	u32 i = clus/hfat->numClusperSector;
	u32 j = clus%hfat->numClusperSector;
	hfat->readBlocks(hfat->firstFATSector + i,buf,1);//读簇所在的扇区
	dat = CuiFat_Valu(((u32*)buf + j),u32);
	free(buf);
	return dat;
}

// 迭代打开hfat->dirlog指向的目录
// 每次调用返回一个directoryStruct
directoryStruct* Iterator_Dir(fat_Handle* hfat)
{
	//如果扇区号是0，则读一个扇区的数据
	if(hfat->dirlog.filnum == 0)
		hfat->readBlocks(ChangeClus2Sector(hfat,hfat->dirlog.clus) + hfat->dirlog.secnum,hfat->buf,1);

	hfat->dir = ((directoryStruct*)hfat->buf) + hfat->dirlog.filnum;	
	hfat->dirlog.filnum ++;
	if(hfat->dirlog.filnum == hfat->dirperSector)
	{
		hfat->dirlog.filnum = 0; 
		hfat->dirlog.secnum ++;
		if(hfat->dirlog.secnum == hfat->secPerClus)
		{
			hfat->dirlog.secnum = 0;
			//下一个簇
			hfat->dirlog.clus = GetClusDate(hfat,hfat->dirlog.clus);
			if(hfat->dirlog.clus == 0x0fffffff)	return 0;// 最后一个簇		
		}
	}
	return hfat->dir;
}

// 显示条目信息
void ShowDirName(directoryStruct* dir)
{
	fileMessage filemess = {0};
	if(dir->dirName[0]!=0xE5)
	{
		ReadDirMessage(&filemess,dir);
		switch(dir->fileNature)
		{
			case ATTR_DIRECTORY:
				printf("%s\r\n",filemess.filename);	
			break;
			case ATTR_ARCHIVE:
				ReadDirMessage(&filemess,dir);
				printf("%s\r\n",filemess.filename);	
			break;
		}
		
	}
}
// 遍历当前的文件夹
void CuiFatInterator_AllDir(fat_Handle* hfat)
{
	while(Iterator_Dir(hfat))	// 打开目录
	{
		ShowDirName(hfat->dir);
	}
	ShowDirName(hfat->dir);
}

// 打开一个文件夹目录项
// 该目录项存在于 hfat->diskMesList 链表中
void OpenDirectory(fat_Handle* hfat,directoryStruct* dir)
{
	hfat->dirlog.clus = DirGetFat(dir);
	hfat->dirlog.filnum = 0;
	hfat->dirlog.secnum = 0;
	ValuPush(hfat->diskMesList,hfat->dirlog.clus);	// 将该目录项的FAT表push进栈
	CuiFatInterator_AllDir(hfat);							// 遍历当前所有的文件夹
}

//判断文件名属不属于目录项的文件名
BOOL JugeDirnameInDir(directoryStruct* dir,const char* dirname)
{
	u16 i = 0;
	while(dirname[i])
	{
		if(dirname[i]^dir->dirName[i]) return FALSE;
		i ++;
	}
	return TRUE;
	
}

directoryStruct* FindDirectoryByName(fat_Handle* hfat,const char* dirname)
{
	while(Iterator_Dir(hfat))	// 打开目录
	{
		if(JugeDirnameInDir(hfat->dir,dirname))
		return hfat->dir;
	}
	if(JugeDirnameInDir(hfat->dir,dirname))return hfat->dir;
	
	return 0;
}

// 按名字打开文件夹
void CuiFatOpenDirByName(fat_Handle* hfat,const char* dirname)
{
	if(FindDirectoryByName(hfat,dirname))
	{
		OpenDirectory(hfat,hfat->dir);
	}
	else	debug("无此文件夹名\r\n");
	
}

// 回退
void CuiFatBack(fat_Handle* hfat)
{
	debug("回退,hfat->diskMesList->num = %d\r\n",hfat->diskMesList->num);
	if(hfat->diskMesList->num > 1)	
		free(ValuPop(hfat->diskMesList));				// 值的出栈pop操作,并释放
	hfat->dirlog.clus = GetValueFormList(hfat->diskMesList);
	hfat->dirlog.filnum = 0;
	hfat->dirlog.secnum = 0;
	CuiFatInterator_AllDir(hfat);							// 遍历当前所有的文件夹
}

// 打开根目录
void CuiFatOpenBootDir(fat_Handle* hfat)
{
	hfat->dirlog.clus = hfat->rootClus;
	hfat->dirlog.filnum = 0;
	hfat->dirlog.secnum = 0;
	ValuPush(hfat->diskMesList,hfat->dirlog.clus);	// 将该目录项的FAT表push进栈
	CuiFatInterator_AllDir(hfat);
}


















































