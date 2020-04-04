/*
*	Ӳ�������ӿ�
*	���ļ������ã���ͬ��Ӳ�������disk_Handle��������ȱʡ��������Ϊ�������ᱻCuiFat_BindingDisk����.
*/

#ifndef __DUSK_H
#define __DUSK_H

#include "uhead.h"



// Ӳ����Ϣ
typedef struct
{ 				  				  
	void		(*DiskInit)(void);										// Ӳ�̳�ʼ������
	u32			(*readBlocks)(u32 sector,u8* pbuff,u32 numberOfBlocks);	// ����������
	u32			(*writeBlocks)(u32 sector,u8* pbuff,u32 numberOfBlocks);// д��������
	
}disk_Handle;


/* ���� */

disk_Handle* SDDisk_Init(void);			// Ӳ�̳�ʼ������


#endif // !__DUSK_H
