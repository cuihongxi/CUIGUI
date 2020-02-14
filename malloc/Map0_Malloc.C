#include "Map0_Malloc.H"

 u8  Map0_MallocArray[Malloc_MAXSIZE] = {0};         // __attribute__((at(0x20000000)))   					// 申请数组
 u8  indexMalloc[Malloc_MAXSIZE/Malloc_UNIT/8] = {0};          	// 索引表
 u8* pFirstMini = 0;																						// 保存第一个小字节内存池的指针

/************************************************************************************
*-函数名称	：动态申请内存,通用函数
*-参数			：leng_th 申请的内存长度,mallocArray 分配内存池的首地址；malloc_unit 最小分配单元
							malloc_maxsize 最大分配长度，本内存池大小 ;indexMalloc 索引表地址
*返回值			：		成功返回所申请的内存地址，不成功返回0
*-函数功能	：Malloc，动态申请内存 ,最后一位为0，做每次申请的结束符，清空时识别
*-创建者	：蓬莱市博硕电子科技
*/
u8* MallocGE(u32  leng_th,u8* mallocArray,u32 malloc_unit,u32 malloc_maxsize,u8* indexMalloc)
{
      u32 length = 0 ,length_counter = 0;
      u32 i = 0;
      u8* array = mallocArray;  // 保持没分配变量的起始位置
      u32 m = 0;        // 保存索引表序号,或总数组的序号
      u8 n = 0;         // 保存索引表中8 bit的位置。或总数组中.array[n]
      
      // 获得申请的长度
      if( leng_th%malloc_unit == 0)
            length = leng_th/malloc_unit+2;
      else
      length = leng_th/malloc_unit + 3; // 最后一位为0，做每次申请的结束符，清空时识别
       Malloc_Log("get memory length：%d leng_th = %d\r\n",length,leng_th);        
      // 查找匹配的位置
			leng_th = 0;											// 这个变量在之后做计数器
      while(leng_th < (malloc_maxsize/malloc_unit))
      {
            u8 j;
						
            for(j = 0;j<8;j++)
            {
                  if((indexMalloc[i] & (0x80>>j)) == 0)  // 找到位置
                  {
										
                        if(length_counter == 0) // 第一次找到这个位置
                        {
                              array = &(mallocArray[(i*8+j)*malloc_unit]);      // 保存指针
                              m = i;
                              n = j;
															Malloc_Log("第一次找到位置********* m=%d,n=%d********\r\n",m,n); 
                        }
                        length_counter ++;
                        if(length_counter == length)                    // 找到足够的内存
                        {
                              Malloc_Log("Find first side: MALLOCArray[%d]\r\n",m*8+n);

                              // 标记索引
                                while(length != 2)
                                    {                                                  
                                          n++;
                                          if(n==8)
                                          {
                                                n = 0;
                                                m ++;
                                          }
                                          indexMalloc[m] |= (0x80>>n);                                          
                                          length --;
                                    }
 #if   DEBUG_Malloc_LEVEL > 1
                            Malloc_Log("indexMalloc 数据：\r\n");  																	
                         for(i=0;i<(malloc_maxsize/malloc_unit+7)/8;i++)
                              {
                                    Malloc_Log("indexMalloc[%d] = %x  ",i,indexMalloc[i]);
                              }
                                    Malloc_Log("\r\n");
#endif                                    
                          
                              array += malloc_unit;// 空出停止位
                              return array; // 返回
                        }
                        
                  }
                  else  
                  {
                        length_counter = 0;
                  }
                  leng_th ++;
									if(leng_th == malloc_maxsize/malloc_unit) return 0;
            }
             i ++;  
      }
      
      return 0;
}

