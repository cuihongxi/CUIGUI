/******************************************************************
*�ļ���:BUTTON�ؼ�
    
*/

#ifndef  __CUIGUI_BUTTON_BMP_H   
#define  __CUIGUI_BUTTON_BMP_H

#include "CUIGUI_UHEAD.H"
#include "CUIGUI_BUTTON.H"


//BMP����
typedef struct ButtonBMP_str
{
    Button button;
    const char* bmproot;                //BMPͼ��·��   
    void(*SetBmpRoot)(struct ButtonBMP_str*,const char*);
}ButtonBMP;


 
/*********************************************************
    ����    
*/

 ButtonBMP*  NewButtonBMP(u16 xpos,u16 ypos,u16 width,u16 height,const char* bmproot);//�½�һ��BMPͼ�İ���


#endif
