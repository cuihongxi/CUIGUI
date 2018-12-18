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
      u16 x ;                                   //�������x
      u16 y ;                                   //�������y
      u16 width;                                //����
      u16 height ;                              //�߶�
      COLOR_DATTYPE BKcolor;                    //����������ɫ
      COLOR_DATTYPE blockcolor;                 //�����鱳��ɫ
      u32 max_dat;                              //���ֵ
      u32 min_dat;                              //��Сֵ      
      void(*blockSkinFun)(struct Slider_struct*,u16,u16);    //���ڰ�����Ƥ��
      void(*SliderSkinFun)(struct Slider_struct*); //����,��Ҫ���������д,Ҳ����ѡ��Ĭ��              
      void (*DrawSlider)(struct Slider_struct*,u32);//valΪ��ǰֵ
      u8 flag_VorH ;                            //�������õı�־0����;1����
}Slider;


typedef     u32* SLIDER_Handle;



Slider*  NewSlider(u16 xpos,u16 ypos,u16 width,u16 height,u32 max_dat,u32 min_dat,u8 flag_VorH);
void DrawSlider(Slider* slider,u32 val);
SLIDER_Handle SLIDER_CreateIndirect(const GUI_WIDGET_CREATE_INFO * pCreate, WM_HWIN hWin, int x0, int y0, WM_CALLBACK * cb);









#endif
