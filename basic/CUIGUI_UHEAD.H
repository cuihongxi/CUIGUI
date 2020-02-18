#ifndef __CUIGUI_UHEAD_H
#define __CUIGUI_UHEAD_H
/**
*   存放GUI运行的最基本头文件
*/

#include "uhead.h"
#include "CUIGUI_COLOR.H"
#include "CUIGUI_LCDDRIVER.H"
#include "GUI.H"
#include "List4_Malloc.H"



typedef struct Obj_struct
{
    u16 x ;                 //起点坐标x
    u16 y ;                 //起点坐标y
    u16 width ;             //宽度
    u16 height ;            //高度
    void (*Draw)(void*);	//画出该控件
}Obj;

#endif // !__CUIGUI_UHEAD_H

