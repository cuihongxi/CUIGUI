#ifndef __List4_Malloc_H
#define __List4_Malloc_H
/**
 *  2020��2��15��09:51:49
 *      ����FreeRTOS��heap_4��˼·ʵ��һ���ڴ����
 *  2020��2��16��11:22:16��
 *      ��heap_4�Ĳ�֮ͬ����
 *      ��   ���ڷ����ڴ�Ĵ�С������List4Type���͵Ĳ�ͬ��������list4Str�Ĵ�С�Ĳ�ͬ�������256���ڵ��ڴ�أ����Զ���Ϊu8���ͣ�
 *          �����64k�ڴ�أ���Ӧ�ö���Ϊu16���ͣ�4G���ڵ��ڴ�Ӧ�ö���Ϊu32���͡�
 *      ��   ������ݽṹlist4Str�еı���offset�Ǽ�¼����һ�����п飬������׵�ַ��ƫ�ơ�������ͨ������List4Type���������ڴ��˷ѡ�
 *      ��   �����������¿ռ�������ʱ���������򣬰��մ�С�����˳�����У����Ը����malloc�ڴ档
 */


#include "uhead.h"


typedef     u32     List4Type;      // ���趨�������������������

typedef struct{
    List4Type blocksize;        // ������������ʵ�ʴ�С��������ͷ��Ϣ
    List4Type offset;           // ��һ�����ÿ飬�����ʼ��ַ��ƫ��
}list4Str;


typedef struct{
    u8* startAddr;      // ���뿪ʼ��ַ
    u8* endAddr;        // ������ַ
    u8* miniAddr;       // ��¼��С�ڴ�����ʼ��ַ
    u32 sizeStr ;       // ��¼������ݽṹ�Ĵ�С��������淴������ = sizeof(list4Str)
}list4Message;


#define     DEBUG_List4_Malloc_LEVEL  0  // ���Կ���

#if   DEBUG_List4_Malloc_LEVEL >0
#include<stdio.h>
#endif

#if (DEBUG_List4_Malloc_LEVEL > 0)
#define  List4_Malloc_Log(...)         printf(__VA_ARGS__);
#else
#define List4_Malloc_Log(...)   
#endif                             

// ����һ������¸���ַ,addr�����ͱ�����list4Str* 
#define  GETSTR_NEXTADDR(addr)     ((u8*)addr + mess->sizeStr + addr->blocksize)
extern list4Message mess;

/* ���� */

void    List4_Malloc_Init(list4Message* mess,u8* mallocAddr,List4Type length);
#define __List4_Malloc_Init(mallocAddr,length)     List4_Malloc_Init(mess,mallocAddr,length)        // ��ʼ���ڴ棬����mess���ڴ���Ϣ
void*   List4_Malloc(list4Message* mess,List4Type length);                                          // �ڴ�����
void    List4_Free(list4Message* mess,void* addr);                                                  // �ڴ��ͷ�

#endif // !1
