/******************************************************************
*�ļ���:�����б��ؼ�
*������:̽���Ƽ���www.explorersoftware.taobao.com
*����ʱ��:2017��9��4��10:24:52
*�ļ�˵��:����CUIGUI
*/

#ifndef  __CUIGUI_DROPDOWN_H  
#define  __CUIGUI_DROPDOWN_H 

#include "CUIGUI_UDATA.H"
#include "CUIGUI_CONFIG.H"
#include "CUIGUI_BUTTON.H"

typedef struct 
{
      u16 x ;                         //�������x
      u16 y ;                         //�������y
      u16 width ;                     //����
      u16 height ;                    //�߶�
      COLOR_DATTYPE BKcolor;          //δ����ʱ�ı���ɫ
      COLOR_DATTYPE BKcolor_on;       //����ʱ�ı���ɫ
      COLOR_DATTYPE fontColor;        //δ����ʱ��������ɫ
      const char* str;                //�ַ���
      u8 ispressed;                   //����1��û����0
      MSTYPE buttonEvent;             //�����¼�
      const char* bmproot;
      u8  chamfer;                    //���Ƕ�
      GUI_CONST_STORAGE GUI_FONT *Font;//����
      
}DropDown;









#endif