/************************************************************************************
*-函数名称	：动态申请小字节内存
*-参数			：leng_th 申请的内存长度
*返回值			：成功返回所申请的内存地址，不成功返回0
*-函数功能	：
*-创建者		：蓬莱市博硕电子科技
*/
u8* Map0_MallocMini(u32 leng_th)
{
	u8* pBuff = 0;
	u8* pBack = 0;
	if(pFirstMini == 0) 										// 没有申请过小内存池
	{
		pFirstMini = MallocGE(Malloc_UNIT,(u8*)&Map0_MallocArray,Malloc_UNIT,Malloc_MAXSIZE,indexMalloc);			// 在大内存池中申请一块
		if(pFirstMini == 0)return 0;											// 申请失败
		else
		{
			pBuff = pFirstMini;
			
			((MiniMallocStr*)pBuff)->pNext = 0;
			((MiniMallocStr*)pBuff)->index = 0;
			// 在小内存池中申请一块内存			
			 pBuff = MallocGE(leng_th,pBuff,Malloc_MINI,Malloc_UNIT-8,(u8*)&(((MiniMallocStr*)pBuff)->index));
			 if(pBuff == 0) return 0;								// 申请失败
				return pBuff;
				
		}
	}else		// 从pFirstMini指向的内存中寻找合适的位置
	{		
			 pBuff = pFirstMini;
			 pBack = pBuff;
		   // 在小内存池中申请一块内存		
			 pBuff = MallocGE(leng_th,pBuff,Malloc_MINI,Malloc_UNIT-8,(u8*)&(((MiniMallocStr*)pBuff)->index));

			while(pBuff == 0)	// 如果没有找到合适位置，执行下面
			{
				pBuff = pBack;
				if(((MiniMallocStr*)pBuff)->pNext == 0) // 重新申请大内存池
				{
					pBuff = MallocGE(Malloc_UNIT,(u8*)&Map0_MallocArray,Malloc_UNIT,Malloc_MAXSIZE,indexMalloc);
					if(pBuff == 0) return 0;								// 申请失败
					else																		// 申请成功	
					{
						((MiniMallocStr*)pBuff)->pNext = 0;	// 初始化小内存特殊功能区
						((MiniMallocStr*)pBuff)->index = 0;
						((MiniMallocStr*)pBack)->pNext = pBuff;	// 保存新地址
						pBack = pBuff;
					  // 在小内存池中申请一块内存		
					  pBuff = MallocGE(leng_th,pBuff,Malloc_MINI,Malloc_UNIT-8,(u8*)&(((MiniMallocStr*)pBuff)->index));
					  if(pBuff == 0) return 0;								// 新的大内存池还是申请失败则退出			
					}
				}else																	// 找下一个链表有没有合适位置
				{
					pBuff = ((MiniMallocStr*)pBuff)->pNext;
					pBack = pBuff;
					  // 在小内存池中申请一块内存		
					pBuff = MallocGE(leng_th,pBuff,Malloc_MINI,Malloc_UNIT-8,(u8*)&(((MiniMallocStr*)pBuff)->index));
					
				}
			}

			return pBuff;
	}
}
/************************************************************************************
*-函数名称	：动态申请内存
*-参数			：leng_th 申请的内存长度
*返回值			：成功返回所申请的内存地址，不成功返回0
*-函数功能	：Malloc，动态申请内存 ,最后一位为0，做每次申请的结束符，清空时识别
*-创建者		：蓬莱市博硕电子科技
*/
void* Mapbit0_Malloc(u32  leng_th)
{
	u8* p;
	if(leng_th <= (Malloc_MINI*Malloc_MINIINDEX))
	{
	
		p = Map0_MallocMini(leng_th);
        if(p){Malloc_Log("已经申请小内存,地址：%#X\r\n",(u32)p);}
        else   Malloc_Log("小内存申请失败，申请返回0");
		return (void*)p;
	}
	else
	{
		p = MallocGE(leng_th,(u8*)&Map0_MallocArray,Malloc_UNIT,Malloc_MAXSIZE,indexMalloc);
        if(p){Malloc_Log("已经申请大内存,地址：%#X\r\n",(u32)p);}
        else Malloc_Log("小内存申请失败，申请返回0");
		return (void*)p;
	}
	
}

