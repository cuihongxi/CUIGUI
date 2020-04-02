/*
* �󲿷ֺ���Ӧ�ο�UNIX���ļ�����������UNIX���ļ���ʱ���ȽϷ��������Լ��Լ�
* �������裺
* 	�� ����disk�ļ��еĳ�ʼ�����������disk_Handle
* 	�� ����CuiFat_BindingDisk �����̷��������fat_Handle
*
*/


#include "cuiFAT.h"
#include "SingleList.h"

SingleList diskList = {0};		// ���һ����¼�̷�������

#define	CuiFat_Valu(parray,type)	*((type*)parray)					// FAT���С�˵�ֵ



//�Ƚ������ַ����Ƿ����
BOOL  CampareString(const char* str1,const char* str2)
{
	u16 i = 0;
	for(i=0;str1[i]!=0;i++)
	{
		if(str1[i]^str2[i])return FALSE;
	}
	if(str2[i] == 0) return TRUE;
	return FALSE;
}



/*******************************************************
*-��������	��CuiFat_BindingDisk
*-��������	�����̷���Ӳ�����а�
*-����		��name �̷��ַ�����dishandle Ӳ�̵ľ��
*-����ֵ	��
*-��ע		�����øú��������̷���Ӳ���󶨺�ѹ�������У����̷��ظ���ʧ��
*******************************************************/
FatErro CuiFat_BindingDisk(const char* driveName,disk_Handle* dishandle)
{
	// �����̷���������̷��ظ����˳�
	SingleListNode* pNode = &diskList;	
	while(SingleList_Iterator(&pNode))
	{
		if(CampareString(SingeListGetnode(fat_Handle,pNode)->driveName,driveName))
		{
			debug("�̷��Ѿ�����");
			return ERRO_DiskNameS;
		}
	}
	
	fat_Handle* fath = (fat_Handle*)malloc(sizeof(fat_Handle));
	fath->ErrorCode = FatOK;
	fath->blockNbr = dishandle->blockNbr;
	fath->blockSize = dishandle->blockSize;
	fath->driveName = driveName;
	fath->readBlocks = dishandle->readBlocks;
	fath->writeBlocks = dishandle->writeBlocks;
	SingleList_InsertEnd(&diskList,fath);
	return FatOK;
}

/*******************************************************
*-��������	��CuiFAT_Open
*-��������	����һ���ļ������û���򴴽���
*-����		��
*-����ֵ	��
*-��ע		��
*******************************************************/
FatErro CuiFAT_Open(const char* path)
{

}
 
