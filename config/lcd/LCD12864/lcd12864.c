#include "LCD12864.h"
volatile static u16 datMap[32][16] = {0};
//********************************************************************
//LCD12864  忙 信号检测,读地址AC
//********************************************************************

void LCD12864_GPIO_INIT()
{

}

//;配置成输入模式
void LCD12864_IO_OUT()
{       

      GPIO_InitTypeDef GPIO_TypeStruc;      
      GPIO_TypeStruc.Mode = GPIO_MODE_OUTPUT_PP;
      GPIO_TypeStruc.Speed = GPIO_SPEED_FREQ_HIGH;
			GPIO_TypeStruc.Pull = GPIO_PULLUP;
      GPIO_TypeStruc.Pin = LCD_D0_Pin|LCD_D1_Pin|LCD_D2_Pin|LCD_D3_Pin|LCD_D4_Pin;
      HAL_GPIO_Init(LCD_D0_GPIO_Port,&GPIO_TypeStruc);
      
      GPIO_TypeStruc.Pin = LCD_D5_Pin|LCD_D6_Pin|LCD_D7_Pin;
      HAL_GPIO_Init(LCD_D5_GPIO_Port,&GPIO_TypeStruc);
      
}

//向12864中的数据位传入数据
void LCD12864_wData(unsigned char dat)
{ 
      LCD12864_IO_OUT(); 
    if((dat&0x01))Set(D0);
        else Clr_D0();
    if((dat&0x02))Set(D1);
        else Clr_D1();
    if((dat&0x04)) Set(D2);
        else Clr_D2();
    if((dat&0x08))Set(D3);
        else Clr_D3();
    if((dat&0x10)) Set(D4);
        else Clr_D4();
    if((dat&0x20))Set(D5);
        else Clr_D5();
    if((dat&0x40))Set(D6);
        else Clr_D6();
    if((dat&0x80)) Set(D7);
        else Clr_D7();
}

//;配置成输入模式
void LCD12864_IO_IN()
{

      GPIO_InitTypeDef GPIO_TypeStruc;      
      GPIO_TypeStruc.Mode = GPIO_MODE_INPUT;
      GPIO_TypeStruc.Speed = GPIO_SPEED_FREQ_HIGH;
			GPIO_TypeStruc.Pull = GPIO_PULLDOWN;
      GPIO_TypeStruc.Pin = LCD_D0_Pin|LCD_D1_Pin|LCD_D2_Pin|LCD_D3_Pin|LCD_D4_Pin;
      HAL_GPIO_Init(LCD_D0_GPIO_Port,&GPIO_TypeStruc);
      
      GPIO_TypeStruc.Pin = LCD_D5_Pin|LCD_D6_Pin|LCD_D7_Pin;
      HAL_GPIO_Init(LCD_D5_GPIO_Port,&GPIO_TypeStruc);
      
}

//读数据位的数据
unsigned char LCD12864_rDat()
{
    unsigned char dat = 0;
    LCD12864_IO_IN();
     
    if(Read(D0)) dat |= 0x01;
    if(Read(D1)) dat |= 0x02;
    if(Read(D2)) dat |= 0x04;
    if(Read(D3)) dat |= 0x08;
    if(Read(D4)) dat |= 0x10;
    if(Read(D5)) dat |= 0x20;
    if(Read(D6)) dat |= 0x40;
    if(Read(D7)) dat |= 0x80;
    return dat;
}

unsigned char LCD_readWait()
{
     LCD12864_IO_IN();
     return(Read(D7));
      
}

unsigned char LCD12864_WaitIdle()
{    
     // unsigned char ac;
	//LCD12864_wData(0xff);
      
	Clr_RS();
	Set_RW();
	Set_E();
    //  //lcd_delay_us(10);
      while(LCD_readWait()==1); /*等待BF 不为1*/
   //   {};
      //ac = LCD12864_rDat();      
	Clr_E();       
      
      return 0;//(ac|0x80);
}

//********************************************************************
//检测忙信号写入命令字	com_da 为待写入的命令字
//********************************************************************
void LCD12864_COM_Write( unsigned char com_da)	
{    
	LCD12864_WaitIdle();    
	Clr_RS();
	Clr_RW();
    	Set_E();
	//lcd_delay_us(1);
	LCD12864_wData(com_da);
    //  lcd_delay_us(1);
	Clr_E();	

}

//********************************************************************
//LCD12864 从地址中读数据
//********************************************************************

