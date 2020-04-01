#include "GUI.H"
#include "CUIGUI_LCDDRIVER.H"


const GUI_FONT* cuigui_Font;  //����ȫ�ֱ���


//�õ��ĺ���
static const WCHAR Array[2][1] = {
	//GBK��
	{
		0xA1CC
	},
	//UNICODE��
	{
		0x221A 
	},
                  
};
/************************************************************************************
*-��������	���趨����
*-����	��
*����ֵ	��
*-��������	��
*-������	�������в�˶���ӿƼ�
*/

void CUIGUI_SetFont(const GUI_FONT* GUI_FontNum)
{
    cuigui_Font = GUI_FontNum;
}

/************************************************************************************
*-��������	���������
*-����	��
*����ֵ	��
*-��������	��
*-������	�������в�˶���ӿƼ�
*/

const GUI_FONT* CUIGUI_GetFont(void)                 //��ȡ��ǰ����
{
      return cuigui_Font;
}


//����GBK�뷵���������е�λ��,���������е�λ�÷���UNICODE��
WCHAR GBK2UNICODE(WCHAR word)
{

   u16 i = 0;
    while((word != Array[0][i]) &&(i<sizeof(Array)/sizeof(WCHAR)))  
    {
      i++;
    }  
     FONT_Log("\r\n find! i= %d",i);
      if(i>=sizeof(Array)/sizeof(WCHAR)) return 0x0020;//û�ҵ����ؿո�
    else return Array[1][i];
      
}


/************************************************************************************
*-��������	���Զ��� ���UNICODE��
*-����	��
*����ֵ	��
*-��������	��
*-������	�������в�˶���ӿƼ�
*/
WCHAR GetUnicode(WCHAR word)
{
      return GBK2UNICODE(word);
}

/************************************************************************************
*-��������	�����һ������Ϊw���߶�Ϊh����ģ����Ļ�� (x,y) ����������ֵ���ɫΪ color��
*-����	�����ֵĵ�������Ϊ p_data ��ָ
*����ֵ	��
*-��������	��
*-������	�������в�˶���ӿƼ�
*/

