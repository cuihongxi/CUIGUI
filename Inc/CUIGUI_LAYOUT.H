/******************************************************************
*�ļ���:���ֹ�����
*������:�����в�˶���ӿƼ�
*����ʱ��:2018��1��18��21:26:17
*�ļ�˵��:����CUIGUI
*/
#ifndef  __CUIGUI_LAYOUT_H   
#define  __CUIGUI_LAYOUT_H 

#include "CUIGUI_UDATA.H"

//���λ��
typedef   enum 
{
      RelaPos_LEFT      = 0x01,        //��
      RelaPos_RIGHT     = 0x02,        //��            
      RelaPos_TOP       = 0X3,         //����
      RelaPos_BOTTOM    = 0x4,         //�ױ�
      
      RelaPos_LEFT_ALIGN      = 0x11,          //�����
      RelaPos_RIGHT_ALIGN     = 0x12,          //�Ҷ���
      RelaPos_TOP_ALIGN       = 0X13,          //���߶���
      RelaPos_BOTTOM_ALIGN   = 0x14,           //�ױ߶���     
}RelaPos;



void FlowLayout(void* thisObject, void* thatObject ,u16 distance ,RelaPos dir);



#endif

