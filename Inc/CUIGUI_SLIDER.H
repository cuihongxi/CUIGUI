#ifndef  __CUIGUI_SLIDER_H   
#define  __CUIGUI_SLIDER_H 

#include "CUIGUI_UDATA.H"
#include "CUIGUI_BUTTON.H"

#define     DEBUG_SLIDER_LEVEL   0         

#if (DEBUG_SLIDER_LEVEL > 0)
#include "stdio.h"
#define  SLIDER_Log(...)         printf(__VA_ARGS__);
#else
#define SLIDER_Log(...)   
#endif 

typedef struct Slider_struct
{
      u16 x ;                                   //起点坐标x
      u16 y ;                                   //起点坐标y
      u16 width;                                //宽度
      u16 height ;                              //高度
      COLOR_DATTYPE BKcolor;                    //滑动条背景色
      COLOR_DATTYPE blockcolor;                 //滑动块背景色
      u32 max_dat;                              //最大值
      u32 min_dat;                              //最小值      
      void(*blockSkinFun)(struct Slider_struct*,u16,u16);    //调节按键的皮肤
      void(*SliderSkinFun)(struct Slider_struct*); //换肤,需要根据情况重写,也可以选择默认              
      void (*DrawSlider)(struct Slider_struct*,u32);//val为当前值
      u8 flag_VorH ;                            //横竖放置的标志0纵向;1横向
}Slider;


typedef     u32* SLIDER_Handle;



Slider*  NewSlider(u16 xpos,u16 ypos,u16 width,u16 height,u32 max_dat,u32 min_dat,u8 flag_VorH);
void DrawSlider(Slider* slider,u32 val);
SLIDER_Handle SLIDER_CreateIndirect(const GUI_WIDGET_CREATE_INFO * pCreate, WM_HWIN hWin, int x0, int y0, WM_CALLBACK * cb);









#endif

