#include "sdio_sdcard.h"
#include "ff.h" 
#include "CUI_MALLCO.H"
#include "CUIGUI_COLOR.H"
#include "CUIGUI_UHEAD.H"
#include "CUIGUI_BMP.H"



#define COLOR_BMP_BLACK  BLACK
#define COLOR_BMP_WHITE  WHITE

FATFS fs_bmp;

/************************************************************************************
*-函数名称	：
*-参数	：
*返回值	：返回LCD GUI颜色
*-函数功能	：返回GUI格式的颜色数据
*-创建者	：探长科技，www.explorersoftware.taobao.com
*/

COLOR_DATTYPE CUIGUI_BMP_Color(RGBQUAD* regquad)   
{
      return  CUIGUI_Color(regquad->rgbRed,regquad->rgbGreen,regquad->rgbBlue); 
}



/************************************************************************************
*-函数名称	：
*-参数	：
*返回值	：0 成功，否则不成功
*-函数功能	：初始化，主要初始化存储介质
*-创建者	：探长科技，www.explorersoftware.taobao.com
*/
u8 CUIGUI_BMP_Init(void)
{      
      u8 dat = SD_Init();   
      dat |= f_mount (&fs_bmp,"1:",1);                     //注册一个工作区
      return  dat;
}

///************************************************************************************
//*-函数名称	：读图像数据
//*-参数	：root 图像路径，datBuff 图像数据保存数组 BMPUsefullMessage 有用的数据 offs_bmpize 偏移地址
//*返回值	：num_read 实际读到的数量
//*-函数功能	：读BMP数据
//*-创建者	：探长科技，www.explorersoftware.taobao.com
//*/

u32  CUIGUI_ReadBMPdat(const char* root,u8* datBuff,u32 read_num,u32 offs_bmpize)
{
      u32 num_read = 0;    
      
      FIL* fdst;   
      fdst = (FIL*)CUI_Mallco(sizeof(FIL));   
      if(f_open(fdst,root,FA_READ))   //打开文件
      {        
            BMP_Log("f_open (fdst,(const char*)root,FA_READ) FAIL\n");
      }            
      f_lseek(fdst,offs_bmpize);     //偏移地址
      
      if(f_read (fdst,datBuff,read_num,&num_read))
      {
           BMP_Log("f_read ( fdst,datBuff,read_num,&num_read) FAIL\n"); 
      } 
      
      f_close (fdst);                   
      FreeCUI_Mallco((u8*)fdst);
      
      return num_read;
}

/************************************************************************************
*-函数名称	：
*-参数	：
*返回值	：
*-函数功能	：根据颜色表值，返回LCD颜色
*-创建者	：探长科技，www.explorersoftware.taobao.com
*/

COLOR_DATTYPE GetREBColor(RGBQUAD* rebquad)
{
     return CUIGUI_Color(rebquad->rgbRed,rebquad->rgbGreen,rebquad->rgbBlue);
}

/************************************************************************************
*-函数名称	：
*-参数	：chamfer倒角度，占宽度的比例
*返回值	：
*-函数功能	：画2 SE BMP图
*-创建者	：探长科技，www.explorersoftware.taobao.com
*/
void CUIGUI_DrawBMP(u16 x,u16 y,u8* datBuff,volatile BMP_UsefullMessage* BMPUsefullMessage,u8 chamfer,u8 fastXOR)
{
      u32   h = 0;      //y方向减量
      u32   i = 0;      //x方向增量
      u32   m = 0;      //m datBuff[]序号
      u16   r;   //半径
      u16 x0,y0,x1,y1,x2,y2,x3,y3;
      u16 left_side,rightside;
      
#if BMP_DRAW565 == 0
      u16 x4;
#endif    
      r = BMPUsefullMessage->biWidth/chamfer;       
      y2 = y + r;
      y3 = y2;
      y = y + BMPUsefullMessage->biHeight;      //偏移到底部
      if(chamfer) //倒角
      {
            x1 = x+BMPUsefullMessage->biWidth - r;
            x0 = x + r; 
            y0 = y - r;      
            y1 = y0;
            x2 = x0;
            x3 = x1;            
            while(h<BMPUsefullMessage->biHeight)
            {
                  if(h<r)
                  {
                        if(GetCircle_X(x0,y0,r,y-h) != 0xffff)    //有交点
                        {
                              left_side = (u16)(GetCircle_X(x0,y0,r,y-h) & 0x0000ffff);//得到左边界
                        }else left_side = x;
                        if(GetCircle_X(x1,y1,r,y-h) != 0xffff)
                        {
                              rightside = (u16)(GetCircle_X(x1,y1,r,y-h)>>16);      //得到右边界
                              
                        }else rightside = x + (BMPUsefullMessage->biWidth);                  
                  }
                  else if(h>(BMPUsefullMessage->biHeight - r -1))
                  {
                        if(GetCircle_X(x2,y2,r,y-h) != 0xffff)    //有交点
                        {
                              left_side = (u16)(GetCircle_X(x2,y2,r,y-h) & 0x0000ffff);//得到左边界
                        }else left_side = x;
                        if(GetCircle_X(x3,y3,r,y-h) != 0xffff)
                        {
                              rightside = (u16)(GetCircle_X(x3,y3,r,y-h)>>16);      //得到右边界
                              
                        }else rightside = x + (BMPUsefullMessage->biWidth);    
                  }else
                  {
                        left_side = x;
                        rightside = x + (BMPUsefullMessage->biWidth); 
                  }
                  
                  m += (left_side - x)/8;
                  BMP_DRAW_BIGEN;          
                  for(i=(left_side-x);i<(rightside-x);i++)
                  {
                        if(((i&7) == 0) && (i!=(left_side-x))) m ++;   
                        if(fastXOR)
                        {
                              if(datBuff[m]&(0x80>>(i&7))) 
                              {
                                   BMP_DRAW_POINT(x4,y-h,GetXORColor(COLOR_BMP_WHITE));  
                                   
                              }
                              else  BMP_DRAW_POINT(x4,y-h,GetXORColor(COLOR_BMP_BLACK)); 
                        }
                        else
                        {
                              if(datBuff[m]&(0x80>>(i&7)))
                                    BMP_DRAW_POINT(x4,y-h,COLOR_BMP_WHITE);
                              else 
                                    BMP_DRAW_POINT(x4,y-h,COLOR_BMP_BLACK);    
                        }
                       BMP_DRAW_ADD;
                  } 
                  m++;                 
                  while((m%BMPUsefullMessage->bytesPerLine) != 0) m++;  
                                    
                  h ++;  
            }
             
      }
      else
      {
            while(h<BMPUsefullMessage->biHeight)
            {
                  BMP_DRAW_BIGEN1;
                  for(i=0;i<BMPUsefullMessage->biWidth;i++)
                  {
                        if((i&7) == 0 && i!=0) m ++;
                        if(fastXOR)
                        {
                              if(datBuff[m]&(0x80>>(i&7))) BMP_DRAW_POINT(x4,y-h,GetXORColor(COLOR_BMP_WHITE));
                              else BMP_DRAW_POINT(x4,y-h,GetXORColor(COLOR_BMP_BLACK));  
                        }
                        else
                        {
                              if(datBuff[m]&(0x80>>(i&7))) BMP_DRAW_POINT(x4,y-h,COLOR_BMP_WHITE);
                              else  BMP_DRAW_POINT(x4,y-h,COLOR_BMP_BLACK);   
                        } 
                       BMP_DRAW_ADD;       
                  } 
                  m++; 
                  while((m%BMPUsefullMessage->bytesPerLine) != 0) m++;                    
                  h ++;
            }
      }
      
}



