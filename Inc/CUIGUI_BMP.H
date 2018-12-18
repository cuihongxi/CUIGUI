/******************************************************************
*�ļ���:CUIGUI_BMP
*������:�����в�˶���ӿƼ�
*����ʱ��:2017��8��12��09:22:05
*�޸���BMP��㺯��,��������ֲ
*�ļ�˵��:BMP��ʽͼƬ�����
*     ������CUI_MALLCO.H ���ļ�ϵͳ
*ע�� :
      ����50k��ͼƬӦ��ʹ�ô�λͼ����  BIG_BMP_Show(u16 x,u16 y,const char* root);
      �޸ĺ�û����ȫ����,������BUG,�еĻ��������޸�

����:
      CUIGUI_BMP_Init();                                    //��ʼ���洢���ʣ�0�ɹ�
      BMP_Show(0,0,"1:/LOGO.bmp",6,0);                      //��ʾСλͼ             
*/
#ifndef  __CUIGUI_BMP_H   
#define  __CUIGUI_BMP_H   

#include "CUIGUI_UDATA.H"


#define     BIG_BMP           0   //֧�ִ�����BMP����

#define     BMPHEADERSIZE     54     //BMP��Ϣͷ��С

#define     BMP_DATMAX        48*1024

#define     DEBUG_BMP_LEVEL         0     //���Կ��� 

#if   DEBUG_BMP_LEVEL >0
#include<stdio.h>
#endif

#if (DEBUG_BMP_LEVEL > 0)
#define  BMP_Log(...)         printf(__VA_ARGS__);
#else
#define BMP_Log(...)   
#endif                             


//��д�����,��BMP������ʾ

#define      BMP_DRAW565       1     //565��ʽ������ʾ,�������565,Ӧ����Ϊ 0

#if          BMP_DRAW565 == 0
#define      BMP_DRAW_POINT(x,y,color)   CUIGUI_DrawPoint(x,y,color)
#define      BMP_DRAW_BIGEN   (x4 =  left_side)
#define      BMP_DRAW_BIGEN1  (x4 = x)
#define      BMP_DRAW_ADD     (x4 ++)
#else       //��������ø���
#define      BMP_DRAW_POINT(x,y,color)   CUIGUI_FastDrawPoint(color)
#define      BMP_DRAW_BIGEN   {CUIGUI_SetPosition(left_side,y-h);CUIGUI_FastStart();}
#define      BMP_DRAW_BIGEN1  {CUIGUI_SetPosition(x,y-h);CUIGUI_FastStart();}
#define      BMP_DRAW_ADD     
#endif
/* λͼ�ļ�ͷ 
   �̶���С��14�ֽ� 
          
*/
typedef struct tagBITMAPFILEHEADER{
      u8 bfType[2];            //��BM����0x4D42
      u8 bfSize[4];           //�ļ���С��������14�ֽ�
      u8 bfReserved1[2];       //����
      u8 bfReserved2[2];       //����
      u8 bfOffBits[4];        //���ļ�ͷ��ʵ�ʵ�λͼ���ݵ�ƫ���ֽ���--62
} BITMAPFILEHEADER;

/*�ڶ�����Ϊλͼ��Ϣͷ
      �̶����ȣ�40�ֽ�
      
*/ 
typedef struct tagBITMAPINFOHEADER{
      u8 biSize[4];           //ָ������ṹ�ĳ��ȣ�Ϊ 40 
      u8  biWidth[4];          //ָ��ͼ��Ŀ��ȣ���λ������
      u8  biHeight[4];         //ָ��ͼ��ĸ߶ȣ���λ������
      u8  biPlanes[2];         //������ 1�����ÿ���
      u8  biBitCount[2];       //ָ����ʾ��ɫʱҪ�õ���λ����Ϊ 1���ڰ׶�ɫͼ��,4��16 ɫͼ��,8��256 ɫ��,24(���ͼ)
      u8 biCompression[4];    //ָ��λͼ�Ƿ�ѹ�� ����ЧֵBI_RGB��BI_RLE8��BI_RLE4��BI_BITFIELDS
      u8 biSizeImage[4];      //ָ��ʵ�ʵ�λͼ����ռ�õ��ֽ���
      u8  biXPelsPerMeter[4];  //ָ��Ŀ���豸��ˮƽ�ֱ���
      u8  biYPelsPerMeter[4];  //ָ��Ŀ���豸�Ĵ�ֱ�ֱ���
      u8 biClrUsed[4];        //ָ����ͼ��ʵ���õ�����ɫ��
      u8 biClrImportant[4];   //ָ����ͼ������Ҫ����ɫ��
} BITMAPINFOHEADER;

//���õ�����
typedef struct 
{
      u32 bfSize;           //�ļ���С
      u32 bfOffBits;        //ƫ�Ƶ�ַ
      u32  biWidth;          //ָ��ͼ��Ŀ��ȣ���λ������
      u32  biHeight;         //ָ��ͼ��ĸ߶ȣ���λ������
      u32 biCompression;    //ָ��λͼ�Ƿ�ѹ�� ����ЧֵBI_RGB��BI_RLE8��BI_RLE4��BI_BITFIELDS
      u32 bytesPerLine;     //ÿһ�е��ֽ�����(((width*biBitCount)+31)>>5)<<2                              //ÿһɨ���е��ֽ��������� 4 ����������Ҳ���� DWORD ����ġ�
      u16  biBitCount;       //ָ����ʾ��ɫʱҪ�õ���λ����Ϊ 1���ڰ׶�ɫͼ��,4��16 ɫͼ��,8��256 ɫ��,24(���ͼ)

}BMP_UsefullMessage;
      
typedef struct 
{
      BITMAPFILEHEADER bmpHeader;
      BITMAPINFOHEADER bmpInfor;
}Handle_BMPMessage;

/*��ɫ��
     
*/
typedef struct tagRGBQUAD
      {
            BYTE rgbBlue;     //����ɫ����ɫ����
            BYTE rgbGreen;    //����ɫ����ɫ����
            BYTE rgbRed;      //����ɫ�ĺ�ɫ����
            BYTE rgbReserved; //����ֵ
            
      } RGBQUAD;

//16ɫλͼ��ɫ��
typedef struct 
{
      RGBQUAD rgb_quad[16];
} RGBQUAD16;


//256ɫλͼ��ɫ��
typedef struct 
{
      RGBQUAD rgb_quad[256];
} RGBQUAD256; 

/*���С�˻���ֵ,�����ǻ��С�˵�ֵ */
#define  Get_LittleORBig_End(array,TYPE)  (*(TYPE*)array)	

/*����*/

u8    CUIGUI_BMP_Init(void);                                //��ʼ���ļ�ϵͳ�洢���ʣ�0�ɹ�
void BMP_Show(u16 x,u16 y,const char* root,u8 chamfer,u8 fastXOR);   //��ʾСλͼ  
//BMP��������
WM_HWIN BMP_CreateIndirect(const GUI_WIDGET_CREATE_INFO * pCreate, WM_HWIN hWin, int x0, int y0, WM_CALLBACK * cb);

#if   BIG_BMP >0
void  BIG_BMP_Show(u16 x,u16 y,const char* root);           //��ʾ��λͼ������50k
#endif
      
#endif
      
      