unsigned char  LCD12864_ReadOneDat()
{
    
     unsigned char dat;
     LCD12864_WaitIdle();
      
  	LCD12864_wData(0);
	Set_RS();
	Set_RW();
	Set_E();
    //  lcd_delay_us(1);     
      dat = LCD12864_rDat();
	Clr_E();

    return (dat);
      
      
}
u16 LCD12864_ReadDAT(unsigned char addr)
{
    u16 dat;
    
    LCD12864_COM_Write(addr);
    LCD12864_ReadOneDat();
    dat = (u16)LCD12864_ReadOneDat()<<8;
    dat |=  LCD12864_ReadOneDat();

    return (dat);
      
}
//********************************************************************
//不检测忙信号写入命令字		com_da 为待写入的命令字
//********************************************************************
void LCD12864_NoWaitIdle_COM_Write(unsigned char com_da)	
{
    
	Clr_RS();
	Clr_RW();
      Set_E();
     // lcd_delay_us(1);
	LCD12864_wData(com_da);
    //  lcd_delay_us(1);
	Clr_E();	
      
}

//********************************************************************
//数据写入		da  为待写入的8位数据
//********************************************************************
void LCD12864_Data_Write(unsigned char da)
{
	LCD12864_WaitIdle(); /*检测忙信号*/    
	Set_RS();
	Clr_RW();
      Set_E();
	//lcd_delay_us(1);
	LCD12864_wData(da);
     // lcd_delay_us(1);
	Clr_E();
      
}


//*************************************************************************************
//写连续字符函数
//*************************************************************************************
void LCD12864_write_word(unsigned char *s)
{
	for(;*s!=0;s++)
	{
        LCD12864_Data_Write(*s);
	}
}



//********************************************************************
//LCD12864初始化
//********************************************************************
void LCD12864_Init()
{
      
	lcd_delay_ms(10); /*适当延时待LCD自动复位完成*/
   LCD12864_GPIO_INIT();
	LCD12864_NoWaitIdle_COM_Write(0x30); /*使用8位并口通讯*/
	lcd_delay_ms(1);
	LCD12864_NoWaitIdle_COM_Write(0x30); /*使用8位并口通讯*/
	lcd_delay_ms(1);
	LCD12864_NoWaitIdle_COM_Write(0x0c); /*显示开及光标设置*/
	lcd_delay_ms(1);
	LCD12864_NoWaitIdle_COM_Write(0x01); /*显示清屏*/
	lcd_delay_ms(3);
	LCD12864_NoWaitIdle_COM_Write(0x06); /*DDRAM的地址计数器（AC）加1*/
	lcd_delay_ms(1);	
}

void LCD12864_Setposition(unsigned char xpos,unsigned char ypos)
{
    unsigned char xy_pos;
    
	if((xpos>=8)||(ypos>=4) ) return; /*X位置超出显示范围退出*/
	if(ypos==0)         xy_pos = 0x80 + xpos;
	else if(ypos==1) xy_pos = 0x90 + xpos; /*计算转换地址*/
	else if(ypos==2) xy_pos = 0x88 + xpos;
	else if(ypos==3) xy_pos = 0x98 + xpos;
    else if(ypos==4) xy_pos = LINE0_N + xpos; /*计算转换地址*/
	else if(ypos==5) xy_pos = LINE1_N + xpos;
	else if(ypos==6) xy_pos = LINE2_N + xpos;
    else if(ypos==7) xy_pos = LINE3_N + xpos;
    LCD12864_COM_Write(0x30);
    LCD12864_COM_Write(xy_pos); /*写地址*/
      
      
}


void LCD12864_String(unsigned char  x,unsigned char  y,unsigned char* str)
{
    //LCD12864_COM_Write(0x30);
      unsetRE();//不执行扩展指令
     LCD12864_Setposition(x,y);
    // lcd_delay_ms(1);
      
    LCD12864_write_word(str);
}

    
///////////////////////////////////
//使用扩展指令
void setRE()
{
    LCD12864_COM_Write(0x34);
}
//不使用扩展指令
void unsetRE()
{
    LCD12864_COM_Write(0x30);
}

//将0x88~0x8f,0x98~0x9f,的数据拷贝到0xA0~0XA8,0XB0~0XB8中
void copySTRtoNex(unsigned char start,unsigned char end)
{
    unsigned char i;
    u16 dat ;
    for(i=0;i<8;i++)
    {
        dat = LCD12864_ReadDAT(start+i);
        LCD12864_COM_Write(end+i);
        LCD12864_Data_Write((unsigned char)(dat>>8));
        LCD12864_Data_Write((unsigned char)dat);
    }
    for(i=0;i<8;i++)
    {
        dat = LCD12864_ReadDAT(start+i+16);
        LCD12864_COM_Write(end+i+16);
        LCD12864_Data_Write((unsigned char)(dat>>8));
        LCD12864_Data_Write((unsigned char)dat);
    }
    
}

