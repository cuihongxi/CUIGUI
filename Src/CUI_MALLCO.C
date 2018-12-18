#include "CUI_MALLCO.H"

CUI_MallcoType  CUI_MallcoArray[MALLCO_MAXSIZE/MALLCO_UNIT];            //申请数组
unsigned char indexMallco[MALLCO_MAXSIZE/MALLCO_UNIT/8] = {0};          //索引表

/************************************************************************************
*-函数名称	：动态申请内存
*-参数	：num 申请的内存长度
*返回值	：成功返回所申请的内存地址，不成功返回0
*-函数功能	：MALLCO，动态申请内存 ,最后一位为0，做每次申请的结束符，清空时识别
*-创建者	：蓬莱市博硕电子科技
*/

u8* CUI_Mallco(u32  leng_th)
{
      u32 length = 0 ,length_counter = 0;
      u32 i = 0;
      u8* array = &CUI_MallcoArray[0].array[0];  //保持没分配变量的起始位置
      u32 m = 0;        //保存索引表序号,或总数组的序号
      u8 n = 0;         //保存索引表中8 bit的位置。或总数组中.array[n]

      
      //获得申请的长度
      
      if( leng_th%MALLCO_UNIT == 0)
            length = leng_th/MALLCO_UNIT+2;
      else
      length = leng_th/MALLCO_UNIT + 3; //最后一位为0，做每次申请的结束符，清空时识别
       MALLCO_Log("get memory length：%d\r\n",length);        
      
      //查找匹配的位置
      while(i<(MALLCO_MAXSIZE/MALLCO_UNIT/8))
      {
            u8 j;
            for(j = 0;j<8;j++)
            {
                  if((indexMallco[i] & (0x80>>j)) == 0)  //找到位置
                  {
                        if(length_counter == 0) //第一次找到这个位置
                        {
                                    
                              array = &(CUI_MallcoArray[i*8+j].array[0]);      //保存指针
                              m = i;
                              n = j;
  
                        }
                        length_counter ++;
                        if(length_counter == length)                    //找到足够的内存
                        {
                              MALLCO_Log("Find first side:array = CUI_MallcoArray[%d]\r\n",m*8+n);

                              //标记1
                                while(length != 2)
                                    {                                                  
                                          n++;
                                          if(n==8)
                                          {
                                                n = 0;
                                                m ++;
                                          }
                                          indexMallco[m] |= (0x80>>n);

                                          
                                          length --;
                                    }
 #if   DEBUG_MALLCO_LEVEL > 1
                            MALLCO_Log("indexMallco 数据：\r\n");        
                         for(i=0;i<MALLCO_MAXSIZE/MALLCO_UNIT/8;i++)
                              {
                                    MALLCO_Log("indexMallco[%d] = %x  ",i,indexMallco[i]);
                              }
                                    MALLCO_Log("\r\n");
#endif                                    
                          
                                    array ++;//空出停止位
                              return array; //返回
                        }
                        
                  }
                  else  
                  {
                        length_counter = 0;
                        
                  }
                     
            }
             i ++;  
      }
      
      return 0;
}

/************************************************************************************
*-函数名称	：释放内存
*-参数	：
*返回值	：成功返回OK，不成功返回0
*-函数功能	：释放动态生成的内存
*-创建者	：蓬莱市博硕电子科技
*/

Flag_Status    FreeCUI_Mallco(u8* mallco)
{
      //获取位置
      u32   i; //索引表项
      u8    j; //当前项字节的位
      
      if(mallco != 0)
      {
            i = (mallco - &(CUI_MallcoArray[0].array[0]))/MALLCO_UNIT/8;
            j = (mallco - &(CUI_MallcoArray[0].array[0]))/MALLCO_UNIT%8; 
 
      MALLCO_Log("Free Mallco :indexMallco[%d].[%d]\r\n",i,j);
   
      j++;
      if(j==8) 
      {
            i ++;
         j = 0;   
      }
      while((indexMallco[i] &(0x80>>j)) != 0 && i < (MALLCO_MAXSIZE/MALLCO_UNIT/8))
      {
            indexMallco[i]  &= (~(0x80>>j));
            j++;
            if(j>=8)
            {
                  j = 0;
                  i ++;
            }
      }
      
 #if   DEBUG_MALLCO_LEVEL > 1
                          MALLCO_Log("释放indexMallco 数据：\r\n");        
                         for(i=0;i<MALLCO_MAXSIZE/MALLCO_UNIT/8;i++)
                              {
                                    MALLCO_Log("indexMallco[%d] = %x  ",i,indexMallco[i]);
                              }
                                    MALLCO_Log("\r\n");
#endif       
      if(i < MALLCO_MAXSIZE/MALLCO_UNIT/8) return ISOK;     
      else
      return ISERROR;
      }
      return ISERROR;
}

