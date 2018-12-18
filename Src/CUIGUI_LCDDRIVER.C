#include "CUIGUI_LCDDRIVER.H"
/*********************************************************

      ����LCD����
*/
#include "LCD.H"

/************************************************************************************
*-��������	����ʼ��LCD
*-����	��
*����ֵ	��
*-��������	��
*-������	�������в�˶���ӿƼ�
*/

void CUIGUI_Init(COLOR_DATTYPE backColor) 
{    
    LCD_Init();
    LCD_Clear(backColor);
	
}

/************************************************************************************
*-��������	������
*-����	��
*����ֵ	��
*-��������	��
*-������	�������в�˶���ӿƼ�
*/

void CUIGUI_DrawPoint(u16 x, u16 y, COLOR_DATTYPE color) 
{
    LCD_Fast_DrawPoint(x,y,color);
}

/************************************************************************************
*-��������	��
*-����	��
*����ֵ	��
*-��������	�����������꣬���ٻ���
*-������	�������в�˶���ӿƼ�
*/

#if FAST_DRAW > 0

void CUIGUI_FastStart()
{
      LCD_WriteRAM_Prepare();         //��ʼд��GRAM
}
void CUIGUI_FastDrawPoint(COLOR_DATTYPE color)
{      
        LCD->LCD_RAM=color;//д������ 
}

#endif
/************************************************************************************
*-��������	��
*-����	��
*����ֵ	��
*-��������	���趨����
*-������	�������в�˶���ӿƼ�
*/
void CUIGUI_SetPosition(u16 x, u16 y)
{
  LCD_SetCursor(x,y);   	    //���ù��λ�� 
}
/************************************************************************************
*-��������	  ����ȡ��
*-����		  ��
*����ֵ		  ��
*-��������	  ��
*-������		  �������в�˶���ӿƼ�
*/
COLOR_DATTYPE CUIGUI_ReadPoint(u16 x, u16 y) 
{
  return(COLOR_DATTYPE)LCD_ReadPoint(x,y);
}


/************************************************************************************
*-��������	  ��������
*-����		  ��
*����ֵ		  ��
*-��������	  ��
*-������		  �������в�˶���ӿƼ�
*/
void CUIGUI_DrawHLine(u16 x0,u16 y,u16 x1,COLOR_DATTYPE color) 
{
      u16 i ;
      LCD_SetCursor(x0,y);   	    //���ù��λ�� 
      LCD_WriteRAM_Prepare();         //��ʼд��GRAM
      for(i=x0;i<x1;i++)
      {
       // CUIGUI_DrawPoint(i,y,color);//���� 
        LCD->LCD_RAM=color;//д������ 
      }
}


/************************************************************************************
*-��������	  ������������
*-����		  ��
*����ֵ		  ��
*-��������	  ��
*-������		  �������в�˶���ӿƼ�
*/
void CUIGUI_FillRect(u16 x0, u16 y0, u16 width, u16 height,COLOR_DATTYPE color)
{
   // u16 height,width;
	u16 i,j;
	//width=x1-x0+1; 			//�õ����Ŀ���
	//height=y1-y0+1;			//�߶�
 	for(i=0;i<height;i++)
	{
 		LCD_SetCursor(x0,y0+i);   	  //���ù��λ�� 
		LCD_WriteRAM_Prepare();         //��ʼд��GRAM
		for(j=0;j<width;j++)
            LCD->LCD_RAM = color;//д������ 
	}   
}

#if FAST_DRAW > 0
/************************************************************************************
*-��������	���Ե�������
*-����	��numColor ��ɫ����colordat ��ɫֵ
*����ֵ	��
*-��������  ��
*-������	�������в�˶���ӿƼ�
*/
void CUIGUI_FastXOR(u32 numColor,u32 colordat)
{
      CUIGUI_FastDrawPoint(numColor-colordat-1);
}
#endif

/************************************************************************************
*-��������	  ���Ե����
*-����		  ��
*����ֵ		  ��
*-��������	  ��
*-������		  �������в�˶���ӿƼ�
*/
void CUIGUI_XORPoint(u16 x, u16 y) 
{    
    u16 dat = CUIGUI_ReadPoint(x,y);
    CUIGUI_DrawPoint(x,y,LCD_NUM_COLORS-dat-1);  
}
       


//���������ɫ
u32 GetXORColor(u32 color)
{
      return(LCD_NUM_COLORS - color -1);
}

/************************************************************************************
*-��������	  ��������
*-����		  ��
*����ֵ		  ��
*-��������	  ��
*-������		  �������в�˶���ӿƼ�
*/
void CUIGUI_DrawVLine(u16 x,u16 y0,u16 y1,COLOR_DATTYPE color) 
{
    u16 i ;
    for(i=y0;i<y1;i++)
    {
        CUIGUI_DrawPoint(x,i,color);//���� 
    }
}

/************************************************************************************
*-��������	����ȡLCD��������
*-����	��
*����ֵ	��
*-��������	��
*-������	�������в�˶���ӿƼ�
*/

u16 CUIGUI_GetHpix()
{
      if(Display_Dir == H_DIR)
      return 800;
      else return 480;
}

/************************************************************************************
*-��������	����ȡLCD��������
*-����	��
*����ֵ	��
*-��������	��
*-������	�������в�˶���ӿƼ�
*/
u16 CUIGUI_GetVpix()
{
      if(Display_Dir == H_DIR)
      return 480;
      else return 800;
}

//��Բ
void CUIGUI_Draw_Circle(u16 x0,u16 y0,u8 r,COLOR_DATTYPE color)
{
	int a,b;
	int di;
	a=0;b=r;	  
	di=3-(r<<1);             //�ж��¸���λ�õı�־
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
		//ʹ��Bresenham�㷨��Բ     
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 						    
	}
} 

//����
//x1,y1:�������
//x2,y2:�յ�����  
void CUIGUI_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2,COLOR_DATTYPE color)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
	delta_x=x2-x1; //������������ 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //���õ������� 
	else if(delta_x==0)incx=0;//��ֱ�� 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//ˮƽ�� 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //ѡȡ�������������� 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//������� 
	{  
		CUIGUI_DrawPoint(uRow,uCol,color);//���� 
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

//��һ��ʵ��Բ
void DrawFullCircle(u16 x0, u16 y0 ,u16 r , u16 color)
{
	int a,b;
	int di;
	a=0;b=r;	  
	di=3-(r<<1);             //�ж��¸���λ�õı�־
	while(a<=b)
	{
            CUIGUI_DrawHLine(x0-a,y0-b,x0+a,color);
            CUIGUI_DrawHLine(x0-b,y0-a,x0+b,color);
            CUIGUI_DrawHLine(x0-a,y0+b,x0+a,color);
            CUIGUI_DrawHLine(x0-b,y0+a,x0+b,color);
            a++;
		//ʹ��Bresenham�㷨��Բ     
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 						    
	}      
}