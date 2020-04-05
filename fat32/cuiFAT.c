/*
* �󲿷ֺ���Ӧ�ο�UNIX���ļ�����������UNIX���ļ���ʱ���ȽϷ��������Լ��Լ�
*/


#include "cuiFAT.h"
#define	CuiFat_Valu(parray,type)	*((type*)parray)					// FAT���С�˵�ֵ

//���ݴغţ�ת������ʼ������
u32 ChangeClus2Sector(fat_Handle* hfat,u32 clus)
{
	return hfat->rootSector + hfat->secPerClus * (clus - hfat->rootClus);
}

// ֵ����ջ��push����
void  ValuPush(SingleList* list,u32 value)
{ 
    valueNode* newnode = (valueNode*)malloc(sizeof(valueNode));
	newnode->next =	list->next;
	list->next = newnode;
	newnode->value = value;    
    list->num ++;
}

// ֵ�ĳ�ջpop����
// ע����ջ֮��Ľڵ㲻��Ҫʱ����Ҫfree
valueNode* ValuPop(SingleList* list)
{
	valueNode* nod;
	list->num --;
	nod = list->next;
	if(list->next)list->next = nod->next;
	return nod;
}

// �����л��ջ����FAT���ַ
u32 GetValueFormList(SingleList* list)
{
	SingleListNode* pNode = list;
	SingleList_Iterator(&pNode);
	return SingeListGetValue(pNode);
}
// ����ṹ��洢MBR��������¼,
// ���0�����ĵ�һ���ֽ���0XEB��˵����������DBR���ô���û��MBR����Ϣ
typedef struct{
	u8  SDguide[446];						//������
	
	/*DPT���̷�����1*/
	u8  zone1_activ;						//0x00����������������־��0x80������
	u8  zone1_CHSstart[3];					//������ʼCHS�ĵ�ַ
	u8  zone1_type;							//�������ͣ�0x0b ��FAT32
	u8  zone1_CHSend[3];					//��������CHS�ĵ�ַ
	u8  zone1_DBRSector[4]; 				//�����ĵ�һ������,DBR�Ŀ�ʼλ��
	u8  zone1_DPTsizeof[4]; 				//�����Ĵ�С
	/*DPT���̷�����2*/
	u8  zone2_activ;						//0x00����������������־��0x80������
	u8  zone2_CHSstart[3];					//������ʼCHS�ĵ�ַ
	u8  zone2_type;							//��������
	u8  zone2_CHSend[3];					//��������CHS�ĵ�ַ
	u8  zone2_DBRSector[4]; 				//�����ĵ�һ������
	u8  zone2_DPTsizeof[4]; 				//�����Ĵ�С
	/*DPT���̷�����3*/
	u8  zone3_activ;						//0x00����������������־��0x80������
	u8  zone3_CHSstart[3];					//������ʼCHS�ĵ�ַ
	u8  zone3_type;							//��������
	u8  zone3_CHSend[3];					//��������CHS�ĵ�ַ
	u8  zone3_DBRSector[4]; 				//�����ĵ�һ������
	u8  zone3_DPTsizeof[4]; 				//�����Ĵ�С
	/*DPT���̷�����4*/
	u8  zone4_activ;						//0x00����������������־��0x80������
	u8  zone4_CHSstart[3];					//������ʼCHS�ĵ�ַ
	u8  zone4_type;							//��������
	u8  zone4_CHSend[3];					//��������CHS�ĵ�ַ
	u8  zone4_DBRSector[4]; 				//�����ĵ�һ������
	u8  zone4_DPTsizeof[4]; 				//�����Ĵ�С
	u8  signature[2];						//ǩ����Ӧ����55��AA
}SDMBR_Struct;

