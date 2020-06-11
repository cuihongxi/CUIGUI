/******************************************************************
*文件名:BUTTON控件
    
*/

#ifndef  __CUIGUI_BUTTON_BMP_H   
#define  __CUIGUI_BUTTON_BMP_H

#include "CUIGUI_UHEAD.H"
#include "CUIGUI_BUTTON.H"


//BMP按键
typedef struct ButtonBMP_str
{
    Button button;
    const char* bmproot;                //BMP图形路径   
    void(*SetBmpRoot)(struct ButtonBMP_str*,const char*);
}ButtonBMP;


 
/*********************************************************
    函数    
*/

 ButtonBMP*  NewButtonBMP(u16 xpos,u16 ypos,u16 width,u16 height,const char* bmproot);//新建一个BMP图的按键


#endif

