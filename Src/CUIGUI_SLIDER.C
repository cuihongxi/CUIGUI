#include "CUIGUI_SLIDER.H"
#include "GUI.H"
#include "CUI_MALLCO.H"
#include "CUIGUI_COLOR.H"
#include "CUIGUI_LCDDRIVER.H"


//������
void ProssSkin(Slider* slider,COLOR_DATTYPE color,u16 length)
{
      if(slider->flag_VorH==0)    //����
      {
            CUIGUI_FillRect(slider->x,slider->y+slider->height/2-slider->height/5/2,length,slider->height/5,color);

      }else
      {
            CUIGUI_FillRect(slider->x+slider->width/2-slider->width/5/2,slider->y,slider->width/5,length,color);            
      }
}

//һ��Ƥ��
void SliderSkin0(Slider* slider,COLOR_DATTYPE color,u16 length)
{
      
      CUIGUI_FillRect(slider->x,slider->y,slider->width,slider->height,CUIGUI_Color(221,222,222));
      ProssSkin(slider,color,length);
}


/************************************************************************************
*-��������	����������Ĭ��Ƥ��
*-����	��
*����ֵ	��
*-��������	��
*-������	�������в�˶���ӿƼ�
*/

void SliderDefaultSkin(Slider* slider)
{
      if(slider->flag_VorH == 0)
      SliderSkin0(slider,slider->BKcolor,slider->width);
      else SliderSkin0(slider,slider->BKcolor,slider->height);
}

/************************************************************************************
*-��������	�������Ĭ��Ƥ��
*-����	��
*����ֵ	��
*-��������	��
*-������	�������в�˶���ӿƼ�
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
*-��������	������������
*-����	��
*����ֵ	��
*-��������	��
*-������	�������в�˶���ӿƼ�
*/
void DrawSlider(Slider* slider,u32 val)
{     
      u16 posx = 0,posy = 0;
      
      if(val>=slider->min_dat && val<=slider->max_dat)      //val��Ч
      {
            slider->SliderSkinFun(slider);    
            if(slider->flag_VorH==0)    //����
            {
              //����VAL���㻬��Ӧ���ڵ�λ��
              posx = slider->x+slider->height/2 + (slider->width-slider->height)*(val - slider->min_dat)/(slider->max_dat - slider->min_dat);//��û����x����
              posy = slider->y;              
              
            }else
            {
                posx = slider->x;  
                posy = slider->y + slider->width/2 + (slider->height-slider->width)*(val - slider->min_dat)/(slider->max_dat - slider->min_dat);
            }
             //������  
            slider->blockSkinFun(slider,posx,posy);
                 
            
      }

      
}


/************************************************************************************
*-��������	���½�һ��������,������Ĭ��ֵ
*-����	��flag_VorH �������ķ���,0Ϊ����,1Ϊ����
*����ֵ	��
*-��������	��
*-������	����˶���ӿƼ���www.explorersoftware.taobao.com
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
      slider->DrawSlider = &DrawSlider;  //ָ��������������Ĭ�Ϻ���
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