/******************************************************************
*文件名:文本控件
*创建者:蓬莱市博硕电子科技
*创建时间:2017年11月5日19:40:16
*文件说明:基于CUIGUI
*/

#ifndef  __CUIGUI_TEXT_H   
#define  __CUIGUI_TEXT_H 

#include "CUIGUI_UDATA.H"

typedef u32*     TEXT_Handle;

//typedef struct 
//{
//      u16 x ;                         //起点坐标x
//      u16 y ;                         //起点坐标y
//      COLOR_DATTYPE color;            //画笔颜色
//      const char* str;                //字符串;
//}Text;

typedef struct Text_struct
      {
            u16 x ;                                   //起点坐标x
            u16 y ;                                   //起点坐标y
            u16 width ;                               //宽度
            u16 height ;                              //高度
            COLOR_DATTYPE BKcolor;                    //背景色
            COLOR_DATTYPE FontColor;                  //字体颜色
            u8* str;                                  //字符串      
            const GUI_FONT* Font;                     //字体
            u8 alig_mode;                             //显示对齐方式
            //GUIFlagStatus isShowCursor;             //EDIT中才有光标
            void(*TextSkinFun)(struct Text_struct*);  //换肤,需要根据情况重写,也可以选择默认
            void(*DrawText)(struct Text_struct*);
      }Text;
 
void TextSkin(Text* text);
void TextShowTXT(Text* text);
Text*  NewText(u16 xpos,u16 ypos,u16 width,u16 height);  
void CUIGUI_DrawText(Text* text);
void Text_SetText(WM_HWIN hObj,const char* str);
void TEXT_SetTextAlign(WM_HWIN hItem, u8 methodR);
      
//重写图形BUILDER函数
void  TEXT_SetFont(WM_HWIN hItem,const GUI_FONT GUI_FontNum);
void  TEXT_SetTextColor(WM_HWIN hItem, COLOR_DATTYPE color);
WM_HWIN TEXT_CreateIndirect(const GUI_WIDGET_CREATE_INFO * pCreate, WM_HWIN hWin, int x0, int y0, WM_CALLBACK * cb);


















#endif
