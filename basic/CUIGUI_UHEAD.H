#ifndef __CUIGUI_UHEAD_H
#define __CUIGUI_UHEAD_H
/**
*   ���GUI���е������ͷ�ļ�
*/

#include "uhead.h"
#include "CUIGUI_COLOR.H"
#include "CUIGUI_LCDDRIVER.H"
#include "GUI.H"
#include "List4_Malloc.H"



typedef struct Obj_struct
{
    u16 x ;                 //�������x
    u16 y ;                 //�������y
    u16 width ;             //����
    u16 height ;            //�߶�
    void (*Draw)(void*);	//�����ÿؼ�
}Obj;

#endif // !__CUIGUI_UHEAD_H
