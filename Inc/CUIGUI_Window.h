/******************************************************************
*文件名:窗口控件
*创建者:蓬莱市博硕电子科技
*创建时间:2017年11月6日20:42:00
*文件说明:
*     这是个EMWIN GUI BUILDER的创建窗口的文件
*使用方法及说明：
*     GUI_CreateDialogBox 函数根据资源列表创建各个控件，返回了储存句柄数组的地址。
*     在主函数中需要接收这个地址保存，不用时释放内存
*     目前没有子窗口的概念
*/

#ifndef  __CUIGUI_WINDOW_H   
#define  __CUIGUI_WINDOW_H  

#include "CUIGUI_UDATA.H"



#define     GUI_ID_USER       0     


#define     WINDOW_DEFAULT_TITLE_COLOR    CUIGUI_Color(200,191,231);    //设置窗口眉头颜色
#define     WINDOW_DEFAULT_BACK_COLOR     CUIGUI_Color(221,222,222);    //窗口背景颜色

#define     DEBUG_WINDOW_LEVEL   0          //按键使用调试开关
#if (DEBUG_WINDOW_LEVEL > 0)
#include "stdio.h"
#define  WINDOW_Log(...)         printf(__VA_ARGS__);
#else
#define WINDOW_Log(...)   
#endif 

typedef struct WIN_struct
{
      u16 x ;                         //起点坐标x
      u16 y ;                         //起点坐标y
      u16 width ;                     //宽度
      u16 height ;                    //高度
      COLOR_DATTYPE BKcolor;          //背景色
      COLOR_DATTYPE titleColor;       //标题色
      COLOR_DATTYPE fontColor;        //字体颜色      
      const GUI_FONT* pFont;          //字体      
      const char* titlestr;           //标题字符串
      u8 title_hight          ;       //标题高度
      void(*WinSkinFun)(struct WIN_struct*);   //换肤,需要根据情况重写,也可以选择默认

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
void        WindowSetColor(Window* window,COLOR_DATTYPE fontColor);      //设置窗口标题颜色
void        WinSkin(Window* window);
Flag_Status GUI_FreeDialogBox(WM_HWIN hwinArray);                       //销毁整个窗口
scan_Message WINDOW_Scan(const GUI_WIDGET_CREATE_INFO* CREATE_INFO,u32 count);      //窗口扫描函数
//TouchXY WINDOW_Scan(const GUI_WIDGET_CREATE_INFO* CREATE_INFO,u32 count,WM_MESSAGE* msg);  //窗口按键扫描

/*
      重写图形BUILDER函数
      
*/

WM_HWIN     WM_GetDesktopWindow(void); 
WM_HWIN     WM_GetDialogItem(WM_HWIN hWin,int Id);                                        //获得父句柄hWin下，ID对应的句柄
WM_HWIN     WINDOW_CreateIndirect(const GUI_WIDGET_CREATE_INFO * pCreate,\
                                    WM_HWIN hWin, int x0, int y0, WM_CALLBACK * cb);      //创建窗口
WM_HWIN     GUI_CreateDialogBox(const GUI_WIDGET_CREATE_INFO* CREATE_INFO,u32 count,\
                                    WM_CALLBACK * cb,WM_HWIN hParent, int x0, int y0);    //根据资源表创建控件
int         WM_GetId(int ID);                         //获取ID号
WM_HWIN     WM_DefaultProc(WM_MESSAGE * pMsg);


#endif

