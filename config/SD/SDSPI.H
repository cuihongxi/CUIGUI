#ifndef __SDSPI__H
#define __SDSPI__H

#include "UHEAD.h"
/******************************************************************************************************
1:	��Ҫ������
	��λ
	��ʼ��
	д����
	������

2:	SD�����6�ֽ�

		�ֽ�1		2-5�ֽ�				6�ֽ�(�ܹ�48λ)
		01(����)+����		��������λ��ǰ��	CRC + 1
	
	��Ӧ��	
		R1��R3��R6��Ӧ����Ϊ48λ��R2��Ӧ����Ϊ136λ��CID/CSD��128λ��
		00(����)+��������  (�ܹ�48λ)��(136λ)������+ 1(ĩλ����1)	
****************************************************************************************************************
�Ĵ�����
	   *1��CID	���� 128bit		����ʶ�Ĵ���
		2��RCA	���� 16bit		��Կ���ַ�Ĵ�����*��SPIģʽ��û��
	   *3��CSD  ���� 128bit		�������Ĵ���������SD�����������С
		4��SCR  ���� 64bit		�����üĴ���
	   *5��OCR	����32bit		���������Ĵ��������ĵ�ѹ�������Ƿ��Ǹ������Ŀ���
		
*******************************************************************************************************	
1:	�������飺	����CMD17ָ��ȴ�R1��Ӧ���ȴ�SD��0xfe��Ӧ.֮���512���ֽڣ���ֹƬѡ����8��ʱ�ӽ������ա�

2��	����飺    ����CMD18ָ��ȴ�R1��Ӧ���ȴ�SD��0xfe��Ӧ.֮���512���ֽڣ��ȴ�SD��0xfe��Ӧ.֮���512���ֽڣ���������/
				�����������ʱ������CMD12ָ���ֹƬѡ����8��ʱ�ӽ������ա�
				
3��	д�����飺	����CMD24ָ��ȴ�SD��Ӧ������0xfe������һ�������ݣ�512��������2��0xff��CRC�����ȴ���Ӧ����ֹƬѡ����8��ʱ�ӽ�����

4��	д����飺	����CMD55������ACMD23������SD����У��������CMD25������0xfc��д��һ�����ݿ飬����0xff��CRC����
				�ȴ�MISO���ߣ�����0xfc��д��һ�����ݿ飬����0xff��CRC��������������0xfd���ƣ�дһ�����ݿ飬����д�롣��ֹƬѡ����8��ʱ�ӽ���д�롣

****************************************************************************************************
	CMD0��		��λSD����	��ӦR1
	CMD1��		��ʼ��SD�� ����ӦR1����MMC��ʹ�ã�ʹ���ʼ��

	CMD8��		���ͽӿ�״̬���������VHS+Check pattern����ӦR7����VHS�����������緶Χ��Check pattern���ͨѶ����2.0�,1.0��
				һ�� VHS = 0x1AA������2.7~3.6V ��2.0�Ժ�Ŀ�֧�ָ�ָ��
				
	CMD9��		�����ض��Ĵ��� SCR����ӦR1
	CMD10:		������־���ݼĴ��� CID����ӦR1
	
	CMD12��		������������Ӧ
	CMD16��		���ÿ��С������������SDHC-2T�������С�̶�Ϊ512�ֽڣ�����:���С
	CMD17��		��1���� ����������ַ�������SDHC����ַ�ĵ�λ��512�ֽ�Ϊ��λ��
	CMD18��		������飬��������ַ
	CMD24��		д�����飬��������ַ
	CMD25��		д����飬��������ַ
	CMD55��		֪ͨSD�������������͵���Ӧ��ָ�APP_CMD
	CMD58��		��OCR�Ĵ���,��ӦR3����30λ��CCS��CCS=1��ΪSDHC ����CCS = 0��ΪSDSC��
	
	ACMD23��	Ԥ�������ݿ飬�����������д���ٶ�
	ACMD41��	���͸���������֧����Ϣ�ͼ����ʼ�����̣�spi��ӦR1��SDģʽ��ӦR3
	
	
*********************************************************************************************************
	Ӧ�������R1~R7

	R1��Ӧ��	λ7����ʼλʼ��Ϊ0��6����������5����ַ����4������ϵ�д���3��CRC����2���Ƿ�ָ��
				1��������λ��0��æ״̬  ������Ϊ1Ϊ��Ч��0Ϊ��Ч
*********************************************************************************************************				
ע�⣺
		��ʼ��ʱʱ������ܳ���400KHZ��2.5us
	
*/