/************************************************************************************
*-函数名称	：
*-参数	：RGBQUAD16 彩色表 chamfer倒角度，占宽度的比例
*返回值	：
*-函数功能	：画16色位图
*-创建者	：探长科技，www.explorersoftware.taobao.com
*/

void CUIGUI_DrawBMP16(u16 x,u16 y,u8* datBuff,volatile BMP_UsefullMessage* BMPUsefullMessage,RGBQUAD16* regquad,u8 chamfer,u8 fastXOR)
{
      u32   h = 0;      //y方向减量
      u32   i = 0;      //x方向增量
      u32   m = 0;      //m datBuff[]序号
      u16   r ;   //半径
      u16 x0,y0,x1,y1,x2,y2,x3,y3;
      u16 left_side,rightside;
#if BMP_DRAW565 == 0
      u16 x4;
#endif  
      r = BMPUsefullMessage->biWidth/chamfer; 
      y2 = y + r;
      y3 = y2;
      y = y + BMPUsefullMessage->biHeight;      //偏移到底部
      if(chamfer) //倒角
      {        
            x1 = x+BMPUsefullMessage->biWidth - r;
            x0 = x + r; 
            y0 = y - r;      
            y1 = y0;
            x2 = x0;
            x3 = x1;            
            while(h<BMPUsefullMessage->biHeight)
            {
                  if(h<r)
                  {
                        if(GetCircle_X(x0,y0,r,y-h) != 0xffff)    //有交点
                        {
                              left_side = (u16)(GetCircle_X(x0,y0,r,y-h) & 0x0000ffff);//得到左边界
                        }else left_side = x;
                        if(GetCircle_X(x1,y1,r,y-h) != 0xffff)
                        {
                              rightside = (u16)(GetCircle_X(x1,y1,r,y-h)>>16);      //得到右边界
                              
                        }else rightside = x + (BMPUsefullMessage->biWidth);                  
                  }
                  else if(h>(BMPUsefullMessage->biHeight - r -1))
                  {
                        if(GetCircle_X(x2,y2,r,y-h) != 0xffff)    //有交点
                        {
                              left_side = (u16)(GetCircle_X(x2,y2,r,y-h) & 0x0000ffff);//得到左边界
                        }else left_side = x;
                        if(GetCircle_X(x3,y3,r,y-h) != 0xffff)
                        {
                              rightside = (u16)(GetCircle_X(x3,y3,r,y-h)>>16);      //得到右边界
                              
                        }else rightside = x + (BMPUsefullMessage->biWidth);    
                  }else
                  {
                        left_side = x;
                        rightside = x + (BMPUsefullMessage->biWidth); 
                  }

                  
                   m += (left_side - x)/2;

                   BMP_DRAW_BIGEN;                  
                  for(i = left_side;i<rightside;i++)
                  { 
                      if(fastXOR)
                      {
                        if((i&1) == 0 )
                        {
                              if(i != 0)m++;
 //                             CUIGUI_FastXOR(16,GetREBColor(&regquad->rgb_quad[datBuff[m]>>4]));
                              BMP_DRAW_POINT(x4,y-h,GetXORColor(GetREBColor(&regquad->rgb_quad[datBuff[m]>>4]))); 
                        }                        
                        else
 //                             CUIGUI_FastXOR(16,GetREBColor(&regquad->rgb_quad[datBuff[m]&0x0f]));   
                              BMP_DRAW_POINT(x4,y-h,GetXORColor(GetREBColor(&regquad->rgb_quad[datBuff[m]&0x0f]))); 
                      }                           
                      else                        
                      {
                        if((i&1) == 0 )
                        {
                              if(i != 0)m++;
 //                             CUIGUI_FastDrawPoint(GetREBColor(&regquad->rgb_quad[datBuff[m]>>4]));
                               BMP_DRAW_POINT(x4,y-h,GetREBColor(&regquad->rgb_quad[datBuff[m]>>4])); 
                        }                        
                        else
 //                             CUIGUI_FastDrawPoint(GetREBColor(&regquad->rgb_quad[datBuff[m]&0x0f]));   
                               BMP_DRAW_POINT(x4,y-h,GetREBColor(&regquad->rgb_quad[datBuff[m]&0x0f])); 

                      }
                      BMP_DRAW_ADD;
                  }
                  m++;                  
                  while((m%BMPUsefullMessage->bytesPerLine) != 0) m++;                                
                  h ++;  
            }
             
      }
      else
      {
            while(h<BMPUsefullMessage->biHeight)
            {
                  BMP_DRAW_BIGEN1;
                  for(i=0;i<BMPUsefullMessage->biWidth;i++)
                  {
                      if(fastXOR)
                      {
                        if((i&1) == 0 )
                        {
                              if(i != 0)m++;
                              BMP_DRAW_POINT(x4,y-h,GetXORColor(GetREBColor(&regquad->rgb_quad[datBuff[m]>>4]))); 
                        }                        
                        else
 //                             CUIGUI_FastXOR(16,GetREBColor(&regquad->rgb_quad[datBuff[m]&0x0f]));   
                              BMP_DRAW_POINT(x4,y-h,GetXORColor(GetREBColor(&regquad->rgb_quad[datBuff[m]&0x0f]))); 

                      }                           
                      else                        
                      {
                        if((i&1) == 0 )
                        {
                              if(i != 0)m++;
//                              CUIGUI_FastDrawPoint(GetREBColor(&regquad->rgb_quad[datBuff[m]>>4]));
                               BMP_DRAW_POINT(x4,y-h,GetREBColor(&regquad->rgb_quad[datBuff[m]>>4])); 

                        }                        
                        else
//                              CUIGUI_FastDrawPoint(GetREBColor(&regquad->rgb_quad[datBuff[m]&0x0f]));   
                               BMP_DRAW_POINT(x4,y-h,GetREBColor(&regquad->rgb_quad[datBuff[m]&0x0f])); 

                      }
                        BMP_DRAW_ADD;                      
                  } 
                  m++; 
                  while((m%BMPUsefullMessage->bytesPerLine) != 0) m++;  
                  
                  h ++;

            }
      }
}

