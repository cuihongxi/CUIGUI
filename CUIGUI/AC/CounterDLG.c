

#include "CounterDLG.h"
#include "CUIGUI_FONT.H"
#include<usart.h>
#include "CUIGUI_BMP.H"
#include "CUIGUI_BUTTON.H"
#include "CUIGUI_Window.H"
#include "CUIGUI_EDIT.H"
#include "CUIGUI_UHEAD.H"
#include "MYMALLCO.H"

 const GUI_WIDGET_CREATE_INFO WindowCreate0[] = {  
  { BMP_CreateIndirect,FILE_NAME3, 0, 45,50, 0, 0,    0, 6, 0 },
  { BMP_CreateIndirect,FILE_NAME6, 0, 190,50, 0, 0,   0, 6, 0 },
  { BMP_CreateIndirect,FILE_NAME1, 0, 335,50, 0, 0,   0, 6, 0 },
  { BUTTON_CreateIndirect,FILE_NAME2, 2,45,200, 100, 100,1, 6, 0 },
  { BMP_CreateIndirect,FILE_NAME4, 0,190,200, 0, 0,   0, 6, 0 },
  { BMP_CreateIndirect,FILE_NAME5, 0, 335,200, 0, 0,  0, 6, 0 },
  { BMP_CreateIndirect,FILE_NAME7, 0,45,350, 0, 0,    0, 6, 0 },
  { BMP_CreateIndirect,FILE_NAME13, 0,45,650, 0, 0,   0, 6, 0 },
  { BMP_CreateIndirect,FILE_NAME9, 0, 190,650, 0, 0,  0, 6, 0 },
  { BMP_CreateIndirect,FILE_NAME8, 0, 335,650, 0, 0,  0, 6, 0 },
  { BUTTON_CreateIndirect,FILE_NAME11, 1, 190, 350, 100, 100, 1, 6, 0 },
//{ BMP_CreateIndirect,FILE_NAME15, 0, 190,470, 0, 0,  0, 6, 0 },
//{ BMP_CreateIndirect,FILE_NAME16, 0, 50,470, 0, 0,  0, 6, 0 },
};

// USER END

/*********************************************************************
*
*       _aDialogCreate
*/
 const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { WINDOW_CreateIndirect, "Counter", ID_WINDOW_0, 0, 0, 480, 800, 0, 0x0, 0 },
  { EDIT_CreateIndirect, "Edit", ID_EDIT_0, 45, 80, 389, 127, 0, 0x64, 0 },
  { BUTTON_CreateIndirect, "CE", ID_BUTTON_0, 45, 260, 75, 55, 0, 8, 0 },
  { BUTTON_CreateIndirect, "×", ID_BUTTON_1, 150, 260, 75, 55, 0, 8, 0 },
  { BUTTON_CreateIndirect, "(", ID_BUTTON_2, 255, 260, 75, 55, 0, 8, 0 },
  { BUTTON_CreateIndirect, ")", ID_BUTTON_3, 360, 260, 75, 55, 0, 8, 0 },
  { BUTTON_CreateIndirect, "7", ID_BUTTON_4, 45, 340, 75, 55, 0, 8, 0 },
  { BUTTON_CreateIndirect, "8", ID_BUTTON_5, 150, 340, 75, 55, 0, 8, 0 },
  { BUTTON_CreateIndirect, "9", ID_BUTTON_6, 255, 340, 75, 55, 0, 8, 0 },
  { BUTTON_CreateIndirect, "4", ID_BUTTON_7, 45, 420, 75, 55, 0, 8, 0 },
  { BUTTON_CreateIndirect, "5", ID_BUTTON_8, 150, 420, 75, 55, 0, 8, 0 },
  { BUTTON_CreateIndirect, "6", ID_BUTTON_9, 255, 420, 75, 55, 0, 8, 0 },
  { BUTTON_CreateIndirect, "1", ID_BUTTON_10, 45, 500, 75, 55, 0, 8, 0 },
  { BUTTON_CreateIndirect, "2", ID_BUTTON_11, 150, 500, 75, 55, 0, 8, 0 },
  { BUTTON_CreateIndirect, "=", ID_BUTTON_12, 255, 580, 75, 55, 0, 8, 0 },
  { BUTTON_CreateIndirect, "3", ID_BUTTON_13, 255, 500, 75, 55, 0, 8, 0 },
  { BUTTON_CreateIndirect, "0", ID_BUTTON_14, 45, 580, 75, 55, 0, 8, 0 },
  { BUTTON_CreateIndirect, "+", ID_BUTTON_15, 360, 340, 75, 55, 0, 8, 0 },
  { BUTTON_CreateIndirect, "-", ID_BUTTON_16, 360, 420, 75, 55, 0, 8, 0 },
  { BUTTON_CreateIndirect, "*", ID_BUTTON_17, 360, 500, 75, 55, 0, 8, 0 },
  { BUTTON_CreateIndirect, "/", ID_BUTTON_18, 360, 580, 75, 55, 0, 8, 0 },
  { BUTTON_CreateIndirect, ".", ID_BUTTON_19, 150, 580, 75, 55, 0, 8, 0 },
  { BUTTON_CreateIndirect, "×", ID_BUTTON_20, 440, 10, 30, 30, 0, 8, 0 },//"1:/off.bmp"

};

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/

