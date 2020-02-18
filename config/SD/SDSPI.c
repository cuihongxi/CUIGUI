#include "sdspi.h"

u8 is_init = 0;//ΪSD����ʼ��׼��
unsigned char SD_Type = 0;
/*******************************************************
*-��������	��
*-��������	����ʱ
*-����		��
*-����ֵ	��
*-��ע		��
*******************************************************/

void delay(unsigned int dat)
{
	while(dat--);
}

/*******************************************************
*-��������	��
*-��������	����SD��дһ���ֽ�
*-����		��
*-����ֵ	��redat ������������
*-��ע		��
*******************************************************/
unsigned char SD_spi_RW(unsigned char sedat)
{
	
	unsigned char redat=0,i;
	 
	SD_SCL_1;
	for(i=0;i<8;i++)
	{
		if( sedat&0x80) SD_SI_1;
		else SD_SI_0;
		sedat = sedat<<1;
		if(is_init) delay(DELAY_TIME);
		SD_SCL_0;
		redat = redat<<1;
		if(is_init) delay(DELAY_TIME);
		redat |= SD_SO;
		SD_SCL_1;
	}
	return redat;
}


/*******************************************************
*-��������	��
*-��������	������ָ��
*-����		��cmd,���argument ������crc У����
*-����ֵ	���������
*-��ע		��
*******************************************************/

unsigned char SD_SendCmd(unsigned char cmd,unsigned long int argument,unsigned char crc)
{
	unsigned char temp;
	unsigned int Retry=0;
	SD_CS_1;
	SD_spi_RW(0xff);//��߼����ԣ����û�������ЩSD�����ܲ�֧��
	
	SD_CS_0;
	SD_spi_RW(0x40|cmd);
	SD_spi_RW((unsigned char)((argument>>24)));
	SD_spi_RW((unsigned char)(argument>>16));
	SD_spi_RW((unsigned char)(argument>>8));
	SD_spi_RW((unsigned char)argument);
	SD_spi_RW(crc);
	
   //�ȴ���Ӧ����ʱ�˳�
   while((temp = SD_spi_RW(0xFF))==0xFF)
   {
	 Retry++;
	 if(Retry > TRY_TIME)break; //����ʵ���ã�������Դ������  
   }
   
   SD_CS_1;
   SD_spi_RW(0xff);
  //������Ӧֵ
  return temp;
}

/*******************************************************************************
* Function Name  : SD_SendCmd_NoDeassert
* Description    : ��SD������һ������(�����ǲ�ʧ��Ƭѡ�����к������ݴ�����
* Input          : unsigned char cmd   ���� 
*                  unsigned long int arg  �������
*                  unsigned char crc   crcУ��ֵ
* Output         : None
* Return         : unsigned char r1 SD�����ص���Ӧ
*******************************************************************************/
unsigned char SD_SendCmdNoDeassert(unsigned char cmd,unsigned long int argument,unsigned char crc)
{
	unsigned char temp;
	unsigned int Retry=0;
	SD_CS_1;
	SD_spi_RW(0xff);//��߼����ԣ����û�������ЩSD�����ܲ�֧��
	
	SD_CS_0;
	SD_spi_RW(0x40|cmd);
	SD_spi_RW((unsigned char)((argument>>24)));
	SD_spi_RW((unsigned char)(argument>>16));
	SD_spi_RW((unsigned char)(argument>>8));
	SD_spi_RW((unsigned char)argument);
	SD_spi_RW(crc);
	
   //�ȴ���Ӧ����ʱ�˳�
   while((temp = SD_spi_RW(0xFF))==0xFF)
   {
	 Retry++;
	 if(Retry > TRY_TIME)break; //����ʵ���ã�������Դ������  
   }
  //������Ӧֵ
  return temp;
}
/*******************************************************
*-��������	��
*-��������	����λSD��
*-����		��
*-����ֵ	��TEMP=0X01Ϊ�ɹ�
*-��ע		��
*******************************************************/