/************************************************************************************
*-函数名称	：
*-参数	：RGBQUAD256 彩色表
*返回值	：
*-函数功能	：画256色位图
*-创建者	：探长科技，www.explorersoftware.taobao.com
*/

void CUIGUI_DrawBMP256(u16 x,u16 y,u8* datBuff,volatile BMP_UsefullMessage* BMPUsefullMessage,RGBQUAD256* regquad,u8 chamfer,u8 fastXOR)
{
      u32   h = 0;      //y方向减量
      u32   i = 0;      //x方向增量
      u32   m = 0;      //m datBuff[]序号
      u16   r ;   //半径
      u16 x0,y0,x1,y1,x2,y2,x3,y3;
      u16 left_side,rightside;
#if BMP_DRAW565 == 0
      u16 x4;
#endif  
      r = BMPUsefullMessage->biWidth/chamfer; 
      y2 = y + r;
      y3 = y2;
      y = y + BMPUsefullMessage->biHeight;      //偏移到底部
      
      if(chamfer) //倒角
      {       
            x1 = x+BMPUsefullMessage->biWidth - r;
            x0 = x + r; 
            y0 = y - r;      
            y1 = y0;
            x2 = x0;
            x3 = x1;            
            while(h<BMPUsefullMessage->biHeight)
            {
                  if(h<r)
                  {
                        if(GetCircle_X(x0,y0,r,y-h) != 0xffff)    //有交点
                        {
                              left_side = (u16)(GetCircle_X(x0,y0,r,y-h) & 0x0000ffff);//得到左边界
                        }else left_side = x;
                        if(GetCircle_X(x1,y1,r,y-h) != 0xffff)
                        {
                              rightside = (u16)(GetCircle_X(x1,y1,r,y-h)>>16);      //得到右边界
                              
                        }else rightside = x + (BMPUsefullMessage->biWidth);                  
                  }
                  else if(h>(BMPUsefullMessage->biHeight - r -1))
                  {
                        if(GetCircle_X(x2,y2,r,y-h) != 0xffff)    //有交点
                        {
                              left_side = (u16)(GetCircle_X(x2,y2,r,y-h) & 0x0000ffff);//得到左边界
                        }else left_side = x;
                        if(GetCircle_X(x3,y3,r,y-h) != 0xffff)
                        {
                              rightside = (u16)(GetCircle_X(x3,y3,r,y-h)>>16);      //得到右边界
                              
                        }else rightside = x + (BMPUsefullMessage->biWidth);    
                  }else
                  {
                        left_side = x;
                        rightside = x + (BMPUsefullMessage->biWidth); 
                  }

                  
                   m += (left_side - x);
                  BMP_DRAW_BIGEN;                 
                  for(i = left_side;i<rightside;i++)
                  {                   
                      if(fastXOR) 
//                           CUIGUI_FastXOR(256,GetREBColor(&regquad->rgb_quad[datBuff[m]]));
                        BMP_DRAW_POINT(x4,y-h,GetXORColor(GetREBColor(&regquad->rgb_quad[datBuff[m]]))); 

                      else                        
//                     CUIGUI_FastDrawPoint(GetREBColor(&regquad->rgb_quad[datBuff[m]]));
                        BMP_DRAW_POINT(x4,y-h,GetREBColor(&regquad->rgb_quad[datBuff[m]])); 

                      m++;
                      BMP_DRAW_ADD;                      
                  }                  
                  while((m%BMPUsefullMessage->bytesPerLine) != 0) m++;                                
                  h ++;  
            }
             
      }
      else
      {
            while(h<BMPUsefullMessage->biHeight)
            {
                  BMP_DRAW_BIGEN1;
                  for(i=0;i<BMPUsefullMessage->biWidth;i++)
                  { 
                      if(fastXOR) 
//                            CUIGUI_FastXOR(256,GetREBColor(&regquad->rgb_quad[datBuff[m]]));
                             BMP_DRAW_POINT(x4,y-h,GetXORColor(GetREBColor(&regquad->rgb_quad[datBuff[m]])));  

                      else                        
//                      CUIGUI_FastDrawPoint(GetREBColor(&regquad->rgb_quad[datBuff[m]]));
                        BMP_DRAW_POINT(x4,y-h,GetREBColor(&regquad->rgb_quad[datBuff[m]])); 

                      m++; 
                      BMP_DRAW_ADD;
                  }                   
                  while((m%BMPUsefullMessage->bytesPerLine) != 0) m++;                    
                  h ++;

            }
      }
}

/************************************************************************************
*-函数名称	：
*-参数	：chamfer 倒角
*返回值	：
*-函数功能	：画24位位图
*-创建者	：探长科技，www.explorersoftware.taobao.com
*/

