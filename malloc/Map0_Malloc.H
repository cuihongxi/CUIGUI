#ifndef     __Map0_Malloc_H
#define     __Map0_Malloc_H

/*
*2020年2月14日20:40:52
*   以位为索引，申请块内存的内存分配函数
*
*/

#include "uhead.H"

#define     DEBUG_Malloc_LEVEL  2  // 调试开关

#define		Malloc_MINI				4										// 小字节分配的最小单元
#define		Malloc_MINIINDEX		31										// 索引数，以字节位数减一，7,15,32
#define     Malloc_UNIT       (Malloc_MINI*Malloc_MINIINDEX+8)              // 大字节分配的最小单元
#define     Malloc_MAXSIZE    150*8*Malloc_UNIT       // 分配最大内存空间，必须是Malloc_UNIT*8的整数倍


#if   DEBUG_Malloc_LEVEL >0
#include<stdio.h>
#endif

#if (DEBUG_Malloc_LEVEL > 0)
#define  Malloc_Log(...)         printf(__VA_ARGS__);
#else
#define Malloc_Log(...)   
#endif                             


// 小字节内存池数据结构
typedef struct{
u8 dat[Malloc_MINI*Malloc_MINIINDEX];		// 数据区
u8* pNext;									// 指向下一个小字节池地址
u32 index;									// 索引表						
}MiniMallocStr;

void* Mapbit0_Malloc(u32  leng_th);                   // 申请内存
Flag_Status  FreeMap0_Malloc(void* Malloc);        // 释放内存



#endif

