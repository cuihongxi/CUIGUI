/******************************************************************
*文件名:公共参数
*创建者:探长科技，www.explorersoftware.taobao.com
*创建时间:2017年6月28日08:22:01
*文件说明:定义公共参数，宏定义，类型
*/

#ifndef  __CUIGUI_UDATA_H  
#define  __CUIGUI_UDATA_H 

#include "stm32f10x.h"
     
#define GUI_COUNTOF(a)          (sizeof(a) / sizeof(a[0]))

#define ExchangeDat(a,b)      a^=b;b^=a;a^=b    //交换两个数的值

typedef unsigned char           MSTYPE;


typedef unsigned short	WCHAR;
typedef unsigned int	UINT;
typedef unsigned char   BYTE;

#include "CUIGUI_CONFIG.H"


typedef u32*     WM_HWIN;

typedef struct Obj_struct
{
      u16 x ;                             		//起点坐标x
      u16 y ;                             		//起点坐标y
      u16 width ;                         		//宽度
      u16 height ;                        		//高度
	    //void (*DrawComp)(struct Obj_struct*); 	//画出控件
}Obj;

//typedef struct WM_MESSAGE WM_MESSAGE;

typedef struct WM_MESSAGE {
  int MsgId;            /* type of message */
  WM_HWIN hWin;         /* Destination window 当前窗口句柄*/
  int hWinSrc;          /* Source window  记录是哪个控件ID发出的消息*/
  union {
    const void * p;            /* Some messages need more info ... Pointer is declared "const" because some systems (M16C) have 4 byte const, byte 2 byte default ptrs */
    int v;
//    COLOR_DATTYPE Color;
  } Data;
}WM_MESSAGE;
typedef void WM_CALLBACK(WM_MESSAGE * pMsg);


#define GUI_CONST_STORAGE   const

typedef enum {NO = 0, YES = !RESET} GUIFlagStatus;
typedef enum {ISOK = 0, ISERROR = 1} Flag_Status;

typedef struct
{
    u8 x;         //宽度
    u8 y;         //高度
    u8 size;      //占用字节数
 const  unsigned char* acGUI_Font;
}GUI_CHARINFO;

typedef struct GUI_FONT_PROP
{
   u16 fc; /* first character */
  u16 lc; /* last character  */
 const GUI_CHARINFO* afc; /* address of first character */
 const struct  GUI_FONT_PROP* pNext;  /* pointer to next GUI_FONT_PROP */
}GUI_FONT_PROP;      
typedef struct 
{
   u8    type ;             /* type of font    */
   u8    height ;           /* height of font  */
   u8    space;             /* space of font y */
   u8    magnification_x ;  /* magnification x */
   u8    magnification_y;   /* magnification y */
   const  GUI_FONT_PROP*  prop;
   u8 bl;                   /* Baseline */
   u8 hl;                   /* Height of lowercase characters */
   u8 hc;                   /* Height of capital characters */
}GUI_FONT;


typedef struct  GUI_WIDGET_CREATE_INFO_struct GUI_WIDGET_CREATE_INFO;

typedef WM_HWIN GUI_WIDGET_CREATE_FUNC (const GUI_WIDGET_CREATE_INFO * pCreate, WM_HWIN hWin, int x0, int y0, WM_CALLBACK * cb);


struct GUI_WIDGET_CREATE_INFO_struct {
  GUI_WIDGET_CREATE_FUNC * pfCreateIndirect;
  const char             * pName;            // Text ... Not used on all widgets
  u16                      Id;               // ID ... should be unique in a dialog
  u16                      x0;               // x position
  u16                      y0;               // y position
  u16                      xSize;            // x size
  u16                      ySize;            // y size
  u16                      Flags;            // Widget specific create flags (opt.)
  u32                      Para;             // Widget specific parameter (opt.)
  u32                      ObjAddr;          //指针,用于接收创建控件的地址
};

//对齐方式
typedef   enum 
{
      GUI_TA_LEFT = 0x01,                 //靠左对齐
      GUI_TA_RIGHT = 0x02,                //靠右对齐      
      GUI_TA_HCENTER = 0x03,              //左右居中
      
      GUI_TA_TOP = 0X10,                  //靠顶边
      GUI_TA_BOTTOM = 0x20,               //靠底边
      GUI_TA_VCENTER = 0x30,              //上下居中
      
}AligMode;

typedef void WM_SCAN (const GUI_WIDGET_CREATE_INFO* CREATE_INFO,u32 count,WM_MESSAGE* msg);




//触摸点坐标
typedef	struct 
{
	u16	x;
	u16   y;
}TouchXY;

typedef struct Scan_struct                //扫描的结构体
{     
      u16 Id;                             //ID号
      TouchXY tp_dev;                     //坐标
}scan_Message;
      


extern      WM_MESSAGE        msg;
extern      WM_CALLBACK*      CALLBACK_Fun;
extern      WM_SCAN*          WindowScan;                   //全局窗口触摸扫描
//extern      const GUI_WIDGET_CREATE_INFO * pWM_Create;    //窗口信息
extern      WM_HWIN           wm_hwin; 
//extern      u32   countINFO;                                //数组大小
#endif