void CUIGUI_DrawBMP24(u16 x,u16 y,u8* datBuff,volatile BMP_UsefullMessage* BMPUsefullMessage,u8 chamfer,u8 fastXOR)
{
      u32   h = 0;      //y方向减量
      u32   i = 0;      //x方向增量
      u32   m = 0;      //m datBuff[]序号
      u16   r ;   //半径
      u16 x0,y0,x1,y1,x2,y2,x3,y3;
      u16 left_side,rightside;
  #if BMP_DRAW565 == 0
      u16 x4;
#endif  

      r = BMPUsefullMessage->biWidth/chamfer;
     //4个倒角原点赋值           
      y2 = y + r;
      y3 = y2;
      y = y + BMPUsefullMessage->biHeight;      //偏移到底部
      if(chamfer) //倒角
      {            
            x1 = x+BMPUsefullMessage->biWidth - r;
            x0 = x + r; 
            y0 = y - r;      
            y1 = y0;
            x2 = x0;
            x3 = x1;
            while(h<BMPUsefullMessage->biHeight)
            {
                  if(h<r)
                  {
                        if(GetCircle_X(x0,y0,r,y-h) != 0xffff)    //有交点
                        {
                              left_side = (u16)(GetCircle_X(x0,y0,r,y-h) & 0x0000ffff);//得到左边界
                        }else left_side = x;
                        if(GetCircle_X(x1,y1,r,y-h) != 0xffff)
                        {
                              rightside = (u16)(GetCircle_X(x1,y1,r,y-h)>>16);      //得到右边界
                              
                        }else rightside = x + (BMPUsefullMessage->biWidth);                  
                  }
                  else if(h>(BMPUsefullMessage->biHeight - r -1))
                  {
                        if(GetCircle_X(x2,y2,r,y-h) != 0xffff)    //有交点
                        {
                              left_side = (u16)(GetCircle_X(x2,y2,r,y-h) & 0x0000ffff);//得到左边界
                        }else left_side = x;
                        if(GetCircle_X(x3,y3,r,y-h) != 0xffff)
                        {
                              rightside = (u16)(GetCircle_X(x3,y3,r,y-h)>>16);      //得到右边界
                              
                        }else rightside = x + (BMPUsefullMessage->biWidth);    
                  }else
                  {
                        left_side = x;
                        rightside = x + (BMPUsefullMessage->biWidth); 
                  }

                  
                   m += (left_side - x)*3;
                  BMP_DRAW_BIGEN;                
                  for(i = left_side;i<rightside;i++)
                  {       
                      if(fastXOR)
//                            CUIGUI_FastXOR(65536,CUIGUI_Color(datBuff[m+2],datBuff[m+1],datBuff[m]));
                      BMP_DRAW_POINT(x4,y-h,GetXORColor(CUIGUI_Color(datBuff[m+2],datBuff[m+1],datBuff[m]))); 
                      else
 //                     CUIGUI_FastDrawPoint(CUIGUI_Color(datBuff[m+2],datBuff[m+1],datBuff[m])); 
                        BMP_DRAW_POINT(x4,y-h,CUIGUI_Color(datBuff[m+2],datBuff[m+1],datBuff[m]));
                     //  CUIGUI_DrawPoint(x4,y-h,CUIGUI_Color(datBuff[m+2],datBuff[m+1],datBuff[m]));
                      m+=3;  
                      BMP_DRAW_ADD;
                  }                  
                  while((m%BMPUsefullMessage->bytesPerLine) != 0) m++;                                
                  h ++;  
            }
             
      }
      else
      {
            while(h < BMPUsefullMessage->biHeight)
            {            
                  BMP_DRAW_BIGEN1;

                  for(i = 0;i<(BMPUsefullMessage->biWidth);i++)
                  {
                      if(fastXOR) 
                      {
                            BMP_DRAW_POINT(x4,y-h,GetXORColor(CUIGUI_Color(datBuff[m+2],datBuff[m+1],datBuff[m])));   
                      }
                      else
                      {
                          
                            
                            BMP_DRAW_POINT(x4,y-h,CUIGUI_Color(datBuff[m+2],datBuff[m+1],datBuff[m]));
                      }                            
                        m+=3;
                     
                        BMP_DRAW_ADD;   
                                             
                  }                  
                  while((m%BMPUsefullMessage->bytesPerLine) != 0) m++;                                
                  h ++;
            }     
      }

}

/************************************************************************************
*-函数名称	：
*-参数	：
*返回值	：
*-函数功能	：画565位图
*-创建者	：探长科技，www.explorersoftware.taobao.com
*/

void CUIGUI_DrawBMP565(u16 x,u16 y,u8* datBuff,volatile BMP_UsefullMessage* BMPUsefullMessage,u8 chamfer,u8 fastXOR)
{
      u32   h = 0;      //y方向减量
      u32   i = 0;      //x方向增量
      u32   m = 0;      //m datBuff[]序号
      u16   r  ;   //半径
      u16 x0,y0,x1,y1,x2,y2,x3,y3;
      u16 left_side,rightside;
#if BMP_DRAW565 == 0
      u16 x4;
#endif  

      r = BMPUsefullMessage->biWidth/chamfer;
      //4个倒角原点赋值           
      y2 = y + r;
      y3 = y2;
      y = y + BMPUsefullMessage->biHeight;      //偏移到底部
      if(chamfer) //倒角
      {            
            x1 = x+BMPUsefullMessage->biWidth - r;
            x0 = x + r; 
            y0 = y - r;      
            y1 = y0;
            x2 = x0;
            x3 = x1;
            while(h<BMPUsefullMessage->biHeight)
            {
                  if(h<r)
                  {
                        if(GetCircle_X(x0,y0,r,y-h) != 0xffff)    //有交点
                        {
                              left_side = (u16)(GetCircle_X(x0,y0,r,y-h) & 0x0000ffff);//得到左边界
                        }else left_side = x;
                        if(GetCircle_X(x1,y1,r,y-h) != 0xffff)
                        {
                              rightside = (u16)(GetCircle_X(x1,y1,r,y-h)>>16);      //得到右边界
                              
                        }else rightside = x + (BMPUsefullMessage->biWidth);                  
                  }
                  else if(h>(BMPUsefullMessage->biHeight - r -1))
                  {
                        if(GetCircle_X(x2,y2,r,y-h) != 0xffff)    //有交点
                        {
                              left_side = (u16)(GetCircle_X(x2,y2,r,y-h) & 0x0000ffff);//得到左边界
                        }else left_side = x;
                        if(GetCircle_X(x3,y3,r,y-h) != 0xffff)
                        {
                              rightside = (u16)(GetCircle_X(x3,y3,r,y-h)>>16);      //得到右边界
                              
                        }else rightside = x + (BMPUsefullMessage->biWidth);    
                  }else
                  {
                        left_side = x;
                        rightside = x + (BMPUsefullMessage->biWidth); 
                  }

                  
                   m += (left_side - x)*2;
                  BMP_DRAW_BIGEN;                
                  for(i = left_side;i<rightside;i++)
                  {                   
                      if(fastXOR)
//                            CUIGUI_FastXOR(65536,*(COLOR_DATTYPE*)(&datBuff[m]));
                      //   BMP_DRAW_POINT(x4,y-h,GetXORColor(*(COLOR_DATTYPE*)(&datBuff[m])));
                       BMP_DRAW_POINT(x4,y-h,GetXORColor(Get_LittleORBig_End(&datBuff[m],COLOR_DATTYPE)));
        

                      else
                       //  BMP_DRAW_POINT(x4,y-h,(*(COLOR_DATTYPE*)(&datBuff[m])));
                         BMP_DRAW_POINT(x4,y-h,(Get_LittleORBig_End(&datBuff[m],COLOR_DATTYPE)));
                      m+=2;
                      BMP_DRAW_ADD;
                  }                  
                  while((m%BMPUsefullMessage->bytesPerLine) != 0) m++;                                
                  h ++;  
            }
             
      }
      else
      {
            while(h<BMPUsefullMessage->biHeight)
            {
                  BMP_DRAW_BIGEN1;
                  for(i=0;i<BMPUsefullMessage->biWidth;i++)
                  {
                      
                      if(fastXOR)
//                            CUIGUI_FastXOR(65536,*(COLOR_DATTYPE*)(&datBuff[m]));
                              //BMP_DRAW_POINT(x4,y-h,GetXORColor(*(COLOR_DATTYPE*)(&datBuff[m])));
                              BMP_DRAW_POINT(x4,y-h,GetXORColor(Get_LittleORBig_End(&datBuff[m],COLOR_DATTYPE)));
                      else
//                      CUIGUI_FastDrawPoint(*(COLOR_DATTYPE*)(&datBuff[m]));
                        // BMP_DRAW_POINT(x4,y-h,*(COLOR_DATTYPE*)(&datBuff[m]));
                       BMP_DRAW_POINT(x4,y-h,(Get_LittleORBig_End(&datBuff[m],COLOR_DATTYPE)));                      
                      m+=2;   
                      BMP_DRAW_ADD;
                  } 
                  while((m%BMPUsefullMessage->bytesPerLine) != 0) m++;  
                  
                  h ++;

            }      
      }

}
/************************************************************************************
*-函数名称	：
*-参数	：倒角半径 = biWidth/chamfer ; fastXOR的值 0 :不异或颜色,1:异或
*返回值	：
*-函数功能	：显示BMP图像
*-创建者	：探长科技，www.explorersoftware.taobao.com
*/

