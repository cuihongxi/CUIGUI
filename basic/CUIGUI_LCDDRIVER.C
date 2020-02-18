#include "CUIGUI_LCDDRIVER.H"
/*********************************************************

      添加LCD驱动
*/
#include "LCD.H"

/************************************************************************************
*-函数名称	：初始化LCD
*-参数	：
*返回值	：
*-函数功能	：
*-创建者	：蓬莱市博硕电子科技
*/

void CUIGUI_Init(COLOR_DATTYPE backColor) 
{    
    LCD_Init();
    LCD_Clear(backColor);
	
}

/************************************************************************************
*-函数名称	：画点
*-参数	：
*返回值	：
*-函数功能	：
*-创建者	：蓬莱市博硕电子科技
*/

void CUIGUI_DrawPoint(u16 x, u16 y, COLOR_DATTYPE color) 
{
    LCD_Fast_DrawPoint(x,y,color);
}

/************************************************************************************
*-函数名称	：
*-参数	：
*返回值	：
*-函数功能	：不输入坐标，快速画点
*-创建者	：蓬莱市博硕电子科技
*/

#if FAST_DRAW > 0

void CUIGUI_FastStart()
{
      LCD_WriteRAM_Prepare();         //开始写入GRAM
}
void CUIGUI_FastDrawPoint(COLOR_DATTYPE color)
{      
        LCD->LCD_RAM=color;//写入数据 
}

/************************************************************************************
*-函数名称	：对点快速异或
*-参数	：numColor 颜色数，colordat 颜色值
*返回值	：
*-函数功能  ：
*-创建者	：蓬莱市博硕电子科技
*/
void CUIGUI_FastXOR(u32 numColor,u32 colordat)
{
      CUIGUI_FastDrawPoint(numColor-colordat-1);
}
#endif
/************************************************************************************
*-函数名称	：
*-参数	：
*返回值	：
*-函数功能	：设定坐标
*-创建者	：蓬莱市博硕电子科技
*/
void CUIGUI_SetPosition(u16 x, u16 y)
{
  LCD_SetCursor(x,y);   	    //设置光标位置 
}
/************************************************************************************
*-函数名称	  ：获取点
*-参数		  ：
*返回值		  ：
*-函数功能	  ：
*-创建者		  ：蓬莱市博硕电子科技
*/
COLOR_DATTYPE CUIGUI_ReadPoint(u16 x, u16 y) 
{
  return(COLOR_DATTYPE)LCD_ReadPoint(x,y);
}


/************************************************************************************
*-函数名称	  ：画横线
*-参数		  ：
*返回值		  ：
*-函数功能	  ：
*-创建者		  ：蓬莱市博硕电子科技
*/
void CUIGUI_DrawHLine(u16 x0,u16 y,u16 x1,COLOR_DATTYPE color) 
{
      u16 i ;
      LCD_SetCursor(x0,y);   	    //设置光标位置 
      LCD_WriteRAM_Prepare();         //开始写入GRAM
      for(i=x0;i<x1;i++)
      {
       // CUIGUI_DrawPoint(i,y,color);//画点 
        LCD->LCD_RAM=color;//写入数据 
      }
}


/************************************************************************************
*-函数名称	  ：填充矩形区域
*-参数		  ：
*返回值		  ：
*-函数功能	  ：
*-创建者		  ：蓬莱市博硕电子科技
*/
void CUIGUI_FillRect(u16 x0, u16 y0, u16 width, u16 height,COLOR_DATTYPE color)
{
   // u16 height,width;
	u16 i,j;
	//width=x1-x0+1; 			//得到填充的宽度
	//height=y1-y0+1;			//高度
 	for(i=0;i<height;i++)
	{
 		LCD_SetCursor(x0,y0+i);   	  //设置光标位置 
		LCD_WriteRAM_Prepare();         //开始写入GRAM
		for(j=0;j<width;j++)
            LCD->LCD_RAM = color;//写入数据 
	}   
}


/************************************************************************************
*-函数名称	  ：对点异或
*-参数		  ：
*返回值		  ：
*-函数功能	  ：
*-创建者		  ：蓬莱市博硕电子科技
*/
void CUIGUI_XORPoint(u16 x, u16 y) 
{    
    u16 dat = CUIGUI_ReadPoint(x,y);
    CUIGUI_DrawPoint(x,y,LCD_NUM_COLORS-dat-1);  
}
       


//返回异或颜色
u32 GetXORColor(u32 color)
{
      return(LCD_NUM_COLORS - color -1);
}

/************************************************************************************
*-函数名称	  ：画竖线
*-参数		  ：
*返回值		  ：
*-函数功能	  ：
*-创建者		  ：蓬莱市博硕电子科技
*/
void CUIGUI_DrawVLine(u16 x,u16 y0,u16 y1,COLOR_DATTYPE color) 
{
    u16 i ;
    for(i=y0;i<y1;i++)
    {
        CUIGUI_DrawPoint(x,i,color);//画点 
    }
}



//画圆
void CUIGUI_Draw_Circle(u16 x0,u16 y0,u8 r,COLOR_DATTYPE color)
{
	int a,b;
	int di;
	a=0;b=r;	  
	di=3-(r<<1);             //判断下个点位置的标志
	while(a<=b)
	{
		CUIGUI_DrawPoint(x0+a,y0-b,color);             //5
 		CUIGUI_DrawPoint(x0+b,y0-a,color);             //0           
		CUIGUI_DrawPoint(x0+b,y0+a,color);             //4               
		CUIGUI_DrawPoint(x0+a,y0+b,color);             //6 
		CUIGUI_DrawPoint(x0-a,y0+b,color);             //1       
 		CUIGUI_DrawPoint(x0-b,y0+a,color);             
		CUIGUI_DrawPoint(x0-a,y0-b,color);             //2             
  		CUIGUI_DrawPoint(x0-b,y0-a,color);             //7     	         
		a++;
		//使用Bresenham算法画圆     
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 						    
	}
} 

//画线
//x1,y1:起点坐标
//x2,y2:终点坐标  
void CUIGUI_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2,COLOR_DATTYPE color)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
	delta_x=x2-x1; //计算坐标增量 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //设置单步方向 
	else if(delta_x==0)incx=0;//垂直线 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//水平线 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//画线输出 
	{  
		CUIGUI_DrawPoint(uRow,uCol,color);//画点 
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
}    

//画一个实心圆
void DrawFullCircle(u16 x0, u16 y0 ,u16 r , u16 color)
{
	int a,b;
	int di;
	a=0;b=r;	  
	di=3-(r<<1);             //判断下个点位置的标志
	while(a<=b)
	{
            CUIGUI_DrawHLine(x0-a,y0-b,x0+a,color);
            CUIGUI_DrawHLine(x0-b,y0-a,x0+b,color);
            CUIGUI_DrawHLine(x0-a,y0+b,x0+a,color);
            CUIGUI_DrawHLine(x0-b,y0+a,x0+b,color);
            a++;
		//使用Bresenham算法画圆     
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 						    
	}      
}
