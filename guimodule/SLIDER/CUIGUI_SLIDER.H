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
	COLOR_DATTYPE BKcolor;                    // ����������ɫ
	COLOR_DATTYPE blockcolor;                 // �����鱳��ɫ
	u32 max_val;                              // ���ֵ
	u32 min_val;                              // ��Сֵ 
	u32 val;								  // ��ǰֵ	
	void(*blockSkinFun)(struct Slider_struct*,u16,u16);    	//���ڻ����Ƥ��
	void(*SliderSkinFun)(struct Slider_struct*); 			// ����,��Ҫ���������д,Ҳ����ѡ��Ĭ��              
	DIR flag_VorH ;                            // �������õı�־0����;1����
}Slider;


Slider*  NewSlider(u16 xpos,u16 ypos,u16 width,u16 height,u32 max_dat,u32 min_dat,DIR flag_VorH);


#endif
