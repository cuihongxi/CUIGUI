#include "sdspi.h"

u8 is_init = 0;//为SD卡初始化准备
unsigned char SD_Type = 0;
/*******************************************************
*-函数名称	：
*-函数作用	：延时
*-参数		：
*-返回值	：
*-备注		：
*******************************************************/

void delay(unsigned int dat)
{
	while(dat--);
}

/*******************************************************
*-函数名称	：
*-函数作用	：从SD读写一个字节
*-参数		：
*-返回值	：redat 读回来的数据
*-备注		：
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
*-函数名称	：
*-函数作用	：发送指令
*-参数		：cmd,命令；argument 参数；crc 校验码
*-返回值	：错误代码
*-备注		：
*******************************************************/

unsigned char SD_SendCmd(unsigned char cmd,unsigned long int argument,unsigned char crc)
{
	unsigned char temp;
	unsigned int Retry=0;
	SD_CS_1;
	SD_spi_RW(0xff);//提高兼容性，如果没有这里，有些SD卡可能不支持
	
	SD_CS_0;
	SD_spi_RW(0x40|cmd);
	SD_spi_RW((unsigned char)((argument>>24)));
	SD_spi_RW((unsigned char)(argument>>16));
	SD_spi_RW((unsigned char)(argument>>8));
	SD_spi_RW((unsigned char)argument);
	SD_spi_RW(crc);
	
   //等待响应，或超时退出
   while((temp = SD_spi_RW(0xFF))==0xFF)
   {
	 Retry++;
	 if(Retry > TRY_TIME)break; //根据实验测得，最好重试次数多点  
   }
   
   SD_CS_1;
   SD_spi_RW(0xff);
  //返回响应值
  return temp;
}

/*******************************************************************************
* Function Name  : SD_SendCmd_NoDeassert
* Description    : 向SD卡发送一个命令(结束是不失能片选，还有后续数据传来）
* Input          : unsigned char cmd   命令 
*                  unsigned long int arg  命令参数
*                  unsigned char crc   crc校验值
* Output         : None
* Return         : unsigned char r1 SD卡返回的响应
*******************************************************************************/
unsigned char SD_SendCmdNoDeassert(unsigned char cmd,unsigned long int argument,unsigned char crc)
{
	unsigned char temp;
	unsigned int Retry=0;
	SD_CS_1;
	SD_spi_RW(0xff);//提高兼容性，如果没有这里，有些SD卡可能不支持
	
	SD_CS_0;
	SD_spi_RW(0x40|cmd);
	SD_spi_RW((unsigned char)((argument>>24)));
	SD_spi_RW((unsigned char)(argument>>16));
	SD_spi_RW((unsigned char)(argument>>8));
	SD_spi_RW((unsigned char)argument);
	SD_spi_RW(crc);
	
   //等待响应，或超时退出
   while((temp = SD_spi_RW(0xFF))==0xFF)
   {
	 Retry++;
	 if(Retry > TRY_TIME)break; //根据实验测得，最好重试次数多点  
   }
  //返回响应值
  return temp;
}
/*******************************************************
*-函数名称	：
*-函数作用	：复位SD卡
*-参数		：
*-返回值	：TEMP=0X01为成功
*-备注		：
*******************************************************/

unsigned char SD_Reset()
{
	unsigned char time ,temp,i;
	
	is_init=1;
	
	SD_CS_1;//关闭片选
	
	for(i=0;i<0x0f;i++)
	{
		SD_spi_RW(0xff);//120个时钟
	}
	
	SD_CS_0;//片选打开
	
	time = 0;
	do
	{
		//temp = SD_Write_Cmd(pcmd);
		temp = SD_SendCmd(CMD0,0,0X95);
		time++;
		if(time >=TRY_TIME)
		{
			SD_CS_1;//关闭片选
			return (temp);
			
		}
	}while(temp != 0x01);
	SD_CS_1;//关闭片选
	SD_spi_RW(0xff);//补充8个时钟
	return temp;
}


/*******************************************************
*-函数名称	：
*-函数作用	：	//-----------------SD卡、MMC卡初始化开始-----------------	 
				//发卡初始化指令CMD55+ACMD41
				// 如果有应答，说明是SD卡，且初始化完成
				// 没有回应，说明是MMC卡，额外进行相应初始化
*-参数		：
*-返回值	：
*-备注		：
*******************************************************/

