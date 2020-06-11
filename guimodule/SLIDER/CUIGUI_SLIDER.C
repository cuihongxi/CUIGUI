#include "CUIGUI_SLIDER.H"



//进度条
void ProssSkin(Slider* slider,COLOR_DATTYPE color,u16 length)
{
	Obj* pobj = (Obj*)slider;
      if(slider->flag_VorH == Horizontal)    //横向
      {
            CUIGUI_FillRect(pobj->x,pobj->y+pobj->height/2-pobj->height/2,length,pobj->height,color);

      }else
      {
            CUIGUI_FillRect(pobj->x+pobj->width/2-pobj->width/2,pobj->y,pobj->width,length,color);            
      }
}

//一种皮肤
void SliderSkin0(Slider* slider,COLOR_DATTYPE color,u16 length)
{
      Obj* pobj = (Obj*)slider;
      CUIGUI_FillRect(pobj->x,pobj->y,pobj->width,pobj->height,CUIGUI_Color(221,222,222));
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
	Obj* pobj = (Obj*)slider;
	if(slider->flag_VorH == 0)
	SliderSkin0(slider,slider->BKcolor,pobj->width);
	else SliderSkin0(slider,slider->BKcolor,pobj->height);
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
*-函数名称	：画出滑动条
*-参数	：
*返回值	：
*-函数功能	：
*-创建者	：蓬莱市博硕电子科技
*/
void DrawSlider(void* slider)
{     
	u16 posx = 0,posy = 0;
	Obj* pobj = (Obj*)slider;
	Slider* pslider = (Slider*)slider;
	if(pslider->val >= pslider->min_val && pslider->val <= pslider->max_val)      //val有效
	{
		pslider->SliderSkinFun(pslider);    
		if(pslider->flag_VorH == Horizontal)    //横向
		{
		  //根据VAL计算滑块应该在的位置
		  posx = pobj->x + pobj->height/2 + (pobj->width - pobj->height)*(pslider->val - pslider->min_val)/(pslider->max_val - pslider->min_val);//获得滑块的x坐标
		  posy = pobj->y;              		  
		}else
		{
			posx = pobj->x;  
			posy = pobj->y + pobj->width/2 + (pobj->height - pobj->width)*(pslider->val - pslider->min_val)/(pslider->max_val - pslider->min_val);
		}
		 //画滑块  
		pslider->blockSkinFun(pslider,posx,posy);
	}  
}


/************************************************************************************
*-函数名称	：新建一个滑动条,并分配默认值
*-参数	：flag_VorH 滑动条的方向,0为横向,1为纵向
*返回值	：
*-函数功能	：
*-创建者	：博硕电子科技，www.explorersoftware.taobao.com
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

