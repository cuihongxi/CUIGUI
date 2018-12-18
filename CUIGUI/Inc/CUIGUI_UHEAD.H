/******************************************************************
*文件名:公共头文件
*创建者:博硕电子科技，www.explorersoftware.taobao.com
*创建时间:2017年6月28日08:22:01
*文件说明:包含公共使用函数，和头文件
*/

#ifndef  __CUIGUI_UHEAD_H  
#define  __CUIGUI_UHEAD_H 

#include "CUIGUI_UDATA.H"
#include "CUIGUI_LCDDRIVER.H"


#define BKCOLLOR             CUIGUI_Color(240,240,240)      //背景
#define BKEDGE0COLLOR        CUIGUI_Color(160,160,160)      //边沿
#define BKEDGE1COLLOR        CUIGUI_Color(227,227,227)      //边沿
#define BKEDGE2COLLOR        CUIGUI_Color(105,105,105)      //边沿
#define BKEDGE3COLLOR        WHITE                          //边沿



/*********************************************************
    函数
*/

      
u32 GetCircle_X(u32 x0,u32 y0,u16 r,u16 y1);//根据坐标和半径，返回与y1相对应的x1坐标
    
void Obj_SkinUnPressed(Obj* obj); 
void Obj_SkinPressed(Obj* obj);

#endif