void BMP_Show(u16 x,u16 y,const char* root,u8 chamfer,u8 fastXOR)
{

      Handle_BMPMessage* mallcoBuff ;             
      volatile BMP_UsefullMessage* BMPUsefullMessage ;            
      u8* datBuff;
      u8*   Handle_RGBQUAD ;   //颜色表
      
#if DEBUG_BMP_LEVEL>1 
      u32 i;
#endif    
     
      mallcoBuff = (Handle_BMPMessage*)CUI_Mallco(BMPHEADERSIZE); //request memory 空间 
      
       CUIGUI_ReadBMPdat(root,(u8*)mallcoBuff,BMPHEADERSIZE,0);  //读InfoHeader        
           
       BMPUsefullMessage = ( BMP_UsefullMessage*)CUI_Mallco(sizeof(BMP_UsefullMessage)); //request memory 空间
          
      BMP_Log("(*(u16*)(mallcoBuff->bmpHeader.bfType) = %x\r\n",*(u16*)(mallcoBuff->bmpHeader.bfType));

      
      if(*(u16*)(mallcoBuff->bmpHeader.bfType) == 0x4D42)      //"BM"
      {           
            BMPUsefullMessage->biBitCount = Get_LittleORBig_End(mallcoBuff->bmpInfor.biBitCount,WORD);            
            BMPUsefullMessage->biCompression =Get_LittleORBig_End(mallcoBuff->bmpInfor.biCompression,DWORD);
            BMPUsefullMessage->biHeight = Get_LittleORBig_End(mallcoBuff->bmpInfor.biHeight,LONG);
            BMPUsefullMessage->bfOffBits = Get_LittleORBig_End(mallcoBuff->bmpHeader.bfOffBits,DWORD);
            BMPUsefullMessage->bfSize = Get_LittleORBig_End(mallcoBuff->bmpHeader.bfSize,DWORD);
            BMPUsefullMessage->biWidth =  Get_LittleORBig_End(mallcoBuff->bmpInfor.biWidth,LONG);
            BMPUsefullMessage->bytesPerLine = (((BMPUsefullMessage->biWidth*BMPUsefullMessage->biBitCount)+31)>>5)<<2;
        
            BMP_Log("BMPUsefullMessage->bfSize = %d\r\n",BMPUsefullMessage->bfSize);
            BMP_Log("BMPUsefullMessage->bfOffBits = %d\r\n",BMPUsefullMessage->bfOffBits);
            BMP_Log("BMPUsefullMessage->biWidth = %d\r\n",BMPUsefullMessage->biWidth);
            BMP_Log("BMPUsefullMessage->biHeight = %d\r\n",BMPUsefullMessage->biHeight); 
            BMP_Log("BMPUsefullMessage->biBitCount = %d\r\n",BMPUsefullMessage->biBitCount);
            BMP_Log("BMPUsefullMessage->biCompression = %d\r\n",BMPUsefullMessage->biCompression); 
            BMP_Log("BMPUsefullMessage->bytesPerLine = %d\r\n",BMPUsefullMessage->bytesPerLine);
      
           FreeCUI_Mallco((u8*)mallcoBuff);     //free memory          
           datBuff = CUI_Mallco(BMPUsefullMessage->bfSize - BMPUsefullMessage->bfOffBits);//申请图像数据内存 
           CUIGUI_ReadBMPdat(root,datBuff,BMPUsefullMessage->bfSize - BMPUsefullMessage->bfOffBits,BMPUsefullMessage->bfOffBits);//读出BMP图像数据
#if DEBUG_BMP_LEVEL>1  
                 
            BMP_Log("Read BMP dat:\r\n");                  
            for(i = 0 ;i <(BMPUsefullMessage->bfSize - BMPUsefullMessage->bfOffBits); i ++)
            {
                  BMP_Log("%x  ",datBuff[i]);
            }                                                            
#endif                      
            //信息
            switch(BMPUsefullMessage->biBitCount)
            {
                  case 1:                       //黑白两色       
                        BMP_Log("Is 2_SE BMP\r\n"); 
                        CUIGUI_DrawBMP(x,y,datBuff,BMPUsefullMessage,chamfer,fastXOR);
                        
                        break;                  
                  case 4:                       //16位色
                        //读，保存颜色表
                        BMP_Log("Is 16_SE BMP\r\n"); 
                        Handle_RGBQUAD = CUI_Mallco(BMPUsefullMessage->bfOffBits - BMPHEADERSIZE);
                        CUIGUI_ReadBMPdat(root, Handle_RGBQUAD,BMPUsefullMessage->bfOffBits - BMPHEADERSIZE,BMPHEADERSIZE);
#if DEBUG_BMP_LEVEL>1  
            BMP_Log("BMPUsefullMessage->bfOffBits - BMPHEADERSIZE = %d \r\n",BMPUsefullMessage->bfOffBits - BMPHEADERSIZE);
            BMP_Log("Read ColorIdex dat:\r\n");                  
            for(i = 0 ;i <BMPUsefullMessage->bfOffBits - BMPHEADERSIZE; i ++)
            {
                  if((i&3) == 0 && i != 0)BMP_Log("\r\n");
                  BMP_Log("%x  ",Handle_RGBQUAD[i]);
                  
            }                                                            
#endif
                        CUIGUI_DrawBMP16(x,y, datBuff,BMPUsefullMessage,(RGBQUAD16*)Handle_RGBQUAD,chamfer,fastXOR);                        
                        FreeCUI_Mallco(Handle_RGBQUAD);
                        break;
                  case 8:                         
                        BMP_Log("Is 256_SE BMP\r\n");                    //256色                                
                        //读，保存颜色表
                        Handle_RGBQUAD = CUI_Mallco(BMPUsefullMessage->bfOffBits - BMPHEADERSIZE);
                        CUIGUI_ReadBMPdat(root, Handle_RGBQUAD,BMPUsefullMessage->bfOffBits - BMPHEADERSIZE,BMPHEADERSIZE);
#if DEBUG_BMP_LEVEL>1  
            BMP_Log("BMPUsefullMessage->bfOffBits - BMPHEADERSIZE = %d \r\n",BMPUsefullMessage->bfOffBits - BMPHEADERSIZE);
            BMP_Log("Read ColorIdex dat:\r\n");                  
            for(i = 0 ;i <BMPUsefullMessage->bfOffBits - BMPHEADERSIZE; i ++)
            {
                  if((i&3) == 0 && i != 0)BMP_Log("\r\n");
                  BMP_Log("%x  ",Handle_RGBQUAD[i]);
                  
            }                                                            
#endif                        
                  
                        CUIGUI_DrawBMP256(x,y,datBuff,BMPUsefullMessage,(RGBQUAD256*) Handle_RGBQUAD,chamfer,fastXOR);            
                        FreeCUI_Mallco(Handle_RGBQUAD);
                        break;
                  case 16:                           
                        BMP_Log("Is 565 BMP\r\n");
                        CUIGUI_DrawBMP565(x,y, datBuff,BMPUsefullMessage,chamfer,fastXOR);

                        break;            
                  default:                      //24位色                        
                        BMP_Log("Is 24 BMP\r\n");
                        CUIGUI_DrawBMP24(x,y,datBuff,BMPUsefullMessage,chamfer,fastXOR);
                        break;
            }
            
            FreeCUI_Mallco(datBuff);                          //释放图像数据内存            
            FreeCUI_Mallco((u8*)BMPUsefullMessage);          //free memory 
      }else 
      {
             
             BMP_Log("Is not BMP\r\n");            
             FreeCUI_Mallco((u8*)mallcoBuff);                 //free memory 
             FreeCUI_Mallco(( u8*)BMPUsefullMessage);         //free memory 
      }
}




