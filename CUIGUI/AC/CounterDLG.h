#ifndef  __CounterDLG_H  
#define  __CounterDLG_H 

#include "CUIGUI_UDATA.H"

#define     FILE_NAME1    "1:/wz565.bmp"//文件名,盘符必须是 1
#define     FILE_NAME2    "1:/wenjian.bmp"
#define     FILE_NAME3    "1:/tb565.bmp"
#define     FILE_NAME4    "1:/zn565.bmp"//文件名,盘符必须是 1
#define     FILE_NAME5    "1:/uc.bmp"
#define     FILE_NAME6    "1:/hh565.bmp"
#define     FILE_NAME7    "1:/tanzhang.bmp"
#define     FILE_NAME8    "1:/qq565.bmp"
#define     FILE_NAME9    "1:/wx565.bmp"
#define     FILE_NAME10    "1:/beijing3.bmp"
#define     FILE_NAME11    "1:/counter.bmp"
#define     FILE_NAME12    "1:/bt565.bmp"
#define     FILE_NAME13    "1:/ph565.bmp"
#define     FILE_NAME14    "1:/back.bmp"
#define     FILE_NAME15    "1:/TXT.bmp"
#define     FILE_NAME16    "1:/BMP.bmp"
#define     FILE_NAME17    "1:/9.bmp"
#define     FILE_NAME18    "1:/8.bmp"

#define ID_WINDOW_0    (GUI_ID_USER + 0x00)
#define ID_EDIT_0      (GUI_ID_USER + 0x01)
#define ID_BUTTON_0    (GUI_ID_USER + 0x02)
#define ID_BUTTON_1    (GUI_ID_USER + 0x03)
#define ID_BUTTON_2    (GUI_ID_USER + 0x04)
#define ID_BUTTON_3    (GUI_ID_USER + 0x05)
#define ID_BUTTON_4    (GUI_ID_USER + 0x06)
#define ID_BUTTON_5    (GUI_ID_USER + 0x07)
#define ID_BUTTON_6    (GUI_ID_USER + 0x08)
#define ID_BUTTON_7    (GUI_ID_USER + 0x09)
#define ID_BUTTON_8    (GUI_ID_USER + 0x0A)
#define ID_BUTTON_9    (GUI_ID_USER + 0x0B)
#define ID_BUTTON_10    (GUI_ID_USER + 0x0C)
#define ID_BUTTON_11    (GUI_ID_USER + 0x0D)
#define ID_BUTTON_12    (GUI_ID_USER + 0x0E)
#define ID_BUTTON_13    (GUI_ID_USER + 0x0F)
#define ID_BUTTON_14    (GUI_ID_USER + 0x10)
#define ID_BUTTON_15    (GUI_ID_USER + 0x11)
#define ID_BUTTON_16    (GUI_ID_USER + 0x12)
#define ID_BUTTON_17    (GUI_ID_USER + 0x13)
#define ID_BUTTON_18    (GUI_ID_USER + 0x14)
#define ID_BUTTON_19    (GUI_ID_USER + 0x15)
#define ID_BUTTON_20    (GUI_ID_USER + 0x16)


extern  const GUI_WIDGET_CREATE_INFO _aDialogCreate[] ;
extern  const GUI_WIDGET_CREATE_INFO WindowCreate0[];

WM_HWIN CreateCounter(void);
void CbFun(WM_MESSAGE * pMsg);
void CreateAC1(void);

#endif
