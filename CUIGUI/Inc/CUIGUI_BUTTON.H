/******************************************************************
*文件名:BUTTON控件
*创建者:蓬莱市博硕电子科技
*创建时间:2017年11月5日09:47:36
*文件说明:基于CUIGUI，特殊按键颜色给出
* 版本1.01 更新内容:

      修改增加 BUTTON的换肤更能
      按键按下，设定字体，设定字体颜色都会重画按键
*使用应该注意: 
1: 使用BMP图时应该打开开关 BT_USE_BMP
2: BUTTON不再用时应该释放内存
3: 通过指定此函数来换肤,否则为默认皮肤 button->BtSkinFun(Button* button)

举例: 
1:
        Button* bt1 = NewButton(50,50,100,60);
        bt1->DrawButton(bt1);      
*/

#ifndef  __CUIGUI_BUTTON_H   
#define  __CUIGUI_BUTTON_H

#include "CUIGUI_UDATA.H"
#include "CUIGUI_COMPONENT.H"

#define     DEBUG_BUTTON_LEVEL  1         

#if (DEBUG_BUTTON_LEVEL > 0)
#include "stdio.h"
#define  BUTTON_Log(...)         printf(__VA_ARGS__);
#else
#define BUTTON_Log(...)   
#endif 

typedef struct BT_struct
{
			Obj obj;
			
      void (*DrawButton)(struct BT_struct*);	//画出按键	
      COLOR_DATTYPE BKcolor_unpressed;    		//未按下时的背景色
      COLOR_DATTYPE BKcolor_pressed;      		//按下时的背景色  
      COLOR_DATTYPE fontColor;            		//字体颜色      
      const GUI_FONT* pFont;              		//字体
      u8 ispressed;                       		//按下1，没按下0   
      const char* str;                    		//字符串  
}Button;

//BMP按键
typedef struct ButtonBMP_str
{
			Button button;
      const char* bmproot;                //BMP图形路径   
			void(*SetBmpRoot)(struct ButtonBMP_str*,const char*);
}ButtonBMP;

typedef void BTFUN(struct BT_struct*);

#define     PRESSED           1
#define     UNPRESSED         0
#define     BT_PRESSED        1   //按下
#define     BT_UNPRESSED      0   //没按下



typedef u32*     BUTTON_Handle;

 
/*********************************************************
    函数    
*/
 Button*  NewButton(u16 xpos,u16 ypos,u16 width,u16 height);           //新建一个按键
 ButtonBMP*  NewButtonBMP(u16 xpos,u16 ypos,u16 width,u16 height,const char* bmproot);//新建一个BMP图的按键

void ButtonSetColor(Button* button,COLOR_DATTYPE fontColor);//设置按键颜色
void ButtonSetFont(Button* button, const GUI_FONT* pFont);  //设置字体

/*
     以下重写图形BUILDER函数
*/

void BUTTON_SetFont(BUTTON_Handle hObj, const GUI_FONT* pFont);
BUTTON_Handle BUTTON_CreateIndirect(const GUI_WIDGET_CREATE_INFO * pCreate, WM_HWIN hWin, int x0, int y0, WM_CALLBACK * cb);
void ButtonSetPressed(WM_HWIN handle ,u8 isPressed );//按键按下


#endif

