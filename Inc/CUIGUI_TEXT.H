/******************************************************************
*�ļ���:�ı��ؼ�
*������:�����в�˶���ӿƼ�
*����ʱ��:2017��11��5��19:40:16
*�ļ�˵��:����CUIGUI
*/

#ifndef  __CUIGUI_TEXT_H   
#define  __CUIGUI_TEXT_H 

#include "CUIGUI_UDATA.H"

typedef u32*     TEXT_Handle;

//typedef struct 
//{
//      u16 x ;                         //�������x
//      u16 y ;                         //�������y
//      COLOR_DATTYPE color;            //������ɫ
//      const char* str;                //�ַ���;
//}Text;

typedef struct Text_struct
      {
            u16 x ;                                   //�������x
            u16 y ;                                   //�������y
            u16 width ;                               //����
            u16 height ;                              //�߶�
            COLOR_DATTYPE BKcolor;                    //����ɫ
            COLOR_DATTYPE FontColor;                  //������ɫ
            u8* str;                                  //�ַ���      
            const GUI_FONT* Font;                     //����
            u8 alig_mode;                             //��ʾ���뷽ʽ
            //GUIFlagStatus isShowCursor;             //EDIT�в��й��
            void(*TextSkinFun)(struct Text_struct*);  //����,��Ҫ���������д,Ҳ����ѡ��Ĭ��
            void(*DrawText)(struct Text_struct*);
      }Text;
 
void TextSkin(Text* text);
void TextShowTXT(Text* text);
Text*  NewText(u16 xpos,u16 ypos,u16 width,u16 height);  
void CUIGUI_DrawText(Text* text);
void Text_SetText(WM_HWIN hObj,const char* str);
void TEXT_SetTextAlign(WM_HWIN hItem, u8 methodR);
      
//��дͼ��BUILDER����
void  TEXT_SetFont(WM_HWIN hItem,const GUI_FONT GUI_FontNum);
void  TEXT_SetTextColor(WM_HWIN hItem, COLOR_DATTYPE color);
WM_HWIN TEXT_CreateIndirect(const GUI_WIDGET_CREATE_INFO * pCreate, WM_HWIN hWin, int x0, int y0, WM_CALLBACK * cb);


















#endif