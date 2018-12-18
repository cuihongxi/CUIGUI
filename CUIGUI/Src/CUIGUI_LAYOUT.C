#include "CUIGUI_LAYOUT.H"
#include "stdio.h"

/************************************************************************************
*-函数名称	：相对布局
*-参数	：thisobj 当前的控件 ,thatObj 相对目标控件 distance 距离像素
*返回值	：
*-函数功能	：
*-创建者	：蓬莱市博硕电子科技
*/
void FlowLayout(void* thisObject, void* thatObject ,u16 distance ,RelaPos dir)
{
      u8 align = 0x10;
			Obj* thisObj = (Obj*)thisObject;
			Obj* thatobj = (Obj*)thisObject;
      align &= (u8)dir;
      switch(dir&0XEF)
      {
            case RelaPos_LEFT:
                  thisObj->x = thatobj->x - distance - thisObj->width;
                  if(align)thisObj->y = thatobj->y;
                  break;
            case RelaPos_RIGHT:
                  thisObj->x = thatobj->x + distance + thatobj->width;
                  if(align)thisObj->y = thatobj->y;
                  break;
            case RelaPos_TOP:
                  thisObj->y = thatobj->y - distance - thisObj->height;
                  if(align)thisObj->x = thatobj->x;
                  break;
            case RelaPos_BOTTOM:
                  thisObj->y = thatobj->y + distance + thatobj->height;
                  if(align)thisObj->x = thatobj->x;
            
                  break;
      }
      
      
}