unsigned char SD_Reset()
{
	unsigned char time ,temp,i;
	
	is_init=1;
	
	SD_CS_1;//�ر�Ƭѡ
	
	for(i=0;i<0x0f;i++)
	{
		SD_spi_RW(0xff);//120��ʱ��
	}
	
	SD_CS_0;//Ƭѡ��
	
	time = 0;
	do
	{
		//temp = SD_Write_Cmd(pcmd);
		temp = SD_SendCmd(CMD0,0,0X95);
		time++;
		if(time >=TRY_TIME)
		{
			SD_CS_1;//�ر�Ƭѡ
			return (temp);
			
		}
	}while(temp != 0x01);
	SD_CS_1;//�ر�Ƭѡ
	SD_spi_RW(0xff);//����8��ʱ��
	return temp;
}


/*******************************************************
*-��������	��
*-��������	��	//-----------------SD����MMC����ʼ����ʼ-----------------	 
				//������ʼ��ָ��CMD55+ACMD41
				// �����Ӧ��˵����SD�����ҳ�ʼ�����
				// û�л�Ӧ��˵����MMC�������������Ӧ��ʼ��
*-����		��
*-����ֵ	��
*-��ע		��
*******************************************************/

//�ж�MMC��
//unsigned char MMC_Init()
//{
//	unsigned char r1;
//	unsigned int retry = 0;
//	//����MMC����ʼ�����û�в��ԣ�
//	do
//	{
//	   r1 = SD_SendCmd(CMD1, 0, 0);
//	   retry++;
//	}while((r1!=0x00)&& (retry<400));
//	if(retry>=400)return 1;   //MMC����ʼ����ʱ		    
//	//д�뿨����
//	SD_Type = SD_TYPE_MMC;
//	return SD_Type;
//}


//V1.0�汾�ж���SD����MMC
//unsigned char SD_MMC_Init()
//{
//	 unsigned char r1;
//	 unsigned int retry = 0;
//	
//     do
//     {
//       //�ȷ�CMD55��Ӧ����0x01���������
//       r1 = SD_SendCmd(CMD55, 0, 0);
//       if(r1 != 0x01)
//         return r1;	  
//       //�õ���ȷ��Ӧ�󣬷�ACMD41��Ӧ�õ�����ֵ0x00����������400��
//       r1 = SD_SendCmd(ACMD41, 0, 0);
//       retry++;
//     }while((r1!=0x00) && (retry<400));
//     // �ж��ǳ�ʱ���ǵõ���ȷ��Ӧ
//     // ���л�Ӧ����SD����û�л�Ӧ����MMC��
//	 SD_Type = SD_TYPE_V1;
//	 if(retry >= 400) SD_Type = MMC_Init();
//	 return SD_Type;
//}


//�ж�V2.0�����ͺ�
unsigned char SDV2_Init()
{
	unsigned int retry;
	unsigned char r1;
	unsigned char buff = 0;
    //V2.0�Ŀ���CMD8�����ᴫ��4�ֽڵ����ݣ�Ҫ�����ٽ���������
//	   buff[0] = SD_spi_RW(0xFF);  //should be 0x00
//	   buff[1] = SD_spi_RW(0xFF);  //should be 0x00
//	   buff[2] = SD_spi_RW(0xFF);  //should be 0x01
//	   buff[3] = SD_spi_RW(0xFF);  //should be 0xAA	   
		SD_spi_RW(0xFF);
		SD_spi_RW(0xFF);
		SD_spi_RW(0xFF);
		SD_spi_RW(0xFF);
//	UART1_SendOneChar(0x55);
//	UART1_SendOneChar(buff[0]); 
//	UART1_SendOneChar(buff[1]); 
//	UART1_SendOneChar(buff[2]); 
//	UART1_SendOneChar(buff[3]); 	
    SD_CS_1;	  
    SD_spi_RW(0xFF);//the next 8 clocks			 
    //�жϸÿ��Ƿ�֧��2.7V-3.6V�ĵ�ѹ��Χ
    //if(buff[2]==0x01 && buff[3]==0xAA) //�粻�жϣ�����֧�ֵĿ�����
   // {	  
       retry = 0;
       //������ʼ��ָ��CMD55+ACMD41
       do
       {
    	  r1 = SD_SendCmd(CMD55, 0, 0);
    	  if(r1!=0x01)return r1;	   
    	  r1 = SD_SendCmd(ACMD41, 0x40000000, 1);
          if(retry>200)return r1;  //��ʱ�򷵻�r1״̬  
       }while(r1!=0);		  
       //��ʼ��ָ�����ɣ���������ȡOCR��Ϣ	
	   
       //-----------����SD2.0���汾��ʼ-----------
       r1 = SD_SendCmdNoDeassert(CMD58, 0, 0);
	   
       if(r1!=0x00)return r1;  //�������û�з�����ȷӦ��ֱ���˳�������Ӧ��		 
       //��OCRָ����󣬽�������4�ֽڵ�OCR��Ϣ
       buff = SD_spi_RW(0xFF);
//       buff[1] = SD_spi_RW(0xFF); 
//       buff[2] = SD_spi_RW(0xFF);
//       buff[3] = SD_spi_RW(0xFF);
	   SD_spi_RW(0xFF);
	   SD_spi_RW(0xFF);
	   SD_spi_RW(0xFF);
//		UART1_SendOneChar(0x56);
//		UART1_SendOneChar(buff[0]); 
//		UART1_SendOneChar(buff[1]); 
//		UART1_SendOneChar(buff[2]); 
//		UART1_SendOneChar(buff[3]); 	
       //OCR������ɣ�Ƭѡ�ø�
       SD_CS_1;
       SD_spi_RW(0xFF);

       //�����յ���OCR�е�bit30λ��CCS����ȷ����ΪSD2.0����SDHC
       //���CCS=1��SDHC   CCS=0��SD2.0
       if(buff&0x40)SD_Type = SD_TYPE_V2SDHC;    //���CCS	 
       else SD_Type = SD_TYPE_V2SDSC;	    
       //-----------����SD2.0���汾����----------- 
       //����SPIΪ����ģʽ
       is_init=0; 
		return 0;
   // }	    
}

