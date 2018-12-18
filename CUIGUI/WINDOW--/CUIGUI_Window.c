#include "CUIGUI_Window.H"
#include "MYTOUCH.H"
#include "CUIGUI_LCDDRIVER.H"
#include "CUIGUI_COLOR.H"
#include "CUI_MALLCO.H"
#include "CUIGUI_FONT.H"


/************************************************************************************
*-��������	��WinƤ�� 
*-˵��	��
*����ֵ	��
*-��������	��
*-������	����˶���ӿƼ�
*/

void WinSkin(Window* window)
{
      window->BKcolor = WINDOW_DEFAULT_BACK_COLOR;
      window->titleColor = WINDOW_DEFAULT_TITLE_COLOR;
      CUIGUI_FillRect( window->x,window->y,window->width,window->height ,window->BKcolor);
      CUIGUI_FillRect( window->x,window->y,window->width,window->title_hight ,window->titleColor);
}


/************************************************************************************
*-��������	��WinĬ��Ƥ�� 
*-˵��	��
*����ֵ	��
*-��������	��
*-������	����˶���ӿƼ�
*/

void WinDefaultSkin(Window* window)
{
      WinSkin(window);
}

/************************************************************************************
*-��������	�������´���,������ֵ
*-����	��
*����ֵ	��
*-��������	��
*-������	�������в�˶���ӿƼ�
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

//���ڱ���
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
*-��������	����������,��������
*-����	��
*����ֵ	��
*-��������	��
*-������	�������в�˶���ӿƼ�
*/
void DrawWindow(Window* window)
{                      
      window->WinSkinFun(window);                           //Ƥ��
      WindowText(window);                                   //�� 
}

//���ô��ڱ�����ɫ
void WindowSetColor(Window* window,COLOR_DATTYPE fontColor)
{
      window->fontColor = fontColor;
      DrawWindow(window);
}




/************************************************************************************
*-��������	������һ���������ڴ�����
*-����	��[0]��������ĸ���
*����ֵ	��
*-��������	��ע��: ��̬�ڴ��������ֽ�Ϊ��λ,������ID�Ų��ᳬ��255..
*-������	�������в�˶���ӿƼ�
*/
WM_HWIN Creat_HandleArray(u32 handlesize)
{
      WM_HWIN wm_handle = (WM_HWIN)CUI_Mallco(handlesize);   //�����ڴ�,[0]��������ĸ���
               
      return wm_handle;
}



/************************************************************************************
*-��������	�����پ������
*-����	��
*����ֵ	���ɹ�����1�����ɹ�����0
*-��������	��
*-������	�������в�˶���ӿƼ�
*/
Flag_Status Del_HandleArray(WM_HWIN hd_array)
{
      return (FreeCUI_Mallco((u8*)hd_array));
}




///************************************************************************************
//*-��������	�����ڰ���ɨ��
//*-����	��
//*����ֵ	��msg��Ϣ,����а����ͻ᷵��ID������ʱmsg->Data.v = WM_NOTIFICATION_RELEASED
//                  �ڻص������д���
//*-��������	��
//*-������	��̽���Ƽ���www.explorersoftware.taobao.com
//*/
//TouchXY WINDOW_Scan(const GUI_WIDGET_CREATE_INFO* CREATE_INFO,u32 count,WM_MESSAGE* msg)
//{
//      static u8 flagPressed = 0;
//      u32 j=0;
//      TouchXY tp_dev = {0};
//            
//      tp_dev = Read_Piont();
//           
//      for(j=1;j<count;j++)    //ö��
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
//            if(flagPressed == 1) //����
//            {
//                  msg->Data.v = WM_NOTIFICATION_RELEASED;
//                  flagPressed = 0;  
//                 // tp_dev.x = mouse->x +1;
//            }else                   //û�а���
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


//���ID��Ӧ�ľ��
WM_HWIN   WM_GetDialogItem(WM_HWIN hWin, int Id)
{      
     return (WM_HWIN)hWin[Id];  
}

//��þ������hWin[Id]�ĵ�ַ
u32* WM_GetAddrArray(WM_HWIN hWin, int Id)
{
      return &hWin[Id];
}

/*
      ��дͼ��BUILDER����
      ��ø����ھ�� 
*/

WM_HWIN WM_GetDesktopWindow(void) 
{
      return 0;
}



/*
      ��дͼ��BUILDER����
      ���ڴ�������     
*/

WM_HWIN WINDOW_CreateIndirect(const GUI_WIDGET_CREATE_INFO * pCreate, WM_HWIN hWin, int x0, int y0, WM_CALLBACK * cb)
{
      
      Window* wd = NewWindow();      
      wd->x = x0;
      wd->y = y0;
      wd->height = pCreate->ySize;
      wd->width = pCreate->xSize;
      wd->titlestr = pCreate->pName;
      DrawWindow(wd);      
      *hWin = (u32)wd;
      
      return hWin;
}

/*
      ��дͼ��BUILDER����
      �����Ի���,hwinArray[ID]��¼ID��,
ע��: ��̬�ڴ����뺯��������ID�Ų��ᳬ��255
*/

WM_HWIN   GUI_CreateDialogBox(const GUI_WIDGET_CREATE_INFO* CREATE_INFO,u32 count,\
      WM_CALLBACK * cb,WM_HWIN hParent, int x0, int y0)
{
      u32 i ;
      WM_HWIN hwinArray = Creat_HandleArray(count+1); //������Դ��ľ������ָ��,[0]��������ĸ���
      hwinArray[0] = count;   //��������ĸ���
      hParent = hParent;
      x0 = x0;
      y0 = y0;
      for(i=0;i<count;i++)
      {             
            CREATE_INFO[i].pfCreateIndirect(&CREATE_INFO[i],(hwinArray+1+CREATE_INFO[i].Id),\
            CREATE_INFO[i].x0,CREATE_INFO[i].y0,cb);                       
            
      }
      
      CALLBACK_Fun = *cb;
      msg.MsgId = WM_INIT_DIALOG;   
      
      return hwinArray;
}

//��ȡID��
int WM_GetId(int ID)
{
      return ID;
}

//ȱʡ����
WM_HWIN WM_DefaultProc(WM_MESSAGE * pMsg)
{
      return 0 ;
}

/************************************************************************************
*-��������	���������ٴ���
*-����	������ʱ����Ĵ��ھ��
*����ֵ	���ɹ� ISOK,ʧ�� ISERROR
*-��������	��������Դ����������,�Ϳؼ��Ķ�̬�����ڴ�
*-������	�������в�˶���ӿƼ�
*/
Flag_Status GUI_FreeDialogBox(WM_HWIN hwinArray)
{
      u8 i = 0;
      u8 count = hwinArray[0];
      //�����پ���еĿؼ�
      for(i=1;i<(count+2);i++)
      {
            FreeCUI_Mallco((u8*)hwinArray[i]);
                
      }          
      //�����پ������
      FreeCUI_Mallco((u8*)hwinArray);

      return ISOK;
      
}

