#if BIG_BMP > 0

/************************************************************************************
*-函数名称	：
*-参数	：
*返回值	：
*-函数功能	：画BIG_BMP图
*-创建者	：探长科技，www.explorersoftware.taobao.com
*/
u32 CUIGUI_DrawBIG_BMP(u16 x,u16 y,u32 nowhigh,u32 numdatBuf, u8* datBuff,volatile BMP_UsefullMessage* BMPUsefullMessage)
{
      u32   h = nowhigh;            //y方向减量
      u32   i = 0;                  //x方向增量
      u32   m = 0;                  //m datBuff[]序号
#if BMP_DRAW565 == 0
      u16 x4;
#endif  
      y = y + BMPUsefullMessage->biHeight;        //偏移到底部
      
      while(1)
      {
            BMP_DRAW_BIGEN1;
            for(i=0;i<BMPUsefullMessage->biWidth;i++)
            {
                  if((i&7) == 0 && i!=0) m ++;
                  if(datBuff[m]&(0x80>>(i&7))) 
                        BMP_DRAW_POINT(x4,y-h,COLOR_BMP_WHITE);
                  else
                        BMP_DRAW_POINT(x4,y-h,COLOR_BMP_BLACK);
                  
                  BMP_DRAW_ADD;
            } 
            m++; 
            while((m%BMPUsefullMessage->bytesPerLine) != 0) m++;  
            
            h ++;

            if(m+BMPUsefullMessage->bytesPerLine >numdatBuf) return h;
      }
     
}

/************************************************************************************
*-函数名称	：
*-参数	：RGBQUAD16 彩色表
*返回值	：
*-函数功能	：画BIG_16色位图
*-创建者	：探长科技，www.explorersoftware.taobao.com
*/

u32 CUIGUI_DrawBIG_BMP16(u16 x,u16 y,u32 nowhigh,u32 numdatBuf, u8* datBuff,volatile BMP_UsefullMessage* BMPUsefullMessage,RGBQUAD16* regquad)
{
      u32   h = nowhigh;      //y方向减量
      u32   i = 0;      //x方向增量
      u32   m = 0;      //m datBuff[]序号
#if BMP_DRAW565 == 0
      u16 x4;
#endif  
      y = y + BMPUsefullMessage->biHeight;      //偏移到底部
      while(1)
      {
            BMP_DRAW_BIGEN1;
            for(i=0;i<BMPUsefullMessage->biWidth;i++)
            {
                  if((i&1) == 0 )
                  {
                        if(i != 0)m++;
//                        CUIGUI_FastDrawPoint(GetREBColor(&regquad->rgb_quad[datBuff[m]>>4]));
                         BMP_DRAW_POINT(x4,y-h,GetREBColor(&regquad->rgb_quad[datBuff[m]>>4]));
                  }                        
                  else
//                        CUIGUI_FastDrawPoint(GetREBColor(&regquad->rgb_quad[datBuff[m]&0x0f]));
                         BMP_DRAW_POINT(x4,y-h,GetREBColor(&regquad->rgb_quad[datBuff[m]&0x0f]));
                  BMP_DRAW_ADD;
            } 
            m++; 
            while((m%BMPUsefullMessage->bytesPerLine) != 0) m++;              
            h ++;
            if(m+BMPUsefullMessage->bytesPerLine >numdatBuf) return h;

      }
   
}

