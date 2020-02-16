#include "Map0_Malloc.H"

 u8  Map0_MallocArray[Malloc_MAXSIZE] = {0};         // __attribute__((at(0x20000000)))   					// ��������
 u8  indexMalloc[Malloc_MAXSIZE/Malloc_UNIT/8] = {0};          	// ������
 u8* pFirstMini = 0;																						// �����һ��С�ֽ��ڴ�ص�ָ��

/************************************************************************************
*-��������	����̬�����ڴ�,ͨ�ú���
*-����			��leng_th ������ڴ泤��,mallocArray �����ڴ�ص��׵�ַ��malloc_unit ��С���䵥Ԫ
							malloc_maxsize �����䳤�ȣ����ڴ�ش�С ;indexMalloc ��������ַ
*����ֵ			��		�ɹ�������������ڴ��ַ�����ɹ�����0
*-��������	��Malloc����̬�����ڴ� ,���һλΪ0����ÿ������Ľ����������ʱʶ��
*-������	�������в�˶���ӿƼ�
*/
u8* MallocGE(u32  leng_th,u8* mallocArray,u32 malloc_unit,u32 malloc_maxsize,u8* indexMalloc)
{
      u32 length = 0 ,length_counter = 0;
      u32 i = 0;
      u8* array = mallocArray;  // ����û�����������ʼλ��
      u32 m = 0;        // �������������,������������
      u8 n = 0;         // ������������8 bit��λ�á�����������.array[n]
      
      // �������ĳ���
      if( leng_th%malloc_unit == 0)
            length = leng_th/malloc_unit+2;
      else
      length = leng_th/malloc_unit + 3; // ���һλΪ0����ÿ������Ľ����������ʱʶ��
       Malloc_Log("get memory length��%d leng_th = %d\r\n",length,leng_th);        
      // ����ƥ���λ��
			leng_th = 0;											// ���������֮����������
      while(leng_th < (malloc_maxsize/malloc_unit))
      {
            u8 j;
						
            for(j = 0;j<8;j++)
            {
                  if((indexMalloc[i] & (0x80>>j)) == 0)  // �ҵ�λ��
                  {
										
                        if(length_counter == 0) // ��һ���ҵ����λ��
                        {
                              array = &(mallocArray[(i*8+j)*malloc_unit]);      // ����ָ��
                              m = i;
                              n = j;
															Malloc_Log("��һ���ҵ�λ��********* m=%d,n=%d********\r\n",m,n); 
                        }
                        length_counter ++;
                        if(length_counter == length)                    // �ҵ��㹻���ڴ�
                        {
                              Malloc_Log("Find first side: MALLOCArray[%d]\r\n",m*8+n);

                              // �������
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
                            Malloc_Log("indexMalloc ���ݣ�\r\n");  																	
                         for(i=0;i<(malloc_maxsize/malloc_unit+7)/8;i++)
                              {
                                    Malloc_Log("indexMalloc[%d] = %x  ",i,indexMalloc[i]);
                              }
                                    Malloc_Log("\r\n");
#endif                                    
                          
                              array += malloc_unit;// �ճ�ֹͣλ
                              return array; // ����
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
*-��������	����̬����С�ֽ��ڴ�
*-����			��leng_th ������ڴ泤��
*����ֵ			���ɹ�������������ڴ��ַ�����ɹ�����0
*-��������	��
*-������		�������в�˶���ӿƼ�
*/
u8* Map0_MallocMini(u32 leng_th)
{
	u8* pBuff = 0;
	u8* pBack = 0;
	if(pFirstMini == 0) 										// û�������С�ڴ��
	{
		pFirstMini = MallocGE(Malloc_UNIT,(u8*)&Map0_MallocArray,Malloc_UNIT,Malloc_MAXSIZE,indexMalloc);			// �ڴ��ڴ��������һ��
		if(pFirstMini == 0)return 0;											// ����ʧ��
		else
		{
			pBuff = pFirstMini;
			
			((MiniMallocStr*)pBuff)->pNext = 0;
			((MiniMallocStr*)pBuff)->index = 0;
			// ��С�ڴ��������һ���ڴ�			
			 pBuff = MallocGE(leng_th,pBuff,Malloc_MINI,Malloc_UNIT-8,(u8*)&(((MiniMallocStr*)pBuff)->index));
			 if(pBuff == 0) return 0;								// ����ʧ��
				return pBuff;
				
		}
	}else		// ��pFirstMiniָ����ڴ���Ѱ�Һ��ʵ�λ��
	{		
			 pBuff = pFirstMini;
			 pBack = pBuff;
		   // ��С�ڴ��������һ���ڴ�		
			 pBuff = MallocGE(leng_th,pBuff,Malloc_MINI,Malloc_UNIT-8,(u8*)&(((MiniMallocStr*)pBuff)->index));

			while(pBuff == 0)	// ���û���ҵ�����λ�ã�ִ������
			{
				pBuff = pBack;
				if(((MiniMallocStr*)pBuff)->pNext == 0) // ����������ڴ��
				{
					pBuff = MallocGE(Malloc_UNIT,(u8*)&Map0_MallocArray,Malloc_UNIT,Malloc_MAXSIZE,indexMalloc);
					if(pBuff == 0) return 0;								// ����ʧ��
					else																		// ����ɹ�	
					{
						((MiniMallocStr*)pBuff)->pNext = 0;	// ��ʼ��С�ڴ����⹦����
						((MiniMallocStr*)pBuff)->index = 0;
						((MiniMallocStr*)pBack)->pNext = pBuff;	// �����µ�ַ
						pBack = pBuff;
					  // ��С�ڴ��������һ���ڴ�		
					  pBuff = MallocGE(leng_th,pBuff,Malloc_MINI,Malloc_UNIT-8,(u8*)&(((MiniMallocStr*)pBuff)->index));
					  if(pBuff == 0) return 0;								// �µĴ��ڴ�ػ�������ʧ�����˳�			
					}
				}else																	// ����һ��������û�к���λ��
				{
					pBuff = ((MiniMallocStr*)pBuff)->pNext;
					pBack = pBuff;
					  // ��С�ڴ��������һ���ڴ�		
					pBuff = MallocGE(leng_th,pBuff,Malloc_MINI,Malloc_UNIT-8,(u8*)&(((MiniMallocStr*)pBuff)->index));
					
				}
			}

			return pBuff;
	}
}
/************************************************************************************
*-��������	����̬�����ڴ�
*-����			��leng_th ������ڴ泤��
*����ֵ			���ɹ�������������ڴ��ַ�����ɹ�����0
*-��������	��Malloc����̬�����ڴ� ,���һλΪ0����ÿ������Ľ����������ʱʶ��
*-������		�������в�˶���ӿƼ�
*/
void* Mapbit0_Malloc(u32  leng_th)
{
	u8* p;
	if(leng_th <= (Malloc_MINI*Malloc_MINIINDEX))
	{
	
		p = Map0_MallocMini(leng_th);
        if(p){Malloc_Log("�Ѿ�����С�ڴ�,��ַ��%#X\r\n",(u32)p);}
        else   Malloc_Log("С�ڴ�����ʧ�ܣ����뷵��0");
		return (void*)p;
	}
	else
	{
		p = MallocGE(leng_th,(u8*)&Map0_MallocArray,Malloc_UNIT,Malloc_MAXSIZE,indexMalloc);
        if(p){Malloc_Log("�Ѿ�������ڴ�,��ַ��%#X\r\n",(u32)p);}
        else Malloc_Log("С�ڴ�����ʧ�ܣ����뷵��0");
		return (void*)p;
	}
	
}

/************************************************************************************
*-��������	���ͷ��ڴ�,ͨ�ú���
*-����			Malloc �ڴ��׵�ַ,mallocArray �����ڴ�ص��׵�ַ��malloc_unit ��С���䵥Ԫ
							malloc_maxsize �����䳤�ȣ����ڴ�ش�С ;indexMalloc ��������ַ
*����ֵ			��		�ɹ�������������ڴ��ַ�����ɹ�����0
*-��������	��Malloc����̬�����ڴ� ,���һλΪ0����ÿ������Ľ����������ʱʶ��
*-������	�������в�˶���ӿƼ�
*/
Flag_Status Free_MallocGE(u8* Malloc,u8* mallocArray,u32 malloc_unit,u32 malloc_maxsize,u8* indexMalloc)
{
      u32   i; // ��������
      u8    j; // ��ǰ���ֽڵ�λ
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
                          Malloc_Log("�ͷ�indexMalloc ���ݣ�\r\n");        
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
*-��������	���ͷ��ڴ�
*-����	��
*����ֵ	���ɹ�����OK�����ɹ�����0
*-��������	���ͷŶ�̬���ɵ��ڴ�
*-������	�������в�˶���ӿƼ�
*/

Flag_Status    FreeMap0_Malloc(void* Malloc)
{
	u8* pBuff = pFirstMini;					// ��ǰ�����׵�ַ
	u8* pBack = pFirstMini;					// ǰһ�������׵�ַ
	Flag_Status status ;						// ִ��״̬
	Malloc_Log("�ͷ��ڴ棬��ַ: %#X\r\n",(u32)Malloc);
	// �ж� Malloc �ڲ���С�ڴ�ط�Χ��
	if(pFirstMini != 0)
	{
		do{
			Malloc_Log("Malloc = %#x,pBuff = %#x,(u32)&(((MiniMallocStr*)pBuff)->dat[Malloc_MINI*Malloc_MINIINDEX]) = %#x\r\n",\
					(u32)Malloc,(u32)pBuff,(u32)&(((MiniMallocStr*)pBuff)->dat[Malloc_MINI*Malloc_MINIINDEX]));
			if((u32)Malloc >= (u32)pBuff && ((u32)Malloc <= (u32)&(((MiniMallocStr*)pBuff)->dat[Malloc_MINI*Malloc_MINIINDEX])))
			{
				// �ͷ�С�ڴ���е��ڴ�
				status = Free_MallocGE((u8*)Malloc,pBuff,Malloc_MINI,Malloc_UNIT-8,(u8*)&(((MiniMallocStr*)pBuff)->index));			
				if(status == ISOK)
				{// ���С�ڴ��Ϊ�գ����ڴ��ڴ�����ͷ�С�ڴ��
					if(((MiniMallocStr*)pBuff)->index == 0)
					{
						Malloc_Log("�ͷ�С�ڴ���С���������\r\n");
						// ������׵�ַ����pFirstMiniָ��
						if(pBuff == pFirstMini)
						{
							Malloc_Log("���pFirstMiniָ�롣��������\r\n");
							if(((MiniMallocStr*)pBuff)->pNext == 0) pFirstMini = 0;	
							else pFirstMini = ((MiniMallocStr*)pBuff)->pNext;
						}
						else
						{									
							while(((MiniMallocStr*)pBack)->pNext != pBuff) pBack = ((MiniMallocStr*)pBack)->pNext;// �ҵ�ǰһ������
							((MiniMallocStr*)pBack)->pNext = ((MiniMallocStr*)pBuff)->pNext; // ���ǰһ�������б���ĵ�ַ	
						}
						//���ڴ�����ͷŸ��ڴ�
						if(Free_MallocGE(pBuff,(u8*)&Map0_MallocArray,Malloc_UNIT,Malloc_MAXSIZE,indexMalloc)!=ISOK)
							Malloc_Log("���ڴ���ͷ�С�ڴ��ʧ��\r\n");
					}
				}
				else
					Malloc_Log("�ͷ�С�ڴ��ʧ��\r\n");
				return status;
				
			}else pBuff = ((MiniMallocStr*)pBuff)->pNext;
			printf("pBuff = %#x\r\n",(u32)pBuff);
		}while(pBuff != 0);	
	}
	// �ͷŴ��ڴ��
	Malloc_Log("�ͷŴ��ڴ��\r\n");
	return Free_MallocGE((u8*)Malloc,(u8*)&Map0_MallocArray,Malloc_UNIT,Malloc_MAXSIZE,indexMalloc);

}