/************************************************************************************
*-函数名称	：释放内存,通用函数
*-参数			Malloc 内存首地址,mallocArray 分配内存池的首地址；malloc_unit 最小分配单元
							malloc_maxsize 最大分配长度，本内存池大小 ;indexMalloc 索引表地址
*返回值			：		成功返回所申请的内存地址，不成功返回0
*-函数功能	：Malloc，动态申请内存 ,最后一位为0，做每次申请的结束符，清空时识别
*-创建者	：蓬莱市博硕电子科技
*/
Flag_Status Free_MallocGE(u8* Malloc,u8* mallocArray,u32 malloc_unit,u32 malloc_maxsize,u8* indexMalloc)
{
      u32   i; // 索引表项
      u8    j; // 当前项字节的位
      if(Malloc != 0)
      {
            i = (Malloc - mallocArray)/malloc_unit/8;
            j = (Malloc - mallocArray)/malloc_unit%8; 
 
      Malloc_Log("Free Malloc :indexMalloc[%d].[%d]\r\n",i,j);
      while((indexMalloc[i] &(0x80>>j)) != 0 && i < ((malloc_maxsize/malloc_unit+7)/8))
      {
            indexMalloc[i]  &= (~(0x80>>j));
            j++;
            if(j>=8)
            {
                  j = 0;
                  i ++;
            }
      }
      
 #if   DEBUG_Malloc_LEVEL > 1
                          Malloc_Log("释放indexMalloc 数据：\r\n");        
                         for(u32 m=0;m<(malloc_maxsize/malloc_unit+7)/8;m++)
                              {
                                    Malloc_Log("indexMalloc[%d] = %x  ",m,indexMalloc[m]);
                              }
                                    Malloc_Log("\r\n");
#endif       
      if(i < (malloc_maxsize/malloc_unit+7)/8) return ISOK;     
      else
      return ISERROR;
      }
      return ISERROR;
}

/************************************************************************************
*-函数名称	：释放内存
*-参数	：
*返回值	：成功返回OK，不成功返回0
*-函数功能	：释放动态生成的内存
*-创建者	：蓬莱市博硕电子科技
*/

Flag_Status    FreeMap0_Malloc(void* Malloc)
{
	u8* pBuff = pFirstMini;					// 当前链表首地址
	u8* pBack = pFirstMini;					// 前一个链表首地址
	Flag_Status status ;						// 执行状态
	Malloc_Log("释放内存，地址: %#X\r\n",(u32)Malloc);
	// 判断 Malloc 在不在小内存池范围内
	if(pFirstMini != 0)
	{
		do{
			Malloc_Log("Malloc = %#x,pBuff = %#x,(u32)&(((MiniMallocStr*)pBuff)->dat[Malloc_MINI*Malloc_MINIINDEX]) = %#x\r\n",\
					(u32)Malloc,(u32)pBuff,(u32)&(((MiniMallocStr*)pBuff)->dat[Malloc_MINI*Malloc_MINIINDEX]));
			if((u32)Malloc >= (u32)pBuff && ((u32)Malloc <= (u32)&(((MiniMallocStr*)pBuff)->dat[Malloc_MINI*Malloc_MINIINDEX])))
			{
				// 释放小内存池中的内存
				status = Free_MallocGE((u8*)Malloc,pBuff,Malloc_MINI,Malloc_UNIT-8,(u8*)&(((MiniMallocStr*)pBuff)->index));			
				if(status == ISOK)
				{// 如果小内存池为空，则在大内存池中释放小内存池
					if(((MiniMallocStr*)pBuff)->index == 0)
					{
						Malloc_Log("释放小内存池中。。。。。\r\n");
						// 如果是首地址则变更pFirstMini指针
						if(pBuff == pFirstMini)
						{
							Malloc_Log("变更pFirstMini指针。。。。。\r\n");
							if(((MiniMallocStr*)pBuff)->pNext == 0) pFirstMini = 0;	
							else pFirstMini = ((MiniMallocStr*)pBuff)->pNext;
						}
						else
						{									
							while(((MiniMallocStr*)pBack)->pNext != pBuff) pBack = ((MiniMallocStr*)pBack)->pNext;// 找到前一个链表
							((MiniMallocStr*)pBack)->pNext = ((MiniMallocStr*)pBuff)->pNext; // 变更前一个链表中保存的地址	
						}
						//大内存池中释放该内存
						if(Free_MallocGE(pBuff,(u8*)&Map0_MallocArray,Malloc_UNIT,Malloc_MAXSIZE,indexMalloc)!=ISOK)
							Malloc_Log("大内存池释放小内存池失败\r\n");
					}
				}
				else
					Malloc_Log("释放小内存池失败\r\n");
				return status;
				
			}else pBuff = ((MiniMallocStr*)pBuff)->pNext;
			printf("pBuff = %#x\r\n",(u32)pBuff);
		}while(pBuff != 0);	
	}
	// 释放大内存池
	Malloc_Log("释放大内存池\r\n");
	return Free_MallocGE((u8*)Malloc,(u8*)&Map0_MallocArray,Malloc_UNIT,Malloc_MAXSIZE,indexMalloc);

}

