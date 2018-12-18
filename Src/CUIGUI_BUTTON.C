/******************************************************************
*�ļ���:BUTTON�ؼ�
*������:��˶���ӿƼ���www.explorersoftware.taobao.com
*����ʱ��:2017��6��28��08:22:01
*�ļ�˵��:����CUIGUI     
*/


#include "CUIGUI_BUTTON.H"
#include "GUI.H"
#include "CUI_MALLCO.H"
#include "CUIGUI_COLOR.H"
#include "CUIGUI_LCDDRIVER.H"
#include "CUIGUI_UHEAD.H"

#if         DEBUG_BUTTON > 0    
#include<usart.h>
#endif  

#include "CUIGUI_BMP.H"



/*********************************************************
   ����0    	
*/
//#define BUTTON0_BKCOLLOR             CUIGUI_Color(240,240,240)      //��������
//#define BUTTON0_BKEDGE0COLLOR        CUIGUI_Color(160,160,160)      //��������
//#define BUTTON0_BKEDGE1COLLOR        CUIGUI_Color(227,227,227)      //��������
//#define BUTTON0_BKEDGE2COLLOR        CUIGUI_Color(105,105,105)      //��������
//#define BUTTON0_BKEDGE3COLLOR        WHITE                          //��������


//Ƥ��1
void BtSkin(Button* button)
{
      if(button->ispressed == BT_PRESSED)            
      {     
            Obj_SkinPressed((Obj*)button);
      }
      else
      {     
            Obj_SkinUnPressed((Obj*)button);      
      }      
}


///************************************************************************************
//*-��������	��BMPƤ�� 
//*-˵��	��
//*����ֵ	��
//*-��������	��
//*-������	����˶���ӿƼ���www.explorersoftware.taobao.com
//*/

void BtBMPSkin(ButtonBMP* button)
{
      if(CUIGUI_BMP_Init())
      {
                  //�˴����ʼ��BMPʧ�ܾ������
      } else
      {
            if(((Button*)button)->ispressed == BT_PRESSED)            
            {     
                 BMP_Show(((Obj*)button)->x,((Obj*)button)->y,button->bmproot,6,1);     
            }
            else
            {     
                 BMP_Show(((Obj*)button)->x,((Obj*)button)->y,button->bmproot,6,0);  
            } 
      }  

      
}

//һ�ֱ�������
void BtBMPText(ButtonBMP* button)
{
      u16  length;
      u16 x,y;    //�ַ���������
      if(((Button*)button)->str != 0) 
      {
            CUIGUI_SetFont(((Button*)button)->pFont); 
            length = GetLengthStr(((Button*)button)->str);
            if(((Button*)button)->ispressed == BT_PRESSED) ((Button*)button)->fontColor = GetXORColor(((Button*)button)->fontColor);
            x = ((Obj*)button)->x+(((Obj*)button)->width-length)/2;
             y = ((Obj*)button)->y + ((Obj*)button)->height+3;
            CUIGUI_DrawStr(x,y,((Button*)button)->fontColor,((Button*)button)->str);              
      }                         
}

/************************************************************************************
*-��������	��Ĭ��Ƥ������ , ��Ҫ��дʱ��д
*-����	��
*����ֵ	��
*-��������	��
*-������	����˶���ӿƼ���www.explorersoftware.taobao.com
*/

void BtDefaultSkin(Button* button)
{
      BtSkin(button);
}    



/************************************************************************************
*-��������	���趨�����ϵ��ı�
*-˵��	��û�ж������С,��Ч�����ж�
*����ֵ	��
*-��������	��
*-������	����˶���ӿƼ���www.explorersoftware.taobao.com
*/
void BtText(Button* button)
{
      u16 length ;    
              
      if(button->str != 0) 
      {
            CUIGUI_SetFont(button->pFont); 
            length = GetLengthStr(button->str);
            if(button->ispressed == BT_PRESSED) button->fontColor = GetXORColor(button->fontColor);
            CUIGUI_DrawStr(((Obj*)button)->x+(((Obj*)button)->width-length)/2,\
            ((Obj*)button)->y+ (((Obj*)button)->height - button->pFont->height)/2,button->fontColor,button->str);              
      }     
}

