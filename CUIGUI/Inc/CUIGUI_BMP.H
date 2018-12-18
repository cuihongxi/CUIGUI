/******************************************************************
*文件名:CUIGUI_BMP
*创建者:蓬莱市博硕电子科技
*创建时间:2017年8月12日09:22:05
*修改了BMP打点函数,更易于移植
*文件说明:BMP格式图片编解码
*     依赖于CUI_MALLCO.H 和文件系统
*注意 :
      超过50k的图片应该使用大位图函数  BIG_BMP_Show(u16 x,u16 y,const char* root);
      修改后没有完全测试,难免有BUG,有的话出现再修改

例子:
      CUIGUI_BMP_Init();                                    //初始化存储介质，0成功
      BMP_Show(0,0,"1:/LOGO.bmp",6,0);                      //显示小位图             
*/
#ifndef  __CUIGUI_BMP_H   
#define  __CUIGUI_BMP_H   

#include "CUIGUI_UDATA.H"


#define     BIG_BMP           0   //支持大容量BMP开关

#define     BMPHEADERSIZE     54     //BMP信息头大小

#define     BMP_DATMAX        48*1024

#define     DEBUG_BMP_LEVEL         0     //调试开关 

#if   DEBUG_BMP_LEVEL >0
#include<stdio.h>
#endif

#if (DEBUG_BMP_LEVEL > 0)
#define  BMP_Log(...)         printf(__VA_ARGS__);
#else
#define BMP_Log(...)   
#endif                             


//改写这个宏,让BMP更快显示

#define      BMP_DRAW565       1     //565格式更快显示,如果不是565,应该设为 0

#if          BMP_DRAW565 == 0
#define      BMP_DRAW_POINT(x,y,color)   CUIGUI_DrawPoint(x,y,color)
#define      BMP_DRAW_BIGEN   (x4 =  left_side)
#define      BMP_DRAW_BIGEN1  (x4 = x)
#define      BMP_DRAW_ADD     (x4 ++)
#else       //下面的配置更快
#define      BMP_DRAW_POINT(x,y,color)   CUIGUI_FastDrawPoint(color)
#define      BMP_DRAW_BIGEN   {CUIGUI_SetPosition(left_side,y-h);CUIGUI_FastStart();}
#define      BMP_DRAW_BIGEN1  {CUIGUI_SetPosition(x,y-h);CUIGUI_FastStart();}
#define      BMP_DRAW_ADD     
#endif
/* 位图文件头 
   固定大小：14字节 
          
*/
typedef struct tagBITMAPFILEHEADER{
      u8 bfType[2];            //“BM”，0x4D42
      u8 bfSize[4];           //文件大小，包括该14字节
      u8 bfReserved1[2];       //保留
      u8 bfReserved2[2];       //保留
      u8 bfOffBits[4];        //从文件头到实际的位图数据的偏移字节数--62
} BITMAPFILEHEADER;

/*第二部分为位图信息头
      固定长度：40字节
      
*/ 
typedef struct tagBITMAPINFOHEADER{
      u8 biSize[4];           //指定这个结构的长度，为 40 
      u8  biWidth[4];          //指定图象的宽度，单位是象素
      u8  biHeight[4];         //指定图象的高度，单位是象素
      u8  biPlanes[2];         //必须是 1，不用考虑
      u8  biBitCount[2];       //指定表示颜色时要用到的位数，为 1（黑白二色图）,4（16 色图）,8（256 色）,24(真彩图)
      u8 biCompression[4];    //指定位图是否压缩 ，有效值BI_RGB，BI_RLE8，BI_RLE4，BI_BITFIELDS
      u8 biSizeImage[4];      //指定实际的位图数据占用的字节数
      u8  biXPelsPerMeter[4];  //指定目标设备的水平分辨率
      u8  biYPelsPerMeter[4];  //指定目标设备的垂直分辨率
      u8 biClrUsed[4];        //指定本图象实际用到的颜色数
      u8 biClrImportant[4];   //指定本图象中重要的颜色数
} BITMAPINFOHEADER;

//有用的数据
typedef struct 
{
      u32 bfSize;           //文件大小
      u32 bfOffBits;        //偏移地址
      u32  biWidth;          //指定图象的宽度，单位是象素
      u32  biHeight;         //指定图象的高度，单位是象素
      u32 biCompression;    //指定位图是否压缩 ，有效值BI_RGB，BI_RLE8，BI_RLE4，BI_BITFIELDS
      u32 bytesPerLine;     //每一行的字节数。(((width*biBitCount)+31)>>5)<<2                              //每一扫描行的字节数必需是 4 的整倍数，也就是 DWORD 对齐的。
      u16  biBitCount;       //指定表示颜色时要用到的位数，为 1（黑白二色图）,4（16 色图）,8（256 色）,24(真彩图)

}BMP_UsefullMessage;
      
typedef struct 
{
      BITMAPFILEHEADER bmpHeader;
      BITMAPINFOHEADER bmpInfor;
}Handle_BMPMessage;

/*彩色表
     
*/
typedef struct tagRGBQUAD
      {
            BYTE rgbBlue;     //该颜色的蓝色分量
            BYTE rgbGreen;    //该颜色的绿色分量
            BYTE rgbRed;      //该颜色的红色分量
            BYTE rgbReserved; //保留值
            
      } RGBQUAD;

//16色位图彩色表
typedef struct 
{
      RGBQUAD rgb_quad[16];
} RGBQUAD16;


//256色位图彩色表
typedef struct 
{
      RGBQUAD rgb_quad[256];
} RGBQUAD256; 

/*获得小端或大端值,本处是获得小端的值 */
#define  Get_LittleORBig_End(array,TYPE)  (*(TYPE*)array)	

/*函数*/

u8    CUIGUI_BMP_Init(void);                                //初始化文件系统存储介质，0成功
void BMP_Show(u16 x,u16 y,const char* root,u8 chamfer,u8 fastXOR);   //显示小位图  
//BMP创建函数
WM_HWIN BMP_CreateIndirect(const GUI_WIDGET_CREATE_INFO * pCreate, WM_HWIN hWin, int x0, int y0, WM_CALLBACK * cb);

#if   BIG_BMP >0
void  BIG_BMP_Show(u16 x,u16 y,const char* root);           //显示大位图，超过50k
#endif
      
#endif
      
      
