/******************************************************************
*文件名:BUTTON控件
*
*/


#include "CUIGUI_BUTTON.H"



/*********************************************************
   按键0    	
*/

#define BKCOLLOR             CUIGUI_Color(240,240,240)      //背景
#define BKEDGE0COLLOR        CUIGUI_Color(160,160,160)      //边沿
#define BKEDGE1COLLOR        CUIGUI_Color(227,227,227)      //边沿
#define BKEDGE2COLLOR        CUIGUI_Color(105,105,105)      //边沿
#define BKEDGE3COLLOR        WHITE                          //边沿
//皮肤1
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


//皮肤1
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
*-函数名称	：默认皮肤函数 , 需要改写时改写
*-参数	：
*返回值	：
*-函数功能	：
*-创建者	：博硕电子科技，www.explorersoftware.taobao.com
*/

void BtDefaultSkin(Button* button)
{
      BtSkin(button);
}    

/************************************************************************************
*-函数名称	：设定按键上的文本
*-说明	：没有对字体大小,有效性做判断
*返回值	：
*-函数功能	：
*-创建者	：博硕电子科技，www.explorersoftware.taobao.com
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

//默认文本样式
void BtDefaultText(Button* button)
{
      BtText(button);
}

/************************************************************************************
*-函数名称	：
*-注意	：
*返回值	：
*-函数功能	：
*-创建者	：博硕电子科技，www.explorersoftware.taobao.com
*/

void DefaultDrawButton(void* button)
{                      
      BtDefaultSkin((Button*)button);                      //皮肤
      BtDefaultText((Button*)button);                      //字 
}

/************************************************************************************
*-函数名称	：新建一个按键,并分配默认值
*-参数	：
*返回值	：
*-函数功能	：
*-创建者	：博硕电子科技，www.explorersoftware.taobao.com
*/

 Button*  NewButton(u16 xpos,u16 ypos,u16 width,u16 height)
{    
      Button* button = (Button*)malloc(sizeof(Button));
      button->ispressed = BT_UNPRESSED;
      button->str = 0;   
      button->fontColor = BLACK;      
      button->pFont = CUIGUI_GetFont();
      ((Obj*)button)->height =height;
      ((Obj*)button)->width = width;
      ((Obj*)button)->x = xpos;
      ((Obj*)button)->y = ypos;
      button->obj.Draw = &DefaultDrawButton;
      return button;
}

//设置按键颜色
void ButtonSetColor(Button* button,COLOR_DATTYPE fontColor)
{
      button->fontColor = fontColor;
      button->obj.Draw(button);
}

//设置字体
void ButtonSetFont(Button* button, const GUI_FONT* pFont)
{
      button->pFont = pFont;
      button->obj.Draw(button);
}


