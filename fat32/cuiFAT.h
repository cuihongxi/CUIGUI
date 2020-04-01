/*
	���ã���ָ���ļ����г���д�����ݣ��ļ�����ǰ���õģ����Բ�Ϊ��
	
	ʹ�÷�����
	�ٵ���Get_SD_valueINFO();��ȡSD������
	�ڵ���File_WriteSTR�����ļ�������������ָ�롱����
	
	˵����
		������ǰҪextern SD_valueINFO_Struct xdata SD_Dat;
		���е������С����С�ڵ���512
		û�ж���Ŀ¼
		������չ�������ж�
*/

#ifndef __CUIFAT__H
#define __CUIFAT__H

#include "uhead.h"
#include "cuiFat_config.h"


#if (DEBUG_LEVEL > 0)
#include "stdio.h"
#define  DEBUG_Log(...) printf(__VA_ARGS__);
#else
#define DEBUG_Log(...)   
#endif 

//��Ŀ¼�������ļ�������
#define ATTR_RW 					0				//��д
#define ATTR_READ_ONLY		1				//ֻ��
#define ATTR_HIDDEN				2				//����
#define ATTR_SYSTEM				4				//ϵͳ
#define ATTR_VOLUME_ID		8				//���
#define ATTR_DIRECTORY		0x10		//��Ŀ¼
#define ATTR_ARCHIVE			0x20		//�鵵
#define	ATTR_LONG_NAME		0x0f		//���ļ���Ϊ���ļ���

//FAT������һ��ֵ
#define FAT_END 0xfffffff


#define  get_valu(x) getvalu(x,sizeof(x))	//xΪ����ָ��


// ����ṹ��洢MBR��������¼,
// ���0�����ĵ�һ���ֽ���0XEB��˵����������DBR���ô���û��MBR����Ϣ
typedef struct{
	u8  SDguide[446];							//������
	
	/*DPT���̷�����1*/
	u8  zone1_bootbit[1];					//0x00����������������־��0x80������
	u8  zone1_CHSstart[3];				//������ʼCHS�ĵ�ַ
	u8  zone1_type[1];						//�������ͣ�0x0b ��FAT32
	u8  zone1_CHSend[3];					//��������CHS�ĵ�ַ
	u8  zone1_DPTFirstSector[4]; 	//�����ĵ�һ������,DBR�Ŀ�ʼλ��
	u8  zone1_DPTsizeof[4]; 			//�����Ĵ�С
	/*DPT���̷�����2*/
	u8  zone2_bootbit[1];					//0x00����������������־��0x80������
	u8  zone2_CHSstart[3];				//������ʼCHS�ĵ�ַ
	u8  zone2_type[1];						//��������
	u8  zone2_CHSend[3];					//��������CHS�ĵ�ַ
	u8  zone2_DPTFirstSector[4]; 	//�����ĵ�һ������
	u8  zone2_DPTsizeof[4]; 			//�����Ĵ�С
	/*DPT���̷�����3*/
	u8  zone3_bootbit[1];					//0x00����������������־��0x80������
	u8  zone3_CHSstart[3];				//������ʼCHS�ĵ�ַ
	u8  zone3_type[1];						//��������
	u8  zone3_CHSend[3];					//��������CHS�ĵ�ַ
	u8  zone3_DPTFirstSector[4]; 	//�����ĵ�һ������
	u8  zone3_DPTsizeof[4]; 			//�����Ĵ�С
	/*DPT���̷�����4*/
	u8  zone4_bootbit[1];					//0x00����������������־��0x80������
	u8  zone4_CHSstart[3];				//������ʼCHS�ĵ�ַ
	u8  zone4_type[1];						//��������
	u8  zone4_CHSend[3];					//��������CHS�ĵ�ַ
	u8  zone4_DPTFirstSector[4]; 	//�����ĵ�һ������
	u8  zone4_DPTsizeof[4]; 			//�����Ĵ�С
	u8  signer[2];								//ǩ����Ӧ����55��AA
}SDMBR_Struct;