void copySTRtoBUF(unsigned char start,u16* buf)
{
    unsigned char i;

    for(i=0;i<8;i++)
    {
        buf[i] = LCD12864_ReadDAT(start+i);
        
    }
        for(i=0;i<8;i++)
    {
        buf[i+8] = LCD12864_ReadDAT(start+i+16);
        
    }
    
}
void copyBUFtoSTR(unsigned char start,u16* buf)
{
    unsigned char i;
    
    for(i=0;i<8;i++)
    {
        
        LCD12864_COM_Write(start+i);
        LCD12864_Data_Write((unsigned char)(buf[i]>>8));
        LCD12864_Data_Write((unsigned char)buf[i]);
    }
        for(i=0;i<8;i++)
    {
        LCD12864_ReadDAT(start+i+16);
        LCD12864_COM_Write(start+i+16);
        LCD12864_Data_Write((unsigned char)(buf[i+8]>>8));
        LCD12864_Data_Write((unsigned char)buf[i+8]);
    }
}
//清除显示区数据
void clearRAM()
{
    unsigned char i;
    LCD12864_COM_Write(0x80);
    for(i=0;i<60;i++)
    {
        LCD12864_Data_Write(' ');
    }
    
}
////滚动输出
//void Rullplay(u16 time)
//{
//    unsigned char i;
//    u16 show_BUF[16]={0};
//    
//    copySTRtoBUF(LINE0_N,show_BUF);
//    copySTRtoNex(LINE2,LINE0_N);//将0x88~0x8f,0x98~0x9f,的数据拷贝到0xA0~0XA8,0XB0~0XB8中
//    setRE();
//    LCD12864_COM_Write(0x03);//允许输入垂直卷动地zhi
//    for(i=0;i<32;i++)
//    {
//         LCD12864_COM_Write(0x40+i);
//         delay_ms(100);
//    }
//    unsetRE();
//    
////    clearRAM();
//   
//    copySTRtoNex(LINE2_N,LINE0);
//    copyBUFtoSTR(LINE2,show_BUF);
//    setRE();
//    LCD12864_COM_Write(0x03);//允许输入垂直卷动地zhi
//    for(i=32;i<64;i++)
//    {
//         LCD12864_COM_Write(0x40+i);
//         delay_ms(time);
//    }
//    unsetRE();
//    
// }


void rollto(unsigned char line)
{
    setRE();
    LCD12864_COM_Write(0x03);//允许输入垂直卷动地zhi
    LCD12864_COM_Write(0x40+line);
    unsetRE();
}

//清空GDRAM,总共就是写1KB的0x00。
void LCD12864_clearGDR(void)
{

    unsigned char x,y; 
    LCD12864_COM_Write(0x34); 
    for(y=0;y<32;y++) 
    {
        for(x=0;x<16;x++) 
        {  
            LCD12864_COM_Write(0x34); 
            LCD12864_COM_Write(y+0x80);        //行地址 
            LCD12864_COM_Write(x+0x80);     //列地址 
            LCD12864_COM_Write(0x30); 
            LCD12864_Data_Write(0); 
            LCD12864_Data_Write(0x00); 
						datMap[y][x] = 0;
        }   
    }

                               
   LCD12864_COM_Write(0x36); 
} 

//x范围：0~127；y范围0~63
//作用：画点
void LCD12864_Drawpoint(unsigned char x,unsigned char y,unsigned char displayornot)
{

    unsigned char zuobiao_x0,zuobiao_y0,zuobiao_y1;
    u16 zuobiao_x1,dat;
    
    zuobiao_y0 = y/32;  //在上行0，在下行1
    zuobiao_y1 = y%32;  //行地址 
    zuobiao_x0 = x/16;  //列地址的分量
    zuobiao_x1 = (u16)0x8000>>(x%16);
    
		dat = datMap[zuobiao_y1][zuobiao_y0*8 + zuobiao_x0];
    if(displayornot==0) zuobiao_x1 = (~zuobiao_x1)&dat;
    else zuobiao_x1 |= dat;
    
    LCD12864_COM_Write(0x80 + zuobiao_y1);     //行地址 
    LCD12864_COM_Write(0x80 + zuobiao_y0*8 + zuobiao_x0);     //列地址 
    
    LCD12864_Data_Write((unsigned char)(zuobiao_x1>>8)); 
    LCD12864_Data_Write((unsigned char)zuobiao_x1);
    datMap[zuobiao_y1][zuobiao_y0*8 + zuobiao_x0] = zuobiao_x1;

}

