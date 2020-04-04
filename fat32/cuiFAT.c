/*
* �󲿷ֺ���Ӧ�ο�UNIX���ļ�����������UNIX���ļ���ʱ���ȽϷ��������Լ��Լ�
*/


#include "cuiFAT.h"
#define	CuiFat_Valu(parray,type)	*((type*)parray)					// FAT���С�˵�ֵ

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
	
	u8 BPB_FSInfo[2];			//�ļ�ϵͳ������Ϣ:1 �������ĵ�һ������
	u8 BPB_BkBootSec[2];		//��������������ͨ��Ϊ6
	u8 BPB_Reserved[12];		//����
	
	u8 BS_DrvNum;				//��������
	u8 BS_Reserved1;			//δ��
	u8 BS_BootSig;				//��չ������ǩ
	u8 BS_VolID[4];				//�����к�
	u8 BS_FilSysType[11];		//���
	u8 BS_FilSysID[8];			//ϵͳID
	u8 remain[420];				//��
	u8 signer[2];				//ǩ����Ӧ����55��AA 
}SDDBR_Struct;

//Ŀ¼��ṹ��
typedef struct{						
	u8 dirName[8];					//�ļ����ĵ�һ��ASCII�롣���û�б�ʹ����Ϊ00�����ʹ�ù���0xE5
	u8 fileType[3];					//�ļ�������
	u8 fileNature;					//�ļ�����
	u8 reserve;						//����Ϊ0
	u8 DIR_CrtTimeTeenth[1];		//�ļ�������ʮ��֮һ��
	u8 crtTime[2];					//�ļ�������ʱ�䣺111 (22222)���һ�� (33333)ʱ 444����Ϊ444111
	u8 crtDate[2];					//�ļ��������ڣ�111 (22222)�� (3333333)��+1980 4����4111
	u8 visitDate[2];				//�ļ����������ڣ�111 (22222)�� (3333333)��+1980 4����4111
	u8 dirClusHigh[2];				//��Ŀ¼��غŵĸ��ֽ�
	u8 writeTime[2];				//�ļ�д��ʱ��
	u8 writeDate[2];				//�ļ���д����
	u8 dirClusLow[2];				//��Ŀ¼��غŵĵ��ֽ�
	u8 fileSize[4];					//�ļ���С
}directoryStruct;

// �����ļ�Ŀ¼�������ļ������֣���Ҫfree
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
//	for(i=0;i<3;i++)	//�����չ��
//	{
//		filename[j + i] = dir->fileType[i];
//	}
	filename[j] = 0;
	return filename;
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
	filemess->filename = CreatFileName(dir);
	filemess->fileSize = CuiFat_Valu(dir->fileSize,u32);
	filemess->fileStartCube = (CuiFat_Valu(dir->dirClusHigh,u16) << 16)|(CuiFat_Valu(dir->dirClusLow,u16));
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
	hfat->sectorSize = CuiFat_Valu(dbr->bytsPerSec,u16);
	hfat->secPerClus = dbr->secPerClus;
	hfat->totalSize = (CuiFat_Valu(dbr->totalSectors,u32)*hfat->sectorSize)>>20;
	hfat->totalSector = CuiFat_Valu(dbr->totalSectors,u32);
	hfat->listFATSize = CuiFat_Valu(dbr->listFATSize,u32);
	hfat->firstFATSector = hfat->DBRSector + CuiFat_Valu(dbr->rsvdSecCnt,u16);
	hfat->firstDirSector = hfat->firstFATSector + (hfat->listFATSize * dbr->numFATs);
	hfat->rootClus = CuiFat_Valu(dbr->rootClus,u32);
	
	debug("����������С��%d byts\r\n",hfat->sectorSize);
	debug("����������%d M\r\n",hfat->totalSize);
	debug("��Ŀ¼�����ţ�%#X \r\n",hfat->firstDirSector);
	debug("FAT�������ţ�%#X \r\n",hfat->firstFATSector);
	debug("FAT��ռ����������%d\r\n",hfat->listFATSize);
	debug("һ���� %d ������\r\n",hfat->secPerClus);

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
	hfat->diskMesList.next = 0;
	hfat->diskMesList.num = 0;
/*	SingleList* pNode = &diskList;
	debug("�����̷���\r\n");
	while(SingleList_Iterator(&pNode))
	{
		debug("%s\r\n",SingeListGetnode(fat_Handle,pNode)->driveName);
	}	*/	
	
	StartReadFatMessage(hfat);

	return hfat;
}



/*******************************************************
*-��������	��CuiFat_OpenDisk
*-��������	����һ���̷������ظ�Ӳ�̵Ļ�����Ϣ
*-����		��
*-����ֵ	���򿪳ɹ����ؾ����ʧ�ܷ���0
*-��ע		��
*******************************************************/
FatErro CuiFat_OpenDisk(const char* disk)
{
	fat_Handle* hfat = 0;
	// �����̷���������̷��ظ����˳�
	SingleListNode* pNode = &diskList;	
	while(SingleList_Iterator(&pNode))
	{
		if(CampareStringIn(disk,SingeListGetnode(fat_Handle,pNode)->driveName))break;
	}
	if(pNode == 0)	
	{
		debug("�̷���Ч\r\n");
		return ERRO_DiskNameE;
	}
	hfat = SingeListGetnode(fat_Handle,pNode);
	ReadFatMessage(hfat);
	
	return FatOK;
}

// ���ļ�
FatErro CuiFat_OpenFile(const char* file)
{
	return FatOK;
}

