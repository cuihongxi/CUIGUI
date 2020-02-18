#include "GUI.H"
#include "CUIGUI_LCDDRIVER.H"


const GUI_FONT* cuigui_Font;  //定义全局变量


//用到的汉字
static const WCHAR Array[2][1] = {
	//GBK码
	{
		0xA1CC
	},
	//UNICODE码
	{
		0x221A 
	},
                  
};
/************************************************************************************
*-函数名称	：设定字体
*-参数	：
*返回值	：
*-函数功能	：
*-创建者	：蓬莱市博硕电子科技
*/

void CUIGUI_SetFont(const GUI_FONT* GUI_FontNum)
{
    cuigui_Font = GUI_FontNum;
}

/************************************************************************************
*-函数名称	：获得字体
*-参数	：
*返回值	：
*-函数功能	：
*-创建者	：蓬莱市博硕电子科技
*/

const GUI_FONT* CUIGUI_GetFont(void)                 //获取当前字体
{
      return cuigui_Font;
}


//根据GBK码返回在数组中的位置,根据数组中的位置返回UNICODE码
WCHAR GBK2UNICODE(WCHAR word)
{

   u16 i = 0;
    while((word != Array[0][i]) &&(i<sizeof(Array)/sizeof(WCHAR)))  
    {
      i++;
    }  
     FONT_Log("\r\n find! i= %d",i);
      if(i>=sizeof(Array)/sizeof(WCHAR)) return 0x0020;//没找到返回空格
    else return Array[1][i];
      
}


/************************************************************************************
*-函数名称	：自定义 获得UNICODE码
*-参数	：
*返回值	：
*-函数功能	：
*-创建者	：蓬莱市博硕电子科技
*/
WCHAR GetUnicode(WCHAR word)
{
      return GBK2UNICODE(word);
}

/************************************************************************************
*-函数名称	：输出一个宽度为w，高度为h的字模到屏幕的 (x,y) 坐标出，文字的颜色为 color，
*-参数	：文字的点阵数据为 p_data 所指
*返回值	：
*-函数功能	：
*-创建者	：蓬莱市博硕电子科技
*/

void  _draw_model(const unsigned char *p_data, int w, int h, int x, int y, COLOR_DATTYPE color)
{
    int     i;    /* 控制行 */
    int     j;    /* 控制一行中的8个点 */
    int     k;    /* 一行中的第几个“8个点”了 */
    int     nc;   /* 到点阵数据的第几个字节了 */
    int     cols; /* 控制列 */
    w  = (w + 7) / 8 * 8;  /* 重新计算w */
    nc = 0;
    for (i=0; i<h; i++)
        {
        cols = 0;
        for (k=0; k<w/8; k++)
            {
            for (j=0; j<8; j++)
                {
                if (p_data[nc]&(0x80>>j))
                    CUIGUI_DrawPoint(x+cols, y+i, color);
                cols++;
                }
            nc++;
            }
        }
}

/************************************************************************************
*-函数名称	：打印一个字符
*-参数	：
*返回值	：返回字宽度
*-函数功能	：
*-创建者	：蓬莱市博硕电子科技
*/

u16 CUIGUI_DrawWord( int x, int y, COLOR_DATTYPE color,const char* word)
{
      u16 w;        //宽
      u16 h;        //高  
      WCHAR dat;   
      const GUI_FONT_PROP*   prop;  
      prop = cuigui_Font->prop;          
   
      if((u8)*word>127)
      {
            dat = (WCHAR)((u16)*word<<8 | (*((u16*)word)>>8));
           // printf("\r\n dat0 = %x",dat);            
#if  CUIGUI_FONT936 > 0            
            
            dat =  ff_convert(dat);               
#else                  
            dat = GetUnicode(dat);  
           // printf("\r\n dat_unicode = %x",dat);
#endif             
      }else      dat = *word;
	  FONT_Log("\r\n dat = %d",dat);
    while((dat< prop->fc || dat> prop->lc) && (prop->pNext != 0))
    {
        prop = prop->pNext;
    }
      w = (prop->afc+(dat- prop->fc))->x;            	//获得宽度
		
      h = cuigui_Font->height;                  		//获得高度
      FONT_Log("\r\n w0 = %d , h = %d",w,h);
    
    _draw_model((prop->afc+(dat-prop->fc))->acGUI_Font ,w,h,x,y,color);
    
    return w;      
}