//画实心矩形
void LCD12864_DrawRectangle(u16 position_x,u16 position_y, u16 datH,u16 datL)
{
		unsigned char zuobiao_x0=0,zuobiao_x1=0,zuobiao_y0=0,zuobiao_y1=0;
    unsigned char datL_left=0,datL_right=0,datL_size=0;
    u16 dat=0,dat0=0,dat1=0;
    unsigned char i=0,j=0;
 
    zuobiao_x0 = position_x/16;  //列地址的分量
    zuobiao_x1 = position_x%16;  //字节中的位置
    datL_left = 16 - zuobiao_x1;
    
    //变换数据并画点
    LCD12864_COM_Write(0x34); //执行扩展指令，关画图显示
    
    
    for(j=0;j<datH;j++)
    {
        zuobiao_y0 = (position_y+j)/32;  //在上行0，在下行1
        zuobiao_y1 = (position_y+j)%32;  //行地址 
        
        LCD12864_COM_Write(0x80 + zuobiao_y1);     //行地址 
        LCD12864_COM_Write(0x80 + zuobiao_y0*8 + zuobiao_x0);     //列地址 


        if(datL>datL_left)//图片大小超出了剩余容量
        {
            datL_right = (datL - datL_left)%16; 
            datL_size = (datL - datL_left)/16;
            //读当前地址数据
            LCD12864_ReadOneDat();//假读一次
            dat = (u16)LCD12864_ReadOneDat()<<8;//读高8位
            dat |=  LCD12864_ReadOneDat();//读低8位
            dat = (dat>>datL_left)<<datL_left;
            
            LCD12864_COM_Write(0x80 + zuobiao_y1);     //行地址 
            LCD12864_COM_Write(0x80 + zuobiao_y0*8 + zuobiao_x0);     //列地址 
            if(datL>8)
            {
                //写头
                dat1 = 0xffff;
                dat0 = (u16)(dat1>>zuobiao_x1);
                dat |= dat0;
                LCD12864_Data_Write((unsigned char)(dat>>8)); 
                LCD12864_Data_Write((unsigned char)dat);
                dat = dat1<<datL_left;
                //写体
                for(i=0;i<datL_size;i++)
                {
                    dat1 = 0xffff;
                    dat0 = dat1>>zuobiao_x1;
                    dat |= dat0;
                    LCD12864_Data_Write((unsigned char)(dat>>8)); 
                    LCD12864_Data_Write((unsigned char)dat);
                    dat = dat1<<datL_left;
                }
                //写尾
                //读当前地址数据
                if(datL_right>0)
                {
										if(datL/8%2)	//剩一个8位
										{
											dat = dat|(0xff<<(datL_left-8));
										}

                    LCD12864_COM_Write(0x80 + zuobiao_y1);     //行地址 
                    LCD12864_COM_Write(0x80 + zuobiao_y0*8 + zuobiao_x0 + datL_size + 1);     //列地址 
                    LCD12864_ReadOneDat();//假读一次
                    dat1 = (u16)LCD12864_ReadOneDat()<<8;//读高8位
                    dat1 |=  LCD12864_ReadOneDat();//读低8位
                    dat1 = ((u16)(dat1<<(zuobiao_x1+(datL/8%2)*8)))>>(zuobiao_x1+(datL/8%2)*8);
                    dat |= dat1;
                    LCD12864_COM_Write(0x80 + zuobiao_y1);     //行地址 
                    LCD12864_COM_Write(0x80 + zuobiao_y0*8 + zuobiao_x0 + datL_size + 1);     //列地址 
                    LCD12864_Data_Write((unsigned char)(dat>>8)); 
                    LCD12864_Data_Write((unsigned char)dat);
                }
            }
            else
            {
                //写头
                
                dat0 = 0xff>>(8-datL_left);
                dat |= dat0;
                LCD12864_Data_Write((unsigned char)(dat>>8)); 
                LCD12864_Data_Write((unsigned char)dat);
                dat =((u16)(0x00ff))<<(8+datL_left);

               
                    LCD12864_COM_Write(0x80 + zuobiao_y1);     //行地址 
                    LCD12864_COM_Write(0x80 + zuobiao_y0*8 + zuobiao_x0 + 1);     //列地址 
                    LCD12864_ReadOneDat();//假读一次
                    dat1 = ((u16)LCD12864_ReadOneDat())<<8;//读高8位
                    dat1 |=  LCD12864_ReadOneDat();//读低8位
                    dat1 = ((u16)(dat1<<(datL-datL_left)))>>(datL-datL_left);
                    dat |= dat1;
                    LCD12864_COM_Write(0x80 + zuobiao_y1);     //行地址 
                    LCD12864_COM_Write(0x80 + zuobiao_y0*8 + zuobiao_x0 + 1);     //列地址 
                    LCD12864_Data_Write((unsigned char)(dat>>8)); 
                    LCD12864_Data_Write((unsigned char)dat);

            }        
        }
        else//图片大小未超出了剩余容量
        {
            dat1 = 0xffff<<datL_left;
            dat1 |= (0xffff>>datL)>>zuobiao_x1;
            //读当前地址数据
            LCD12864_ReadOneDat();//假读一次
            dat = (u16)LCD12864_ReadOneDat()<<8;//读高8位
            dat |=  LCD12864_ReadOneDat();//读低8位
            dat &= dat1;
            
            LCD12864_COM_Write(0x80 + zuobiao_y1);     //行地址 
            LCD12864_COM_Write(0x80 + zuobiao_y0*8 + zuobiao_x0);     //列地址 
            if(datL>8)
            {
                //写头
                dat1 = 0xffff;
                dat0 = dat1>>zuobiao_x1;
                dat |= dat0;
                LCD12864_Data_Write((unsigned char)(dat>>8)); 
                LCD12864_Data_Write((unsigned char)dat);

            }
            else
            {
                //写头
                dat0 = ((u16)(0x00ff)<<8)>>zuobiao_x1;
                dat |= dat0;
                LCD12864_Data_Write((unsigned char)(dat>>8)); 
                LCD12864_Data_Write((unsigned char)dat);
               
            }        
        }
            
    }
    LCD12864_COM_Write(0x36); //开画图显示
}