/*******************************************************
*-��������	��
*-��������	����ʼ��
*-����		��
*-����ֵ	��temp Ϊ0��˵���ɹ�
*-��ע		��
*******************************************************/

unsigned char SD_Init(void)
{
	unsigned char temp;
	temp = SD_Reset();//дCMD0��λ
	temp = SD_SendCmdNoDeassert(CMD8,0X1AA,0X87);

 //�����Ƭ�汾��Ϣ��v1.0�汾�ģ���temp=0x05����������³�ʼ��
	if(temp==0x05)
	{
		//���ÿ�����ΪSDV1.0����������⵽ΪMMC�������޸�ΪMMC
		 //SD_Type = SD_TYPE_V1;	   
		 //�����V1.0����CMD8ָ���û�к�������
		 //Ƭѡ�øߣ�������������
//		 SD_CS_1;
//		 //�෢8��CLK����SD������������
//		 SD_spi_RW(0xFF);	
//		 SD_Type = SD_MMC_Init();
//		 is_init=0;
//		 SD_spi_RW(0xFF);
//		
//		 //��ֹCRCУ��	   
//		 temp = SD_SendCmd(CMD59, 0, 0x95);
//		 if(temp != 0x00)return temp;  //������󣬷���r1   	   
//		 //����Sector Size
//		 temp = SD_SendCmd(CMD16, block, 0x95);
//		 if(temp != 0x00)return temp;//������󣬷���r1	
	}
  //������V2.0���ĳ�ʼ��
  //������Ҫ��ȡOCR���ݣ��ж���SD2.0����SD2.0HC��
	else if(temp == 0x01)
	  {
		 temp = SDV2_Init();
	  }
	// UART1_SendOneChar(temp);
//		 temp = SD_SendCmd(CMD59, 0, 0x95);//��ֹCRCУ��	
//		 if(temp != 0x00)return temp;  //������󣬷���r1  
//	temp = SD_SendCmd(CMD16,block, 0x95);

    return temp;
}