//判断MMC卡
//unsigned char MMC_Init()
//{
//	unsigned char r1;
//	unsigned int retry = 0;
//	//发送MMC卡初始化命令（没有测试）
//	do
//	{
//	   r1 = SD_SendCmd(CMD1, 0, 0);
//	   retry++;
//	}while((r1!=0x00)&& (retry<400));
//	if(retry>=400)return 1;   //MMC卡初始化超时		    
//	//写入卡类型
//	SD_Type = SD_TYPE_MMC;
//	return SD_Type;
//}


//V1.0版本判断是SD还是MMC
//unsigned char SD_MMC_Init()
//{
//	 unsigned char r1;
//	 unsigned int retry = 0;
//	
//     do
//     {
//       //先发CMD55，应返回0x01；否则出错
//       r1 = SD_SendCmd(CMD55, 0, 0);
//       if(r1 != 0x01)
//         return r1;	  
//       //得到正确响应后，发ACMD41，应得到返回值0x00，否则重试400次
//       r1 = SD_SendCmd(ACMD41, 0, 0);
//       retry++;
//     }while((r1!=0x00) && (retry<400));
//     // 判断是超时还是得到正确回应
//     // 若有回应：是SD卡；没有回应：是MMC卡
//	 SD_Type = SD_TYPE_V1;
//	 if(retry >= 400) SD_Type = MMC_Init();
//	 return SD_Type;
//}


//判断V2.0卡的型号
unsigned char SDV2_Init()
{
	unsigned int retry;
	unsigned char r1;
	unsigned char buff = 0;
    //V2.0的卡，CMD8命令后会传回4字节的数据，要跳过再结束本命令
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
    //判断该卡是否支持2.7V-3.6V的电压范围
    //if(buff[2]==0x01 && buff[3]==0xAA) //如不判断，让其支持的卡更多
   // {	  
       retry = 0;
       //发卡初始化指令CMD55+ACMD41
       do
       {
    	  r1 = SD_SendCmd(CMD55, 0, 0);
    	  if(r1!=0x01)return r1;	   
    	  r1 = SD_SendCmd(ACMD41, 0x40000000, 1);
          if(retry>200)return r1;  //超时则返回r1状态  
       }while(r1!=0);		  
       //初始化指令发送完成，接下来获取OCR信息	
	   
       //-----------鉴别SD2.0卡版本开始-----------
       r1 = SD_SendCmdNoDeassert(CMD58, 0, 0);
	   
       if(r1!=0x00)return r1;  //如果命令没有返回正确应答，直接退出，返回应答		 
       //读OCR指令发出后，紧接着是4字节的OCR信息
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
       //OCR接收完成，片选置高
       SD_CS_1;
       SD_spi_RW(0xFF);

       //检查接收到的OCR中的bit30位（CCS），确定其为SD2.0还是SDHC
       //如果CCS=1：SDHC   CCS=0：SD2.0
       if(buff&0x40)SD_Type = SD_TYPE_V2SDHC;    //检查CCS	 
       else SD_Type = SD_TYPE_V2SDSC;	    
       //-----------鉴别SD2.0卡版本结束----------- 
       //设置SPI为高速模式
       is_init=0; 
		return 0;
   // }	    
}

/*******************************************************
*-函数名称	：
*-函数作用	：初始化
*-参数		：
*-返回值	：temp 为0，说明成功
*-备注		：
*******************************************************/

