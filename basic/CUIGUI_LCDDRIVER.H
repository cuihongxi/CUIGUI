#ifndef  __CUIGUI_LCDDRIVER_H   
#define  __CUIGUI_LCDDRIVER_H 

#include "uhead.h"
#include "CUIGUI_COLOR.H"
#include "GUI.H"

#define     PIXEL_H             480// 横向像素点

#define     CUIGUI_GetHpix()    PIXEL_H     // 获取LCD横向像素 
#define     CUIGUI_GetVpix()    PIXEL_V     //获取LCD纵向像素 

/*********************************************************

    函数
*/
void            CUIGUI_Init(COLOR_DATTYPE backColor);                                      //初始化
void            CUIGUI_DrawPoint(u16 x, u16 y, COLOR_DATTYPE color);    //画点
COLOR_DATTYPE   CUIGUI_ReadPoint(u16 x, u16 y);                         //读点
void            CUIGUI_DrawHLine(u16 x0,u16 y,u16 x1,COLOR_DATTYPE color); 
void            CUIGUI_DrawVLine(u16 x,u16 y0,u16 y1,COLOR_DATTYPE color);
void            CUIGUI_FillRect(u16 x0, u16 y0, u16 width, u16 height,COLOR_DATTYPE color);

void CUIGUI_XORPoint(u16 x, u16 y);

u32 GetXORColor(u32 color);         //返回异或颜色
void CUIGUI_Draw_Circle(u16 x0,u16 y0,u8 r,COLOR_DATTYPE color);  //画圆
void CUIGUI_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2,COLOR_DATTYPE color);
void DrawFullCircle(u16 x0,u16 y0,u16 r,u16 color);               //画一个实心圆
void CUIGUI_FastXOR(u32 numColor,u32 colordat);

#define     FAST_DRAW         1                                   //快速画点,565格式快速画点
#if FAST_DRAW > 0             
void CUIGUI_FastStart(void);
void CUIGUI_FastDrawPoint(COLOR_DATTYPE color);
void CUIGUI_SetPosition(u16 x, u16 y);
#endif
#endif
