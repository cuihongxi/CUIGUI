/******************************************************************
*文件名:布局管理器
*创建者:蓬莱市博硕电子科技
*创建时间:2018年1月18日21:26:17
*文件说明:基于CUIGUI
*/
#ifndef  __CUIGUI_LAYOUT_H   
#define  __CUIGUI_LAYOUT_H 

#include "CUIGUI_UDATA.H"

//相对位置
typedef   enum 
{
      RelaPos_LEFT      = 0x01,        //左
      RelaPos_RIGHT     = 0x02,        //右            
      RelaPos_TOP       = 0X3,         //顶边
      RelaPos_BOTTOM    = 0x4,         //底边
      
      RelaPos_LEFT_ALIGN      = 0x11,          //左对齐
      RelaPos_RIGHT_ALIGN     = 0x12,          //右对齐
      RelaPos_TOP_ALIGN       = 0X13,          //顶边对齐
      RelaPos_BOTTOM_ALIGN   = 0x14,           //底边对齐     
}RelaPos;



void FlowLayout(void* thisObject, void* thatObject ,u16 distance ,RelaPos dir);



#endif