unsigned char SD_Init(void)
{
	unsigned char temp;
	temp = SD_Reset();//写CMD0复位
	temp = SD_SendCmdNoDeassert(CMD8,0X1AA,0X87);

 //如果卡片版本信息是v1.0版本的，即temp=0x05，则进行以下初始化
	if(temp==0x05)
	{
		//设置卡类型为SDV1.0，如果后面检测到为MMC卡，再修改为MMC
		 //SD_Type = SD_TYPE_V1;	   
		 //如果是V1.0卡，CMD8指令后没有后续数据
		 //片选置高，结束本次命令
//		 SD_CS_1;
//		 //多发8个CLK，让SD结束后续操作
//		 SD_spi_RW(0xFF);	
//		 SD_Type = SD_MMC_Init();
//		 is_init=0;
//		 SD_spi_RW(0xFF);
//		
//		 //禁止CRC校验	   
//		 temp = SD_SendCmd(CMD59, 0, 0x95);
//		 if(temp != 0x00)return temp;  //命令错误，返回r1   	   
//		 //设置Sector Size
//		 temp = SD_SendCmd(CMD16, block, 0x95);
//		 if(temp != 0x00)return temp;//命令错误，返回r1	
	}
  //下面是V2.0卡的初始化
  //其中需要读取OCR数据，判断是SD2.0还是SD2.0HC卡
	else if(temp == 0x01)
	  {
		 temp = SDV2_Init();
	  }
	// UART1_SendOneChar(temp);
//		 temp = SD_SendCmd(CMD59, 0, 0x95);//禁止CRC校验	
//		 if(temp != 0x00)return temp;  //命令错误，返回r1  
//	temp = SD_SendCmd(CMD16,block, 0x95);

    return temp;
}


/*******************************************************************************
* Function Name  : SD_ReceiveData
* Description    : 从SD卡中读回指定长度的数据，放置在给定位置
* Input          : unsigned char *dat(存放读回数据的内存>len)
*                  unsigned int len(数据长度）
*                  unsigned char release(传输完成后是否释放总线CS置高 0：不释放 1：释放）
* Output         : None
* Return         : unsigned char 
*                  0：NO_ERR
*                  other：错误信息
*******************************************************************************/
//unsigned char SD_ReceiveData(unsigned char *dat, unsigned int len, unsigned char release)
//{
//  unsigned int retry;
//  unsigned char r1;
//  // 启动一次传输
//  SD_CS_ENABLE();
//  //等待SD卡发回数据起始令牌0xFE
//  retry = 0;										   
//  do
//  {
//    r1 = SD_spi_RW(0xFF);
//    retry++;
//    if(retry>4000)  //4000次等待后没有应答，退出报错(根据实验测试，此处最好多试几次
//    {
//      SD_CS_DISABLE();
//        return 1;
//    }
//    }while(r1 != 0xFE);
//		   
//    //开始接收数据
//    while(len--)
//    {
//      *dat = SD_spi_RW(0xFF);
//      dat++;
//    }
//    //下面是2个伪CRC（dummy CRC）
//    SD_spi_RW(0xFF);
//    SD_spi_RW(0xFF);
//    //按需释放总线，将CS置高
//    if(release == RELEASE)
//    {
//      //传输结束
//      SD_CS_DISABLE();
//      SD_spi_RW(0xFF);
//    }											  					    
//    return 0;
//}
unsigned char SD_ReceiveData32(unsigned char *dat, unsigned int len, unsigned char release,unsigned char side)
{
  unsigned int retry;
  unsigned char r1;
  // 启动一次传输
  SD_CS_ENABLE();
  //等待SD卡发回数据起始令牌0xFE
  retry = 0;										   
  do
  {
    r1 = SD_spi_RW(0xFF);
    retry++;
    if(retry>4000)  //4000次等待后没有应答，退出报错(根据实验测试，此处最好多试几次
    {
      SD_CS_DISABLE();
        return 1;
    }
    }while(r1 != 0xFE);
		   
    //开始接收数据
    while(len)
    {
		if((512-len)>=side*32 && (512-len)<(side+1)*32)
		{
			*dat = SD_spi_RW(0xFF);
			dat++;
		}else SD_spi_RW(0xFF);
		len--;
     }
    //下面是2个伪CRC（dummy CRC）
    SD_spi_RW(0xFF);
    SD_spi_RW(0xFF);
    //按需释放总线，将CS置高
    if(release == RELEASE)
    {
      //传输结束
      SD_CS_DISABLE();
      SD_spi_RW(0xFF);
    }											  					    
    return 0;
}

/*******************************************************************************
* Function Name  : SD_GetCID
* Description    : 获取SD卡的CID信息，包括制造商信息
* Input          : unsigned char *cid_data(存放CID的内存，至少16Byte）
* Output         : None
* Return         : unsigned char 
*                  0：NO_ERR
*                  1：TIME_OUT
*                  other：错误信息
*******************************************************************************/
//unsigned char SD_GetCID(unsigned char *cid_data)
//{
//  unsigned char r1;

