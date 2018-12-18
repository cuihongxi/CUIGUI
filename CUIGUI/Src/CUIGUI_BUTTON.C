/******************************************************************
*文件名:BUTTON控件
*创建者:博硕电子科技，www.explorersoftware.taobao.com
*创建时间:2017年6月28日08:22:01
*文件说明:基于CUIGUI     
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
   按键0    	
*/
//#define BUTTON0_BKCOLLOR             CUIGUI_Color(240,240,240)      //按键背景
//#define BUTTON0_BKEDGE0COLLOR        CUIGUI_Color(160,160,160)      //按键边沿
//#define BUTTON0_BKEDGE1COLLOR        CUIGUI_Color(227,227,227)      //按键边沿
//#define BUTTON0_BKEDGE2COLLOR        CUIGUI_Color(105,105,105)      //按键边沿
//#define BUTTON0_BKEDGE3COLLOR        WHITE                          //按键边沿


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


///************************************************************************************
//*-函数名称	：BMP皮肤 
//*-说明	：
//*返回值	：
//*-函数功能	：
//*-创建者	：博硕电子科技，www.explorersoftware.taobao.com
//*/

void BtBMPSkin(ButtonBMP* button)
{
      if(CUIGUI_BMP_Init())
      {
                  //此处填初始化BMP失败警告代码
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

//一种本部布局
void BtBMPText(ButtonBMP* button)
{
      u16  length;
      u16 x,y;    //字符串的坐标
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

void DefaultDrawButton(Button* button)
{                      
      BtDefaultSkin(button);                      //皮肤
      BtDefaultText(button);                      //字 
}
//BMP BUTTON
void DrawButtonBMP(Button* button)
{                      
      BtBMPSkin((ButtonBMP*)button);                      //皮肤
      BtBMPText((ButtonBMP*)button);                      //字 
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

//设置BMP BUTTON的图片
void SetBmpRoot(ButtonBMP* button,const char* bmproot)
{
	button->bmproot = bmproot;
}

//新建一个BMP图的按键
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
//按键按下
void ButtonSetPressed(WM_HWIN handle ,u8 isPressed )
{
      Button* bt = (Button*)handle;
      bt->ispressed = isPressed;
      bt->DrawButton(bt);
}



//设置按键颜色
void ButtonSetColor(Button* button,COLOR_DATTYPE fontColor)
{
      button->fontColor = fontColor;
      button->DrawButton(button);
}

//设置字体
void ButtonSetFont(Button* button, const GUI_FONT* pFont)
{
      button->pFont = pFont;
      button->DrawButton(button);
}

/*
      重写图形BUILDER函数
*/

void BUTTON_SetFont(BUTTON_Handle hObj, const GUI_FONT* pFont)
{
      Button* bt = (Button*)hObj;
      ButtonSetFont(bt,pFont);
}


/*
      重写图形BUILDER函数
*/

BUTTON_Handle BUTTON_CreateIndirect(const GUI_WIDGET_CREATE_INFO * pCreate, WM_HWIN hWin, int x0, int y0, WM_CALLBACK * cb)
{
      Button* bt  =  NewButton(pCreate->x0,pCreate->y0,pCreate->xSize,pCreate->ySize); 
      bt->str = pCreate->pName;   
      bt->DrawButton(bt);
      *hWin = (u32)bt;
      return (BUTTON_Handle)hWin;     
}

