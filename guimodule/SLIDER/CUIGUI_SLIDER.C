#include "CUIGUI_SLIDER.H"



//������
void ProssSkin(Slider* slider,COLOR_DATTYPE color,u16 length)
{
	Obj* pobj = (Obj*)slider;
      if(slider->flag_VorH == Horizontal)    //����
      {
            CUIGUI_FillRect(pobj->x,pobj->y+pobj->height/2-pobj->height/2,length,pobj->height,color);

      }else
      {
            CUIGUI_FillRect(pobj->x+pobj->width/2-pobj->width/2,pobj->y,pobj->width,length,color);            
      }
}

//һ��Ƥ��
void SliderSkin0(Slider* slider,COLOR_DATTYPE color,u16 length)
{
      Obj* pobj = (Obj*)slider;
      CUIGUI_FillRect(pobj->x,pobj->y,pobj->width,pobj->height,CUIGUI_Color(221,222,222));
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
	Obj* pobj = (Obj*)slider;
	if(slider->flag_VorH == 0)
	SliderSkin0(slider,slider->BKcolor,pobj->width);
	else SliderSkin0(slider,slider->BKcolor,pobj->height);
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
      Obj* pobj = (Obj*)slider;
      if(slider->flag_VorH == Horizontal)
      {
          //  DrawFullCircle(x,y+pobj->height/2,pobj->height/4,slider->blockcolor);  
            ProssSkin(slider,slider->blockcolor,x - pobj->x);      
      }else
      {
         //   DrawFullCircle(x+pobj->width/2,y,pobj->width/4,slider->blockcolor);
            ProssSkin(slider,slider->blockcolor,y-pobj->y); 
      }
           
}


/************************************************************************************
*-��������	������������
*-����	��
*����ֵ	��
*-��������	��
*-������	�������в�˶���ӿƼ�
*/
void DrawSlider(void* slider)
{     
	u16 posx = 0,posy = 0;
	Obj* pobj = (Obj*)slider;
	Slider* pslider = (Slider*)slider;
	if(pslider->val >= pslider->min_val && pslider->val <= pslider->max_val)      //val��Ч
	{
		pslider->SliderSkinFun(pslider);    
		if(pslider->flag_VorH == Horizontal)    //����
		{
		  //����VAL���㻬��Ӧ���ڵ�λ��
		  posx = pobj->x + pobj->height/2 + (pobj->width - pobj->height)*(pslider->val - pslider->min_val)/(pslider->max_val - pslider->min_val);//��û����x����
		  posy = pobj->y;              		  
		}else
		{
			posx = pobj->x;  
			posy = pobj->y + pobj->width/2 + (pobj->height - pobj->width)*(pslider->val - pslider->min_val)/(pslider->max_val - pslider->min_val);
		}
		 //������  
		pslider->blockSkinFun(pslider,posx,posy);
	}  
}


/************************************************************************************
*-��������	���½�һ��������,������Ĭ��ֵ
*-����	��flag_VorH �������ķ���,0Ϊ����,1Ϊ����
*����ֵ	��
*-��������	��
*-������	����˶���ӿƼ���www.explorersoftware.taobao.com
*/

Slider*  NewSlider(u16 xpos,u16 ypos,u16 width,u16 height,u32 max_val,u32 min_val,DIR flag_VorH)
{    
	Slider* slider = (Slider*)malloc(sizeof(Slider));
	InitOBJParam(slider,xpos,ypos,width,height,DrawSlider);
	slider->max_val = max_val;
	__NOP();
	slider->min_val = min_val;
	slider->val = min_val;
	slider->flag_VorH = flag_VorH;  
	slider->BKcolor = WHITE;
	slider->blockcolor = GREEN;
	slider->SliderSkinFun = &SliderDefaultSkin;
	slider->blockSkinFun = &BlockDefaultSkin;
	return slider;
}