//����ṹ��洢DBR������
typedef struct{

	u8 SDguide[3];				//��תָ��BS_jmpBoot
	u8 BS_OEMName[8];			//�ļ�ϵͳ�Ͱ汾��
	u8 bytsPerSec[2];			//ÿ�������ֽ���
	u8 secPerClus;				//ÿ���ص�������
	u8 rsvdSecCnt[2];			//����������
	
	u8 numFATs;					//FAT����� =2
	u8 rootEntCnt[2];			//��Ŀ¼������ɵ�Ŀ¼������FAT32��0
	u8 totSec16[2];				//FAT16��������
	u8 media;					//���ʣ�Ӳ��ΪF8��F0Ϊ���ƶ�
	u8 FATSz16[2];				//FAT16��һ��FAT����������FAT32��0
	u8 secPerTrk[2];			//�ŵ���������FAT32��0
	u8 numHeads[2];				//��ͷ��
	u8 hiddSec[4];				//����ǰ�Ѿ�ʹ�õ�������������MBR��
	
	u8 totalSectors[4];			//��������
/*************************����FAT12/FAT16������FAT32*************************************/
	u8 listFATSize[4];			//һ��FAT32��ռ��������
	u8 BPB_ExtFlags[2];			//���
	u8 BPB_FSVer[2];			//�汾��
	u8 rootClus[4];				//��Ŀ¼���ڵĴغ�
	
	u8 FSInfo[2];			//FSINFO���ļ�ϵͳ��Ϣ������������,������Ϊ����ϵͳ�ṩ���ڿմ���������һ���ôص���Ϣ
	u8 BPB_BkBootSec[2];		//��������������ͨ��Ϊ6
	u8 BPB_Reserved[12];		//����
	
	u8 BS_DrvNum;				//��������
	u8 BS_Reserved1;			//δ��
	u8 BS_BootSig;				//��չ������ǩ
	u8 BS_VolID[4];				//�����к�
	u8 BS_FilSysType[11];		//���:NO NAME
	u8 BS_FilSysID[8];			//ϵͳID�ļ�ϵͳ��ʽ�� ASCII �룬FAT32
	u8 remain[420];				//��
	u8 signer[2];				//ǩ����Ӧ����55��AA 
}SDDBR_Struct;



// �����ļ�Ŀ¼�������ļ������֣���Ҫfree
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
		for(u8 i=0;i<3;i++)	//�����չ��
		{
			filemess->filename[j] = dir->fileType[i];
			j++;
		}		
	}

	filemess->filename[j] = 0;
	return filemess->filename;
}

// ���Ŀ¼���µ�FAT��
u32 DirGetFat(directoryStruct* dir)
{
	return (CuiFat_Valu(dir->dirClusHigh,u16) << 16)|(CuiFat_Valu(dir->dirClusLow,u16));
}

// ���Ŀ¼���ʱ���������ʱ��
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

// ���ļ�Ŀ¼�����ļ���Ϣ
void ReadDirMessage(fileMessage* filemess,directoryStruct* dir)
{
	GetFileName(filemess,dir);
	filemess->fileSize = CuiFat_Valu(dir->fileSize,u32);
	filemess->fileStartClus = (CuiFat_Valu(dir->dirClusHigh,u16) << 16)|(CuiFat_Valu(dir->dirClusLow,u16));
	Fat_GetTimeDate(dir,&filemess->time);
	filemess->nature = (fileNature)dir->fileNature;
	
}

//�Ƚ��ַ���disk�Ƿ������path�ַ�����
BOOL  CampareStringIn(const char* path,const char* disk)
{
	u16 i = 0;
	for(i=0;disk[i]!=0;i++)
	{
		if(disk[i]^path[i])return FALSE;
	}
	return TRUE;
}

// ��DBR��������
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
	hfat->dirperSector = hfat->sectorSize/32;				// һ�������м���Ŀ¼��
	hfat->dirlog.clus = hfat->rootClus;
	hfat->dirlog.filnum = 0;
	hfat->dirlog.secnum = 0;
	hfat->numClusperSector = hfat->sectorSize/4;
	hfat->diskMesList = NewSingleList();							// ����������

	
	debug("����������С��%d byts\r\n",hfat->sectorSize);
	debug("����������%d M\r\n",hfat->totalSize);
	debug("��Ŀ¼�����ţ�%#X \r\n",hfat->rootSector);
	debug("FAT1�������ţ�%#X \r\n",hfat->firstFATSector);
	debug("FSInfo�����ţ�%#X\r\n",hfat->FSInfo);
	debug("ÿ�� %d ������\r\n",hfat->secPerClus);
	debug("ÿ�������� %d ��Ŀ¼��\r\n",hfat->dirperSector);
	debug("ǩ����־: %#X\r\n",CuiFat_Valu(dbr->signer,u16));
	//���ݴغţ�ת����������
 
}

// ��ʼ��0����
void StartReadFatMessage(fat_Handle* hfat)
{
	hfat->readBlocks(0,hfat->buf,1);
	if(hfat->buf[0] == DBR_IS) 				
	{
		debug("0������DBR��\r\n");
		hfat->DBRSector = 0;
		ReadDBRSector(hfat);
	}else								
	{
		SDMBR_Struct* mbr = (SDMBR_Struct*)hfat->buf;	
		if(mbr->zone1_activ == DPT_ACTIVE)				// �÷�����Ч	
		{
			debug("0������MBR��\r\n");
			if(mbr->zone1_type == FAT32_ID)				// FAT32ϵͳ
			{
				hfat->DBRSector = CuiFat_Valu(mbr->zone1_DBRSector,u32);	// ����DBR������
				ReadDBRSector(hfat);					// ��DBR��������
			}else  	hfat->ErrorCode |= ERRO_NotFat32;
		}else hfat->ErrorCode |= ERRO_DBTNotActive;			
	}
}

