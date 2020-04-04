/*
ע��û���̷��ĸ��ֱ�Ӳ���hfat���
	
	Ӧ�ÿ��Ը�ʽ��һ���̷�
*/

/*
*	����Ӳ���ӿ���disk�ļ���
*	ʹ�ò��裺
	�� ����Ӳ����ͬ���������ã������Ӳ����� disk_Handle
	�� ʹ��	hfat = CuiFat_BindingDisk(disk_Handle);	��Ӳ�������hfat���
	
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

//��Ŀ¼�������ļ�������
typedef enum{
	ATTR_RW = 0,								// ��д
	ATTR_READ_ONLY = 1,							// ֻ��
	ATTR_HIDDEN = 2,							// ����
	ATTR_SYSTEM	= 4,							// ϵͳ
	ATTR_VOLUME_ID = 8,							// ���
	ATTR_DIRECTORY = 0x10,						// �ļ���
	ATTR_ARCHIVE = 0x20,						// �鵵���ļ�
	ATTR_LONG_NAME = 0x0f,						// ���ļ���Ϊ���ļ���
}fileNature;

#define FAT_END 				0xfffffff		// FAT������һ��ֵ

/*fat32���ݵĺ�*/
#define	DPT_ACTIVE				0x80			// �÷����Ƿ���Ч
#define	SIGNATURE				0xAA55			// ����ǩ��
#define	DBR_IS					0XEB			// DBR������־
#define	FAT32_ID				0x0B			// FAT32�ļ�ϵͳ��ʶ



//#define  get_valu(x) getvalu(x,sizeof(x))		//xΪ����ָ��

// FSINFO�������ļ�ϵͳ������Ϣ���ṩ�մ���������һ���ص���Ϣ,����������
typedef struct{

	u8 SDguide[4];				//��չ������ʶ52526141 FSI_LeadSig
	u8 undef0[480];				//δ������
	u8 FSINFOsig[4];			//ǩ��72724161�������������Ѿ���ռ��
	u8 FSI_Free_Count[4];		//���д���
	u8 FSI_Nxt_free[4];			//�¸������õĴغţ������ֵΪ0XFFFFFFFF��Ҫ�ӵ�2�ؿ�ʼ����
	u8 undef1[14];				//δ������
	u8 signer[2];				//ǩ����Ӧ����0xAA55 
}SDFSINFO_Struct;



//FAT�����ݽṹ
typedef struct{

    u8 FATListArray[128][4];
}FATListStruct;
//�ļ���Ϣ
typedef struct{
    u32 CuID;
    u32 FileSize;
    u32 i;							//��¼�ڼ�����¼
}FileInforStruct;


typedef enum{
	FatOK = 0,
	ERRO_DBTNotActive	= 0x01, 		// ������Ч
	ERRO_NotFat32 		= 0x02,			// ����Fat32�ļ�ϵͳ
	
}FatErro;

// �ļ��Ĵ����ͷ���ʱ��
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
	u32 dirSector;		// Ŀ¼�����ڵ�����	
	u8 	num;			// ���ڵ���Ŀ
	
}dirOneMes;

// �ļ���Ϣ��
typedef struct{
//	dirOneMes 	mes;			
	u32 		fileStartClus;		// �ļ���Ӧ����ʼ��
	u32 		fileSize;			// �ļ���С
	u8 			filename[9];			// �ļ���
	fileNature 	nature;				// �ļ�����
	dirDateTime	time;				// �ļ���ʱ��
	
//	u32 		preClus;			// ��ǰ��
//	u32 		preSector;			// ��ǰ����
//	u32			haseread;			// �Ѷ����ļ�����
//	u16 		preByte;			// �����е�ƫ��	
}fileMessage;


typedef struct{
	u32 clus;		// FAT����
	u16	filnum;		// Ŀ¼��
	u8 	secnum;		// һ�����ڵ��������
}dirlislog;

#define	dirSecList		SingleList		// ����Ŀ¼��Ϣ������������Ŀ¼��dirOneMes

// fat�ļ�ϵͳ�ľ��
typedef struct
{ 
	FatErro  	ErrorCode;					  	
	void		(*DiskInit)(void);										// Ӳ�̳�ʼ������
	u32			(*readBlocks)(u32 sector,u8* pbuff,u32 numberOfBlocks);	// ����������
	u32			(*writeBlocks)(u32 sector,u8* pbuff,u32 numberOfBlocks);// д��������
	
	u8*			buf;					// ���ݻ�����ָ��	
	u32			DBRSector;				// DBR������
	u32 		totalSector;			// ��������
	u32			listFATSize;			// һ��FAT��ռ��������
	u32			totalSize;				// ��λ��M�ֽ�
	u32    		firstFATSector;			// ��һ��FAT��������ţ�DBRSector + BPB_RsvdSecCnt
	u32    		SecondFATSector;		// ��2��FAT���������
	u32 		rootClus;				// ��Ŀ¼���ڵĴغ�
	u32    		rootSector;				// ��Ŀ¼�����ţ�FirstFATSector +dbr->numFATs*BPB_FATSz32.
	u16			dirperSector;			// һ���������Ա�������Ŀ¼����
	u8   		secPerClus;				// ÿ���ص�������
	u16 		sectorSize;             // ������С	
	u16			FSInfo;					// FSINFO�������ţ��ļ�ϵͳ��Ϣ������
	u16			numClusperSector;		// FAT��һ�������ܱ��漸����
	dirlislog	dirlog;					// ���浱ǰ������Ŀ¼��
	fileMessage filemess;				// ��ǰ��Ŀ¼����Ϣ
	
	
	dirSecList 	diskMesList;			// Ŀ¼�ṹ��������·����Ϣ
}fat_Handle;



/*����*/

fat_Handle* CuiFat_BindingDisk(disk_Handle* dishandle);		// ���̷���Ӳ�����а�
void CuiFat_OpenDisk(fat_Handle* hfat);						// ���̷�

#endif