/*******************************************************************************
* Function Name  : SD_ReceiveData
* Description    : ��SD���ж���ָ�����ȵ����ݣ������ڸ���λ��
* Input          : unsigned char *dat(��Ŷ������ݵ��ڴ�>len)
*                  unsigned int len(���ݳ��ȣ�
*                  unsigned char release(������ɺ��Ƿ��ͷ�����CS�ø� 0�����ͷ� 1���ͷţ�
* Output         : None
* Return         : unsigned char 
*                  0��NO_ERR
*                  other��������Ϣ
*******************************************************************************/
//unsigned char SD_ReceiveData(unsigned char *dat, unsigned int len, unsigned char release)
//{
//  unsigned int retry;
//  unsigned char r1;
//  // ����һ�δ���
//  SD_CS_ENABLE();
//  //�ȴ�SD������������ʼ����0xFE
//  retry = 0;										   
//  do
//  {
//    r1 = SD_spi_RW(0xFF);
//    retry++;
//    if(retry>4000)  //4000�εȴ���û��Ӧ���˳�����(����ʵ����ԣ��˴���ö��Լ���
//    {
//      SD_CS_DISABLE();
//        return 1;
//    }
//    }while(r1 != 0xFE);
//		   
//    //��ʼ��������
//    while(len--)
//    {
//      *dat = SD_spi_RW(0xFF);
//      dat++;
//    }
//    //������2��αCRC��dummy CRC��
//    SD_spi_RW(0xFF);
//    SD_spi_RW(0xFF);
//    //�����ͷ����ߣ���CS�ø�
//    if(release == RELEASE)
//    {
//      //�������
//      SD_CS_DISABLE();
//      SD_spi_RW(0xFF);
//    }											  					    
//    return 0;
//}
unsigned char SD_ReceiveData32(unsigned char *dat, unsigned int len, unsigned char release,unsigned char side)
{
  unsigned int retry;
  unsigned char r1;
  // ����һ�δ���
  SD_CS_ENABLE();
  //�ȴ�SD������������ʼ����0xFE
  retry = 0;										   
  do
  {
    r1 = SD_spi_RW(0xFF);
    retry++;
    if(retry>4000)  //4000�εȴ���û��Ӧ���˳�����(����ʵ����ԣ��˴���ö��Լ���
    {
      SD_CS_DISABLE();
        return 1;
    }
    }while(r1 != 0xFE);
		   
    //��ʼ��������
    while(len)
    {
		if((512-len)>=side*32 && (512-len)<(side+1)*32)
		{
			*dat = SD_spi_RW(0xFF);
			dat++;
		}else SD_spi_RW(0xFF);
		len--;
     }
    //������2��αCRC��dummy CRC��
    SD_spi_RW(0xFF);
    SD_spi_RW(0xFF);
    //�����ͷ����ߣ���CS�ø�
    if(release == RELEASE)
    {
      //�������
      SD_CS_DISABLE();
      SD_spi_RW(0xFF);
    }											  					    
    return 0;
}

/*******************************************************************************
* Function Name  : SD_GetCID
* Description    : ��ȡSD����CID��Ϣ��������������Ϣ
* Input          : unsigned char *cid_data(���CID���ڴ棬����16Byte��
* Output         : None
* Return         : unsigned char 
*                  0��NO_ERR
*                  1��TIME_OUT
*                  other��������Ϣ
*******************************************************************************/
//unsigned char SD_GetCID(unsigned char *cid_data)
//{
//  unsigned char r1;

//  //��CMD10�����CID
//  r1 = SD_SendCmd(CMD10, 0, 0xFF);
//  if(r1 != 0x00)return r1;  //û������ȷӦ�����˳�������    
//  //����16���ֽڵ�����
//  SD_ReceiveData(cid_data, 16, RELEASE);	 
//  return 0;
//}

/*******************************************************************************
* Function Name  : SD_GetCSD
* Description    : ��ȡSD����CSD��Ϣ�������������ٶ���Ϣ
* Input          : unsigned char *cid_data(���CID���ڴ棬����16Byte��
* Output         : None
* Return         : unsigned char 
*                  0��NO_ERR
*                  1��TIME_OUT
*                  other��������Ϣ
*******************************************************************************/
//unsigned char SD_GetCSD(unsigned char *csd_data)
//{
//  unsigned char r1;

//  //��CMD9�����CSD
//  r1 = SD_SendCmd(CMD9, 0, 0xFF);
//  if(r1 != 0x00)return r1;  //û������ȷӦ�����˳�������  
//  //����16���ֽڵ�����
//  SD_ReceiveData(csd_data, 16, RELEASE);

//  return 0;
//}
/*******************************************************************************
* Function Name  : SD_GetCapacity
* Description    : ��ȡSD�����������ֽڣ�
* Input          : None
* Output         : None
* Return         : unsigned long int capacity 
*                   0�� ȡ�������� 
*******************************************************************************/
//unsigned long int SD_GetCapacity(void)
//{
//  unsigned char csd[16];
//  unsigned long int Capacity;
//  unsigned char r1;
//  unsigned int i;
//  unsigned int temp;

