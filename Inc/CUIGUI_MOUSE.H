/******************************************************************
*�ļ���:���ؼ�
*������:̽���Ƽ���www.explorersoftware.taobao.com
*����ʱ��:2017��8��31��09:29:06
*�ļ�˵��:����CUIGUI
*/
#ifndef  __CUIGUI_MOUSE_H   
#define  __CUIGUI_MOUSE_H  

#include "CUIGUI_UDATA.H"


typedef struct 
{
      u16 x ;                                   //�������x
      u16 y ;                                   //�������y
      u16 width;
      u16 height;
      COLOR_DATTYPE datbuf[25][16];               //�洢���ǵ�����
      const u8* map;                            //���ͼƬ
      const u8* map2;
}Mouse;



extern      Mouse* mouse;


Mouse* CUIGUI_NewMouse(void);//�½�һ�����
void CUIGUI_DrawMouse(u16 x0, u16 y0,Mouse* mouse);//�������

void CUIGUI_MOUSE_Move(u16 x0,u16 y0,Mouse* mouse);//����ƶ�


#endif   
