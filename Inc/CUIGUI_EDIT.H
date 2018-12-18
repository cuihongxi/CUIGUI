/******************************************************************
*�ļ���:�༭��ؼ�
*������:̽���Ƽ���www.explorersoftware.taobao.com
*����ʱ��:2017��8��25��19:20:14
*�ļ�˵��:����CUIGUI
*/
#ifndef  __CUIGUI_EDIT_H   
#define  __CUIGUI_EDIT_H 

#include "CUIGUI_UDATA.H"


#define     DEBUG_EDIT 0      //���Կ���

#if DEBUG_EDIT >0     
#include "usart.h"
#endif
         
typedef u32*     EDIT_Handle;

//�������
typedef	struct 
{
	u16 x;
	u16 y;
}cursorXY;



typedef struct Edit_struct
{
      u16 x ;                                   //�������x
      u16 y ;                                   //�������y
      u16 width ;                               //����
      u16 height ;                              //�߶�
      COLOR_DATTYPE BKcolor;                    //����ɫ
      COLOR_DATTYPE FontColor;                  //������ɫ
      u8* str;                                  //�ַ���      
      const GUI_FONT* Font;         //����
      u8 alig_mode;                             //��ʾ���뷽ʽ
      GUIFlagStatus isShowCursor;                 //�Ƿ���ʾ���|:1��ʾ��0 ����ʾ
      void(*EditSkinFun)(struct Edit_struct*);  //����,��Ҫ���������д,Ҳ����ѡ��Ĭ��

}Edit;


/*����*/

Edit*  NewEdit(void);
void  CUIGUI_DrawEdit(Edit* edit);

//��дͼ��BUILDER����
void EDIT_SetText(EDIT_Handle hObj,const char* str);
void EDIT_SetTextAlign(EDIT_Handle hObj,u8 method);      //�����ı���ʾλ�÷�ʽ
void EDIT_SetFont(EDIT_Handle hObj,const GUI_FONT GUI_FontNum);
EDIT_Handle EDIT_CreateIndirect(const GUI_WIDGET_CREATE_INFO * pCreate, WM_HWIN hWin, int x0, int y0, WM_CALLBACK * cb);
void EDIT_SetTextAlign(EDIT_Handle hObj,u8 mode);     //�����ı����뷽ʽ


#endif