#define DELAY_TIME 10	//SD����λ��ʼ��ʱʹ����ʱ�
#define TRY_TIME 200 //��SD����д�����ݣ���������Ĵ���

#define	SD_CS_1			GPIO_SET(SD_CS)
#define	SD_CS_0			GPIO_RESET(SD_CS)
#define	SD_SO				GPIOReadPIN(SD_DO)
#define	SD_SCL_1		GPIO_SET(SD_SCK)
#define	SD_SCL_0		GPIO_RESET(SD_SCK)
#define	SD_SI_1			GPIO_SET(SD_DI)
#define	SD_SI_0			GPIO_RESET(SD_DI)

#define SD_CS_DISABLE() SD_CS_1
#define SD_CS_ENABLE()  SD_CS_0



/* SD�������ݽ������Ƿ��ͷ����ߺ궨�� */
#define NO_RELEASE      0
#define RELEASE         1	//�ͷ�

//�����붨��

#define INIT_CMD0_ERROR 	0X01//CMD0����
#define INIT_CMD1_ERROR 	0X02//CMD1����
#define WRITE_BLOCK_ERROR 	0X03//д�����
#define READ_BLOCK_ERROR 	0X04//�������

/* SD��ָ��� */
#define CMD0    0      //����λ  (Ӧ���ʽ��R1)
#define CMD1    1      //MMC����ʼ��ʼ��
#define CMD8	8      //ʶ�𿨵İ汾
#define CMD9    9      //����9 ����CSD����   (Ӧ���ʽ��R1)
#define CMD10   10     //����10����CID����   (Ӧ���ʽ��R1)
#define CMD12   12     //����12��ֹͣ���ݴ���    (Ӧ���ʽ��R1b)
#define CMD16   16     //����16������SectorSize Ӧ����0x00   (Ӧ���ʽ��R1)
#define CMD17   17     //����17����sector    (Ӧ���ʽ��R1)
#define CMD18   18     //����18����Multi sector    (Ӧ���ʽ��R1)
#define ACMD23  23     //����23�����ö�sectorд��ǰԤ�Ȳ���N��block    (Ӧ���ʽ��R1)
#define CMD24   24     //����24��дsector    (Ӧ���ʽ��R1)
#define CMD25   25     //����25��дMulti sector    (Ӧ���ʽ��R1)
#define ACMD41  41     //����41��Ӧ����0x00    (Ӧ���ʽ��R1)
#define CMD55   55     //����55��Ӧ����0x01    (Ӧ���ʽ��R1)
#define CMD58   58     //����58����OCR��Ϣ     (Ӧ���ʽ��R1)
#define CMD59   59     //����59��ʹ��/��ֹCRC��Ӧ����0x00    (Ӧ���ʽ��R1)

/* SD�����Ͷ��� */
#define SD_TYPE_MMC       10
#define SD_TYPE_V1        11
#define SD_TYPE_V2SDSC    12
#define SD_TYPE_V2SDHC    14

/*������С*/
#define block 512
//��������

unsigned char SD_Init(void);//��ʼ��������SD���汾

//unsigned char SD_GetCID(unsigned char *cid_data);//��ȡSD����CID��Ϣ��������������Ϣ
//unsigned char SD_GetCSD(unsigned char *csd_data);//��ȡSD����CSD��Ϣ�������������ٶ���Ϣ
//unsigned long int SD_GetCapacity(void);//��ȡSD�����������ֽڣ�
//unsigned char SD_ReadSingleBlock(unsigned long int sector, unsigned char *buffer);//��SD����һ��block
//unsigned char SD_WriteSingleBlock(unsigned long int sector, const unsigned char *dat);//д��SD����һ��block
//unsigned char SD_ReadMultiBlock(unsigned long int sector, unsigned char *buffer, unsigned char count);//��SD���Ķ��block
//unsigned char SD_WriteMultiBlock(unsigned long int sector, const unsigned char *dat, unsigned char count);
//unsigned char SD_Read_Bytes(unsigned long address,unsigned char *buf,unsigned int offset,unsigned int bytes);//��ָ������,��offset��ʼ����bytes���ֽ�
//unsigned char SD_WaitReady(void)

unsigned char SD_ReadSingleBlock(unsigned long int sector, unsigned char *buffer,unsigned char side);//��SD����һ��block,side����32��512��λ��0~15
unsigned char SD_WriteSingleBlock(unsigned long int sector, const unsigned char *dat);//д��SD����һ��block

#endif