/************************************************************************************
*-函数名称	：打印一个字符串
*-参数	：
*返回值	：返回最后位置
*-函数功能	：
*-创建者	：蓬莱市博硕电子科技
*/

u16 CUIGUI_DrawStr( int x, int y, COLOR_DATTYPE color,const char* str)
{
      u16 w;   
      while(*str != 0)
      { 
          w = CUIGUI_DrawWord(x,y,color,str); 

          if((u8)*str>127)
          {
                 str += 2; 
                
          }else //ASCII码
           
            {              
                str ++;  
            }            
            x += w;
      }
      return (u16)x;
}


//获得一个字符的宽度
u16 GetWordWidth(const char* word)
{
      u16 w;        //宽  
      WCHAR dat;   
      const GUI_FONT_PROP*   prop;  
      prop = cuigui_Font->prop;             
      if((u8)*word>127)
      {
            dat = (WCHAR)((u16)*word<<8 | (*((u16*)word)>>8));
           // printf("\r\n dat0 = %x",dat);            
#if  CUIGUI_FONT936 > 0            
            
            dat =  ff_convert(dat);               
#else                  
            dat = GetUnicode(dat);  
           // printf("\r\n dat_unicode = %x",dat);
#endif             
      }else      dat = *word;
    while((dat< prop->fc || dat> prop->lc) && (prop->pNext != 0))
    {
        prop = prop->pNext;
    }
      w = (prop->afc+(dat- prop->fc))->x;            	//获得宽度
    return w;   
}
/************************************************************************************
*-函数名称	：
*-参数	：
*返回值	：
*-函数功能	：获得字符串长度
*-创建者	：博硕电子科技，www.explorersoftware.taobao.com
*/

u16 GetLengthStr(const char* str)
{
    u16 length =0;      
      while(*str != 0)
      {
             
          length += GetWordWidth(str);            //获得宽度; 
          


          if(*str>127)
          {
                 str += 2; 
                
          }else //ASCII码
          
            {
              
                str ++;  
            }            
  
      }
      return length;
}

/************************************************************************************
*-函数名称	：获取一个字体的高度
*-参数	：
*返回值	：
*-函数功能	：
*-创建者	：蓬莱市博硕电子科技
*/
u16 GetHeightFont(const GUI_FONT* font)
{
      return (font->height);
}


 /******************************************************************
 - 功能描述：将一个32位的变量dat转为字符串，比如把1234转为"1234"
 - 参数说明：dat:带转的long型的变量
             str:指向字符数组的指针，转换后的字节串放在其中   
				width 显示占宽
 - 返回说明：无
 ******************************************************************/

void u32_tostr(unsigned long dat,char *str,u16 width)
{
	 unsigned char  temp[11]={0};
	 unsigned char  i=0,j=0;

	 while(dat)
	 {
		temp[i]=dat%10+0x30;
		i++;
		dat/=10;
	 }
	 if(i > width) i = width;
	 j=i;
	 for(i=0;i<j;i++)
	 {
		str[i]=temp[j-i-1];
	 }
	 
	 if(i < width)
	 while(i < width) {str[i++]=' ';}
	 str[width]=0;
}


