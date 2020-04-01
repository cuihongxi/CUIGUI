#ifndef  __CUIGUI_LCDDRIVER_H   
#define  __CUIGUI_LCDDRIVER_H 

#include "uhead.h"
#include "CUIGUI_COLOR.H"
#include "GUI.H"

#define     PIXEL_H             480// �������ص�

#define     CUIGUI_GetHpix()    PIXEL_H     // ��ȡLCD�������� 
#define     CUIGUI_GetVpix()    PIXEL_V     //��ȡLCD�������� 

/*********************************************************

    ����
*/
void            CUIGUI_Init(COLOR_DATTYPE backColor);                                      //��ʼ��
void            CUIGUI_DrawPoint(u16 x, u16 y, COLOR_DATTYPE color);    //����
COLOR_DATTYPE   CUIGUI_ReadPoint(u16 x, u16 y);                         //����
void            CUIGUI_DrawHLine(u16 x0,u16 y,u16 x1,COLOR_DATTYPE color); 
void            CUIGUI_DrawVLine(u16 x,u16 y0,u16 y1,COLOR_DATTYPE color);
void            CUIGUI_FillRect(u16 x0, u16 y0, u16 width, u16 height,COLOR_DATTYPE color);

void CUIGUI_XORPoint(u16 x, u16 y);

u32 GetXORColor(u32 color);         //���������ɫ
void CUIGUI_Draw_Circle(u16 x0,u16 y0,u8 r,COLOR_DATTYPE color);  //��Բ
void CUIGUI_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2,COLOR_DATTYPE color);
void DrawFullCircle(u16 x0,u16 y0,u16 r,u16 color);               //��һ��ʵ��Բ
void CUIGUI_FastXOR(u32 numColor,u32 colordat);

#define     FAST_DRAW         1                                   //���ٻ���,565��ʽ���ٻ���
#if FAST_DRAW > 0             
void CUIGUI_FastStart(void);
void CUIGUI_FastDrawPoint(COLOR_DATTYPE color);
void CUIGUI_SetPosition(u16 x, u16 y);
#endif
#endif