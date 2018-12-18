#include "CUIGUI_SLIDER.H"
#include "GUI.H"
#include "CUI_MALLCO.H"
#include "CUIGUI_COLOR.H"
#include "CUIGUI_LCDDRIVER.H"


//进度条
void ProssSkin(Slider* slider,COLOR_DATTYPE color,u16 length)
{
      if(slider->flag_VorH==0)    //横向
      {
            CUIGUI_FillRect(slider->x,slider->y+slider->height/2-slider->height/5/2,length,slider->height/5,color);

      }else
      {
            CUIGUI_FillRect(slider->x+slider->width/2-slider->width/5/2,slider->y,slider->width/5,length,color);            
      }
}

//一种皮肤
void SliderSkin0(Slider* slider,COLOR_DATTYPE color,u16 length)
{
      
      CUIGUI_FillRect(slider->x,slider->y,slider->width,slider->height,CUIGUI_Color(221,222,222));
      ProssSkin(slider,color,length);
}


/************************************************************************************
*-函数名称	：滑动条的默认皮肤
*-参数	：
*返回值	：
*-函数功能	：
*-创建者	：蓬莱市博硕电子科技
*/

void SliderDefaultSkin(Slider* slider)
{
      if(slider->flag_VorH == 0)
      SliderSkin0(slider,slider->BKcolor,slider->width);
      else SliderSkin0(slider,slider->BKcolor,slider->height);
}

/************************************************************************************
*-函数名称	：滑块的默认皮肤
*-参数	：
*返回值	：
*-函数功能	：
*-创建者	：蓬莱市博硕电子科技
*/

void BlockDefaultSkin(Slider* slider,u16 x,u16 y)
{
      
      if(slider->flag_VorH==0)
      {
            DrawFullCircle(x,y+slider->height/2,slider->height/2,slider->blockcolor);  
            ProssSkin(slider,slider->blockcolor,x - slider->x);  
            ProssSkin(slider,slider->blockcolor,x - slider->x);      
      }else
      {
            DrawFullCircle(x+slider->width/2,y,slider->width/2,slider->blockcolor);
            ProssSkin(slider,slider->blockcolor,y-slider->y); 
      }
           
}
/************************************************************************************
*-函数名称	：画出滑动条
*-参数	：
*返回值	：
*-函数功能	：
*-创建者	：蓬莱市博硕电子科技
*/
void DrawSlider(Slider* slider,u32 val)
{     
      u16 posx = 0,posy = 0;
      
      if(val>=slider->min_dat && val<=slider->max_dat)      //val有效
      {
            slider->SliderSkinFun(slider);    
            if(slider->flag_VorH==0)    //横向
            {
              //根据VAL计算滑块应该在的位置
              posx = slider->x+slider->height/2 + (slider->width-slider->height)*(val - slider->min_dat)/(slider->max_dat - slider->min_dat);//获得滑块的x坐标
              posy = slider->y;              
              
            }else
            {
                posx = slider->x;  
                posy = slider->y + slider->width/2 + (slider->height-slider->width)*(val - slider->min_dat)/(slider->max_dat - slider->min_dat);
            }
             //画滑块  
            slider->blockSkinFun(slider,posx,posy);
                 
            
      }

      
}


/************************************************************************************
*-函数名称	：新建一个滑动条,并分配默认值
*-参数	：flag_VorH 滑动条的方向,0为横向,1为纵向
*返回值	：
*-函数功能	：
*-创建者	：博硕电子科技，www.explorersoftware.taobao.com
*/

 Slider*  NewSlider(u16 xpos,u16 ypos,u16 width,u16 height,u32 max_dat,u32 min_dat,u8 flag_VorH)
{    
      Slider* slider = (Slider*)CUI_Mallco(sizeof(Slider));

      slider->x = xpos;

      slider->y = ypos;

      slider->width = width;
      slider->height = height;
      slider->max_dat = max_dat;
      __NOP();
      slider->min_dat = min_dat;
      slider->flag_VorH = flag_VorH;  
      slider->BKcolor = BLACK;
      slider->blockcolor = WHITE;
      slider->DrawSlider = &DrawSlider;  //指定画出滑动条的默认函数
      slider->SliderSkinFun = &SliderDefaultSkin;
      slider->blockSkinFun = &BlockDefaultSkin;
      return slider;
}

SLIDER_Handle SLIDER_CreateIndirect(const GUI_WIDGET_CREATE_INFO * pCreate, WM_HWIN hWin, int x0, int y0, WM_CALLBACK * cb)
{
      Slider* slider  =  NewSlider(pCreate->x0,pCreate->y0,pCreate->xSize,pCreate->ySize,pCreate->Para,pCreate->ObjAddr,pCreate->Flags);   
      slider->DrawSlider(slider,pCreate->ObjAddr);
      *hWin = (u32)slider;
      return (SLIDER_Handle)hWin;     
}
