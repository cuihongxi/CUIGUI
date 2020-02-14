/******************************************************************
2020年2月14日13:26:11
    内存分配极简方案：
    适用于不释放或者整体释放的场合

*******************************************************************/
#ifndef     __Simple_Malloc_H
#define     __Simple_Malloc_H

#include "UHEAD.H"

/*该分配方案需要从总内存池中申请一小块，所以需要一个申请大内存的函数*/
#include "Map0_Malloc.H"

#define     SIMPLE_MALLOC_FUN(a)   Mapbit0_Malloc(a) 

/**********************************************************************/



#define     DEBUG_Simple_Malloc_LEVEL  0  // 调试开关

#if   DEBUG_Simple_Malloc_LEVEL >0
#include<stdio.h>
#endif

#if (DEBUG_Simple_Malloc_LEVEL > 0)
#define  Simple_Malloc_Log(...)         printf(__VA_ARGS__);
#else
#define Simple_Malloc_Log(...)   
#endif                             


     

//该模式的内存状态
typedef struct{
	void* array;		// 数组地址
	u32 size;		    // 数组大小
	u8* addr;		    // 未分配的起始地址
}Simple_MallocStr;

void* Simple_Malloc_Init(Simple_MallocStr* mallocMesg , u32  maxlength);    // 内存申请初始化    
void* Simple_Malloc(Simple_MallocStr* mallocMesg , u32  leng_th);        	// 申请内存



#endif