/************************************************************************************
*-函数名称	：
*-参数	：RGBQUAD256 彩色表
*返回值	：
*-函数功能	：画BIG_256色位图
*-创建者	：探长科技，www.explorersoftware.taobao.com
*/

u32 CUIGUI_DrawBIG_BMP256(u16 x,u16 y,u32 nowhigh,u32 numdatBuf, u8* datBuff,volatile BMP_UsefullMessage* BMPUsefullMessage,RGBQUAD256* regquad)
{
      u32   h = nowhigh;      //y方向减量
      u32   i = 0;      //x方向增量
      u32   m = 0;      //m datBuff[]序号
#if BMP_DRAW565 == 0
      u16 x4;
#endif  
      y = y + BMPUsefullMessage->biHeight;      //偏移到底部

      while(1)
      {            
            
            BMP_DRAW_BIGEN1;
            
            for(i=0;i<BMPUsefullMessage->biWidth;i++)
            {
                if(i != 0)m++;
//                CUIGUI_FastDrawPoint(GetREBColor(&regquad->rgb_quad[datBuff[m]]));
                 BMP_DRAW_POINT(x4,y-h,GetREBColor(&regquad->rgb_quad[datBuff[m]]));
                  BMP_DRAW_ADD;
            } 
            
            m++; 
            while((m%BMPUsefullMessage->bytesPerLine) != 0) m++;  
            h ++;
            if(m+BMPUsefullMessage->bytesPerLine >numdatBuf) return h;
      }
      
}

/************************************************************************************
*-函数名称	：
*-参数	：
*返回值	：
*-函数功能	：画BIG_24位位图
*-创建者	：探长科技，www.explorersoftware.taobao.com
*/

u32 CUIGUI_DrawBIG_BMP24(u16 x,u16 y,u32 nowhigh, u32 numdatBuf, u8* datBuff,volatile BMP_UsefullMessage* BMPUsefullMessage)
{
      u32   h = nowhigh;      //y方向减量
      u32   i = 0;      //x方向增量
      u32   m = 0;      //m datBuff[]序号
#if BMP_DRAW565 == 0
      u16 x4 = 0;
#endif  
      y = y + BMPUsefullMessage->biHeight;      //偏移到底部
      while(1)
      {
            BMP_DRAW_BIGEN1;
            for(i=0;i<BMPUsefullMessage->biWidth;i++)
            {
                if(i!=0) m += 3;
//                CUIGUI_FastDrawPoint(CUIGUI_Color(datBuff[m+2],datBuff[m+1],datBuff[m])); 
                  if(m+2<numdatBuf)
                  {
                        BMP_DRAW_POINT(x4,y-h,CUIGUI_Color(datBuff[m+2],datBuff[m+1],datBuff[m]));
                        BMP_DRAW_ADD;
                  }else break;

                 
            } 
            m += 3; 
            
            while((m%BMPUsefullMessage->bytesPerLine) != 0) m++;            
            h ++;           
            if(m>=numdatBuf) 
            {

                  return h;
            }
      }
   
}

/************************************************************************************
*-函数名称	：
*-参数	：
*返回值	：
*-函数功能	：画BIG_565位图
*-创建者	：探长科技，www.explorersoftware.taobao.com
*/

u32 CUIGUI_DrawBIG_BMP565(u16 x,u16 y,u32 nowhigh,u32 numdatBuf, u8* datBuff,volatile BMP_UsefullMessage* BMPUsefullMessage)
{
      u32   h = nowhigh;      //y方向减量
      u32   i = 0;      //x方向增量
      u32   m = 0;      //m datBuff[]序号
#if BMP_DRAW565 == 0
      u16 x4;
#endif  
      y = y + BMPUsefullMessage->biHeight;      //偏移到底部

      while(1)
      {
            BMP_DRAW_BIGEN1;
            for(i=0;i<BMPUsefullMessage->biWidth;i++)
            {
                if(i!=0) m += 2;
//                CUIGUI_FastDrawPoint(*(COLOR_DATTYPE*)(&datBuff[m])); 
                  BMP_DRAW_POINT(x4,y-h,Get_LittleORBig_End(&datBuff[m],COLOR_DATTYPE));
               BMP_DRAW_ADD;  
            } 
            m+=2; 
            while((m%BMPUsefullMessage->bytesPerLine) != 0) m++;              
            h ++;
            if(m+BMPUsefullMessage->bytesPerLine >numdatBuf) return h;
      }
      
}
/************************************************************************************
*-函数名称	：
*-参数	：
*返回值	：
*-函数功能	：显示BIG_BMP图像
*-创建者	：探长科技，www.explorersoftware.taobao.com
*/

