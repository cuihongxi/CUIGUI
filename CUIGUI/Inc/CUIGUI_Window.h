/******************************************************************
*�ļ���:���ڿؼ�
*������:�����в�˶���ӿƼ�
*����ʱ��:2017��11��6��20:42:00
*�ļ�˵��:
*     ���Ǹ�EMWIN GUI BUILDER�Ĵ������ڵ��ļ�
*ʹ�÷�����˵����
*     GUI_CreateDialogBox ����������Դ�б��������ؼ��������˴���������ĵ�ַ��
*     ������������Ҫ���������ַ���棬����ʱ�ͷ��ڴ�
*     Ŀǰû���Ӵ��ڵĸ���
*/

#ifndef  __CUIGUI_WINDOW_H   
#define  __CUIGUI_WINDOW_H  

#include "CUIGUI_UDATA.H"



#define     GUI_ID_USER       0     


#define     WINDOW_DEFAULT_TITLE_COLOR    CUIGUI_Color(200,191,231);    //���ô���üͷ��ɫ
#define     WINDOW_DEFAULT_BACK_COLOR     CUIGUI_Color(221,222,222);    //���ڱ�����ɫ

#define     DEBUG_WINDOW_LEVEL   0          //����ʹ�õ��Կ���
#if (DEBUG_WINDOW_LEVEL > 0)
#include "stdio.h"
#define  WINDOW_Log(...)         printf(__VA_ARGS__);
#else
#define WINDOW_Log(...)   
#endif 

typedef struct WIN_struct
{
      u16 x ;                         //�������x
      u16 y ;                         //�������y
      u16 width ;                     //���
      u16 height ;                    //�߶�
      COLOR_DATTYPE BKcolor;          //����ɫ
      COLOR_DATTYPE titleColor;       //����ɫ
      COLOR_DATTYPE fontColor;        //������ɫ      
      const GUI_FONT* pFont;          //����      
      const char* titlestr;           //�����ַ���
      u8 title_hight          ;       //����߶�
      void(*WinSkinFun)(struct WIN_struct*);   //����,��Ҫ���������д,Ҳ����ѡ��Ĭ��

}Window;


/*********************************************************************
*
*       Messages Ids
*
* The following is the list of windows messages.
*/
#define WM_CREATE                   0x0001  /* The first message received, right after client has actually been created */
#define WM_MOVE                     0x0003  /* window has been moved (Same as WIN32) */

#define WM_SIZE                     0x0005  /* Is sent to a window after its size has changed (Same as WIN32, do not change !) */

#define WM_DELETE                   11      /* Delete (Destroy) command: This tells the client to free its data strutures since the window
                                               it is associates with no longer exists.*/
#define WM_TOUCH                    0x0240  /* Touch screen message */
#define WM_TOUCH_CHILD              13      /* Touch screen message to ancestors */
#define WM_KEY                      14      /* Key has been pressed */

#define WM_PAINT                    0x000F  /* Repaint window (because content is (partially) invalid */

#if GUI_SUPPORT_MOUSE
#define WM_MOUSEOVER                16      /* Mouse has moved, no key pressed */
#define WM_MOUSEOVER_END            18      /* Mouse has moved, no key pressed */
#endif

#define WM_PID_STATE_CHANGED        17      /* Pointer input device state has changed */

#define WM_GET_INSIDE_RECT          20      /* get inside rectangle: client rectangle minus pixels lost to effect */
#define WM_GET_ID                   21      /* Get id of widget */
#define WM_SET_ID                   22      /* Set id of widget */
#define WM_GET_CLIENT_WINDOW        23      /* Get window handle of client window. Default is the same as window */
#define WM_CAPTURE_RELEASED         24      /* Let window know that mouse capture is over */

#define WM_INIT_DIALOG              29      /* Inform dialog that it is ready for init */

#define WM_SET_FOCUS                30      /* Inform window that it has gotten or lost the focus */
#define WM_GET_ACCEPT_FOCUS         31      /* Find out if window can accept the focus */
#define WM_NOTIFY_CHILD_HAS_FOCUS   32      /* Sent to parent when child receives / loses focus */

#define WM_NOTIFY_OWNER_KEY         33      /* Some widgets (e.g. listbox) notify owner when receiving key messages */

#define WM_GET_BKCOLOR              34      /* Return back ground color (only frame window and similar) */
#define WM_GET_SCROLL_STATE         35      /* Query state of scroll bar */