//  //发CMD10命令，读CID
//  r1 = SD_SendCmd(CMD10, 0, 0xFF);
//  if(r1 != 0x00)return r1;  //没返回正确应答，则退出，报错    
//  //接收16个字节的数据
//  SD_ReceiveData(cid_data, 16, RELEASE);	 
//  return 0;
//}

/*******************************************************************************
* Function Name  : SD_GetCSD
* Description    : 获取SD卡的CSD信息，包括容量和速度信息
* Input          : unsigned char *cid_data(存放CID的内存，至少16Byte）
* Output         : None
* Return         : unsigned char 
*                  0：NO_ERR
*                  1：TIME_OUT
*                  other：错误信息
*******************************************************************************/
//unsigned char SD_GetCSD(unsigned char *csd_data)
//{
//  unsigned char r1;

//  //发CMD9命令，读CSD
//  r1 = SD_SendCmd(CMD9, 0, 0xFF);
//  if(r1 != 0x00)return r1;  //没返回正确应答，则退出，报错  
//  //接收16个字节的数据
//  SD_ReceiveData(csd_data, 16, RELEASE);

//  return 0;
//}
/*******************************************************************************
* Function Name  : SD_GetCapacity
* Description    : 获取SD卡的容量（字节）
* Input          : None
* Output         : None
* Return         : unsigned long int capacity 
*                   0： 取容量出错 
*******************************************************************************/
//unsigned long int SD_GetCapacity(void)
//{
//  unsigned char csd[16];
//  unsigned long int Capacity;
//  unsigned char r1;
//  unsigned int i;
//  unsigned int temp;

//  //取CSD信息，如果期间出错，返回0
//  if(SD_GetCSD(csd)!=0) return 0;	    
//  //如果为SDHC卡，按照下面方式计算
//  if((csd[0]&0xC0)==0x40)
//  {									  
//    Capacity=((unsigned long int)csd[8])<<8;
//    Capacity+=(unsigned long int)csd[9]+1;	 
//    Capacity = (Capacity)*1024;//得到扇区数
//    Capacity*=512;//得到字节数			   
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
//    Capacity *= (unsigned long int)temp;//字节为单位 	  
//  }
//  return (unsigned long int)Capacity;
//}

/*******************************************************************************
* Function Name  : SD_WaitReady
* Description    : 等待SD卡Ready
* Input          : None
* Output         : None
* Return         : u8 
*                   0： 成功
*                   other：失败
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
* Description    : 读SD卡的一个block
* Input          : unsigned long int sector 取地址（sector值，非物理地址） 
*                  unsigned char *buffer 数据存储地址（大小至少512byte） 
* Output         : None
* Return         : unsigned char r1 
*                   0： 成功
*                   other：失败
*******************************************************************************/
//unsigned char SD_ReadSingleBlock(unsigned long int sector, unsigned char *buffer)
//{
//  unsigned char r1;
//  //设置为高速模式
//  is_init=0;

// if(SD_Type!=SD_TYPE_V2SDHC)
//  {
//	 //如果不是SDHC，将sector地址转成byte地址
//     sector = sector<<9;//512*sector即物理扇区的边界对齐地址
//  }
//    
//  r1 = SD_SendCmd(CMD17, sector, 1);//读命令
//												    
//  if(r1 != 0x00)return r1; 		   							  
//  r1 = SD_ReceiveData(buffer, block, RELEASE);		 
//  if(r1 != 0)
//    return r1;   //读数据出错！
//  else 
//    return 0; 
//}