//Ĭ���ı���ʽ
void BtDefaultText(Button* button)
{
      BtText(button);
}

/************************************************************************************
*-��������	��
*-ע��	��
*����ֵ	��
*-��������	��
*-������	����˶���ӿƼ���www.explorersoftware.taobao.com
*/

void DefaultDrawButton(Button* button)
{                      
      BtDefaultSkin(button);                      //Ƥ��
      BtDefaultText(button);                      //�� 
}
//BMP BUTTON
void DrawButtonBMP(Button* button)
{                      
      BtBMPSkin((ButtonBMP*)button);                      //Ƥ��
      BtBMPText((ButtonBMP*)button);                      //�� 
}
/************************************************************************************
*-��������	���½�һ������,������Ĭ��ֵ
*-����	��
*����ֵ	��
*-��������	��
*-������	����˶���ӿƼ���www.explorersoftware.taobao.com
*/

 Button*  NewButton(u16 xpos,u16 ypos,u16 width,u16 height)
{    
      Button* button = (Button*)CUI_Mallco(sizeof(Button));
	
      button->ispressed = BT_UNPRESSED;
      button->str = 0;   
      button->fontColor = BLACK;      
      button->pFont = CUIGUI_GetFont();
      ((Obj*)button)->height =height;
      ((Obj*)button)->width = width;
      ((Obj*)button)->x = xpos;
      ((Obj*)button)->y = ypos;
      button->DrawButton = &DefaultDrawButton;
      return button;
}

//����BMP BUTTON��ͼƬ
void SetBmpRoot(ButtonBMP* button,const char* bmproot)
{
	button->bmproot = bmproot;
}

//�½�һ��BMPͼ�İ���
 ButtonBMP*  NewButtonBMP(u16 xpos,u16 ypos,u16 width,u16 height,const char* bmproot)
{    
      ButtonBMP* button = (ButtonBMP*)CUI_Mallco(sizeof(ButtonBMP));
	
      ((Button*)button)->ispressed = BT_UNPRESSED;
      ((Button*)button)->str = 0;   
      ((Button*)button)->fontColor = BLACK;      
      ((Button*)button)->pFont = CUIGUI_GetFont();
      ((Obj*)button)->height =height;
      ((Obj*)button)->width = width;
      ((Obj*)button)->x = xpos;
      ((Obj*)button)->y = ypos;
      ((Button*)button)->DrawButton = DrawButtonBMP;
			button->bmproot = bmproot;
			button->SetBmpRoot = SetBmpRoot;
      return button;
}
//��������
void ButtonSetPressed(WM_HWIN handle ,u8 isPressed )
{
      Button* bt = (Button*)handle;
      bt->ispressed = isPressed;
      bt->DrawButton(bt);
}



//���ð�����ɫ
void ButtonSetColor(Button* button,COLOR_DATTYPE fontColor)
{
      button->fontColor = fontColor;
      button->DrawButton(button);
}

//��������
void ButtonSetFont(Button* button, const GUI_FONT* pFont)
{
      button->pFont = pFont;
      button->DrawButton(button);
}

/*
      ��дͼ��BUILDER����
*/

void BUTTON_SetFont(BUTTON_Handle hObj, const GUI_FONT* pFont)
{
      Button* bt = (Button*)hObj;
      ButtonSetFont(bt,pFont);
}


/*
      ��дͼ��BUILDER����
*/

BUTTON_Handle BUTTON_CreateIndirect(const GUI_WIDGET_CREATE_INFO * pCreate, WM_HWIN hWin, int x0, int y0, WM_CALLBACK * cb)
{
      Button* bt  =  NewButton(pCreate->x0,pCreate->y0,pCreate->xSize,pCreate->ySize); 
      bt->str = pCreate->pName;   
      bt->DrawButton(bt);
      *hWin = (u32)bt;
      return (BUTTON_Handle)hWin;     
}