/*******************************************************
*-��������	��CuiFat_BindingDisk
*-��������	�����̷���Ӳ�����а�
*-����		��dishandle Ӳ�̵ľ��
*-����ֵ	��hfat �ļ���Ϣ��������������ļ���Ϣ
*-��ע		�����øú��������̷���Ӳ���󶨺�ѹ�������У����̷��ظ���ʧ��
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
	debug("�����̷���\r\n");
	while(SingleList_Iterator(&pNode))
	{
		debug("%s\r\n",SingeListGetnode(fat_Handle,pNode)->driveName);
	}	*/	
	
	StartReadFatMessage(hfat);

	return hfat;
}
// ���ĳ�������ֵ
u32 GetClusDate(fat_Handle* hfat,u32 clus)
{
	u8* buf = (u8*)malloc(hfat->sectorSize);
	u32 dat = 0;
	u32 i = clus/hfat->numClusperSector;
	u32 j = clus%hfat->numClusperSector;
	hfat->readBlocks(hfat->firstFATSector + i,buf,1);//�������ڵ�����
	dat = CuiFat_Valu(((u32*)buf + j),u32);
	free(buf);
	return dat;
}

// ������hfat->dirlogָ���Ŀ¼
// ÿ�ε��÷���һ��directoryStruct
directoryStruct* Iterator_Dir(fat_Handle* hfat)
{
	//�����������0�����һ������������
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
			//��һ����
			hfat->dirlog.clus = GetClusDate(hfat,hfat->dirlog.clus);
			if(hfat->dirlog.clus == 0x0fffffff)	return 0;// ���һ����		
		}
	}
	return hfat->dir;
}

// ��ʾ��Ŀ��Ϣ
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
// ������ǰ���ļ���
void CuiFatInterator_AllDir(fat_Handle* hfat)
{
	while(Iterator_Dir(hfat))	// ��Ŀ¼
	{
		ShowDirName(hfat->dir);
	}
	ShowDirName(hfat->dir);
}

// ��һ���ļ���Ŀ¼��
// ��Ŀ¼������� hfat->diskMesList ������
void OpenDirectory(fat_Handle* hfat,directoryStruct* dir)
{
	hfat->dirlog.clus = DirGetFat(dir);
	hfat->dirlog.filnum = 0;
	hfat->dirlog.secnum = 0;
	ValuPush(hfat->diskMesList,hfat->dirlog.clus);	// ����Ŀ¼���FAT��push��ջ
	CuiFatInterator_AllDir(hfat);							// ������ǰ���е��ļ���
}

//�ж��ļ�����������Ŀ¼����ļ���
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
	while(Iterator_Dir(hfat))	// ��Ŀ¼
	{
		if(JugeDirnameInDir(hfat->dir,dirname))
		return hfat->dir;
	}
	if(JugeDirnameInDir(hfat->dir,dirname))return hfat->dir;
	
	return 0;
}

// �����ִ��ļ���
void CuiFatOpenDirByName(fat_Handle* hfat,const char* dirname)
{
	if(FindDirectoryByName(hfat,dirname))
	{
		OpenDirectory(hfat,hfat->dir);
	}
	else	debug("�޴��ļ�����\r\n");
	
}

// ����
void CuiFatBack(fat_Handle* hfat)
{
	debug("����,hfat->diskMesList->num = %d\r\n",hfat->diskMesList->num);
	if(hfat->diskMesList->num > 1)	
		free(ValuPop(hfat->diskMesList));				// ֵ�ĳ�ջpop����,���ͷ�
	hfat->dirlog.clus = GetValueFormList(hfat->diskMesList);
	hfat->dirlog.filnum = 0;
	hfat->dirlog.secnum = 0;
	CuiFatInterator_AllDir(hfat);							// ������ǰ���е��ļ���
}

// �򿪸�Ŀ¼
void CuiFatOpenBootDir(fat_Handle* hfat)
{
	hfat->dirlog.clus = hfat->rootClus;
	hfat->dirlog.filnum = 0;
	hfat->dirlog.secnum = 0;
	ValuPush(hfat->diskMesList,hfat->dirlog.clus);	// ����Ŀ¼���FAT��push��ջ
	CuiFatInterator_AllDir(hfat);
}


















































