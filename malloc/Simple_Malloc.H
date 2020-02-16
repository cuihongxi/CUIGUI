/******************************************************************
2020��2��14��13:26:11
    �ڴ���伫�򷽰���
    �����ڲ��ͷŻ��������ͷŵĳ���

*******************************************************************/
#ifndef     __Simple_Malloc_H
#define     __Simple_Malloc_H

#include "UHEAD.H"

/*�÷��䷽����Ҫ�����ڴ��������һС�飬������Ҫһ��������ڴ�ĺ���*/
#include "Map0_Malloc.H"

#define     SIMPLE_MALLOC_FUN(a)   Mapbit0_Malloc(a) 

/**********************************************************************/



#define     DEBUG_Simple_Malloc_LEVEL  0  // ���Կ���

#if   DEBUG_Simple_Malloc_LEVEL >0
#include<stdio.h>
#endif

#if (DEBUG_Simple_Malloc_LEVEL > 0)
#define  Simple_Malloc_Log(...)         printf(__VA_ARGS__);
#else
#define Simple_Malloc_Log(...)   
#endif                             


     

//��ģʽ���ڴ�״̬
typedef struct{
	void* array;		// �����ַ
	u32 size;		    // �����С
	u8* addr;		    // δ�������ʼ��ַ
}Simple_MallocStr;

void* Simple_Malloc_Init(Simple_MallocStr* mallocMesg , u32  maxlength);    // �ڴ������ʼ��    
void* Simple_Malloc(Simple_MallocStr* mallocMesg , u32  leng_th);        	// �����ڴ�



#endif