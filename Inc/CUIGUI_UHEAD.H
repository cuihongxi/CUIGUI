/******************************************************************
*�ļ���:����ͷ�ļ�
*������:��˶���ӿƼ���www.explorersoftware.taobao.com
*����ʱ��:2017��6��28��08:22:01
*�ļ�˵��:��������ʹ�ú�������ͷ�ļ�
*/

#ifndef  __CUIGUI_UHEAD_H  
#define  __CUIGUI_UHEAD_H 

#include "CUIGUI_UDATA.H"
#include "CUIGUI_LCDDRIVER.H"


#define BKCOLLOR             CUIGUI_Color(240,240,240)      //����
#define BKEDGE0COLLOR        CUIGUI_Color(160,160,160)      //����
#define BKEDGE1COLLOR        CUIGUI_Color(227,227,227)      //����
#define BKEDGE2COLLOR        CUIGUI_Color(105,105,105)      //����
#define BKEDGE3COLLOR        WHITE                          //����



/*********************************************************
    ����
*/

      
u32 GetCircle_X(u32 x0,u32 y0,u16 r,u16 y1);//��������Ͱ뾶��������y1���Ӧ��x1����
    
void Obj_SkinUnPressed(Obj* obj); 
void Obj_SkinPressed(Obj* obj);

#endif