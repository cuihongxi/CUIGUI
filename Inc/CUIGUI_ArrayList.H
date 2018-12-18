#ifndef  __CUIGUI_ARRAYLIST_H   
#define  __CUIGUI_ARRAYLIST_H

#include "CUIGUI_UDATA.H"

#define    ARRAY_LIST_LENGTH       50   //定义一个数组的长度


typedef struct ArrayList_struct
{
     u32 List[ARRAY_LIST_LENGTH];         //存放地址
     u32 size;                            //数组内的个数
     void(*add)(struct ArrayList_struct*,u8*);                     //增加一个
     void(*remove)(struct ArrayList_struct*,u32);                  //移除一个
}ArrayList;


ArrayList* NewArrayList(void);

#endif

