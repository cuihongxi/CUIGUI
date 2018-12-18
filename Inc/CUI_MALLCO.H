/******************************************************************
*文件名:公共头文件
*创建者:探长科技，www.explorersoftware.taobao.com
*创建时间:2017年8月9日14:07:03
*文件说明:动态分配内存

*/


#ifndef     __CUI_MALLCO_H
#define     __CUI_MALLCO_H


#include "CUIGUI_UDATA.H"

#define     DEBUG_MALLCO_LEVEL  0  //调试开关



#define     MALLCO_UNIT       64                      //分配的最小单位
#define     MALLCO_MAXSIZE    50*16*MALLCO_UNIT       //分配最大内存空间，必须是MALLCO_UNIT*8的整数倍,51200字节


#if   DEBUG_MALLCO_LEVEL >0
#include<stdio.h>
#endif

#if (DEBUG_MALLCO_LEVEL > 0)
#define  MALLCO_Log(...)         printf(__VA_ARGS__);
#else
#define MALLCO_Log(...)   
#endif                             



typedef struct
{
  u8  array[MALLCO_UNIT];
}CUI_MallcoType;


u8* CUI_Mallco(u32  leng_th);                   //申请内存
Flag_Status  FreeCUI_Mallco(u8* mallco);        //释放内存
//u32 GetNumMalloc(u8* mallco);                   //获取该malloc的数量
#endif
