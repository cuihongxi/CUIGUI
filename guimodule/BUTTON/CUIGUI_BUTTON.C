/******************************************************************
*�ļ���:BUTTON�ؼ�
*
*/

#include "CUIGUI_BUTTON.H"
/*********************************************************
   ����0    	
*/

#define BKCOLLOR             CUIGUI_Color(240,240,240)      //����
#define BKEDGE0COLLOR        CUIGUI_Color(160,160,160)      //����
#define BKEDGE1COLLOR        CUIGUI_Color(227,227,227)      //����
#define BKEDGE2COLLOR        CUIGUI_Color(105,105,105)      //����
#define BKEDGE3COLLOR        WHITE                          //����
//Ƥ��1
void Obj_SkinUnPressed(Obj* obj)
{
      CUIGUI_FillRect(obj->x,obj->y,obj->width,obj->height,BKCOLLOR);                 
      CUIGUI_DrawHLine(obj->x-1,obj->y+obj->height+1,obj->x+obj->width+1,BKEDGE0COLLOR);	
      CUIGUI_DrawVLine(obj->x+obj->width+1,obj->y,obj->y+obj->height,BKEDGE0COLLOR);
      CUIGUI_DrawHLine(obj->x-2,obj->y+obj->height+2,obj->x+obj->width+2,BKEDGE2COLLOR);	
      CUIGUI_DrawVLine(obj->x+obj->width+2,obj->y-1,obj->y+obj->height+2,BKEDGE2COLLOR);
      CUIGUI_DrawHLine(obj->x-1,obj->y-1,obj->x+obj->width,BKEDGE1COLLOR);	
      CUIGUI_DrawVLine(obj->x-1,obj->y,obj->y+obj->height,BKEDGE1COLLOR);
      CUIGUI_DrawHLine(obj->x-2,obj->y-2,obj->x+obj->width+1,BKEDGE3COLLOR);	
      CUIGUI_DrawVLine(obj->x-1,obj->y-1,obj->y+obj->height+1,BKEDGE3COLLOR);      
}

void Obj_SkinPressed(Obj* obj)
{
      CUIGUI_FillRect(obj->x,obj->y,obj->width,obj->height,BKCOLLOR);      
      CUIGUI_DrawHLine(obj->x-1,obj->y+obj->height+1,obj->x+obj->width+1,BKEDGE1COLLOR);	
      CUIGUI_DrawVLine(obj->x+obj->width+1,obj->y,obj->y+obj->height,BKEDGE1COLLOR);
      CUIGUI_DrawHLine(obj->x-2,obj->y+obj->height+2,obj->x+obj->width+2,BKEDGE3COLLOR);	
      CUIGUI_DrawVLine(obj->x+obj->width+2,obj->y-1,obj->y+obj->height+2,BKEDGE3COLLOR);
      CUIGUI_DrawHLine(obj->x-1,obj->y-1,obj->x+obj->width,BKEDGE0COLLOR);	
      CUIGUI_DrawVLine(obj->x-1,obj->y,obj->y+obj->height,BKEDGE0COLLOR);
      CUIGUI_DrawHLine(obj->x-2,obj->y-2,obj->x+obj->width+1,BKEDGE2COLLOR);	
      CUIGUI_DrawVLine(obj->x-1,obj->y-1,obj->y+obj->height+1,BKEDGE2COLLOR);       

}


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

void DefaultDrawButton(void* button)
{                      
      BtDefaultSkin((Button*)button);                      //Ƥ��
      BtDefaultText((Button*)button);                      //�� 
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
	Button* button = (Button*)malloc(sizeof(Button));

	InitOBJParam(button,xpos,ypos,width,height,DefaultDrawButton);
	button->ispressed = BT_UNPRESSED;
	button->str = 0;   
	button->fontColor = BLACK;      
	button->pFont = CUIGUI_GetFont();

	return button;
}

//���ð�����ɫ
void ButtonSetColor(Button* button,COLOR_DATTYPE fontColor)
{
      button->fontColor = fontColor;
      button->obj.Draw(button);
}

//��������
void ButtonSetFont(Button* button, const GUI_FONT* pFont)
{
      button->pFont = pFont;
      button->obj.Draw(button);
}