#define	POWER 10000
//double类型转换成ASCII码存到数组中保存正值的函数
void DoubletoStringSaveZheng(unsigned char* resultArray0,float dat)
{
	  unsigned char i=0; 
    unsigned long power = 0;  
    unsigned  long dat0 =0;
		u8 j = 0 ;
		for(power = POWER;power>=10;power /= 10)
		{
			j ++;
		}
			dat0 = dat;
			for( power=1;dat0>=10;dat0/=10)  
							power*=10;  
			dat0 = dat;
			for(i=0;power>0;power/=10)  
			{  
						resultArray0[i] = '0'+dat0/power; 			
						dat0%=power;  
						i++;
			}           
        dat0 = dat;
        power = 0;
      //存小数部分		
			power = POWER*dat - POWER*dat0;

			if(power>0)
			{				
					for(dat0 = j;power%10 == 0;)
					{
							power /= 10;
							dat0--;						 
					}	
					for( power=1;dat0>0;dat0--)  
							power *= 10;
			}
			dat0 = dat*power;
			 if(dat0>0)
			 {						
				 resultArray0[i] = '.';
				 i++;
				 dat0 %= power;  
				 power/=10;
					for(;power>0;power/=10)  
					{  
								resultArray0[i] = '0'+dat0/power;							
								dat0 %= power;  
								i++;
					}
					resultArray0[i] = '\0';  //添加结束符
							
			 } else
			 resultArray0[i] = '\0';//添加结束符            
          
}
//double类型转换成ASCII码存到数组中
void DoubletoString(unsigned char* resultArray0,double dat)
{
    if(dat > 0)
    {
				DoubletoStringSaveZheng(resultArray0,dat);
    }
    else if(dat<0)
    {
        resultArray0[0] = '-';
        dat =  (-dat);
				DoubletoStringSaveZheng(&resultArray0[1],dat);
    }else
    {
         resultArray0[0] = '0';
         resultArray0[1] = 0;
    }
}

/*******************************************************
*-函数名称	：字符串类型数据转化成double类型
*-函数作用	：tab中的数变成浮点数,steptdir为正负号判断
*-参数		：
*-返回值	：如果字符串内容不对则不改变dat的原始值
*-备注		：判断正负小数
*******************************************************/
void StrtoDouble(unsigned char* resultArray,float* dat) 
{
    unsigned char i=0,m=0;
    float j = 0,k = 0;
		u8 steptdir = 0;
		unsigned char* tab = resultArray;
	   //有效性判断
		if((tab[0]<'0' && tab[0] != '-')||tab[0]>'9' ||tab[0] == 0)	return;
		
	
		//至多有一个'.',且不能在最后一位,中间不能有'-'
		i=1;
		while(tab[i] != 0)
		{
			if(tab[i] == '.') j ++;
			if(tab[i] == '-') k ++;
			i++;
		}
		if(k>0 ||j>1) return;
		
		j = 0; 
		k = 0;
	
    if(tab[0]=='-')
    {
        steptdir = 1;  //方向为负
        m=1;
    }else steptdir = 0;
    for(i = m;tab[i]!='.';i++)
    {
        if(tab[i]=='\0') 
        {
            *dat = k;
						return;
        }
        k = k*10+(tab[i]-0x30); //取出整数部分
         
    }
    i++;
    for(j = 1;tab[i]!='\0';i++)
    {   
        j = 0.1f*j;
        k = k+(tab[i]-0x30)*j;
    }
		
    *dat = k;
		if(steptdir) *dat = -*dat;
}



//显示数字
void CUIGUI_DrawNum(int x,int y,unsigned int num,u16 width)
{
	char numArray[11]={0};
	u32_tostr(num,numArray,width);
	CUIGUI_DrawStr(x,y,1,numArray);
}

//显示double类型数字
void CUIGUI_DrawDoubleNum(int x,int y,float num,unsigned char* numArray)
{
	DoubletoString(numArray,num);
	CUIGUI_DrawStr(x,y,1,(const char*)numArray);
}

//显示double类型数字
void CUIGUI_DrawDoubleColorNum(int x,int y,COLOR_DATTYPE color,float num,unsigned char* numArray)
{
	DoubletoString(numArray,num);
	CUIGUI_DrawStr(x,y,color,(const char*)numArray);
}