//  //ȡCSD��Ϣ������ڼ��������0
//  if(SD_GetCSD(csd)!=0) return 0;	    
//  //���ΪSDHC�����������淽ʽ����
//  if((csd[0]&0xC0)==0x40)
//  {									  
//    Capacity=((unsigned long int)csd[8])<<8;
//    Capacity+=(unsigned long int)csd[9]+1;	 
//    Capacity = (Capacity)*1024;//�õ�������
//    Capacity*=512;//�õ��ֽ���			   
//  }
//  else
//  {		    
//    i = csd[6]&0x03;
//    i<<=8;
//    i += csd[7];
//    i<<=2;
//    i += ((csd[8]&0xc0)>>6);
//    
//    //C_SIZE_MULT
//    r1 = csd[9]&0x03;
//    r1<<=1;
//    r1 += ((csd[10]&0x80)>>7);	 
//    r1+=2;//BLOCKNR
//    temp = 1;
//    while(r1)
//    {
//      temp*=2;
//      r1--;
//    }
//    Capacity = ((unsigned long int)(i+1))*((unsigned long int)temp);	 
//    // READ_BL_LEN
//    i = csd[5]&0x0f;
//    //BLOCK_LEN
//    temp = 1;
//    while(i)
//    {
//      temp*=2;
//      i--;
//    }
//    //The final result
//    Capacity *= (unsigned long int)temp;//�ֽ�Ϊ��λ 	  
//  }
//  return (unsigned long int)Capacity;
//}

/*******************************************************************************
* Function Name  : SD_WaitReady
* Description    : �ȴ�SD��Ready
* Input          : None
* Output         : None
* Return         : u8 
*                   0�� �ɹ�
*                   other��ʧ��
*******************************************************************************/
//unsigned char SD_WaitReady(void)
//{
//  unsigned char r1;
//  unsigned int retry=0;
//  do
//  {
//    r1 = SD_spi_RW(0xFF);
//    retry++;
//    if(retry==0xfffe)
//      return 1; 
//  }while(r1!=0xFF); 
//    return 0;
//}	 
/*******************************************************************************
* Function Name  : SD_ReadSingleBlock
* Description    : ��SD����һ��block
* Input          : unsigned long int sector ȡ��ַ��sectorֵ���������ַ�� 
*                  unsigned char *buffer ���ݴ洢��ַ����С����512byte�� 
* Output         : None
* Return         : unsigned char r1 
*                   0�� �ɹ�
*                   other��ʧ��
*******************************************************************************/
//unsigned char SD_ReadSingleBlock(unsigned long int sector, unsigned char *buffer)
//{
//  unsigned char r1;
//  //����Ϊ����ģʽ
//  is_init=0;

// if(SD_Type!=SD_TYPE_V2SDHC)
//  {
//	 //�������SDHC����sector��ַת��byte��ַ
//     sector = sector<<9;//512*sector�����������ı߽�����ַ
//  }
//    
//  r1 = SD_SendCmd(CMD17, sector, 1);//������
//												    
//  if(r1 != 0x00)return r1; 		   							  
//  r1 = SD_ReceiveData(buffer, block, RELEASE);		 
//  if(r1 != 0)
//    return r1;   //�����ݳ���
//  else 
//    return 0; 
//}