/*******************************************************
*-函数名称	：
*-函数作用	：画一张指定大小的图片
*-参数		：位置：position_x,position_y  高度：datH 宽度datL,图片数组picture
*-返回值	：
*-备注		：
*******************************************************/
void LCD12864_DrawPicture(unsigned char position_x,unsigned char position_y,\
                          unsigned char datH,unsigned char datL,unsigned char* picture,unsigned char color)
{

    unsigned char zuobiao_x0=0,zuobiao_x1=0,zuobiao_y0=0,zuobiao_y1=0;
    unsigned char datL_left=0,datL_right=0,datL_size=0;
    u16 dat=0,dat0=0,dat1=0;
    unsigned char j=0;
 
    zuobiao_x0 = position_x/16;  //列地址的分量
    zuobiao_x1 = position_x%16;  //字节中的位置
    datL_left = 16 - zuobiao_x1;
    
    //变换数据并画点
    for(j=0;j<datH;j++)
    {
        zuobiao_y0 = (position_y+j)/32;  //在上行0，在下行1
        zuobiao_y1 = (position_y+j)%32;  //行地址 

        if(datL>datL_left)//图片大小超出了剩余容量
        {
            datL_right = (datL - datL_left)%16; 
            datL_size = (datL - datL_left)/16;
            //读当前地址数据
					  dat = datMap[zuobiao_y1][zuobiao_y0*8 + zuobiao_x0]; 
            dat = (dat>>datL_left)<<datL_left;
            
            LCD12864_COM_Write(0x80 + zuobiao_y1);     //行地址 
            LCD12864_COM_Write(0x80 + zuobiao_y0*8 + zuobiao_x0);     //列地址 
					  dat1 = ~((0xffff>>datL_left)<<datL_left);
            if(datL>8)
            {
                //写头
                dat0 = ((u16)(*(picture+j*2))<<8)|(*(picture+j*2+1));
                dat0 = (u16)(dat0>>zuobiao_x1);
							if(color == 0)
                dat0 ^= dat1;
							dat |= dat0;
                LCD12864_Data_Write((unsigned char)(dat>>8)); 
                LCD12864_Data_Write((unsigned char)dat);
							datMap[zuobiao_y1][zuobiao_y0*8 + zuobiao_x0] = dat;

                //写体

                //写尾
                //读当前地址数据
                if(datL_right>0)
                {

										dat0 = ((u16)(*(picture+j*2))<<8)|(*(picture+j*2+1));
										dat = dat0<<datL_left;

									  dat0 = datMap[zuobiao_y1][zuobiao_y0*8 + zuobiao_x0 + datL_size + 1]; 
                    dat0 = (u16)((dat0 << datL_right));
									  dat0 =  dat0 >> datL_right;
                    dat1 = 0xffff << (16 - datL_right);
										if(color == 0)dat ^= dat1;
										dat |= dat0;
                    LCD12864_COM_Write(0x80 + zuobiao_y1);     //行地址 
                    LCD12864_COM_Write(0x80 + zuobiao_y0*8 + zuobiao_x0 + datL_size + 1);     //列地址 
                    LCD12864_Data_Write((unsigned char)(dat>>8)); 
                    LCD12864_Data_Write((unsigned char)dat);
									  datMap[zuobiao_y1][zuobiao_y0*8 + zuobiao_x0 + datL_size + 1] = dat;
                }
            }
            else
            {
                //写头
                
                dat0 = ((u8)(*(picture+j)))>>(8-datL_left);
               if(color)
                dat |= dat0;
							else
							{
								dat1 = ~((0xffff>>datL_left)<<datL_left);
								dat |= dat1;
								dat &= ~dat0;
							}								
							
							LCD12864_Data_Write((unsigned char)(dat>>8)); 
							LCD12864_Data_Write((unsigned char)dat);
							datMap[zuobiao_y1][zuobiao_y0*8 + zuobiao_x0] = dat;
							
								//写尾巴
							dat0 =((u16)(*(picture+j)))<<(8+datL_left);
							dat = datMap[zuobiao_y1][zuobiao_y0*8 + zuobiao_x0 + 1];
							dat = ((u16)(dat<<(datL-datL_left)))>>(datL-datL_left);
							
							if(color)
                dat |= dat0;
							else 
							{
								dat1 = 0xffff<<(datL - datL_left);
								dat1 >>= (datL - datL_left);
								dat1 = ~dat1;
								dat0 = ~dat0;
								dat0 &= dat1;
								dat |= dat0;
							}
              LCD12864_COM_Write(0x80 + zuobiao_y1);     //行地址 
              LCD12864_COM_Write(0x80 + zuobiao_y0*8 + zuobiao_x0 + 1);     //列地址 
              LCD12864_Data_Write((unsigned char)(dat>>8)); 
              LCD12864_Data_Write((unsigned char)dat);
							datMap[zuobiao_y1][zuobiao_y0*8 + zuobiao_x0+ 1] = dat;
            }        
        }
        else//图片大小未超出了剩余容量
        {
            dat1 = (u16)(0xffff>>(datL_left - datL));
						dat1 = (u16)(dat1<<(16 - datL));
					  dat1 = ~(dat1 >> zuobiao_x1);
            //读当前地址数据
					  dat = datMap[zuobiao_y1][zuobiao_y0*8 + zuobiao_x0];
            dat &= dat1;
          //  printf("dat = 0x%x ",dat);
            LCD12864_COM_Write(0x80 + zuobiao_y1);     //行地址 
            LCD12864_COM_Write(0x80 + zuobiao_y0*8 + zuobiao_x0);     //列地址 
            if(datL>8)
            {
                
                dat0 = ((u16)(*(picture + j*2))<<8)|(*(picture+j*2+1));
                dat0 = dat0>>zuobiao_x1;

							if(color == 0)
							 dat0 ^= ~dat1;
							dat |= dat0;
							
                LCD12864_Data_Write((unsigned char)(dat>>8)); 
                LCD12864_Data_Write((unsigned char)dat);
								datMap[zuobiao_y1][zuobiao_y0*8 + zuobiao_x0] = dat;
            }
            else
            {
                //写头
              dat0 = ((u16)(*(picture+j))<<8)>>zuobiao_x1;
							//printf(" dat0 = 0x%x",dat0);
							if(color)
                dat |= dat0;
							else 
							{
							dat1 = (u16)(0xffff>>(16 - datL));
							dat1 = (u16)(dat1<<(16 - datL));		
							dat1 = dat1 >> zuobiao_x1;
							dat0 = (~dat0)&dat1;
								
							dat |= dat0;

							}
							//printf("  dat = 0x%x\r\n",dat);
							
                LCD12864_Data_Write((unsigned char)(dat>>8)); 
                LCD12864_Data_Write((unsigned char)dat);
               datMap[zuobiao_y1][zuobiao_y0*8 + zuobiao_x0] = dat;
            }        
        }
            
    }
   // LCD12864_COM_Write(0x36); //开画图显示
}

