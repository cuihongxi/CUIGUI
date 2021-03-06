#ifndef __SDSPI__H
#define __SDSPI__H

#include "UHEAD.h"
/******************************************************************************************************
1:	主要操作：
	复位
	初始化
	写扇区
	读扇区

2:	SD卡命令共6字节

		字节1		2-5字节				6字节(总共48位)
		01(主机)+命令		参数（高位在前）	CRC + 1
	
	响应：	
		R1，R3，R6响应长度为48位，R2响应长度为136位（CID/CSD，128位）
		00(返回)+。。。。  (总共48位)或(136位)。。。+ 1(末位总是1)	
****************************************************************************************************************
寄存器：
	   *1：CID	宽度 128bit		卡标识寄存器
		2：RCA	宽度 16bit		相对卡地址寄存器，*在SPI模式中没有
	   *3：CSD  宽度 128bit		卡描述寄存器，包含SD卡容量，块大小
		4：SCR  宽度 64bit		卡配置寄存器
	   *5：OCR	宽度32bit		操作条件寄存器，卡的电压条件，是否是高容量的卡等
		
*******************************************************************************************************	
1:	读单个块：	发送CMD17指令，等待R1响应，等待SD卡0xfe响应.之后读512个字节，禁止片选，发8个时钟结束接收。

2：	读多块：    发送CMD18指令，等待R1响应，等待SD卡0xfe响应.之后读512个字节；等待SD卡0xfe响应.之后读512个字节；。。。　/
				不想接收数据时，发送CMD12指令，禁止片选，发8个时钟结束接收。
				
3：	写单个块：	发送CMD24指令，等待SD响应，发送0xfe，发送一个块数据（512），发送2个0xff（CRC），等待响应，禁止片选，发8个时钟结束。

4：	写多个块：	发送CMD55，发送ACMD23（仅对SD卡有校），发送CMD25，发送0xfc，写入一个数据块，发送0xff（CRC）；
				等待MISO拉高，发送0xfc，写入一个数据块，发送0xff（CRC）。。。。发送0xfd令牌，写一个数据块，结束写入。禁止片选，发8个时钟结束写入。

****************************************************************************************************
	CMD0：		复位SD卡，	回应R1
	CMD1：		初始化SD卡 ，回应R1。仅MMC卡使用，使其初始化

	CMD8：		发送接口状态命令，参数：VHS+Check pattern，回应R7。（VHS告诉主机供电范围，Check pattern检测通讯区分2.0�,1.0）
				一般 VHS = 0x1AA，供电2.7~3.6V 。2.0以后的卡支持该指令
				
	CMD9：		读卡特定寄存器 SCR，回应R1
	CMD10:		读卡标志数据寄存器 CID，回应R1
	
	CMD12：		结束多块读，回应
	CMD16：		设置块大小。高容量卡（SDHC-2T），块大小固定为512字节，参数:块大小
	CMD17：		读1个块 ，参数：地址，如果是SDHC，地址的单位以512字节为单位。
	CMD18：		读多个块，参数：地址
	CMD24：		写单个块，参数：地址
	CMD25：		写多个块，参数：地址
	CMD55：		通知SD卡，接下来发送的是应用指令，APP_CMD
	CMD58：		读OCR寄存器,回应R3。第30位是CCS，CCS=1，为SDHC 卡，CCS = 0，为SDSC卡
	
	ACMD23：	预擦除数据块，可以提高数据写入速度
	ACMD41：	发送给主机容量支持信息和激活卡初始化过程，spi回应R1，SD模式回应R3
	
	
*********************************************************************************************************
	应答可以是R1~R7

	R1的应答：	位7：开始位始终为0；6：参数错误；5：地址错误；4：擦除系列错误；3：CRC错误；2：非法指令
				1：擦除复位；0：忙状态  ；参数为1为有效，0为无效
*********************************************************************************************************				
注意：
		初始化时时钟最大不能超过400KHZ，2.5us
	
*/


#define DELAY_TIME 10	//SD卡复位初始化时使用延时�
#define TRY_TIME 200 //向SD卡中写入数据，允许错误的次数

