#include "CUIGUI_COLOR.H"

/************************************************************************************
*-��������      ��GUI��ɫ��ȡ
*-����          ��
*����ֵ         ��
*-��������      ��
*-������		  �������в�˶���ӿƼ�
*/

COLOR_DATTYPE CUIGUI_Color(u8 r,u8 g,u8 b)
{
  // return (((COLOR_DATTYPE)r>>(8-RGB_R))<<(RGB_G + RGB_B)) | (((COLOR_DATTYPE)g>>(8-RGB_G))<<(RGB_B)) | \
      (((COLOR_DATTYPE)b>>(8-RGB_B))) ;

      return (((COLOR_DATTYPE)r>>3)<<11) | (((COLOR_DATTYPE)g>>2)<<5) | \
      (((COLOR_DATTYPE)b>>3)) ;
}


/************************************************************************************
*-��������	��
*-����	��24λɫת�����Լ���Ҫ����ɫ,���������д
*����ֵ	��
*-��������	��
*-������	�������в�˶���ӿƼ�
*/

COLOR_DATTYPE CUIGUI_GetColor(u32 color)
{
      RGB_Struct rgb;
      rgb.R = (u8)(color>>16);
      rgb.G = (u8)(color>>8);
      rgb.B = (u8)(color);
      
      return CUIGUI_Color(rgb.R,rgb.G,rgb.B);
}

/************************************************************************************
*-��������	��565��ʽ���RGB��ֵ
*-����	��
*����ֵ	��
*-��������	��
*-������	�������в�˶���ӿƼ�
*/
void CUIGUI_GETRGB(COLOR_DATTYPE color,RGB_Struct* rgb)
{
      rgb->R = (u8)((color >> 11)<<3);
      rgb->G = (u8)((color>>5)<<2);
      rgb->B = (u8)(color<<3);
}