/************************************************************************************
*-函数名称	：
*-参数			：side位置,在str的位置;oldFontNum旧字体,newFontNum新字体
*-返回值		：返回新字的位置
*-备注			：新字体代替旧字体显示side位置的字符
*-创建者		：蓬莱市博硕电子科技
*/

Position CUIGUI_NumSelect(int x,int y,const char* str,u8 side,const GUI_FONT* oldFontNum,const GUI_FONT* newFontNum)
{
    u16 w;
    u8 i=0;
    u8 width[15] = {0};
    Position pos = {0};
    int x0 = x;
    const char* str0 = str;
    CUIGUI_SetFont(oldFontNum); 	
      while(*str0 != 0)
      { 
				if(i==side)
					w = CUIGUI_DrawWord(x0,y,2,str0); 	//清空
				else
          w = GetWordWidth(&str0[i]); 
					width[i] = w;								//保存每个字的宽度
          if((u8)*str0>127)
          {
                 str0 += 2; 
                
          }else //ASCII码
           
            {              
                str0 ++;  
            }            
            x0 += w;
						if(i<side) 
						{
								pos.posx +=  w;
						}
						i ++;
      }
			//保存高度
			pos.posy = GetHeightFont(oldFontNum);
			pos.posx += x;
			CUIGUI_SetFont(newFontNum);		
			pos.posx = pos.posx + width[side]/2 - GetWordWidth((const char*)&str[side])/2;
			pos.posy = y + pos.posy/2 - GetHeightFont(newFontNum)/2;
			CUIGUI_DrawWord(pos.posx,pos.posy,3,(const char*)&str[side]);
			CUIGUI_SetFont(oldFontNum);	
			return pos;
			
}

//保存每个字符的相对位置,并返回字符串长度
u16 GetStrLengthAndSaveSide(const char* str,u8* sideArray)
{
    u16 length =0;
		u8 i = 0;
		sideArray[0] = 0;	
		while(*str != 0)
		{
				i++;
				sideArray[i]  = GetWordWidth(str);		//获得宽度; 
				length += sideArray[i];            
				sideArray[i] = length;
				if(*str>127)
				{
							 str += 2; 
							
				}else //ASCII码				
					{
						
							str ++;  
					}            

		}
		return length;
}

//获得字符串长度个数,不包括停止位0
u16 GetNumOfStr(const char* str)
{
	u16 i = 0;
	while(*str != 0)
	{
		i++;
		if(*str>127)
		{
					 str += 2; 
					
		}else //ASCII码				
			{
				
					str ++;  
			}   
	}
	return i;
}

/************************************************************************************
*-函数名称	：
*-参数			：side位置,在str的位置;oldFontNum旧字体,newFontNum新字体;widthArray 保存宽度的数组
*-返回值		：返回新字的位置
*-备注			：新字体代替旧字体显示side位置的字符
*-创建者		：蓬莱市博硕电子科技
*/

Position CUIGUI_NumSelect2(int x,int y,const char* str,u8* widthArray,u8 side,const GUI_FONT* oldFontNum,const GUI_FONT* newFontNum)
{
			Position pos = {0};
			CUIGUI_SetFont(oldFontNum); 	
			
			//保存高度
			pos.posy = GetHeightFont(oldFontNum);
			pos.posx = x + widthArray[side];
			//清除旧数
			CUIGUI_DrawWord(pos.posx,y,2,(const char*)&str[side]);
			CUIGUI_SetFont(newFontNum);		
			pos.posx = pos.posx + (widthArray[side+1]-widthArray[side])/2 - GetWordWidth((const char*)&str[side])/2;
			pos.posy = y + pos.posy/2 - GetHeightFont(newFontNum)/2;
			CUIGUI_DrawWord(pos.posx,pos.posy,3,(const char*)&str[side]);
			CUIGUI_SetFont(oldFontNum);	
			return pos;
			
}