// USER START (Optionally insert additional static code)
void CbFun(WM_MESSAGE * pMsg) 
{
        u32* hItem;
        int     NCode;
        int     Id;
        switch (pMsg->MsgId) 
        {
            case WM_INIT_DIALOG:   
                  break;
            case WM_NOTIFY_PARENT:
                  Id = WM_GetId(pMsg->hWinSrc);
                  NCode = pMsg->Data.v;
                  switch(Id) {
                        case 0: msg.MsgId = 0;
                             // printf(" BMPCLICKED\r\n");
                              break;
                        case 1: 
                              switch(NCode) 
                                    {
                                          case WM_NOTIFICATION_CLICKED:
                                                
                                                hItem = WM_GetDialogItem(hwinArray,1);
                                                ButtonSetPressed(hItem,BT_PRESSED);                                                
                                                                                                                                        

                                               
                                            break;
                                          case WM_NOTIFICATION_RELEASED:

                                               for(Id=0;Id<=2;Id++)
                                                {
                                                      FreeMyMallco((u8*)WM_GetDialogItem(hwinArray,Id));
                                                }
                                                Del_HandleArray(hwinArray);
                                                CUIGUI_SetFont(&GUI_Fontyahei40);                                           
                                                CreateCounter();
                                                 
                                            break;

                                    }
                                   
                         case 2: 
                              switch(NCode) 
                                    {
                                          case WM_NOTIFICATION_CLICKED:
                                                
                                                hItem = WM_GetDialogItem(hwinArray,2);
                                                ButtonSetPressed(hItem,BT_PRESSED);                                                
                                                                                                                                        
                                               
                                            break;
                                          case WM_NOTIFICATION_RELEASED:
                                                      
                                                hItem = WM_GetDialogItem(hwinArray,2);
                                                ButtonSetPressed(hItem,BT_UNPRESSED);  
                                            break;

                                    }
                                             
                              }
      }
}


// USER END

