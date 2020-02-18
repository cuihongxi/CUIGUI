#ifndef __List4_Malloc_H
#define __List4_Malloc_H
/**
 *  2020年2月15日09:51:49
 *      依据FreeRTOS的heap_4的思路实现一个内存分配
 *  2020年2月16日11:22:16：
 *      与heap_4的不同之处：
 *      ①   在于分配内存的大小，定义List4Type类型的不同，决定了list4Str的大小的不同。如果是256以内的内存池，可以定义为u8类型，
 *          如果是64k内存池，则应该定义为u16类型，4G以内的内存应该定义为u32类型。
 *      ②   块的数据结构list4Str中的变量offset是记录了下一个空闲块，相对于首地址的偏移。这样就通过定义List4Type大大减少了内存浪费。
 *      ③   链表在生成新空间插入操作时进行了排序，按照从小到大的顺序排列，可以更快的malloc内存。
 *      ④   没有内存对齐
 *  2020年2月16日17:28:11
 *      功能完成，进入测试阶段
 *  2020年2月16日21:32:38
 *      测试正常 V1.0
 * 
 */


#include "uhead.h"


typedef     u16                 List4Type;      // 该设定决定最大可以申请的数量
#define     MAX_MALLOCSIZE      50*1024         // 最大的内存数            


typedef struct{
    List4Type blocksize;        // 本块可以申请的实际大小，不包含头信息
    List4Type offset;           // 下一个可用块，相对起始地址的偏移
}list4Str;


typedef struct{
    u8* startAddr;      // 申请开始地址
    u8* endAddr;        // 结束地址
    u8* miniAddr;       // 记录最小内存块的起始地址
    u32 sizeStr ;       // 记录这个数据结构的大小，以免后面反复计算 = sizeof(list4Str)
}list4Message;


#define     DEBUG_List4_Malloc_LEVEL  1  // 调试开关

#if   DEBUG_List4_Malloc_LEVEL >0
#include<stdio.h>
#endif

#if (DEBUG_List4_Malloc_LEVEL > 0)
#define  List4_Malloc_Log(...)         printf(__VA_ARGS__);
#else
#define List4_Malloc_Log(...)   
#endif                             

// 返回一个块的下个地址,addr的类型必须是list4Str* 
#define  GETSTR_NEXTADDR(addr)     ((u8*)addr + mess->sizeStr + addr->blocksize)
extern list4Message list4mess;
extern u8  list4mallocArray[MAX_MALLOCSIZE];
/* 函数 */

void    List4_Malloc_Init(list4Message* mess,u8* mallocAddr,List4Type length);
#define __List4_Malloc_Init()     List4_Malloc_Init(&list4mess,list4mallocArray,MAX_MALLOCSIZE)   // 初始化内存，其中mess是内存信息
void*   List4_Malloc(list4Message* mess,List4Type length);                                  // 内存申请
void    List4_Free(list4Message* mess,void* addr);                                          // 内存释放

#endif // !1

