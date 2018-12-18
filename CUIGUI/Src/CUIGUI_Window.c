#include "CUIGUI_Window.H"
#include "MYTOUCH.H"
#include "CUIGUI_LCDDRIVER.H"
#include "CUIGUI_COLOR.H"
#include "CUI_MALLCO.H"
#include "GUI.H"


/************************************************************************************
*-函数名称	：Win皮肤 
*-说明	：
*返回值	：
*-函数功能	：
*-创建者	：博硕电子科技
*/

void WinSkin(Window* window)
{
      window->BKcolor = WINDOW_DEFAULT_BACK_COLOR;
      window->titleColor = WINDOW_DEFAULT_TITLE_COLOR;
      CUIGUI_FillRect( window->x,window->y,window->width,window->height ,window->BKcolor);
      CUIGUI_FillRect( window->x,window->y,window->width,window->title_hight ,window->titleColor);
}


/************************************************************************************
*-函数名称	：Win默认皮肤 
*-说明	：
*返回值	：
*-函数功能	：
*-创建者	：博硕电子科技
*/

void WinDefaultSkin(Window* window)
{
      WinSkin(window);
}

/************************************************************************************
*-函数名称	：创建新窗口,并赋初值
*-参数	：
*返回值	：
*-函数功能	：
*-创建者	：蓬莱市博硕电子科技
*/

Window*  NewWindow(void)
{    
      Window* window = (Window*)CUI_Mallco(sizeof(Window));

      window->titlestr = 0;   
      window->fontColor = BLACK;  
            
      window->WinSkinFun = &WinDefaultSkin;
      window->pFont = CUIGUI_GetFont();
      window->height =CUIGUI_GetVpix();
      window->width = CUIGUI_GetHpix();
      window->title_hight = window->height/15;
      window->x = 0;
      window->y = 0;
      return window;
}

//窗口标题
void WindowText(Window* window)
{
      u16 length ;    
      if(window->titlestr != 0) 
      {
            CUIGUI_SetFont(window->pFont); 
            length = GetLengthStr(window->titlestr);
            CUIGUI_DrawStr(window->x+(window->width-length)/2,\
                  window->y+ (window->title_hight - window->pFont->height)/2,window->fontColor,window->titlestr);              
      } 
    
}
/************************************************************************************
*-函数名称	：根据配置,画出窗口
*-参数	：
*返回值	：
*-函数功能	：
*-创建者	：蓬莱市博硕电子科技
*/
void DrawWindow(Window* window)
{                 
      window->WinSkinFun(window);                           //皮肤 
      WindowText(window);                                   //字 
}

//设置窗口标题颜色
void WindowSetColor(Window* window,COLOR_DATTYPE fontColor)
{
      window->fontColor = fontColor;
      DrawWindow(window);
}




/************************************************************************************
*-函数名称	：创建一个数组用于储存句柄
*-参数	：[0]保存申请的个数
*返回值	：
*-函数功能	：注意: 动态内存申请以字节为单位,决定了ID号不会超过255..
*-创建者	：蓬莱市博硕电子科技
*/
WM_HWIN Creat_HandleArray(u32 handlesize)
{
      WM_HWIN wm_handle = (WM_HWIN)CUI_Mallco(handlesize);   //申请内存,[0]保存申请的个数
               
      return wm_handle;
}



/************************************************************************************
*-函数名称	：销毁句柄数组
*-参数	：
*返回值	：成功返回1，不成功返回0
*-函数功能	：
*-创建者	：蓬莱市博硕电子科技
*/
Flag_Status Del_HandleArray(WM_HWIN hd_array)
{
      return (FreeCUI_Mallco((u8*)hd_array));
}




///************************************************************************************
//*-函数名称	：窗口按键扫描
//*-参数	：
//*返回值	：msg消息,如果有按到就会返回ID，松手时msg->Data.v = WM_NOTIFICATION_RELEASED
//                  在回调函数中处理
//*-函数功能	：
//*-创建者	：探长科技，www.explorersoftware.taobao.com
//*/
//TouchXY WINDOW_Scan(const GUI_WIDGET_CREATE_INFO* CREATE_INFO,u32 count,WM_MESSAGE* msg)
//{
//      static u8 flagPressed = 0;
//      u32 j=0;
//      TouchXY tp_dev = {0};
//            
//      tp_dev = Read_Piont();
//           
//      for(j=1;j<count;j++)    //枚举
//      {
//            if(tp_dev.x>CREATE_INFO[j].x0 && tp_dev.y>CREATE_INFO[j].y0 && tp_dev.x<(CREATE_INFO[j].x0+ CREATE_INFO[j].xSize)&& tp_dev.y<(CREATE_INFO[j].y0+CREATE_INFO[j].ySize))
//            {                
//                        if(flagPressed == 0)
//                        {
//                              msg->MsgId = WM_NOTIFY_PARENT;
//                              msg->hWinSrc = CREATE_INFO[j].Id;
//                              msg->Data.v = WM_NOTIFICATION_CLICKED;
//                              flagPressed = 1;
//                             // printf("CREATE_INFO[j].Id = %d  ",CREATE_INFO[j].Id);
//                              //printf("%s\r\n",CREATE_INFO[j].pName);
//                        }else
//                        {
//                              msg->Data.v=0;
//                        }