/*******************************************************
*-函数名称	：
*-函数作用	：//作用：读点
*-参数		：//x范围：0~127；y范围0~63...改点为0返回0，为1返回1
*-返回值	：
*-备注		：
*******************************************************/
unsigned char LCD12864_Readpoint(unsigned char x,unsigned char y)
{
    unsigned char zuobiao_x0,zuobiao_y0,zuobiao_y1;
    u16 zuobiao_x1,dat;
    
    zuobiao_y0 = y/32;  //在上行0，在下行1
    zuobiao_y1 = y%32;  //行地址 
    zuobiao_x0 = x/16;  //列地址的分量
    zuobiao_x1 = 0x8000>>(x%16);
    
//    LCD12864_COM_Write(0x36); //执行扩展指令，开画图显示
    LCD12864_COM_Write(0x80 + zuobiao_y1);     //行地址 
    LCD12864_COM_Write(0x80 + zuobiao_y0*8 + zuobiao_x0);     //列地址 
    
    //读当前地址数据
    LCD12864_ReadOneDat();//假读一次
    dat = (u16)LCD12864_ReadOneDat()<<8;//读高8位
    dat |=  LCD12864_ReadOneDat();//读低8位
    
    if((dat&zuobiao_x1) == 0) return 0;  
    
    else return 1;
}


void LCD12864_Clear()
{
    LCD12864_COM_Write(0x30);
    LCD12864_COM_Write(0x01);
    lcd_delay_us(42);  
}

void LCD12864_CGRAM_ON(void)
{
 LCD12864_COM_Write(0x36);
}
                         
void LCD12864_CGRAM_OFF(void)
{
 LCD12864_COM_Write(0x34);
}
  

