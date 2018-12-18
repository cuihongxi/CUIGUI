#include "CUIGUI_TEXT.H"
#include "CUIGUI_COLOR.H"
#include "CUI_MALLCO.H"
#include "GUI.H"
#include "CUIGUI_UHEAD.H"
#include "CUIGUI_LCDDRIVER.H"
/************************************************************************************
*-函数名称	：皮肤函数
*-参数	：
*返回值	：
*-函数功能	：
*-创建者	：蓬莱市博硕电子科技
*/

void TextSkin(Text* text)
{
     // Obj_SkinUnPressed((Obj*) text);
      CUIGUI_FillRect(text->x,text->y,text->width,text->height,text->BKcolor);
}

/************************************************************************************
*-函数名称	：显示文本
*-参数	：
*返回值	：
*-函数功能	：
*-创建者	：蓬莱市博硕电子科技
*/
//显示文本
void TextShowTXT(Text* text)
{
      u16  length;
      u16 x,y;    //字符串的坐标
      if(text->str != 0) 
      {
            length = GetLengthStr((const char*)text->str);
            switch(text->alig_mode&0x0f)
            {
                  case GUI_TA_LEFT :      x = text->x;
                        break;
                  case GUI_TA_RIGHT:      x = text->x +text->width - length;
                        break;
                  case GUI_TA_HCENTER:    x = text->x+(text->width-length)/2;
                        break;
            }
            switch(text->alig_mode&0xf0)
            {
                  case GUI_TA_TOP:        y = text->y;
                        break;
                  case GUI_TA_BOTTOM:     y = text->y + text->height - GetHeightFont(text->Font); 
                        break;
                  case GUI_TA_VCENTER:    y = text->y+ (text->height - GetHeightFont(text->Font))/2;
                        break;                                                      
            }
            CUIGUI_DrawStr(x,y,text->FontColor,(const char*)text->str);
      }
      
}

void TextDefaultSkin(Text* text)
{
      TextSkin(text);
}

void DrawText(Text* text)
{
      TextDefaultSkin(text);
      TextShowTXT(text);
}
//新建一个文本
 Text*  NewText(u16 xpos,u16 ypos,u16 width,u16 height)
{    
      Text* text = (Text*)CUI_Mallco(sizeof(Text));
      text->alig_mode = GUI_TA_LEFT|GUI_TA_VCENTER;
      text->BKcolor = WHITE;
      text->TextSkinFun = TextDefaultSkin;
      text->Font = CUIGUI_GetFont();
      text->FontColor = BLACK;
      text->height = height;
      text->str = 0;
      text->width = width;
      text->x = xpos;
      text->y = ypos;
      text->DrawText = &DrawText;
      
      return text;
}

/************************************************************************************
*-函数名称	：画出文本框
*-参数	：
*返回值	：
*-函数功能	：
*-创建者	：蓬莱市博硕电子科技
*/
void CUIGUI_DrawText(Text* text)
{      
      text->TextSkinFun(text);
      TextShowTXT(text);
}

/*
      重写图形BUILDER函数
      设置文本内容
*/
void Text_SetText(WM_HWIN hObj,const char* str)
{
      Text* txt = (Text*)hObj;
      txt->str = (u8*)str;
      CUIGUI_DrawText(txt);
}

/*
      重写图形BUILDER函数

*/

void  TEXT_SetFont(WM_HWIN hItem,const GUI_FONT GUI_FontNum)
{
      Text* txt = (Text*)hItem;
      txt->Font = &GUI_FontNum;
      CUIGUI_DrawText(txt);
}
/*
      重写图形BUILDER函数
      设置文本显示位置方式
*/
void TEXT_SetTextAlign(WM_HWIN hItem, u8 mode)
{
      Text* text = (Text*)hItem;
      text->alig_mode = mode;
      CUIGUI_DrawText(text);
}

/*
      重写图形BUILDER函数

*/
void  TEXT_SetTextColor(WM_HWIN hItem, COLOR_DATTYPE color)
{
      Text* txt = (Text*)hItem;
      txt->FontColor = color;
      CUIGUI_DrawText(txt);
}

/*
      重写图形BUILDER函数
      创建文本流
      之前字体需要设定好
      资源表中需要将pCreate->Flags位替换成颜色值
*/
TEXT_Handle TEXT_CreateIndirect(const GUI_WIDGET_CREATE_INFO * pCreate, WM_HWIN hWin, int x0, int y0, WM_CALLBACK * cb)
{
      Text* txt =  NewText(x0,y0,pCreate->xSize,pCreate->ySize); 
      txt->FontColor = pCreate->Flags;
      txt->str =(u8*) pCreate->pName;
      CUIGUI_DrawText(txt);
      
      *hWin = (u32)txt;
      return (TEXT_Handle)hWin;
}