void BIG_BMP_Show(u16 x,u16 y,const char* root)
{

      Handle_BMPMessage* mallcoBuff ;             
      volatile BMP_UsefullMessage* BMPUsefullMessage ;            
       u8* datBuff;
      u8*   Handle_RGBQUAD ;        //颜色表
      u32 left_dat = 0;             //需要显示的数据量
      u32 show_dat = 0;             //已经显示的数据量
      u32 mallco_dat = 0;           //申请的显示内存
      u32 num_read = 0;   
      u32 nowhigh = 0;                    //保存当前画的高度
      u32 nowhigh2 = 0;                   //保存上次画的高度
#if DEBUG_BMP_LEVEL>1 
      u32 i;
#endif      
      FIL fdst;      
     
      mallcoBuff = (Handle_BMPMessage*)CUI_Mallco(BMPHEADERSIZE); //request memory 空间 
      BMP_Log("BMPHEADERSIZE = %d\n",BMPHEADERSIZE);
      CUIGUI_ReadBMPdat(root,(u8*)mallcoBuff,BMPHEADERSIZE,0);  //读InfoHeader 

#if DEBUG_BMP_LEVEL>1 
      BMP_Log("InfoHeader : \r\n"); 
      for(i=0;i<BMPHEADERSIZE;i++)
      {
            
           BMP_Log("%x  ",((u8*)mallcoBuff)[i]);
           
      }
       BMP_Log("\r\n"); 
#endif         
           
       BMPUsefullMessage = ( BMP_UsefullMessage*)CUI_Mallco(sizeof(BMP_UsefullMessage)); //request memory 空间
  
      
      if(*(u16*)(mallcoBuff->bmpHeader.bfType) == 0x4D42)      //"BM"
      {
            BMPUsefullMessage->biBitCount = *(WORD*)(mallcoBuff->bmpInfor.biBitCount);            
            BMPUsefullMessage->biCompression = *(DWORD*)(mallcoBuff->bmpInfor.biCompression);
            BMPUsefullMessage->biHeight = *(LONG*)(mallcoBuff->bmpInfor.biHeight) ;
            BMPUsefullMessage->bfOffBits = *(DWORD*)(mallcoBuff->bmpHeader.bfOffBits);
            BMPUsefullMessage->bfSize = *(DWORD*)(mallcoBuff->bmpHeader.bfSize);
            BMPUsefullMessage->biWidth =  *(LONG*)(mallcoBuff->bmpInfor.biWidth);
            BMPUsefullMessage->bytesPerLine = (((BMPUsefullMessage->biWidth*BMPUsefullMessage->biBitCount)+31)>>5)<<2; 

            BMP_Log("BMPUsefullMessage->bfSize = %d\r\n",BMPUsefullMessage->bfSize);
            BMP_Log("BMPUsefullMessage->bfOffBits = %d\r\n",BMPUsefullMessage->bfOffBits);
            BMP_Log("BMPUsefullMessage->biWidth = %d\r\n",BMPUsefullMessage->biWidth);
            BMP_Log("BMPUsefullMessage->biHeight = %d\r\n",BMPUsefullMessage->biHeight); 
            BMP_Log("BMPUsefullMessage->biBitCount = %d\r\n",BMPUsefullMessage->biBitCount);
            BMP_Log("BMPUsefullMessage->biCompression = %d\r\n",BMPUsefullMessage->biCompression); 
            BMP_Log("BMPUsefullMessage->bytesPerLine = %d\r\n",BMPUsefullMessage->bytesPerLine);
            
            FreeCUI_Mallco((u8*)mallcoBuff);     //free memory 
           
           
           
/*   显示BMP数据*/  

            //读出颜色表
            if(BMPUsefullMessage->biBitCount == 4 ||BMPUsefullMessage->biBitCount == 8 )
            { 
                 
                  Handle_RGBQUAD = CUI_Mallco(BMPUsefullMessage->bfOffBits - BMPHEADERSIZE);
                  CUIGUI_ReadBMPdat(root, Handle_RGBQUAD,BMPUsefullMessage->bfOffBits - BMPHEADERSIZE,BMPHEADERSIZE);
            }                           
            
            left_dat = BMPUsefullMessage->bfSize - BMPUsefullMessage->bfOffBits; 
            
            if(left_dat > BMP_DATMAX) datBuff = CUI_Mallco(BMP_DATMAX);       //申请图像数据内存
            else datBuff = CUI_Mallco(left_dat);
            
            f_open (&fdst,root,FA_READ);   //打开文件   
       
            while(show_dat< (BMPUsefullMessage->bfSize - BMPUsefullMessage->bfOffBits))
            {
                  if(left_dat>BMP_DATMAX)
                  {
                       mallco_dat =  BMP_DATMAX;
                  }else mallco_dat = left_dat;
                  
                  mallco_dat  -= mallco_dat%BMPUsefullMessage->bytesPerLine;
                  
                 // printf("mallco_dat = %d\r\n",mallco_dat);
                  
                  f_lseek(&fdst,BMPUsefullMessage->bfOffBits + show_dat);     //偏移地址
                  f_read (&fdst,(void*)datBuff,mallco_dat,&num_read);
                //  printf("num_read = %d\r\n",num_read);           
                  switch(BMPUsefullMessage->biBitCount)
                  {
                        case 1: 
                              BMP_Log("2 BMPr\n");                      //黑白两色                                 
                                nowhigh = CUIGUI_DrawBIG_BMP(x,y,nowhigh,mallco_dat,datBuff,BMPUsefullMessage);                              
                              break;                  
                        case 4:     BMP_Log("16 BMPr\n");                   //16位色
                             
                                nowhigh = CUIGUI_DrawBIG_BMP16(x,y,nowhigh,mallco_dat, datBuff,BMPUsefullMessage,(RGBQUAD16*)Handle_RGBQUAD);                                                          
                              
                        break;
                        case 8:    BMP_Log("256 BMP\r\n");                    //256色                                           
                                nowhigh = CUIGUI_DrawBIG_BMP256(x,y,nowhigh,mallco_dat,datBuff,BMPUsefullMessage,(RGBQUAD256*) Handle_RGBQUAD);                              
                              break;
                        case 16:    BMP_Log("565 BMP\r\n"); 
                                nowhigh = CUIGUI_DrawBIG_BMP565(x,y,nowhigh,mallco_dat, datBuff,BMPUsefullMessage);
                              break;            
                        default:       
                               BMP_Log("24 BMP\r\n");               //24位色
                                nowhigh = CUIGUI_DrawBIG_BMP24(x,y,nowhigh,mallco_dat,datBuff,BMPUsefullMessage);
                              break;
                  }
                                        
                   left_dat -= (nowhigh - nowhigh2)*(BMPUsefullMessage->bytesPerLine);
                   show_dat += (nowhigh - nowhigh2)*(BMPUsefullMessage->bytesPerLine);
                  
                  BMP_Log("left_dat = %d\r\n",left_dat);   
                  BMP_Log("show_dat = %d\r\n",show_dat); 
                  
                  nowhigh2 = nowhigh; 
                  
            }
            if(BMPUsefullMessage->biBitCount == 4 ||BMPUsefullMessage->biBitCount == 8 )
            {
                  FreeCUI_Mallco(Handle_RGBQUAD);
            }
           
      f_close (&fdst);                                         
      FreeCUI_Mallco((u8*)datBuff);                          //释放图像数据内存    
      FreeCUI_Mallco((u8*)BMPUsefullMessage);          //free memory 
      }else 
      {            
            BMP_Log("Is not BMP\r\n");                      
            FreeCUI_Mallco((u8*)mallcoBuff);                 //free memory      
            FreeCUI_Mallco((u8*)BMPUsefullMessage);         //free memory 
      }
}

#endif


//BMP创建函数
WM_HWIN BMP_CreateIndirect(const GUI_WIDGET_CREATE_INFO * pCreate, WM_HWIN hWin, int x0, int y0, WM_CALLBACK * cb)
{      
      BMP_Show(x0,y0,pCreate->pName,pCreate->Para,pCreate->Flags);            
      return 0;
}