void LCD12864_ClearPicture(unsigned char position_x,unsigned char position_y,unsigned char datH,unsigned char datL)
{

    unsigned char zuobiao_x0=0,zuobiao_x1=0,zuobiao_y0=0,zuobiao_y1=0;
    unsigned char datL_left=0;
    u16 dat=0,dat1=0;
    unsigned char j=0;
 
    zuobiao_x0 = position_x/16;  //列地址的分量
    zuobiao_x1 = position_x%16;  //字节中的位置
    datL_left = 16 - zuobiao_x1;
    
    //变换数据并画点
    LCD12864_COM_Write(0x34); //执行扩展指令，关画图显示
    
    
    for(j=0;j<datH;j++)
    {
        zuobiao_y0 = (position_y+j)/32;  //在上行0，在下行1
        zuobiao_y1 = (position_y+j)%32;  //行地址 
        
        LCD12864_COM_Write(0x80 + zuobiao_y1);     //行地址 
        LCD12864_COM_Write(0x80 + zuobiao_y0*8 + zuobiao_x0);     //列地址 


        if(datL>datL_left)//图片大小超出了剩余容量
        {

            //读当前地址数据
            LCD12864_ReadOneDat();//假读一次
            dat = (u16)LCD12864_ReadOneDat()<<8;//读高8位
            dat |=  LCD12864_ReadOneDat();//读低8位
            dat = (dat>>datL_left)<<datL_left;
            
            LCD12864_COM_Write(0x80 + zuobiao_y1);     //行地址 
            LCD12864_COM_Write(0x80 + zuobiao_y0*8 + zuobiao_x0);     //列地址 

            
                //写头

                LCD12864_Data_Write((unsigned char)(dat>>8)); 
                LCD12864_Data_Write((unsigned char)dat);
				LCD12864_COM_Write(0x80 + zuobiao_y1);     //行地址 
				LCD12864_COM_Write(0x80 + zuobiao_y0*8 + zuobiao_x0 + 1);     //列地址 
				LCD12864_ReadOneDat();//假读一次
				dat1 = ((u16)LCD12864_ReadOneDat())<<8;//读高8位
				dat1 |=  LCD12864_ReadOneDat();//读低8位
				dat1 = ((u16)(dat1<<(datL-datL_left)))>>(datL-datL_left);
				LCD12864_COM_Write(0x80 + zuobiao_y1);     //行地址 
				LCD12864_COM_Write(0x80 + zuobiao_y0*8 + zuobiao_x0 + 1);     //列地址 
				LCD12864_Data_Write((unsigned char)(dat1>>8)); 
				LCD12864_Data_Write((unsigned char)dat1);
        }
        else//图片大小未超出了剩余容量
        {
            dat1 = 0xffff<<datL_left;
            dat1 |= (0xffff>>datL)>>zuobiao_x1;
            //读当前地址数据
            LCD12864_ReadOneDat();//假读一次
            dat = (u16)LCD12864_ReadOneDat()<<8;//读高8位
            dat |=  LCD12864_ReadOneDat();//读低8位
			dat &= dat1;
            LCD12864_COM_Write(0x80 + zuobiao_y1);     //行地址 
            LCD12864_COM_Write(0x80 + zuobiao_y0*8 + zuobiao_x0);     //列地址 

			LCD12864_Data_Write((unsigned char)(dat>>8)); 
			LCD12864_Data_Write((unsigned char)dat);
               
                    
        }
            
    }
    LCD12864_COM_Write(0x36); //开画图显示
}

