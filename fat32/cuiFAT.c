/*
* 大部分函数应参考UNIX的文件操作，但是UNIX的文件有时候会比较繁琐，所以加以简化
* 操作步骤：
* 	① 调用disk文件中的初始化函数，获得disk_Handle
* 	② 调用CuiFat_BindingDisk ，绑定盘符，并获得fat_Handle
*
*/


#include "cuiFAT.h"
#include "SingleList.h"

SingleList diskList = {0};		// 获得一个记录盘符的链表

#define	CuiFat_Valu(parray,type)	*((type*)parray)					// FAT获得小端的值



//比较两个字符串是否相等
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
*-函数名称	：CuiFat_BindingDisk
*-函数作用	：将盘符跟硬件进行绑定
*-参数		：name 盘符字符串；dishandle 硬盘的句柄
*-返回值	：
*-备注		：调用该函数，将盘符与硬件绑定后压到链表中；当盘符重复则失败
*******************************************************/
FatErro CuiFat_BindingDisk(const char* driveName,disk_Handle* dishandle)
{
	// 遍历盘符链表，如果盘符重复则退出
	SingleListNode* pNode = &diskList;	
	while(SingleList_Iterator(&pNode))
	{
		if(CampareString(SingeListGetnode(fat_Handle,pNode)->driveName,driveName))
		{
			debug("盘符已经存在");
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
*-函数名称	：CuiFAT_Open
*-函数作用	：打开一个文件，如果没有则创建它
*-参数		：
*-返回值	：
*-备注		：
*******************************************************/
FatErro CuiFAT_Open(const char* path)
{

}
 
