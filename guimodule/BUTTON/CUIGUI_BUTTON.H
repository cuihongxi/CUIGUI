/******************************************************************
*�ļ���:BUTTON�ؼ�
*������:�����в�˶���ӿƼ�
*����ʱ��:2017��11��5��09:47:36
*�ļ�˵��:����CUIGUI�����ⰴ����ɫ����
* �汾1.01 ��������:

      �޸����� BUTTON�Ļ�������
      �������£��趨���壬�趨������ɫ�����ػ�����
*ʹ��Ӧ��ע��: 
1: ʹ��BMPͼʱӦ�ô򿪿��� BT_USE_BMP
2: BUTTON������ʱӦ���ͷ��ڴ�
3: ͨ��ָ���˺���������,����ΪĬ��Ƥ�� button->BtSkinFun(Button* button)

����: 
1:
        Button* bt1 = NewButton(50,50,100,60);
        bt1->DrawButton(bt1);      
*/

#ifndef  __CUIGUI_BUTTON_H   
#define  __CUIGUI_BUTTON_H

#include "CUIGUI_UHEAD.H"


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
    COLOR_DATTYPE BKcolor_unpressed;    		//δ����ʱ�ı���ɫ
    COLOR_DATTYPE BKcolor_pressed;      		//����ʱ�ı���ɫ  
    COLOR_DATTYPE fontColor;            		//������ɫ      
    const GUI_FONT* pFont;              		//����
    u8 ispressed;                       		//����1��û����0   
    const char* str;                    		//�ַ���  
}Button;


#define     PRESSED           1
#define     UNPRESSED         0
#define     BT_PRESSED        1   //����
#define     BT_UNPRESSED      0   //û����



/*********************************************************
    ����    
*/
 Button*  NewButton(u16 xpos,u16 ypos,u16 width,u16 height);    //�½�һ������
void ButtonSetColor(Button* button,COLOR_DATTYPE fontColor);    //���ð�����ɫ
void ButtonSetFont(Button* button, const GUI_FONT* pFont);      //��������

#endif
