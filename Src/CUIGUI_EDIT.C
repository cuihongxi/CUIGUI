#include "CUIGUI_EDIT.H"
#include "CUIGUI_COLOR.H"
#include "CUIGUI_LCDDRIVER.H"
#include "CUI_MALLCO.H"
#include "CUIGUI_UHEAD.H"
#include "GUI.H"

/************************************************************************************
*-��������	��Ƥ������
*-����	��
*����ֵ	��
*-��������	��
*-������	�������в�˶���ӿƼ�
*/

void EditSkin(Edit* edit)
{
      Obj_SkinPressed((Obj*)edit);
}

/************************************************************************************
*-��������	��Ĭ��Ƥ������ , ��Ҫ��дʱ��д
*-����	��
*����ֵ	��
*-��������	��
*-������	����˶���ӿƼ���www.explorersoftware.taobao.com
*/

void EditDefaultSkin(Edit* edit)
{
      EditSkin(edit);
}    

/************************************************************************************
*-��������	����ʾ�ı�
*-����	��
*����ֵ	��
*-��������	��
*-������	�������в�˶���ӿƼ�
*/
//��ʾ�ı�
void EditShowTXT(Edit* edit)
{
      u16  length;
      u16 x,y;    //�ַ���������
      if(edit->str != 0) 
      {
            length = GetLengthStr((const char*)edit->str);
            switch(edit->alig_mode&0x0f)
            {
                  case GUI_TA_LEFT :      x = edit->x;
                        break;
                  case GUI_TA_RIGHT:      x = edit->x +edit->width - length;
                        break;
                  case GUI_TA_HCENTER:    x = edit->x+(edit->width-length)/2;
                        break;
            }
            switch(edit->alig_mode&0xf0)
            {
                  case GUI_TA_TOP:        y = edit->y;
                        break;
                  case GUI_TA_BOTTOM:     y = edit->y + edit->height - GetHeightFont(edit->Font); 
                        break;
                  case GUI_TA_VCENTER:    y = edit->y+ (edit->height - GetHeightFont(edit->Font))/2;
                        break;                                                      
            }
            CUIGUI_DrawStr(x,y,edit->FontColor,(const char*)edit->str);
      }
      
}


/************************************************************************************
*-��������	���½�һ��Edit,������һ��Ĭ��ֵ
*-����	��
*����ֵ	��
*-��������	��
*-������	�������в�˶���ӿƼ�
*/

 Edit*  NewEdit(void)
{    
      Edit* edit = (Edit*)CUI_Mallco(sizeof(Edit));
      edit->alig_mode = GUI_TA_LEFT|GUI_TA_VCENTER;
      edit->BKcolor = WHITE;
      edit->EditSkinFun = EditDefaultSkin;
      edit->Font = CUIGUI_GetFont();
      edit->FontColor = BLACK;
      edit->height = 50;
      edit->isShowCursor = NO;
      edit->str = 0;
      edit->width = 100;
      edit->x = CUIGUI_GetHpix()/2;
      edit->y = CUIGUI_GetVpix()/2;
      return edit;
}

/************************************************************************************
*-��������	�������༭��
*-����	��
*����ֵ	��
*-��������	��
*-������	�������в�˶���ӿƼ�
*/
void CUIGUI_DrawEdit(Edit* edit)
{      
      edit->EditSkinFun(edit);
      EditShowTXT(edit);
}

/*
      ��дͼ��BUILDER����
      ������Դ������EDIT
*/      
EDIT_Handle EDIT_CreateIndirect(const GUI_WIDGET_CREATE_INFO * pCreate, WM_HWIN hWin, int x0, int y0, WM_CALLBACK * cb)
{      
      Edit* ed = NewEdit();      
      ed->x = x0;
      ed->y = y0;
      ed->width = pCreate->xSize;
      ed->height = pCreate->ySize;
      ed->BKcolor = WHITE;
      ed->Font = CUIGUI_GetFont();
      *hWin = (u32)ed;               
      ed->isShowCursor = NO;
      CUIGUI_DrawEdit(ed);      
      return (EDIT_Handle)hWin;
}

/*
      ��дͼ��BUILDER����
      �����ı�����
*/
void EDIT_SetText(EDIT_Handle hObj,const char* str)
{
      Edit* edit = (Edit*)hObj;
      CUIGUI_FillRect(edit->x,edit->y,edit->width,edit->height,edit->BKcolor); //�屳��ɫ
      edit->str = (u8*)str;
      
      CUIGUI_DrawEdit(edit);
}
/*
      ��дͼ��BUILDER����
      �����ı����뷽ʽ
*/

void EDIT_SetTextAlign(EDIT_Handle hObj,u8 mode)
{
      Edit* ed = (Edit*)hObj;
      ed->alig_mode = mode;
      CUIGUI_DrawEdit(ed);
}      

/*
      ��дͼ��BUILDER����
      ���û�����ɫ
*/
void EDIT_SetBkColor(EDIT_Handle hObj, unsigned int Index, COLOR_DATTYPE color)
{
       Edit* ed = (Edit*)hObj;
       ed->FontColor = color;
       CUIGUI_DrawEdit(ed);
}

/*
      ��дͼ��BUILDER����
      ��������
*/

void EDIT_SetFont(EDIT_Handle hObj,const GUI_FONT GUI_FontNum)
{
      Edit* ed = (Edit*)hObj;
      ed->Font = &GUI_FontNum;
      CUIGUI_DrawEdit(ed);
}