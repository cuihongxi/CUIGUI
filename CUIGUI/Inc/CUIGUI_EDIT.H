/******************************************************************
*文件名:编辑框控件
*创建者:探长科技，www.explorersoftware.taobao.com
*创建时间:2017年8月25日19:20:14
*文件说明:基于CUIGUI
*/
#ifndef  __CUIGUI_EDIT_H   
#define  __CUIGUI_EDIT_H 

#include "CUIGUI_UDATA.H"


#define     DEBUG_EDIT 0      //调试开关

#if DEBUG_EDIT >0     
#include "usart.h"
#endif
         
typedef u32*     EDIT_Handle;

//光标坐标
typedef	struct 
{
	u16 x;
	u16 y;
}cursorXY;



typedef struct Edit_struct
{
      u16 x ;                                   //起点坐标x
      u16 y ;                                   //起点坐标y
      u16 width ;                               //宽度
      u16 height ;                              //高度
      COLOR_DATTYPE BKcolor;                    //背景色
      COLOR_DATTYPE FontColor;                  //字体颜色
      u8* str;                                  //字符串      
      const GUI_FONT* Font;         //字体
      u8 alig_mode;                             //显示对齐方式
      GUIFlagStatus isShowCursor;                 //是否显示光标|:1显示，0 不显示
      void(*EditSkinFun)(struct Edit_struct*);  //换肤,需要根据情况重写,也可以选择默认

}Edit;


/*函数*/

Edit*  NewEdit(void);
void  CUIGUI_DrawEdit(Edit* edit);

//重写图形BUILDER函数
void EDIT_SetText(EDIT_Handle hObj,const char* str);
void EDIT_SetTextAlign(EDIT_Handle hObj,u8 method);      //设置文本显示位置方式
void EDIT_SetFont(EDIT_Handle hObj,const GUI_FONT GUI_FontNum);
EDIT_Handle EDIT_CreateIndirect(const GUI_WIDGET_CREATE_INFO * pCreate, WM_HWIN hWin, int x0, int y0, WM_CALLBACK * cb);
void EDIT_SetTextAlign(EDIT_Handle hObj,u8 mode);     //设置文本对齐方式


#endif
