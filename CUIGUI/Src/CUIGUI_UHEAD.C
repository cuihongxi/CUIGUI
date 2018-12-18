#include "CUIGUI_UHEAD.H"
#include "CUIGUI_COLOR.H"

//根据坐标和半径，返回与y1相对应的x1坐标
u32 GetCircle_X(u32 x0,u32 y0,u16 r,u16 y1)
{
	int a,b;
	int di;
	a=0;b=r;	  
	di=3-(r<<1);             //判断下个点位置的标志
	while(a<=b)
	{
            if(y0-b == y1) return (u32)(x0+a)<<16 |(x0-a);
            if(y0-a == y1) return (u32)(x0+b)<<16 |(x0-b);
            if(y0+a == y1) return (u32)(x0+b)<<16 |(x0-b);
            if(y0+b == y1) return (u32)(x0+a)<<16 |(x0-a);

		a++;
		//使用Bresenham算法画圆     
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 						    
	}
      return 0xffff;      //没有相交的点返回0xffff
}




//皮肤1
void Obj_SkinUnPressed(Obj* obj)
{
      CUIGUI_FillRect(obj->x,obj->y,obj->width,obj->height,BKCOLLOR);      
      //画3D边沿            
      CUIGUI_DrawHLine(obj->x-1,obj->y+obj->height+1,obj->x+obj->width+1,BKEDGE0COLLOR);	
      CUIGUI_DrawVLine(obj->x+obj->width+1,obj->y,obj->y+obj->height,BKEDGE0COLLOR);

      CUIGUI_DrawHLine(obj->x-2,obj->y+obj->height+2,obj->x+obj->width+2,BKEDGE2COLLOR);	
      CUIGUI_DrawVLine(obj->x+obj->width+2,obj->y-1,obj->y+obj->height+2,BKEDGE2COLLOR);

      CUIGUI_DrawHLine(obj->x-1,obj->y-1,obj->x+obj->width,BKEDGE1COLLOR);	
      CUIGUI_DrawVLine(obj->x-1,obj->y,obj->y+obj->height,BKEDGE1COLLOR);

      CUIGUI_DrawHLine(obj->x-2,obj->y-2,obj->x+obj->width+1,BKEDGE3COLLOR);	
      CUIGUI_DrawVLine(obj->x-1,obj->y-1,obj->y+obj->height+1,BKEDGE3COLLOR);      

}

void Obj_SkinPressed(Obj* obj)
{
      CUIGUI_FillRect(obj->x,obj->y,obj->width,obj->height,BKCOLLOR);
      //画3D边沿          
      CUIGUI_DrawHLine(obj->x-1,obj->y+obj->height+1,obj->x+obj->width+1,BKEDGE1COLLOR);	
      CUIGUI_DrawVLine(obj->x+obj->width+1,obj->y,obj->y+obj->height,BKEDGE1COLLOR);

      CUIGUI_DrawHLine(obj->x-2,obj->y+obj->height+2,obj->x+obj->width+2,BKEDGE3COLLOR);	
      CUIGUI_DrawVLine(obj->x+obj->width+2,obj->y-1,obj->y+obj->height+2,BKEDGE3COLLOR);

      CUIGUI_DrawHLine(obj->x-1,obj->y-1,obj->x+obj->width,BKEDGE0COLLOR);	
      CUIGUI_DrawVLine(obj->x-1,obj->y,obj->y+obj->height,BKEDGE0COLLOR);

      CUIGUI_DrawHLine(obj->x-2,obj->y-2,obj->x+obj->width+1,BKEDGE2COLLOR);	
      CUIGUI_DrawVLine(obj->x-1,obj->y-1,obj->y+obj->height+1,BKEDGE2COLLOR);       

}










