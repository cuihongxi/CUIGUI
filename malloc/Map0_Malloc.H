#ifndef     __Map0_Malloc_H
#define     __Map0_Malloc_H

/*
*2020��2��14��20:40:52
*   ��λΪ������������ڴ���ڴ���亯��
*
*/

#include "uhead.H"

#define     DEBUG_Malloc_LEVEL  2  // ���Կ���

#define		Malloc_MINI				4										// С�ֽڷ������С��Ԫ
#define		Malloc_MINIINDEX		31										// �����������ֽ�λ����һ��7,15,32
#define     Malloc_UNIT       (Malloc_MINI*Malloc_MINIINDEX+8)              // ���ֽڷ������С��Ԫ
#define     Malloc_MAXSIZE    150*8*Malloc_UNIT       // ��������ڴ�ռ䣬������Malloc_UNIT*8��������


#if   DEBUG_Malloc_LEVEL >0
#include<stdio.h>
#endif

#if (DEBUG_Malloc_LEVEL > 0)
#define  Malloc_Log(...)         printf(__VA_ARGS__);
#else
#define Malloc_Log(...)   
#endif                             


// С�ֽ��ڴ�����ݽṹ
typedef struct{
u8 dat[Malloc_MINI*Malloc_MINIINDEX];		// ������
u8* pNext;									// ָ����һ��С�ֽڳص�ַ
u32 index;									// ������						
}MiniMallocStr;

void* Mapbit0_Malloc(u32  leng_th);                   // �����ڴ�
Flag_Status  FreeMap0_Malloc(void* Malloc);        // �ͷ��ڴ�



#endif
