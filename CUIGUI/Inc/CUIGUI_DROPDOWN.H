/******************************************************************
*文件名:下拉列表控件
*创建者:探长科技，www.explorersoftware.taobao.com
*创建时间:2017年9月4日10:24:52
*文件说明:基于CUIGUI
*/

#ifndef  __CUIGUI_DROPDOWN_H  
#define  __CUIGUI_DROPDOWN_H 

#include "CUIGUI_UDATA.H"
#include "CUIGUI_CONFIG.H"
#include "CUIGUI_BUTTON.H"

typedef struct 
{
      u16 x ;                         //起点坐标x
      u16 y ;                         //起点坐标y
      u16 width ;                     //宽度
      u16 height ;                    //高度
      COLOR_DATTYPE BKcolor;          //未按下时的背景色
      COLOR_DATTYPE BKcolor_on;       //按下时的背景色
      COLOR_DATTYPE fontColor;        //未按下时的字体颜色
      const char* str;                //字符串
      u8 ispressed;                   //按下1，没按下0
      MSTYPE buttonEvent;             //按键事件
      const char* bmproot;
      u8  chamfer;                    //倒角度
      GUI_CONST_STORAGE GUI_FONT *Font;//字体
      
}DropDown;









#endif