#define WM_SET_SCROLL_STATE         36      /* Set scroll info ... only effective for scrollbars */

#define WM_NOTIFY_CLIENTCHANGE      37      /* Client area may have changed */
#define WM_NOTIFY_PARENT            38      /* Notify parent. Information is detailed as notification code */
#define WM_NOTIFY_PARENT_REFLECTION 39      /* Notify parent reflection. 
                                               Sometimes send back as a result of the WM_NOTIFY_PARENT message
                                               to let child react on behalf of its parent.
                                               Information is detailed as notification code */
#define WM_NOTIFY_ENABLE            40      /* Enable or disable widget */
#define WM_NOTIFY_VIS_CHANGED       41      /* Visibility of a window has or may have changed */

#define WM_HANDLE_DIALOG_STATUS     42      /* Set or get dialog status */
#define WM_GET_RADIOGROUP           43      /* Send to all siblings and children of a radio control when
                                               selection changed */
#define WM_MENU                     44      /* Send to owner window of menu widget */
#define WM_SCREENSIZE_CHANGED       45      /* Send to all windows when size of screen has changed */
#define WM_PRE_PAINT                46      /* Send to a window before it receives a WM_PAINT message */
#define WM_POST_PAINT               47      /* Send to a window after (the last) WM_PAINT message */

#define WM_MOTION                   48      /* Automatic motion messages */

#define WM_GESTURE                  0x0119  /* Gesture message */

#define WM_TIMER                    0x0113  /* Timer has expired              (Keep the same as WIN32) */
#define WM_WIDGET                   0x0300  /* 256 messages reserved for Widget messages */
#define WM_USER                     0x0400  /* Reserved for user messages ... (Keep the same as WIN32) */


/*********************************************************************
*
*       Notification codes
*
* The following is the list of notification codes send
* with the WM_NOTIFY_PARENT message
*/
#define WM_NOTIFICATION_CLICKED             1
#define WM_NOTIFICATION_RELEASED            2
#define WM_NOTIFICATION_MOVED_OUT           3
#define WM_NOTIFICATION_SEL_CHANGED         4
#define WM_NOTIFICATION_VALUE_CHANGED       5
#define WM_NOTIFICATION_SCROLLBAR_ADDED     6      /* Scroller added */
#define WM_NOTIFICATION_CHILD_DELETED       7      /* Inform window that child is about to be deleted */
#define WM_NOTIFICATION_GOT_FOCUS           8
#define WM_NOTIFICATION_LOST_FOCUS          9
#define WM_NOTIFICATION_SCROLL_CHANGED     10

#define WM_NOTIFICATION_WIDGET             11      /* Space for widget defined notifications */
#define WM_NOTIFICATION_USER               16      /* Space for  application (user) defined notifications */




#define WM_HBKWIN      WM_GetDesktopWindow()                /* Handle of background window */

/*********************************************************************
*
*       Structures
*
**********************************************************************
*/


Window*     NewWindow(void);
void        DrawWindow(Window* window);
void        WindowSetColor(Window* window,COLOR_DATTYPE fontColor);      //���ô��ڱ�����ɫ
void        WinSkin(Window* window);
Flag_Status GUI_FreeDialogBox(WM_HWIN hwinArray);                       //������������
scan_Message WINDOW_Scan(const GUI_WIDGET_CREATE_INFO* CREATE_INFO,u32 count);      //����ɨ�躯��
//TouchXY WINDOW_Scan(const GUI_WIDGET_CREATE_INFO* CREATE_INFO,u32 count,WM_MESSAGE* msg);  //���ڰ���ɨ��

/*
      ��дͼ��BUILDER����
      
*/

WM_HWIN     WM_GetDesktopWindow(void); 
WM_HWIN     WM_GetDialogItem(WM_HWIN hWin,int Id);                                        //��ø����hWin�£�ID��Ӧ�ľ��
WM_HWIN     WINDOW_CreateIndirect(const GUI_WIDGET_CREATE_INFO * pCreate,\
                                    WM_HWIN hWin, int x0, int y0, WM_CALLBACK * cb);      //��������
WM_HWIN     GUI_CreateDialogBox(const GUI_WIDGET_CREATE_INFO* CREATE_INFO,u32 count,\
                                    WM_CALLBACK * cb,WM_HWIN hParent, int x0, int y0);    //������Դ�����ؼ�
int         WM_GetId(int ID);                         //��ȡID��
WM_HWIN     WM_DefaultProc(WM_MESSAGE * pMsg);


#endif