unsigned char SD_ReadSingleBlock(unsigned long int sector, unsigned char *buffer,unsigned char side)
{
  unsigned char r1;
  //����Ϊ����ģʽ
  is_init=0;

 if(SD_Type!=SD_TYPE_V2SDHC)
  {
	 //�������SDHC����sector��ַת��byte��ַ
     sector = sector<<9;//512*sector�����������ı߽�����ַ
  }
    
  r1 = SD_SendCmd(CMD17, sector, 1);//������
												    
  if(r1 != 0x00)return r1; 		   							  
  r1 = SD_ReceiveData32(buffer, block, RELEASE,side);		 
  if(r1 != 0)
    return r1;   //�����ݳ���
  else 
    return 0; 
}
/*******************************************************************************
* Function Name  : SD_WriteSingleBlock
* Description    : д��SD����һ��block
* Input          : unsigned long int sector ������ַ��sectorֵ���������ַ�� 
*                  unsigned char *buffer ���ݴ洢��ַ����С����512byte�� 
* Output         : None
* Return         : unsigned char r1 
*                   0�� �ɹ�
*                   other��ʧ��
*******************************************************************************/
unsigned char SD_WriteSingleBlock(unsigned long int sector, const unsigned char *dat)
{
  unsigned char r1;
  unsigned int i;
  unsigned int retry;

  //����Ϊ����ģʽ
  is_init=0;

  //�������SDHC����������sector��ַ������ת����byte��ַ
  if(SD_Type!=SD_TYPE_V2SDHC)
  {
     sector = sector<<9;//512*sector�����������ı߽�����ַ
  }

  r1 = SD_SendCmd(CMD24, sector, 0x00);
  if(r1 != 0x00)
  {
    return r1;  //Ӧ����ȷ��ֱ�ӷ���
  }
    
  //��ʼ׼�����ݴ���
  SD_CS_ENABLE();
  //�ȷ�3�������ݣ��ȴ�SD��׼����
  SD_spi_RW(0xff);
  SD_spi_RW(0xff);
  SD_spi_RW(0xff);
  //����ʼ����0xFE
  SD_spi_RW(0xFE);

  //��һ��sector������
  for(i=0;i<block;i++)
  {
     SD_spi_RW(*dat++);
  }
  //��2��Byte��dummy CRC
  SD_spi_RW(0xff);
  SD_spi_RW(0xff);
    
  //�ȴ�SD��Ӧ��
  r1 = SD_spi_RW(0xff);
  if((r1&0x1F)!=0x05)
  {
     SD_CS_DISABLE();
     return r1;
  }
    
  //�ȴ��������
  retry = 0;
  while(!SD_spi_RW(0xff))//���Ա��ʱ�������߱�����
  {
     retry++;
     if(retry>65534)        //�����ʱ��д��û����ɣ������˳�
     {
        SD_CS_DISABLE();
        return 1;           //д�볬ʱ����1
     }
  }

  //д����ɣ�Ƭѡ��1
  SD_CS_DISABLE();
  SD_spi_RW(0xff);

  return 0;
}

/*******************************************************************************
* Function Name  : SD_ReadMultiBlock
* Description    : ��SD���Ķ��block
* Input          : unsigned long int sector ȡ��ַ��sectorֵ���������ַ�� 
*                  unsigned char *buffer ���ݴ洢��ַ����С����512byte��
*                  unsigned char count ������count��block
* Output         : None
* Return         : unsigned char r1 
*                   0�� �ɹ�
*                   other��ʧ��
*******************************************************************************/
//unsigned char SD_ReadMultiBlock(unsigned long int sector, unsigned char *buffer, unsigned char count)
//{
//  unsigned char r1;	 			 
//  is_init=1;//����Ϊ����ģʽ 
//  
//  if(SD_Type != SD_TYPE_V2SDHC)
//    sector = sector<<9;//�������SDHC����������sector��ַ������ת����byte��ַ   
// // sector = sector<<9;//�������SDHC����sector��ַת��byte��ַ
// //SD_WaitReady();
// //�����������
//  r1 = SD_SendCmd(CMD18, sector, 1);//������
//  if(r1 != 0x00)return r1;	 
//  do//��ʼ��������
//  {
//    if(SD_ReceiveData(buffer, block, NO_RELEASE) != 0x00)
//    {
//       break;
//    }
//    buffer += block;
//  } while(--count);		 
//  //ȫ��������ϣ�����ֹͣ����
//  SD_SendCmd(CMD12, 0, 1);
//  //�ͷ�����
//  SD_CS_DISABLE();
//  SD_spi_RW(0xFF);    
//  if(count != 0)
//    return count;   //���û�д��꣬����ʣ�����	 
//  else 
//    return 0;	 
//}