//                  break;
//                 
//            }              
//      }

//      if(j>=count)   
//      {
//            if(flagPressed == 1) //松手
//            {
//                  msg->Data.v = WM_NOTIFICATION_RELEASED;
//                  flagPressed = 0;  
//                 // tp_dev.x = mouse->x +1;
//            }else                   //没有按到
//            {
//                  msg->Data.v = 0;
//                  msg->MsgId = 0;
//                  msg->hWinSrc = 0;
//            }
//            
//      }
//      
//     return tp_dev;
//}
/************************************************************************************
*-函数名称	：窗口按键扫描函数
*-参数	：
*返回值	：返回ID号和触摸坐标
*-函数功能	：
*-创建者	：蓬莱市博硕电子科技
*/
scan_Message WINDOW_Scan(const GUI_WIDGET_CREATE_INFO* CREATE_INFO,u32 count)
{
      u32 j=0;
      scan_Message scan_ms = {0};      
      scan_ms.tp_dev = Read_Piont();
           
      for(j=1;j<count;j++)    //枚举
      {
            if(IsPressed((Obj*)(&(CREATE_INFO[j].x0)),scan_ms.tp_dev))
            {                
                  
                  scan_ms.Id = CREATE_INFO[j].Id;
                  
                  WINDOW_Log("ispressed ID = %d",scan_ms.Id);
                  
                  return scan_ms;                 
            }              
      }
      
     return scan_ms;
}

//获得ID对应的句柄
WM_HWIN   WM_GetDialogItem(WM_HWIN hWin, int Id)
{      //printf("hWin[Id+1] = %x\r\n",hWin[Id+1]);
     return (WM_HWIN)hWin[Id+1];  
}

//获得句柄数组hWin[Id]的地址
u32* WM_GetAddrArray(WM_HWIN hWin, int Id)
{
      return &hWin[Id+1];
}

/*
      重写图形BUILDER函数
      获得父窗口句柄 
*/

WM_HWIN WM_GetDesktopWindow(void) 
{
      return 0;
}



/*
      重写图形BUILDER函数
      窗口创建函数     
*/

WM_HWIN WINDOW_CreateIndirect(const GUI_WIDGET_CREATE_INFO * pCreate, WM_HWIN hWin, int x0, int y0, WM_CALLBACK * cb)
{
      
      Window* wd = NewWindow();      
      wd->x = x0;
      wd->y = y0;
      wd->height = pCreate->ySize;
      wd->width = pCreate->xSize;
      wd->titlestr = pCreate->pName;
      wd->BKcolor = pCreate->Flags;
      DrawWindow(wd);      
      *hWin = (u32)wd;
      
      return hWin;
}

/*
      重写图形BUILDER函数
      创建对话框,hwinArray[ID]记录ID号,
注意: 动态内存申请函数决定了ID号不会超过255
      应当接收WM_HWIN指针,当结束窗口时用GUI_FreeDialogBox销毁,释放内存
*/

WM_HWIN   GUI_CreateDialogBox(const GUI_WIDGET_CREATE_INFO* CREATE_INFO,u32 count,\
      WM_CALLBACK * cb,WM_HWIN hParent, int x0, int y0)
{
      u32 i ; 
      WM_HWIN hwinArray = Creat_HandleArray(count+1); //保持资源表的句柄数组指针,[0]保存申请的个数
      hwinArray[0] = count;   //保存申请的个数
      hParent = hParent;
      x0 = x0;
      y0 = y0;
      for(i=0;i<count;i++)
      {             
            CREATE_INFO[i].pfCreateIndirect(&CREATE_INFO[i],(hwinArray+1+CREATE_INFO[i].Id),\
            CREATE_INFO[i].x0,CREATE_INFO[i].y0,cb);                        
            //printf("hwinArray[%d] = %x\r\n",i+1,hwinArray[i+1]);
      }
      
      CALLBACK_Fun = *cb;
      msg.MsgId = WM_INIT_DIALOG;   
      //countINFO = count;      //保存数组的大小
      return hwinArray;
}

//获取ID号
int WM_GetId(int ID)
{
      return ID;
}

//缺省函数
WM_HWIN WM_DefaultProc(WM_MESSAGE * pMsg)
{
      return 0 ;
}

/************************************************************************************
*-函数名称	：完整销毁窗口
*-参数	：创建时保存的窗口句柄
*返回值	：成功 ISOK,失败 ISERROR
*-函数功能	：销毁资源表整个窗口,和控件的动态申请内存
*-创建者	：蓬莱市博硕电子科技
*/
Flag_Status GUI_FreeDialogBox(WM_HWIN hwinArray)
{
      u8 i = 0;
      u8 count = hwinArray[0];
      //先销毁句柄中的控件
      for(i=1;i<(count+2);i++)
      {
            FreeCUI_Mallco((u8*)hwinArray[i]);
                
      }          
      //再销毁句柄数组
      FreeCUI_Mallco((u8*)hwinArray);

      return ISOK;
      
}

































