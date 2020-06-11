#ifndef  __CUIGUI_SLIDER_H   
#define  __CUIGUI_SLIDER_H 

#include "CUIGUI_UHEAD.H"

#define     DEBUG_SLIDER_LEVEL   0         

#if (DEBUG_SLIDER_LEVEL > 0)
#include "stdio.h"
#define  SLIDER_Log(...)         printf(__VA_ARGS__);
#else
#define SLIDER_Log(...)   
#endif 

typedef enum{
	Horizontal = 0,
	Vertical,
}DIR;

typedef struct Slider_struct
{
	Obj	obj;
	COLOR_DATTYPE BKcolor;                    // 滑动条背景色
	COLOR_DATTYPE blockcolor;                 // 滑动块背景色
	u32 max_val;                              // 最大值
	u32 min_val;                              // 最小值 
	u32 val;								  // 当前值	
	void(*blockSkinFun)(struct Slider_struct*,u16,u16);    	//调节滑块的皮肤
	void(*SliderSkinFun)(struct Slider_struct*); 			// 换肤,需要根据情况重写,也可以选择默认              
	DIR flag_VorH ;                            // 横竖放置的标志0纵向;1横向
}Slider;


Slider*  NewSlider(u16 xpos,u16 ypos,u16 width,u16 height,u32 max_dat,u32 min_dat,DIR flag_VorH);


#endif