void LCD12864_DrawPicture2(unsigned char position_x,unsigned char position_y,\
                          unsigned char datH,unsigned char datL,unsigned char* picture)
{

    unsigned char zuobiao_x0=0,zuobiao_x1=0,zuobiao_y0=0,zuobiao_y1=0;
    unsigned char datL_left=0,datL_right=0,datL_size=0;
    u16 dat=0,dat0=0,dat1=0;
    unsigned char i=0,j=0;
 
    zuobiao_x0 = position_x/16;  //列地址的分量
    zuobiao_x1 = position_x%16;  //字节中的位置
    datL_left = 16 - zuobiao_x1;
    
    //变换数据并画点
    LCD12864_COM_Write(0x34); //执行扩展指令，关画图显示
    
    
    for(j=0;j<datH;j++)
    {
        zuobiao_y0 = (position_y+j)/32;  //在上行0，在下行1
        zuobiao_y1 = (position_y+j)%32;  //行地址 
        
        LCD12864_COM_Write(0x80 + zuobiao_y1);     //行地址 
        LCD12864_COM_Write(0x80 + zuobiao_y0*8 + zuobiao_x0);     //列地址 


        if(datL>datL_left)//图片大小超出了剩余容量
        {
            datL_right = (datL - datL_left)%16; 
            datL_size = (datL - datL_left)/16;
//            //读当前地址数据
//            LCD12864_ReadOneDat();//假读一次
//            dat = (u16)LCD12864_ReadOneDat()<<8;//读高8位
//            dat |=  LCD12864_ReadOneDat();//读低8位
//            dat = (dat>>datL_left)<<datL_left;
            
            LCD12864_COM_Write(0x80 + zuobiao_y1);     //行地址 
            LCD12864_COM_Write(0x80 + zuobiao_y0*8 + zuobiao_x0);     //列地址 
            if(datL>8)
            {
                //写头
                dat1 = ((u16)(*(picture+j*(datL/8)+0))<<8)|(*(picture+j*(datL/8)+1));
                dat0 = (u16)(dat1>>zuobiao_x1);
                dat |= dat0;
                LCD12864_Data_Write((unsigned char)(dat>>8)); 
                LCD12864_Data_Write((unsigned char)dat);
                dat = dat1<<datL_left;
                //写体
                for(i=0;i<datL_size;i++)
                {
                    dat1 = ((u16)(*(picture+j*(datL/8)+2*(i+1)))<<8)|(*(picture+j*(datL/8)+2*(i+1)+1));
                    dat0 = dat1>>zuobiao_x1;
                    dat |= dat0;
                    LCD12864_Data_Write((unsigned char)(dat>>8)); 
                    LCD12864_Data_Write((unsigned char)dat);
                    dat = dat1<<datL_left;
                }
                //写尾
                //读当前地址数据
                if(datL_right>0)
                {
										if(datL/8%2)	//剩一个8位
										{
											dat = dat|((u16)(*(picture+j*(datL/8)+2*(datL_size+1)))<<(datL_left-8));
										}

                    LCD12864_COM_Write(0x80 + zuobiao_y1);     //行地址 
                    LCD12864_COM_Write(0x80 + zuobiao_y0*8 + zuobiao_x0 + datL_size + 1);     //列地址 
//                    LCD12864_ReadOneDat();//假读一次
//                    dat1 = (u16)LCD12864_ReadOneDat()<<8;//读高8位
//                    dat1 |=  LCD12864_ReadOneDat();//读低8位
                    dat1 = ((u16)(dat1<<(zuobiao_x1+(datL/8%2)*8)))>>(zuobiao_x1+(datL/8%2)*8);
                    dat |= dat1;
                    LCD12864_COM_Write(0x80 + zuobiao_y1);     //行地址 
                    LCD12864_COM_Write(0x80 + zuobiao_y0*8 + zuobiao_x0 + datL_size + 1);     //列地址 
                    LCD12864_Data_Write((unsigned char)(dat>>8)); 
                    LCD12864_Data_Write((unsigned char)dat);
                }
            }
            else
            {
                //写头
                
                dat0 = ((u8)(*(picture+j)))>>(8-datL_left);
                dat |= dat0;
                LCD12864_Data_Write((unsigned char)(dat>>8)); 
                LCD12864_Data_Write((unsigned char)dat);
                dat =((u16)(*(picture+j)))<<(8+datL_left);

               
                    LCD12864_COM_Write(0x80 + zuobiao_y1);     //行地址 
                    LCD12864_COM_Write(0x80 + zuobiao_y0*8 + zuobiao_x0 + 1);     //列地址 
//                    LCD12864_ReadOneDat();//假读一次
//                    dat1 = ((u16)LCD12864_ReadOneDat())<<8;//读高8位
//                    dat1 |=  LCD12864_ReadOneDat();//读低8位
                    dat1 = ((u16)(dat1<<(datL-datL_left)))>>(datL-datL_left);
                    dat |= dat1;
                    LCD12864_COM_Write(0x80 + zuobiao_y1);     //行地址 
                    LCD12864_COM_Write(0x80 + zuobiao_y0*8 + zuobiao_x0 + 1);     //列地址 
                    LCD12864_Data_Write((unsigned char)(dat>>8)); 
                    LCD12864_Data_Write((unsigned char)dat);

            }        
        }
        else//图片大小未超出了剩余容量
        {
            dat1 = 0xffff<<datL_left;
            dat1 |= (0xffff>>datL)>>zuobiao_x1;
            //读当前地址数据
//            LCD12864_ReadOneDat();//假读一次
//            dat = (u16)LCD12864_ReadOneDat()<<8;//读高8位
//            dat |=  LCD12864_ReadOneDat();//读低8位
            dat &= dat1;
            
            LCD12864_COM_Write(0x80 + zuobiao_y1);     //行地址 
            LCD12864_COM_Write(0x80 + zuobiao_y0*8 + zuobiao_x0);     //列地址 
            if(datL>8)
            {
                //写头
                dat1 = ((u16)(*(picture+j*(datL/8)+0))<<8)|(*(picture+j*(datL/8)+1));
                dat0 = dat1>>zuobiao_x1;
                dat |= dat0;
                LCD12864_Data_Write((unsigned char)(dat>>8)); 
                LCD12864_Data_Write((unsigned char)dat);

            }
            else
            {
                //写头
                dat0 = ((u16)(*(picture+j*(datL/8)+0))<<8)>>zuobiao_x1;
                dat |= dat0;
                LCD12864_Data_Write((unsigned char)(dat>>8)); 
                LCD12864_Data_Write((unsigned char)dat);
               
            }        
        }
            
    }
    LCD12864_COM_Write(0x36); //开画图显示
}
