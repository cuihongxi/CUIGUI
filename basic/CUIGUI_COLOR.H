/******************************************************************
*文件名:颜色
*创建时间:2017年6月28日08:22:01
*文件说明:基于CUIGUI，通用颜色宏定义都在这里
*/

#ifndef  __CUIGUI_COLOR_H   
#define  __CUIGUI_COLOR_H 

#include "uhead.H"


/*********************************************************
    通用颜色表	
*/
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	 0x001F  
#define BRED             0XF81F
#define GRED 		 0XFFE0
#define GBLUE		 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 		 0XBC40 //棕色
#define BRRED 		 0XFC07 //棕红色
#define GRAY  		 0X8430 //灰色
//GUI颜色
#define     PDFCOLOR     0XE696
#define DARKBLUE      	 0X01CF	//深蓝色
#define LIGHTBLUE      	 0X7D7C	//浅蓝色  
#define GRAYBLUE       	 0X5458 //灰蓝色
//以上三色为PANEL的颜色 
 
#define LIGHTGREEN     	 0X841F //浅绿色
#define LIGHTGRAY        0XEF5B //浅灰色(PANNEL)
#define LGRAY 			 0XC618 //浅灰色(PANNEL),窗体背景色

#define LGRAYBLUE        0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE           0X2B12 //浅棕蓝色(选择条目的反色)

#define CUIGUI_INVALID_COLOR 0xFFFFFFF      /* Invalid color - more than 24 bits */

//定义（黑白二色图）像素颜色
#define     COLOR_BMP_WHITE       WHITE   
#define     COLOR_BMP_BLACK       BLACK

typedef struct
{
      u8 R;
      u8 G;
      u8 B;
}RGB_Struct;

#define     COLOR_DATTYPE     u16               //定义颜色的数据类型
#define     LCD_NUM_COLORS    65536             //颜色数



COLOR_DATTYPE CUIGUI_GetColor(u32 color);       //24位色转换成自己需要的颜色,根据情况改写
COLOR_DATTYPE CUIGUI_Color(u8 r,u8 g,u8 b);     //GUI颜色获取
void CUIGUI_GETRGB(COLOR_DATTYPE color,RGB_Struct* rgb);            //获得RGB值

#endif