#define	SD_CS_1			GPIO_SET(SD_CS)
#define	SD_CS_0			GPIO_RESET(SD_CS)
#define	SD_SO				GPIOReadPIN(SD_DO)
#define	SD_SCL_1		GPIO_SET(SD_SCK)
#define	SD_SCL_0		GPIO_RESET(SD_SCK)
#define	SD_SI_1			GPIO_SET(SD_DI)
#define	SD_SI_0			GPIO_RESET(SD_DI)

#define SD_CS_DISABLE() SD_CS_1
#define SD_CS_ENABLE()  SD_CS_0



/* SD传输数据结束后是否释放总线宏定义 */
#define NO_RELEASE      0
#define RELEASE         1	//释放

//错误码定义

#define INIT_CMD0_ERROR 	0X01//CMD0错误
#define INIT_CMD1_ERROR 	0X02//CMD1错误
#define WRITE_BLOCK_ERROR 	0X03//写块错误
#define READ_BLOCK_ERROR 	0X04//读块错误

/* SD卡指令表 */
#define CMD0    0      //卡复位  (应答格式：R1)
#define CMD1    1      //MMC卡开始初始化
#define CMD8	8      //识别卡的版本
#define CMD9    9      //命令9 ，读CSD数据   (应答格式：R1)
#define CMD10   10     //命令10，读CID数据   (应答格式：R1)
#define CMD12   12     //命令12，停止数据传输    (应答格式：R1b)
#define CMD16   16     //命令16，设置SectorSize 应返回0x00   (应答格式：R1)
#define CMD17   17     //命令17，读sector    (应答格式：R1)
#define CMD18   18     //命令18，读Multi sector    (应答格式：R1)
#define ACMD23  23     //命令23，设置多sector写入前预先擦除N个block    (应答格式：R1)
#define CMD24   24     //命令24，写sector    (应答格式：R1)
#define CMD25   25     //命令25，写Multi sector    (应答格式：R1)
#define ACMD41  41     //命令41，应返回0x00    (应答格式：R1)
#define CMD55   55     //命令55，应返回0x01    (应答格式：R1)
#define CMD58   58     //命令58，读OCR信息     (应答格式：R1)
#define CMD59   59     //命令59，使能/禁止CRC，应返回0x00    (应答格式：R1)

/* SD卡类型定义 */
#define SD_TYPE_MMC       10
#define SD_TYPE_V1        11
#define SD_TYPE_V2SDSC    12
#define SD_TYPE_V2SDHC    14

/*定义块大小*/
#define block 512
//函数声明

unsigned char SD_Init(void);//初始化，返回SD卡版本

//unsigned char SD_GetCID(unsigned char *cid_data);//获取SD卡的CID信息，包括制造商信息
//unsigned char SD_GetCSD(unsigned char *csd_data);//获取SD卡的CSD信息，包括容量和速度信息
//unsigned long int SD_GetCapacity(void);//获取SD卡的容量（字节）
//unsigned char SD_ReadSingleBlock(unsigned long int sector, unsigned char *buffer);//读SD卡的一个block
//unsigned char SD_WriteSingleBlock(unsigned long int sector, const unsigned char *dat);//写入SD卡的一个block
//unsigned char SD_ReadMultiBlock(unsigned long int sector, unsigned char *buffer, unsigned char count);//读SD卡的多个block
//unsigned char SD_WriteMultiBlock(unsigned long int sector, const unsigned char *dat, unsigned char count);
//unsigned char SD_Read_Bytes(unsigned long address,unsigned char *buf,unsigned int offset,unsigned int bytes);//在指定扇区,从offset开始读出bytes个字节
//unsigned char SD_WaitReady(void)

unsigned char SD_ReadSingleBlock(unsigned long int sector, unsigned char *buffer,unsigned char side);//读SD卡的一个block,side是以32分512的位置0~15
unsigned char SD_WriteSingleBlock(unsigned long int sector, const unsigned char *dat);//写入SD卡的一个block

#endif