unsigned char SD_ReadSingleBlock(unsigned long int sector, unsigned char *buffer,unsigned char side)
{
  unsigned char r1;
  //设置为高速模式
  is_init=0;

 if(SD_Type!=SD_TYPE_V2SDHC)
  {
	 //如果不是SDHC，将sector地址转成byte地址
     sector = sector<<9;//512*sector即物理扇区的边界对齐地址
  }
    
  r1 = SD_SendCmd(CMD17, sector, 1);//读命令
												    
  if(r1 != 0x00)return r1; 		   							  
  r1 = SD_ReceiveData32(buffer, block, RELEASE,side);		 
  if(r1 != 0)
    return r1;   //读数据出错！
  else 
    return 0; 
}
/*******************************************************************************
* Function Name  : SD_WriteSingleBlock
* Description    : 写入SD卡的一个block
* Input          : unsigned long int sector 扇区地址（sector值，非物理地址） 
*                  unsigned char *buffer 数据存储地址（大小至少512byte） 
* Output         : None
* Return         : unsigned char r1 
*                   0： 成功
*                   other：失败
*******************************************************************************/
unsigned char SD_WriteSingleBlock(unsigned long int sector, const unsigned char *dat)
{
  unsigned char r1;
  unsigned int i;
  unsigned int retry;

  //设置为高速模式
  is_init=0;

  //如果不是SDHC，给定的是sector地址，将其转换成byte地址
  if(SD_Type!=SD_TYPE_V2SDHC)
  {
     sector = sector<<9;//512*sector即物理扇区的边界对齐地址
  }

  r1 = SD_SendCmd(CMD24, sector, 0x00);
  if(r1 != 0x00)
  {
    return r1;  //应答不正确，直接返回
  }
    
  //开始准备数据传输
  SD_CS_ENABLE();
  //先放3个空数据，等待SD卡准备好
  SD_spi_RW(0xff);
  SD_spi_RW(0xff);
  SD_spi_RW(0xff);
  //放起始令牌0xFE
  SD_spi_RW(0xFE);

  //发一个sector的数据
  for(i=0;i<block;i++)
  {
     SD_spi_RW(*dat++);
  }
  //发2个Byte的dummy CRC
  SD_spi_RW(0xff);
  SD_spi_RW(0xff);
    
  //等待SD卡应答
  r1 = SD_spi_RW(0xff);
  if((r1&0x1F)!=0x05)
  {
     SD_CS_DISABLE();
     return r1;
  }
    
  //等待操作完成
  retry = 0;
  while(!SD_spi_RW(0xff))//卡自编程时，数据线被拉低
  {
     retry++;
     if(retry>65534)        //如果长时间写入没有完成，报错退出
     {
        SD_CS_DISABLE();
        return 1;           //写入超时返回1
     }
  }

  //写入完成，片选置1
  SD_CS_DISABLE();
  SD_spi_RW(0xff);

  return 0;
}

/*******************************************************************************
* Function Name  : SD_ReadMultiBlock
* Description    : 读SD卡的多个block
* Input          : unsigned long int sector 取地址（sector值，非物理地址） 
*                  unsigned char *buffer 数据存储地址（大小至少512byte）
*                  unsigned char count 连续读count个block
* Output         : None
* Return         : unsigned char r1 
*                   0： 成功
*                   other：失败
*******************************************************************************/
//unsigned char SD_ReadMultiBlock(unsigned long int sector, unsigned char *buffer, unsigned char count)
//{
//  unsigned char r1;	 			 
//  is_init=1;//设置为高速模式 
//  
//  if(SD_Type != SD_TYPE_V2SDHC)
//    sector = sector<<9;//如果不是SDHC，给定的是sector地址，将其转换成byte地址   
// // sector = sector<<9;//如果不是SDHC，将sector地址转成byte地址
// //SD_WaitReady();
// //发读多块命令
//  r1 = SD_SendCmd(CMD18, sector, 1);//读命令
//  if(r1 != 0x00)return r1;	 
//  do//开始接收数据
//  {
//    if(SD_ReceiveData(buffer, block, NO_RELEASE) != 0x00)
//    {
//       break;
//    }
//    buffer += block;
//  } while(--count);		 
//  //全部传输完毕，发送停止命令
//  SD_SendCmd(CMD12, 0, 1);
//  //释放总线
//  SD_CS_DISABLE();
//  SD_spi_RW(0xFF);    
//  if(count != 0)
//    return count;   //如果没有传完，返回剩余个数	 
//  else 
//    return 0;	 
//}