/*******************************************************************************
* Function Name  : SD_WriteMultiBlock
* Description    : д��SD����N��block
* Input          : unsigned long int sector ������ַ��sectorֵ���������ַ�� 
*                  unsigned char *buffer ���ݴ洢��ַ����С����512byte��
*                  unsigned char count д���block��Ŀ
* Output         : None
* Return         : unsigned char r1 
*                   0�� �ɹ�
*                   other��ʧ��
*******************************************************************************/
//unsigned char SD_WriteMultiBlock(unsigned long int sector, const unsigned char *dat, unsigned char count)
//{
//  unsigned char r1;
//  unsigned int i;	 		 
//  is_init=1;//����Ϊ����ģʽ	 
//  if(SD_Type != SD_TYPE_V2SDHC)
//    sector = sector<<9;//�������SDHC����������sector��ַ������ת����byte��ַ  
//  if(SD_Type != SD_TYPE_MMC) 
//    r1 = SD_SendCmd(ACMD23, count, 0x01);//���Ŀ�꿨����MMC��������ACMD23ָ��ʹ��Ԥ����   
//  r1 = SD_SendCmd(CMD25, sector, 0x01);//�����д��ָ��
//  if(r1 != 0x00)return r1;  //Ӧ����ȷ��ֱ�ӷ���	 
//  SD_CS_ENABLE();//��ʼ׼�����ݴ���   
//  SD_spi_RW(0xff);//�ȷ�3�������ݣ��ȴ�SD��׼����
//  SD_spi_RW(0xff);   
//  //--------������N��sectorд���ѭ������
//  do
//  {
//    //����ʼ����0xFC �����Ƕ��д��
//     SD_spi_RW(0xFC);	  
//     //��һ��sector������
//     for(i=0;i<block;i++)
//     {
//        SD_spi_RW(*dat++);
//     }
//     //��2��Byte��dummy CRC
//     SD_spi_RW(0xff);
//     SD_spi_RW(0xff);
//        
//     //�ȴ�SD��Ӧ��
//     r1 = SD_spi_RW(0xff);
//     if((r1&0x1F)!=0x05)
//     {
//        SD_CS_DISABLE();    //���Ӧ��Ϊ��������������ֱ���˳�
//        return r1;
//     }

//     //�ȴ�SD��д�����
//     if(SD_WaitReady()==1)
//     {
//        SD_CS_DISABLE();    //�ȴ�SD��д����ɳ�ʱ��ֱ���˳�����
//        return 1;
//     }	   
//    }while(--count);//��sector���ݴ������
//    
//    //��������������0xFD
//    r1 = SD_spi_RW(0xFD);
//    if(r1==0x00)
//    {
//       count =  0xfe;
//    }		   
//    if(SD_WaitReady()) //�ȴ�׼����
//    {
//      SD_CS_DISABLE();
//      return 1;  
//    }
//    //д����ɣ�Ƭѡ��1
//    SD_CS_DISABLE();
//    SD_spi_RW(0xff);  
//    return count;   //����countֵ�����д����count=0������count=1
//}
	
/*******************************************************************************
* Function Name  : SD_Read_Bytes
* Description    : ��ָ������,��offset��ʼ����bytes���ֽ�
* Input          : unsigned long int address ������ַ��sectorֵ���������ַ�� 
*                  unsigned char *buf     ���ݴ洢��ַ����С<=512byte��
*                  unsigned int offset  �����������ƫ����
                   unsigned int bytes   Ҫ�������ֽ���
* Output         : None
* Return         : unsigned char r1 
*                   0�� �ɹ�
*                   other��ʧ��
*******************************************************************************/
//unsigned char SD_Read_Bytes(unsigned long address,unsigned char *buf,unsigned int offset,unsigned int bytes)
//{
//  unsigned char r1;unsigned int i=0;  
//  r1=SD_SendCmd(CMD17,address<<9,1);//���Ͷ���������      
//  if(r1!=0x00)return r1;  //Ӧ����ȷ��ֱ�ӷ���
//  SD_CS_ENABLE();//ѡ��SD��
//  while (SD_spi_RW(0xff)!= 0xFE)//ֱ����ȡ�������ݵĿ�ʼͷ0XFE���ż���
//  {
//    i++;
//    if(i>3000)
//    {
//      SD_CS_DISABLE();//�ر�SD��
//      return 1;//��ȡʧ��
//    }
//  }; 		 
//  for(i=0;i<offset;i++)
//    SD_spi_RW(0xff);//����offsetλ 
//  for(;i<offset+bytes;i++)
//    *buf++=SD_spi_RW(0xff);//��ȡ��������	
//  for(;i<block;i++) 
//	SD_spi_RW(0xff); 	 //����ʣ���ֽ�
//  SD_spi_RW(0xff);//����αCRC��
//  SD_spi_RW(0xff);  
//  SD_CS_DISABLE();//�ر�SD��
//  return 0;
//}

