/******************************************************************
*文件名:公共头文件
*创建时间:2020年2月14日13:42:58
*/

#ifndef  _UHEAD_H  
#define  _UHEAD_H 


#include "main.h"
     

typedef unsigned short	        u16;
typedef unsigned int	        u32;
typedef unsigned char           u8;

typedef __IO u16    vu16;
typedef __IO u32    vu32;
typedef __IO u8     vu8;

typedef enum {ISOK = 0, ISERROR = -1} Flag_Status;

#define malloc(length)      List4_Malloc(&list4mess,length)
#define free(a)             List4_Free(&list4mess,a)



#define     DEBUG_LEVEL  1  // 调试开关

#if   DEBUG_LEVEL >0
#include<stdio.h>
#endif

#if (DEBUG_LEVEL > 0)
#define  debug(...)         printf(__VA_ARGS__);
#else
#define debug(...)   
#endif    


#define     delay_ms(t)         HAL_Delay(t)
#define     delay_us(t)         do{u8 i = t;while(i--);}while(0)

#define     GPIO_SET(pin)       HAL_GPIO_WritePin(pin,GPIO_PIN_SET)
#define     GPIO_RESET(pin)     HAL_GPIO_WritePin(pin,GPIO_PIN_RESET)


#endif