//����ṹ��洢DBR������
typedef struct{

	u8 SDguide[3];					//��תָ��BS_jmpBoot
	u8 BS_OEMName[8];				//�ļ�ϵͳ�Ͱ汾��
	u8 BPB_BytsPerSec[2];		//ÿ�������ֽ���
	u8 BPB_SecPerClus[1];		//ÿ���ص�������
	u8 BPB_RsvdSecCnt[2];		//����������
	
	u8 BPB_NumFATs[1];			//FAT����� =2
	u8 BPB_RootEntCnt[2];		//��Ŀ¼������ɵ�Ŀ¼������FAT32��0
	u8 BPB_TotSec16[2];			//FAT16��������
	u8 BPB_Media[1];				//���ʣ�Ӳ��ΪF8��F0Ϊ���ƶ�
	u8 BPB_FATSz16[2];			//FAT16��һ��FAT����������FAT32��0
	u8 BPB_SecPerTrk[2];		//�ŵ���������FAT32��0
	u8 BPB_NumHeads[2];			//��ͷ��
	u8 BPB_HiddSec[4];			//����ǰ�Ѿ�ʹ�õ�������������MBR��
	
	u8 BPB_TotSec32[4];			//FAT32��������
/*************************����FAT12/FAT16������FAT32*************************************/
	u8 BPB_FATSz32[4];			//һ��FAT32��ռ��������
	u8 BPB_ExtFlags[2];			//���
	u8 BPB_FSVer[2];				//�汾��
	u8 BPB_RootClus[4];			//��Ŀ¼���ڵĴغ�
	
	u8 BPB_FSInfo[2];				//�ļ�ϵͳ������Ϣ:1 �������ĵ�һ������
	u8 BPB_BkBootSec[2];		//��������������ͨ��Ϊ6
	u8 BPB_Reserved[12];		//����
	
	u8 BS_DrvNum[1];				//��������
	u8 BS_Reserved1[1];			//δ��
	u8 BS_BootSig[1];				//��չ������ǩ
	u8 BS_VolID[4];					//�����к�
	u8 BS_FilSysType[11];		//���
	u8 BS_FilSysID[8];			//ϵͳID
	u8 remain[420];				//��
	u8 signer[2];				//ǩ����Ӧ����55��AA 
}SDDBR_Struct;


// FSINFO�������ļ�ϵͳ������Ϣ���ṩ�մ���������һ���ص���Ϣ,����������

typedef struct{

	u8 SDguide[4];				//��չ������ʶ52526141 FSI_LeadSig
	u8 undef0[480];				//δ������
	u8 FSINFOsig[4];			//ǩ��72724161�������������Ѿ���ռ��
	u8 FSI_Free_Count[4];	//���д���
	u8 FSI_Nxt_free[4];		//�¸������õĴغţ������ֵΪ0XFFFFFFFF��Ҫ�ӵ�2�ؿ�ʼ����
	u8 undef1[14];				//δ������
	u8 signer[2];					//ǩ����Ӧ����55��AA 
}SDFSINFO_Struct;


//����Ŀ¼��ṹ��
typedef struct{							//Сд��ĸ�����������DIRNAME��
	u8 DirName0[1];						//�ļ����ĵ�һ��ASCII�롣���û�б�ʹ����Ϊ00�����ʹ�ù���0xE5
	u8 DirName1to7[7];				//�ļ����ĵ�2~8��ASCII��
	u8 DirName8to10[3];				//�ļ�������
	u8 DIR_Attr[1];						//�ļ�����
	u8 DIR_NTRes[1];					//����Ϊ0
	u8 DIR_CrtTimeTeenth[1];	//�ļ�������ʮ��֮һ��
	u8 DIR_CrtTime[2];				//�ļ�������ʱ�䣺111 (22222)���һ�� (33333)ʱ 444����Ϊ444111
	u8 DIR_CrtDate[2];				//�ļ��������ڣ�111 (22222)�� (3333333)��+1980 4����4111
	u8 DIR_LastAccDate[2];		//�ļ����������ڣ�111 (22222)�� (3333333)��+1980 4����4111
	u8 DIR_FstClusHI[2];			//��Ŀ¼��غŵĸ��ֽ�
	u8 DIR_WrtTime[2];				//�ļ�д��ʱ��
	u8 DIR_WrtDate[2];				//�ļ���д����
	u8 DIR_FstClusLO[2];			//��Ŀ¼��غŵĵ��ֽ�
	u8 DIR_FileSize[4];				//�ļ���С
}SD_shortDirName_Struct;

//Ŀ¼��ṹ��
typedef struct{
    SD_shortDirName_Struct Menu[16];
}MenuStruct;


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

//���õĲ����ṹ��
typedef struct{
	
	u32    	FirstFATSector;			    //��һ��FAT��������ţ�zone1_DPTFirstSector + BPB_RsvdSecCnt
	u32    	FATSz32;								//һ��FAT32��ռ��������
	u32    	FirstDirSector;			    //��Ŀ¼�������ţ�FirstFATSector +2*BPB_FATSz32
	u8   		FSInfo;					    		//FSINFO��������
  u32  		FSINFO_secoter;         //FSINFO�ļ�¼�����ʹ�õ�������
	u8   		SecPerClus;				    	//ÿ���ص�������
	u32    	ClusBase;								//0�����ڵ�������
	u8    	FirstDirCuNum;					//��Ŀ¼���ڵĴغ�
	
}SD_valueINFO_Struct;

//����

//void u32tostr(u32 dat,u8 *str);	

void Get_SD_valueINFO(void);												    //���SD�����õ���Ϣ
void File_Write(u8* filename,u8* str,u32 lenth);	//���ļ�filename��д����str

#define  File_WriteSTR(filename,str)    File_Write(filename,str,(u8)sizeof(str))	




#endif
