/******************************************************************
*文件名:鼠标控件
*创建者:探长科技，www.explorersoftware.taobao.com
*创建时间:2017年8月31日09:29:06
*文件说明:基于CUIGUI
*/
#ifndef  __CUIGUI_MOUSE_H   
#define  __CUIGUI_MOUSE_H  

#include "CUIGUI_UDATA.H"


typedef struct 
{
      u16 x ;                                   //起点坐标x
      u16 y ;                                   //起点坐标y
      u16 width;
      u16 height;
      COLOR_DATTYPE datbuf[25][16];               //存储覆盖的数据
      const u8* map;                            //鼠标图片
      const u8* map2;
}Mouse;



extern      Mouse* mouse;


Mouse* CUIGUI_NewMouse(void);//新建一个鼠标
void CUIGUI_DrawMouse(u16 x0, u16 y0,Mouse* mouse);//画出鼠标

void CUIGUI_MOUSE_Move(u16 x0,u16 y0,Mouse* mouse);//鼠标移动


#endif   