void  _draw_model(const unsigned char *p_data, int w, int h, int x, int y, COLOR_DATTYPE color)
{
    int     i;    /* ������ */
    int     j;    /* ����һ���е�8���� */
    int     k;    /* һ���еĵڼ�����8���㡱�� */
    int     nc;   /* ���������ݵĵڼ����ֽ��� */
    int     cols; /* ������ */
    w  = (w + 7) / 8 * 8;  /* ���¼���w */
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
*-��������	����ӡһ���ַ�
*-����	��
*����ֵ	�������ֿ���
*-��������	��
*-������	�������в�˶���ӿƼ�
*/

u16 CUIGUI_DrawWord( int x, int y, COLOR_DATTYPE color,const char* word)
{
      u16 w;        //��
      u16 h;        //��  
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
      w = (prop->afc+(dat- prop->fc))->x;            	//��ÿ���
		
      h = cuigui_Font->height;                  		//��ø߶�
      FONT_Log("\r\n w0 = %d , h = %d",w,h);
    
    _draw_model((prop->afc+(dat-prop->fc))->acGUI_Font ,w,h,x,y,color);
    
    return w;      
}

/************************************************************************************
*-��������	����ӡһ���ַ���
*-����	��
*����ֵ	���������λ��
*-��������	��
*-������	�������в�˶���ӿƼ�
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
                
          }else //ASCII��
           
            {              
                str ++;  
            }            
            x += w;
      }
      return (u16)x;
}


//���һ���ַ��Ŀ���
u16 GetWordWidth(const char* word)
{
      u16 w;        //��  
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
      w = (prop->afc+(dat- prop->fc))->x;            	//��ÿ���
    return w;   
}
/************************************************************************************
*-��������	��
*-����	��
*����ֵ	��
*-��������	������ַ�������
*-������	����˶���ӿƼ���www.explorersoftware.taobao.com
*/

u16 GetLengthStr(const char* str)
{
    u16 length =0;      
      while(*str != 0)
      {
             
          length += GetWordWidth(str);            //��ÿ���; 
          


          if(*str>127)
          {
                 str += 2; 
                
          }else //ASCII��
          
            {
              
                str ++;  
            }            
  
      }
      return length;
}

/************************************************************************************
*-��������	����ȡһ������ĸ߶�
*-����	��
*����ֵ	��
*-��������	��
*-������	�������в�˶���ӿƼ�
*/
u16 GetHeightFont(const GUI_FONT* font)
{
      return (font->height);
}


 /******************************************************************
 - ������������һ��32λ�ı���datתΪ�ַ����������1234תΪ"1234"
 - ����˵����dat:��ת��long�͵ı���
             str:ָ���ַ������ָ�룬ת������ֽڴ���������   
				width ��ʾռ��
 - ����˵������
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
//double����ת����ASCII��浽�����б�����ֵ�ĺ���
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
      //��С������		
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
					resultArray0[i] = '\0';  //���ӽ�����
							
			 } else
			 resultArray0[i] = '\0';//���ӽ�����            
          
}
//double����ת����ASCII��浽������
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
*-��������	���ַ�����������ת����double����
*-��������	��tab�е�����ɸ�����,steptdirΪ�������ж�
*-����		��
*-����ֵ	������ַ������ݲ����򲻸ı�dat��ԭʼֵ
*-��ע		���ж�����С��
*******************************************************/
void StrtoDouble(unsigned char* resultArray,float* dat) 
{
    unsigned char i=0,m=0;
    float j = 0,k = 0;
		u8 steptdir = 0;
		unsigned char* tab = resultArray;
	   //��Ч���ж�
		if((tab[0]<'0' && tab[0] != '-')||tab[0]>'9' ||tab[0] == 0)	return;
		
	
		//������һ��'.',�Ҳ��������һλ,�м䲻����'-'
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
        steptdir = 1;  //����Ϊ��
        m=1;
    }else steptdir = 0;
    for(i = m;tab[i]!='.';i++)
    {
        if(tab[i]=='\0') 
        {
            *dat = k;
						return;
        }
        k = k*10+(tab[i]-0x30); //ȡ����������
         
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



//��ʾ����
void CUIGUI_DrawNum(int x,int y,unsigned int num,u16 width)
{
	char numArray[11]={0};
	u32_tostr(num,numArray,width);
	CUIGUI_DrawStr(x,y,1,numArray);
}

//��ʾdouble��������
void CUIGUI_DrawDoubleNum(int x,int y,float num,unsigned char* numArray)
{
	DoubletoString(numArray,num);
	CUIGUI_DrawStr(x,y,1,(const char*)numArray);
}

//��ʾdouble��������
void CUIGUI_DrawDoubleColorNum(int x,int y,COLOR_DATTYPE color,float num,unsigned char* numArray)
{
	DoubletoString(numArray,num);
	CUIGUI_DrawStr(x,y,color,(const char*)numArray);
}

/************************************************************************************
*-��������	��
*-����			��sideλ��,��str��λ��;oldFontNum������,newFontNum������
*-����ֵ		���������ֵ�λ��
*-��ע			������������������ʾsideλ�õ��ַ�
*-������		�������в�˶���ӿƼ�
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
					w = CUIGUI_DrawWord(x0,y,2,str0); 	//���
				else
          w = GetWordWidth(&str0[i]); 
					width[i] = w;								//����ÿ���ֵĿ���
          if((u8)*str0>127)
          {
                 str0 += 2; 
                
          }else //ASCII��
           
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
			//����߶�
			pos.posy = GetHeightFont(oldFontNum);
			pos.posx += x;
			CUIGUI_SetFont(newFontNum);		
			pos.posx = pos.posx + width[side]/2 - GetWordWidth((const char*)&str[side])/2;
			pos.posy = y + pos.posy/2 - GetHeightFont(newFontNum)/2;
			CUIGUI_DrawWord(pos.posx,pos.posy,3,(const char*)&str[side]);
			CUIGUI_SetFont(oldFontNum);	
			return pos;
			
}

//����ÿ���ַ������λ��,�������ַ�������
u16 GetStrLengthAndSaveSide(const char* str,u8* sideArray)
{
    u16 length =0;
		u8 i = 0;
		sideArray[0] = 0;	
		while(*str != 0)
		{
				i++;
				sideArray[i]  = GetWordWidth(str);		//��ÿ���; 
				length += sideArray[i];            
				sideArray[i] = length;
				if(*str>127)
				{
							 str += 2; 
							
				}else //ASCII��				
					{
						
							str ++;  
					}            

		}
		return length;
}

//����ַ������ȸ���,������ֹͣλ0
u16 GetNumOfStr(const char* str)
{
	u16 i = 0;
	while(*str != 0)
	{
		i++;
		if(*str>127)
		{
					 str += 2; 
					
		}else //ASCII��				
			{
				
					str ++;  
			}   
	}
	return i;
}

/************************************************************************************
*-��������	��
*-����			��sideλ��,��str��λ��;oldFontNum������,newFontNum������;widthArray ������ȵ�����
*-����ֵ		���������ֵ�λ��
*-��ע			������������������ʾsideλ�õ��ַ�
*-������		�������в�˶���ӿƼ�
*/

Position CUIGUI_NumSelect2(int x,int y,const char* str,u8* widthArray,u8 side,const GUI_FONT* oldFontNum,const GUI_FONT* newFontNum)
{
			Position pos = {0};
			CUIGUI_SetFont(oldFontNum); 	
			
			//����߶�
			pos.posy = GetHeightFont(oldFontNum);
			pos.posx = x + widthArray[side];
			//�������
			CUIGUI_DrawWord(pos.posx,y,2,(const char*)&str[side]);
			CUIGUI_SetFont(newFontNum);		
			pos.posx = pos.posx + (widthArray[side+1]-widthArray[side])/2 - GetWordWidth((const char*)&str[side])/2;
			pos.posy = y + pos.posy/2 - GetHeightFont(newFontNum)/2;
			CUIGUI_DrawWord(pos.posx,pos.posy,3,(const char*)&str[side]);
			CUIGUI_SetFont(oldFontNum);	
			return pos;
			
}



