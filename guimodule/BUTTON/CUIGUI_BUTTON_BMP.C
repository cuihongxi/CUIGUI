/******************************************************************
*�ļ���:BUTTON�ؼ�
*
*/


#include "CUIGUI_BUTTON.H"

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
//BMP BUTTON
void DrawButtonBMP(Button* button)
{                      
      BtBMPSkin((ButtonBMP*)button);                      //Ƥ��
      BtBMPText((ButtonBMP*)button);                      //�� 
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