/*********************************************************************
*
*       _cbDialog
*/
static void _cbDialog(WM_MESSAGE * pMsg) 
{
  WM_HWIN hItem;
  int     NCode;
  int     Id;

  hItem = WM_GetDialogItem(hwinArray,ID_EDIT_0);
  EDIT_ShowCursor((Edit*)hItem,&flag_Edit_cursor);
      
  switch (pMsg->MsgId) 
    {
        case WM_INIT_DIALOG:
          break;
        case WM_NOTIFY_PARENT:
          Id    = WM_GetId(pMsg->hWinSrc);
          NCode = pMsg->Data.v;

          switch(Id) 
                {
                  case ID_EDIT_0: // Notifications sent by 'Edit'
                        switch(NCode) 
                              {
                                    case WM_NOTIFICATION_CLICKED:
                                      break;
                                    case WM_NOTIFICATION_RELEASED:
                                      break;
                                    case WM_NOTIFICATION_VALUE_CHANGED:
                                          

                                      break;

                              }
                        break;
                  case ID_BUTTON_0: // Notifications sent by 'CE'
                        switch(NCode) 
                              {
                                    case WM_NOTIFICATION_CLICKED:
                                          hItem = WM_GetDialogItem(hwinArray,ID_BUTTON_0);
                                          ButtonSetPressed(hItem,BT_PRESSED);
                                          hItem = WM_GetDialogItem(hwinArray,ID_EDIT_0);                                     
                                          CUIGUI_CEEdit((Edit*) hItem);
                                    
                                      break;
                                    case WM_NOTIFICATION_RELEASED:           
                                          hItem = WM_GetDialogItem(hwinArray,ID_BUTTON_0);
                                          ButtonSetPressed(hItem,BT_UNPRESSED);   
                                        //  CUIGUI_MOUSE_Move(mouse->x,mouse->y,mouse);//画出鼠标                                    
                                      break;

                              }
                        break;
          case ID_BUTTON_1: // Notifications sent by '×'
                  switch(NCode) 
                        {
                              case WM_NOTIFICATION_CLICKED:
                                    hItem = WM_GetDialogItem(hwinArray,ID_BUTTON_1);
                                    ButtonSetPressed(hItem,BT_PRESSED);
                                    hItem = WM_GetDialogItem(hwinArray,ID_EDIT_0);                                   
                                    EDIT_DelText((Edit*)hItem);           
                                    EditShowTXT((Edit*)hItem);  
                                break;
                              case WM_NOTIFICATION_RELEASED:
                                    hItem = WM_GetDialogItem(hwinArray,ID_BUTTON_1);
                                    ButtonSetPressed(hItem,BT_UNPRESSED);
                                   // CUIGUI_MOUSE_Move(mouse->x,mouse->y,mouse);//画出鼠标
                                break;
                        }
                  break;
          case ID_BUTTON_2: // Notifications sent by '('
                  switch(NCode) 
                        {
                              case WM_NOTIFICATION_CLICKED:
                                    hItem = WM_GetDialogItem(hwinArray,ID_BUTTON_2);
                                    ButtonSetPressed(hItem,BT_PRESSED);
                                    hItem = WM_GetDialogItem(hwinArray,ID_EDIT_0);
                                    
                                    EDIT_AddText((Edit*)hItem,(u8*)"(");            
                                    EditShowTXT((Edit*)hItem);
                                    
                                break;
                              case WM_NOTIFICATION_RELEASED:
                                    hItem = WM_GetDialogItem(hwinArray,ID_BUTTON_2);
                                    ButtonSetPressed(hItem,BT_UNPRESSED);
                                  //  CUIGUI_MOUSE_Move(mouse->x,mouse->y,mouse);//画出鼠标
                                break;

                        }
            break;
          case ID_BUTTON_3: // Notifications sent by ')'
                  switch(NCode) 
                        {
                              case WM_NOTIFICATION_CLICKED:
                                    hItem = WM_GetDialogItem(hwinArray,ID_BUTTON_3);
                                    ButtonSetPressed(hItem,BT_PRESSED);
                                    hItem = WM_GetDialogItem(hwinArray,ID_EDIT_0);
                                    EDIT_AddText((Edit*)hItem,")");
                                    EditShowTXT((Edit*)hItem);
                                break;
                              case WM_NOTIFICATION_RELEASED:
                                    hItem = WM_GetDialogItem(hwinArray,ID_BUTTON_3);
                                    ButtonSetPressed(hItem,BT_UNPRESSED);
                                //    CUIGUI_MOUSE_Move(mouse->x,mouse->y,mouse);//画出鼠标
                                break;

                        }
                  break;
          case ID_BUTTON_4: // Notifications sent by '7'
                  switch(NCode) 
                        {
                              case WM_NOTIFICATION_CLICKED:
                                    hItem = WM_GetDialogItem(hwinArray,ID_BUTTON_4);
                                    ButtonSetPressed(hItem,BT_PRESSED);
                                    hItem = WM_GetDialogItem(hwinArray,ID_EDIT_0);
                                    EDIT_AddText((Edit*)hItem,"7");
                                    EditShowTXT((Edit*)hItem);
                                break;
                              case WM_NOTIFICATION_RELEASED:
                                    hItem = WM_GetDialogItem(hwinArray,ID_BUTTON_4);
                                    ButtonSetPressed(hItem,BT_UNPRESSED);
                                //    CUIGUI_MOUSE_Move(mouse->x,mouse->y,mouse);//画出鼠标
                                break;

                        }
                  break;
          case ID_BUTTON_5: // Notifications sent by '8'
                  switch(NCode) 
                        {
                              case WM_NOTIFICATION_CLICKED:
                                    hItem = WM_GetDialogItem(hwinArray,ID_BUTTON_5);
                                    ButtonSetPressed(hItem,BT_PRESSED);
                                    hItem = WM_GetDialogItem(hwinArray,ID_EDIT_0);
                                    EDIT_AddText((Edit*)hItem,"8");
                                    EditShowTXT((Edit*)hItem);
                                break;
                              case WM_NOTIFICATION_RELEASED:
                                    hItem = WM_GetDialogItem(hwinArray,ID_BUTTON_5);
                                    ButtonSetPressed(hItem,BT_UNPRESSED);
                               //     CUIGUI_MOUSE_Move(mouse->x,mouse->y,mouse);//画出鼠标
                                break;

                        }
            break;
          case ID_BUTTON_6: // Notifications sent by '9'
                  switch(NCode) 
                        {
                              case WM_NOTIFICATION_CLICKED:
                                    hItem = WM_GetDialogItem(hwinArray,ID_BUTTON_6);
                                    ButtonSetPressed(hItem,BT_PRESSED);
                                    hItem = WM_GetDialogItem(hwinArray,ID_EDIT_0);
                                    EDIT_AddText((Edit*)hItem,"9");
                                    EditShowTXT((Edit*)hItem);
                                break;
                              case WM_NOTIFICATION_RELEASED:
                                    hItem = WM_GetDialogItem(hwinArray,ID_BUTTON_6);
                                    ButtonSetPressed(hItem,BT_UNPRESSED);
                               //     CUIGUI_MOUSE_Move(mouse->x,mouse->y,mouse);//画出鼠标
                                break;

                        }
                  break;
          case ID_BUTTON_7: // Notifications sent by '4'
                  switch(NCode) 
                        {
                              case WM_NOTIFICATION_CLICKED:
                                hItem = WM_GetDialogItem(hwinArray,ID_BUTTON_7);
                                    ButtonSetPressed(hItem,BT_PRESSED);
                                    hItem = WM_GetDialogItem(hwinArray,ID_EDIT_0);
                                    EDIT_AddText((Edit*)hItem,(u8*)"4");
                                    EditShowTXT((Edit*)hItem);
                              
                                break;
                              case WM_NOTIFICATION_RELEASED:
                                    hItem = WM_GetDialogItem(hwinArray,ID_BUTTON_7);
                                    ButtonSetPressed(hItem,BT_UNPRESSED);
                               //     CUIGUI_MOUSE_Move(mouse->x,mouse->y,mouse);//画出鼠标
                                break;

                        }
                  break;
          case ID_BUTTON_8: // Notifications sent by '5'
                  switch(NCode) 
                        {
                              case WM_NOTIFICATION_CLICKED:
                                hItem = WM_GetDialogItem(hwinArray,ID_BUTTON_8);
                                    ButtonSetPressed(hItem,BT_PRESSED);
                                    hItem = WM_GetDialogItem(hwinArray,ID_EDIT_0);
                                    EDIT_AddText((Edit*)hItem,(u8*)"5");
                                    EditShowTXT((Edit*)hItem);
                                break;
                              case WM_NOTIFICATION_RELEASED:
                                    hItem = WM_GetDialogItem(hwinArray,ID_BUTTON_8);
                                    ButtonSetPressed(hItem,BT_UNPRESSED);
                                 //   CUIGUI_MOUSE_Move(mouse->x,mouse->y,mouse);//画出鼠标
                                break;

                        }
                  break;
          case ID_BUTTON_9: // Notifications sent by '6'
                  switch(NCode) 
                        {
                              case WM_NOTIFICATION_CLICKED:
                                hItem = WM_GetDialogItem(hwinArray,ID_BUTTON_9);
                                    ButtonSetPressed(hItem,BT_PRESSED);
                                    hItem = WM_GetDialogItem(hwinArray,ID_EDIT_0);
                                    EDIT_AddText((Edit*)hItem,"6");
                                    EditShowTXT((Edit*)hItem);
                                break;
                              case WM_NOTIFICATION_RELEASED:
                                    hItem = WM_GetDialogItem(hwinArray,ID_BUTTON_9);
                                    ButtonSetPressed(hItem,BT_UNPRESSED);
                                 //   CUIGUI_MOUSE_Move(mouse->x,mouse->y,mouse);//画出鼠标
                                break;

                        }
                  break;
          case ID_BUTTON_10: // Notifications sent by '1'
                  switch(NCode) 
                        {
                              case WM_NOTIFICATION_CLICKED:
                                    hItem = WM_GetDialogItem(hwinArray,ID_BUTTON_10);
                                    ButtonSetPressed(hItem,BT_PRESSED);
                                    hItem = WM_GetDialogItem(hwinArray,ID_EDIT_0);
                                    EDIT_AddText((Edit*)hItem,"1");
                                    EditShowTXT((Edit*)hItem);
                                break;
                              case WM_NOTIFICATION_RELEASED:
                                    hItem = WM_GetDialogItem(hwinArray,ID_BUTTON_10);
                                    ButtonSetPressed(hItem,BT_UNPRESSED);
                                //    CUIGUI_MOUSE_Move(mouse->x,mouse->y,mouse);//画出鼠标
                                break;

                        }
                  break;
          case ID_BUTTON_11: // Notifications sent by '2'
                  switch(NCode) 
                        {
                              case WM_NOTIFICATION_CLICKED:
                                    hItem = WM_GetDialogItem(hwinArray,ID_BUTTON_11);
                                    ButtonSetPressed(hItem,BT_PRESSED);
                                          hItem = WM_GetDialogItem(hwinArray,ID_EDIT_0);
                                    EDIT_AddText((Edit*)hItem,(u8*)"2");
                                    EditShowTXT((Edit*)hItem);
                                break;
                              case WM_NOTIFICATION_RELEASED:
                                    hItem = WM_GetDialogItem(hwinArray,ID_BUTTON_11);
                                    ButtonSetPressed(hItem,BT_UNPRESSED);
                               //     CUIGUI_MOUSE_Move(mouse->x,mouse->y,mouse);//画出鼠标
                                break;

                        }
                  break;
          case ID_BUTTON_12: // Notifications sent by '='
                  switch(NCode) 
                        {
                              case WM_NOTIFICATION_CLICKED:
                                    hItem = WM_GetDialogItem(hwinArray,ID_BUTTON_12);
                                    ButtonSetPressed(hItem,BT_PRESSED);
                                          hItem = WM_GetDialogItem(hwinArray,ID_EDIT_0);
                                    EDIT_AddText((Edit*)hItem,"=");
                                    EditShowTXT((Edit*)hItem);
                                break;
                              case WM_NOTIFICATION_RELEASED:
                                    hItem = WM_GetDialogItem(hwinArray,ID_BUTTON_12);
                                    ButtonSetPressed(hItem,BT_UNPRESSED);
                                //    CUIGUI_MOUSE_Move(mouse->x,mouse->y,mouse);//画出鼠标
                                break;

                        }
                  break;
          case ID_BUTTON_13: // Notifications sent by '3'
                  switch(NCode) 
                        {
                              case WM_NOTIFICATION_CLICKED:
                                hItem = WM_GetDialogItem(hwinArray,ID_BUTTON_13);
                                    ButtonSetPressed(hItem,BT_PRESSED);
                                          hItem = WM_GetDialogItem(hwinArray,ID_EDIT_0);
                                    EDIT_AddText((Edit*)hItem,(u8*)"3");
                                    EditShowTXT((Edit*)hItem);
                                break;
                              case WM_NOTIFICATION_RELEASED:
                                    hItem = WM_GetDialogItem(hwinArray,ID_BUTTON_13);
                                    ButtonSetPressed(hItem,BT_UNPRESSED);
                                //    CUIGUI_MOUSE_Move(mouse->x,mouse->y,mouse);//画出鼠标
                                break;

                        }
                  break;
          case ID_BUTTON_14: // Notifications sent by '0'
                  switch(NCode) 
                        {
                              case WM_NOTIFICATION_CLICKED:
                                    hItem = WM_GetDialogItem(hwinArray,ID_BUTTON_14);
                                    ButtonSetPressed(hItem,BT_PRESSED);
                                          hItem = WM_GetDialogItem(hwinArray,ID_EDIT_0);
                                    EDIT_AddText((Edit*)hItem,"0");
                                    EditShowTXT((Edit*)hItem);
                                break;
                              case WM_NOTIFICATION_RELEASED:
                                    hItem = WM_GetDialogItem(hwinArray,ID_BUTTON_14);
                                    ButtonSetPressed(hItem,BT_UNPRESSED);
                                //    CUIGUI_MOUSE_Move(mouse->x,mouse->y,mouse);//画出鼠标
                                break;

                        }
                  break;
          case ID_BUTTON_15: // Notifications sent by '+'
                  switch(NCode) 
                        {
                              case WM_NOTIFICATION_CLICKED:
                                    hItem = WM_GetDialogItem(hwinArray,ID_BUTTON_15);
                                    ButtonSetPressed(hItem,BT_PRESSED);
                                          hItem = WM_GetDialogItem(hwinArray,ID_EDIT_0);
                                    EDIT_AddText((Edit*)hItem,(u8*)"+");
                                    EditShowTXT((Edit*)hItem);
                                break;
                              case WM_NOTIFICATION_RELEASED:
                                    hItem = WM_GetDialogItem(hwinArray,ID_BUTTON_15);
                                    ButtonSetPressed(hItem,BT_UNPRESSED);
                                //    CUIGUI_MOUSE_Move(mouse->x,mouse->y,mouse);//画出鼠标
                                break;

                        }
                  break;
          case ID_BUTTON_16: // Notifications sent by '-'
                  switch(NCode) 
                        {
                              case WM_NOTIFICATION_CLICKED:
                                    hItem = WM_GetDialogItem(hwinArray,ID_BUTTON_16);
                                    ButtonSetPressed(hItem,BT_PRESSED);
                                    hItem = WM_GetDialogItem(hwinArray,ID_EDIT_0);
                                    EDIT_AddText((Edit*)hItem,"-");
                                    EditShowTXT((Edit*)hItem);
                                break;
                              case WM_NOTIFICATION_RELEASED:
                                    hItem = WM_GetDialogItem(hwinArray,ID_BUTTON_16);
                                    ButtonSetPressed(hItem,BT_UNPRESSED);
                                 //   CUIGUI_MOUSE_Move(mouse->x,mouse->y,mouse);//画出鼠标
                                break;

                        }
                  break;
          case ID_BUTTON_17: // Notifications sent by '*'
                  switch(NCode) 
                        {
                              case WM_NOTIFICATION_CLICKED:
                                    hItem = WM_GetDialogItem(hwinArray,ID_BUTTON_17);
                                    ButtonSetPressed(hItem,BT_PRESSED);
                                          hItem = WM_GetDialogItem(hwinArray,ID_EDIT_0);
                                    EDIT_AddText((Edit*)hItem,(u8*)"*");
                                    EditShowTXT((Edit*)hItem);
                                break;
                              case WM_NOTIFICATION_RELEASED:
                                    hItem = WM_GetDialogItem(hwinArray,ID_BUTTON_17);
                                    ButtonSetPressed(hItem,BT_UNPRESSED);
                                //    CUIGUI_MOUSE_Move(mouse->x,mouse->y,mouse);//画出鼠标
                                break;

                        }
                  break;
          case ID_BUTTON_18: // Notifications sent by '/'
                  switch(NCode) 
                        {
                              case WM_NOTIFICATION_CLICKED:
                                    hItem = WM_GetDialogItem(hwinArray,ID_BUTTON_18);
                                    ButtonSetPressed(hItem,BT_PRESSED);
                                    hItem = WM_GetDialogItem(hwinArray,ID_EDIT_0);
                                    EDIT_AddText((Edit*)hItem,"/");
                                    EditShowTXT((Edit*)hItem);
                                break;
                              case WM_NOTIFICATION_RELEASED:
                                    hItem = WM_GetDialogItem(hwinArray,ID_BUTTON_18);
                                    ButtonSetPressed(hItem,BT_UNPRESSED);
                               //     CUIGUI_MOUSE_Move(mouse->x,mouse->y,mouse);//画出鼠标
                                break;

                        }
                  break;
          case ID_BUTTON_19: // Notifications sent by '.'
                  switch(NCode) 
                        {
                              case WM_NOTIFICATION_CLICKED:
                                    hItem = WM_GetDialogItem(hwinArray,ID_BUTTON_19);
                                    ButtonSetPressed(hItem,BT_PRESSED);
                                    hItem = WM_GetDialogItem(hwinArray,ID_EDIT_0);
                                    EDIT_AddText((Edit*)hItem,(u8*)".");
                                    EditShowTXT((Edit*)hItem);
                                break;
                              case WM_NOTIFICATION_RELEASED:
                                    hItem = WM_GetDialogItem(hwinArray,ID_BUTTON_19);
                                    ButtonSetPressed(hItem,BT_UNPRESSED);
                              //      CUIGUI_MOUSE_Move(mouse->x,mouse->y,mouse);//画出鼠标
                                break;

                        }
                  break;
            case ID_BUTTON_20: // Notifications sent by 'back'
                  switch(NCode) 
                        {
                              case WM_NOTIFICATION_CLICKED:
                                    hItem = WM_GetDialogItem(hwinArray,ID_BUTTON_20);
                                    ButtonSetPressed(hItem,BT_PRESSED);
                                break;
                              case WM_NOTIFICATION_RELEASED:
                                    
                                    hItem = WM_GetDialogItem(hwinArray,ID_EDIT_0);
                                    FreeMyMallco(((Edit*)hItem)->str);  //先释放编辑框内存
                                    for(Id=0;Id<=ID_BUTTON_20;Id++)
                                    {
                                          FreeMyMallco((u8*)WM_GetDialogItem(hwinArray,Id));
                                    }
                                    Del_HandleArray(hwinArray);
                                    CreateAC1();         
                                break;

                        }
                  break;
          }
          break;

        default:
            WM_DefaultProc(pMsg);
          break;
    }
}

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       CreateCounter
*/


WM_HWIN CreateCounter(void) {
  WM_HWIN hWin;
      
  hwinArray = Creat_HandleArray(20);
      
  hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);

  return hWin;
}


// USER START (Optionally insert additional public code)
void CreateAC1(void)
{
       hwinArray = Creat_HandleArray(5);
      
       BIG_BMP_Show(0,0,FILE_NAME10);//背景
       GUI_CreateDialogBox(WindowCreate0, GUI_COUNTOF(WindowCreate0),CbFun, WM_HBKWIN, 0, 0);
}

// USER END

/*************************** End of file ****************************/