/*******************************************************************************
* Function Name  : SD_WriteMultiBlock
* Description    : 写入SD卡的N个block
* Input          : unsigned long int sector 扇区地址（sector值，非物理地址） 
*                  unsigned char *buffer 数据存储地址（大小至少512byte）
*                  unsigned char count 写入的block数目
* Output         : None
* Return         : unsigned char r1 
*                   0： 成功
*                   other：失败
*******************************************************************************/
//unsigned char SD_WriteMultiBlock(unsigned long int sector, const unsigned char *dat, unsigned char count)
//{
//  unsigned char r1;
//  unsigned int i;	 		 
//  is_init=1;//设置为高速模式	 
//  if(SD_Type != SD_TYPE_V2SDHC)
//    sector = sector<<9;//如果不是SDHC，给定的是sector地址，将其转换成byte地址  
//  if(SD_Type != SD_TYPE_MMC) 
//    r1 = SD_SendCmd(ACMD23, count, 0x01);//如果目标卡不是MMC卡，启用ACMD23指令使能预擦除   
//  r1 = SD_SendCmd(CMD25, sector, 0x01);//发多块写入指令
//  if(r1 != 0x00)return r1;  //应答不正确，直接返回	 
//  SD_CS_ENABLE();//开始准备数据传输   
//  SD_spi_RW(0xff);//先放3个空数据，等待SD卡准备好
//  SD_spi_RW(0xff);   
//  //--------下面是N个sector写入的循环部分
//  do
//  {
//    //放起始令牌0xFC 表明是多块写入
//     SD_spi_RW(0xFC);	  
//     //放一个sector的数据
//     for(i=0;i<block;i++)
//     {
//        SD_spi_RW(*dat++);
//     }
//     //发2个Byte的dummy CRC
//     SD_spi_RW(0xff);
//     SD_spi_RW(0xff);
//        
//     //等待SD卡应答
//     r1 = SD_spi_RW(0xff);
//     if((r1&0x1F)!=0x05)
//     {
//        SD_CS_DISABLE();    //如果应答为报错，则带错误代码直接退出
//        return r1;
//     }

//     //等待SD卡写入完成
//     if(SD_WaitReady()==1)
//     {
//        SD_CS_DISABLE();    //等待SD卡写入完成超时，直接退出报错
//        return 1;
//     }	   
//    }while(--count);//本sector数据传输完成
//    
//    //发结束传输令牌0xFD
//    r1 = SD_spi_RW(0xFD);
//    if(r1==0x00)
//    {
//       count =  0xfe;
//    }		   
//    if(SD_WaitReady()) //等待准备好
//    {
//      SD_CS_DISABLE();
//      return 1;  
//    }
//    //写入完成，片选置1
//    SD_CS_DISABLE();
//    SD_spi_RW(0xff);  
//    return count;   //返回count值，如果写完则count=0，否则count=1
//}
	
/*******************************************************************************
* Function Name  : SD_Read_Bytes
* Description    : 在指定扇区,从offset开始读出bytes个字节
* Input          : unsigned long int address 扇区地址（sector值，非物理地址） 
*                  unsigned char *buf     数据存储地址（大小<=512byte）
*                  unsigned int offset  在扇区里面的偏移量
                   unsigned int bytes   要读出的字节数
* Output         : None
* Return         : unsigned char r1 
*                   0： 成功
*                   other：失败
*******************************************************************************/
//unsigned char SD_Read_Bytes(unsigned long address,unsigned char *buf,unsigned int offset,unsigned int bytes)
//{
//  unsigned char r1;unsigned int i=0;  
//  r1=SD_SendCmd(CMD17,address<<9,1);//发送读扇区命令      
//  if(r1!=0x00)return r1;  //应答不正确，直接返回
//  SD_CS_ENABLE();//选中SD卡
//  while (SD_spi_RW(0xff)!= 0xFE)//直到读取到了数据的开始头0XFE，才继续
//  {
//    i++;
//    if(i>3000)
//    {
//      SD_CS_DISABLE();//关闭SD卡
//      return 1;//读取失败
//    }
//  }; 		 
//  for(i=0;i<offset;i++)
//    SD_spi_RW(0xff);//跳过offset位 
//  for(;i<offset+bytes;i++)
//    *buf++=SD_spi_RW(0xff);//读取有用数据	
//  for(;i<block;i++) 
//	SD_spi_RW(0xff); 	 //读出剩余字节
//  SD_spi_RW(0xff);//发送伪CRC码
//  SD_spi_RW(0xff);  
//  SD_CS_DISABLE();//关闭SD卡
//  return 0;
//}

