#include "CUIGUI_COLOR.H"

/************************************************************************************
*-函数名称      ：GUI颜色获取
*-参数          ：
*返回值         ：
*-函数功能      ：
*-创建者		：蓬莱市博硕电子科技
*/

COLOR_DATTYPE CUIGUI_Color(u8 r,u8 g,u8 b)
{
  // return (((COLOR_DATTYPE)r>>(8-RGB_R))<<(RGB_G + RGB_B)) | (((COLOR_DATTYPE)g>>(8-RGB_G))<<(RGB_B)) | \
      (((COLOR_DATTYPE)b>>(8-RGB_B))) ;

      return (((COLOR_DATTYPE)r>>3)<<11) | (((COLOR_DATTYPE)g>>2)<<5) | \
      (((COLOR_DATTYPE)b>>3)) ;
}


/************************************************************************************
*-函数名称	：
*-参数	：24位色转换成自己需要的颜色,根据情况改写
*返回值	：
*-函数功能	：
*-创建者	：蓬莱市博硕电子科技
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
*-函数名称	：565格式获得RGB的值
*-参数	：
*返回值	：
*-函数功能	：
*-创建者	：蓬莱市博硕电子科技
*/
void CUIGUI_GETRGB(COLOR_DATTYPE color,RGB_Struct* rgb)
{
      rgb->R = (u8)((color >> 11)<<3);
      rgb->G = (u8)((color>>5)<<2);
      rgb->B = (u8)(color<<3);
}
