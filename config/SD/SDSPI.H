#ifndef __SDSPI__H
#define __SDSPI__H

#include "UHEAD.h"
/******************************************************************************************************
1:	Ö÷Òª²Ù×÷£º
	¸´Î»
	³õÊ¼»¯
	Ğ´ÉÈÇø
	¶ÁÉÈÇø

2:	SD¿¨ÃüÁî¹²6×Ö½Ú

		×Ö½Ú1		2-5×Ö½Ú				6×Ö½Ú(×Ü¹²48Î»)
		01(Ö÷»ú)+ÃüÁî		²ÎÊı£¨¸ßÎ»ÔÚÇ°£©	CRC + 1
	
	ÏìÓ¦£º	
		R1£¬R3£¬R6ÏìÓ¦³¤¶ÈÎª48Î»£¬R2ÏìÓ¦³¤¶ÈÎª136Î»£¨CID/CSD£¬128Î»£©
		00(·µ»Ø)+¡£¡£¡£¡£  (×Ü¹²48Î»)»ò(136Î»)¡£¡£¡£+ 1(Ä©Î»×ÜÊÇ1)	
****************************************************************************************************************
¼Ä´æÆ÷£º
	   *1£ºCID	¿í¶È 128bit		¿¨±êÊ¶¼Ä´æÆ÷
		2£ºRCA	¿í¶È 16bit		Ïà¶Ô¿¨µØÖ·¼Ä´æÆ÷£¬*ÔÚSPIÄ£Ê½ÖĞÃ»ÓĞ
	   *3£ºCSD  ¿í¶È 128bit		¿¨ÃèÊö¼Ä´æÆ÷£¬°üº¬SD¿¨ÈİÁ¿£¬¿é´óĞ¡
		4£ºSCR  ¿í¶È 64bit		¿¨ÅäÖÃ¼Ä´æÆ÷
	   *5£ºOCR	¿í¶È32bit		²Ù×÷Ìõ¼ş¼Ä´æÆ÷£¬¿¨µÄµçÑ¹Ìõ¼ş£¬ÊÇ·ñÊÇ¸ßÈİÁ¿µÄ¿¨µÈ
		
*******************************************************************************************************	
1:	¶Áµ¥¸ö¿é£º	·¢ËÍCMD17Ö¸Áî£¬µÈ´ıR1ÏìÓ¦£¬µÈ´ıSD¿¨0xfeÏìÓ¦.Ö®ºó¶Á512¸ö×Ö½Ú£¬½ûÖ¹Æ¬Ñ¡£¬·¢8¸öÊ±ÖÓ½áÊø½ÓÊÕ¡£

2£º	¶Á¶à¿é£º    ·¢ËÍCMD18Ö¸Áî£¬µÈ´ıR1ÏìÓ¦£¬µÈ´ıSD¿¨0xfeÏìÓ¦.Ö®ºó¶Á512¸ö×Ö½Ú£»µÈ´ıSD¿¨0xfeÏìÓ¦.Ö®ºó¶Á512¸ö×Ö½Ú£»¡£¡£¡£¡¡/
				²»Ïë½ÓÊÕÊı¾İÊ±£¬·¢ËÍCMD12Ö¸Áî£¬½ûÖ¹Æ¬Ñ¡£¬·¢8¸öÊ±ÖÓ½áÊø½ÓÊÕ¡£
				
3£º	Ğ´µ¥¸ö¿é£º	·¢ËÍCMD24Ö¸Áî£¬µÈ´ıSDÏìÓ¦£¬·¢ËÍ0xfe£¬·¢ËÍÒ»¸ö¿éÊı¾İ£¨512£©£¬·¢ËÍ2¸ö0xff£¨CRC£©£¬µÈ´ıÏìÓ¦£¬½ûÖ¹Æ¬Ñ¡£¬·¢8¸öÊ±ÖÓ½áÊø¡£

4£º	Ğ´¶à¸ö¿é£º	·¢ËÍCMD55£¬·¢ËÍACMD23£¨½ö¶ÔSD¿¨ÓĞĞ££©£¬·¢ËÍCMD25£¬·¢ËÍ0xfc£¬Ğ´ÈëÒ»¸öÊı¾İ¿é£¬·¢ËÍ0xff£¨CRC£©£»
				µÈ´ıMISOÀ­¸ß£¬·¢ËÍ0xfc£¬Ğ´ÈëÒ»¸öÊı¾İ¿é£¬·¢ËÍ0xff£¨CRC£©¡£¡£¡£¡£·¢ËÍ0xfdÁîÅÆ£¬Ğ´Ò»¸öÊı¾İ¿é£¬½áÊøĞ´Èë¡£½ûÖ¹Æ¬Ñ¡£¬·¢8¸öÊ±ÖÓ½áÊøĞ´Èë¡£

****************************************************************************************************
	CMD0£º		¸´Î»SD¿¨£¬	»ØÓ¦R1
	CMD1£º		³õÊ¼»¯SD¿¨ £¬»ØÓ¦R1¡£½öMMC¿¨Ê¹ÓÃ£¬Ê¹Æä³õÊ¼»¯

	CMD8£º		·¢ËÍ½Ó¿Ú×´Ì¬ÃüÁî£¬²ÎÊı£ºVHS+Check pattern£¬»ØÓ¦R7¡££¨VHS¸æËßÖ÷»ú¹©µç·¶Î§£¬Check pattern¼ì²âÍ¨Ñ¶Çø·Ö2.0£,1.0£©
				Ò»°ã VHS = 0x1AA£¬¹©µç2.7~3.6V ¡£2.0ÒÔºóµÄ¿¨Ö§³Ö¸ÃÖ¸Áî
				
	CMD9£º		¶Á¿¨ÌØ¶¨¼Ä´æÆ÷ SCR£¬»ØÓ¦R1
	CMD10:		¶Á¿¨±êÖ¾Êı¾İ¼Ä´æÆ÷ CID£¬»ØÓ¦R1
	
	CMD12£º		½áÊø¶à¿é¶Á£¬»ØÓ¦
	CMD16£º		ÉèÖÃ¿é´óĞ¡¡£¸ßÈİÁ¿¿¨£¨SDHC-2T£©£¬¿é´óĞ¡¹Ì¶¨Îª512×Ö½Ú£¬²ÎÊı:¿é´óĞ¡
	CMD17£º		¶Á1¸ö¿é £¬²ÎÊı£ºµØÖ·£¬Èç¹ûÊÇSDHC£¬µØÖ·µÄµ¥Î»ÒÔ512×Ö½ÚÎªµ¥Î»¡£
	CMD18£º		¶Á¶à¸ö¿é£¬²ÎÊı£ºµØÖ·
	CMD24£º		Ğ´µ¥¸ö¿é£¬²ÎÊı£ºµØÖ·
	CMD25£º		Ğ´¶à¸ö¿é£¬²ÎÊı£ºµØÖ·
	CMD55£º		Í¨ÖªSD¿¨£¬½ÓÏÂÀ´·¢ËÍµÄÊÇÓ¦ÓÃÖ¸Áî£¬APP_CMD
	CMD58£º		¶ÁOCR¼Ä´æÆ÷,»ØÓ¦R3¡£µÚ30Î»ÊÇCCS£¬CCS=1£¬ÎªSDHC ¿¨£¬CCS = 0£¬ÎªSDSC¿¨
	
	ACMD23£º	Ô¤²Á³ıÊı¾İ¿é£¬¿ÉÒÔÌá¸ßÊı¾İĞ´ÈëËÙ¶È
	ACMD41£º	·¢ËÍ¸øÖ÷»úÈİÁ¿Ö§³ÖĞÅÏ¢ºÍ¼¤»î¿¨³õÊ¼»¯¹ı³Ì£¬spi»ØÓ¦R1£¬SDÄ£Ê½»ØÓ¦R3
	
	
*********************************************************************************************************
	Ó¦´ğ¿ÉÒÔÊÇR1~R7

	R1µÄÓ¦´ğ£º	Î»7£º¿ªÊ¼Î»Ê¼ÖÕÎª0£»6£º²ÎÊı´íÎó£»5£ºµØÖ·´íÎó£»4£º²Á³ıÏµÁĞ´íÎó£»3£ºCRC´íÎó£»2£º·Ç·¨Ö¸Áî
				1£º²Á³ı¸´Î»£»0£ºÃ¦×´Ì¬  £»²ÎÊıÎª1ÎªÓĞĞ§£¬0ÎªÎŞĞ§
*********************************************************************************************************				
×¢Òâ£º
		³õÊ¼»¯Ê±Ê±ÖÓ×î´ó²»ÄÜ³¬¹ı400KHZ£¬2.5us
	
*/


#define DELAY_TIME 10	//SD¿¨¸´Î»³õÊ¼»¯Ê±Ê¹ÓÃÑÓÊ±£
#define TRY_TIME 200 //ÏòSD¿¨ÖĞĞ´ÈëÊı¾İ£¬ÔÊĞí´íÎóµÄ´ÎÊı

#define	SD_CS_1			GPIO_SET(SD_CS)
#define	SD_CS_0			GPIO_RESET(SD_CS)
#define	SD_SO				GPIOReadPIN(SD_DO)
#define	SD_SCL_1		GPIO_SET(SD_SCK)
#define	SD_SCL_0		GPIO_RESET(SD_SCK)
#define	SD_SI_1			GPIO_SET(SD_DI)
#define	SD_SI_0			GPIO_RESET(SD_DI)

#define SD_CS_DISABLE() SD_CS_1
#define SD_CS_ENABLE()  SD_CS_0



/* SD´«ÊäÊı¾İ½áÊøºóÊÇ·ñÊÍ·Å×ÜÏßºê¶¨Òå */
#define NO_RELEASE      0
#define RELEASE         1	//ÊÍ·Å

//´íÎóÂë¶¨Òå

#define INIT_CMD0_ERROR 	0X01//CMD0´íÎó
#define INIT_CMD1_ERROR 	0X02//CMD1´íÎó
#define WRITE_BLOCK_ERROR 	0X03//Ğ´¿é´íÎó
#define READ_BLOCK_ERROR 	0X04//¶Á¿é´íÎó

/* SD¿¨Ö¸Áî±í */
#define CMD0    0      //¿¨¸´Î»  (Ó¦´ğ¸ñÊ½£ºR1)
#define CMD1    1      //MMC¿¨¿ªÊ¼³õÊ¼»¯
#define CMD8	8      //Ê¶±ğ¿¨µÄ°æ±¾
#define CMD9    9      //ÃüÁî9 £¬¶ÁCSDÊı¾İ   (Ó¦´ğ¸ñÊ½£ºR1)
#define CMD10   10     //ÃüÁî10£¬¶ÁCIDÊı¾İ   (Ó¦´ğ¸ñÊ½£ºR1)
#define CMD12   12     //ÃüÁî12£¬Í£Ö¹Êı¾İ´«Êä    (Ó¦´ğ¸ñÊ½£ºR1b)
#define CMD16   16     //ÃüÁî16£¬ÉèÖÃSectorSize Ó¦·µ»Ø0x00   (Ó¦´ğ¸ñÊ½£ºR1)
#define CMD17   17     //ÃüÁî17£¬¶Ásector    (Ó¦´ğ¸ñÊ½£ºR1)
#define CMD18   18     //ÃüÁî18£¬¶ÁMulti sector    (Ó¦´ğ¸ñÊ½£ºR1)
#define ACMD23  23     //ÃüÁî23£¬ÉèÖÃ¶àsectorĞ´ÈëÇ°Ô¤ÏÈ²Á³ıN¸öblock    (Ó¦´ğ¸ñÊ½£ºR1)
#define CMD24   24     //ÃüÁî24£¬Ğ´sector    (Ó¦´ğ¸ñÊ½£ºR1)
#define CMD25   25     //ÃüÁî25£¬Ğ´Multi sector    (Ó¦´ğ¸ñÊ½£ºR1)
#define ACMD41  41     //ÃüÁî41£¬Ó¦·µ»Ø0x00    (Ó¦´ğ¸ñÊ½£ºR1)
#define CMD55   55     //ÃüÁî55£¬Ó¦·µ»Ø0x01    (Ó¦´ğ¸ñÊ½£ºR1)
#define CMD58   58     //ÃüÁî58£¬¶ÁOCRĞÅÏ¢     (Ó¦´ğ¸ñÊ½£ºR1)
#define CMD59   59     //ÃüÁî59£¬Ê¹ÄÜ/½ûÖ¹CRC£¬Ó¦·µ»Ø0x00    (Ó¦´ğ¸ñÊ½£ºR1)

/* SD¿¨ÀàĞÍ¶¨Òå */
#define SD_TYPE_MMC       10
#define SD_TYPE_V1        11
#define SD_TYPE_V2SDSC    12
#define SD_TYPE_V2SDHC    14

/*¶¨Òå¿é´óĞ¡*/
#define block 512
//º¯ÊıÉùÃ÷

unsigned char SD_Init(void);//³õÊ¼»¯£¬·µ»ØSD¿¨°æ±¾

//unsigned char SD_GetCID(unsigned char *cid_data);//»ñÈ¡SD¿¨µÄCIDĞÅÏ¢£¬°üÀ¨ÖÆÔìÉÌĞÅÏ¢
//unsigned char SD_GetCSD(unsigned char *csd_data);//»ñÈ¡SD¿¨µÄCSDĞÅÏ¢£¬°üÀ¨ÈİÁ¿ºÍËÙ¶ÈĞÅÏ¢
//unsigned long int SD_GetCapacity(void);//»ñÈ¡SD¿¨µÄÈİÁ¿£¨×Ö½Ú£©
//unsigned char SD_ReadSingleBlock(unsigned long int sector, unsigned char *buffer);//¶ÁSD¿¨µÄÒ»¸öblock
//unsigned char SD_WriteSingleBlock(unsigned long int sector, const unsigned char *dat);//Ğ´ÈëSD¿¨µÄÒ»¸öblock
//unsigned char SD_ReadMultiBlock(unsigned long int sector, unsigned char *buffer, unsigned char count);//¶ÁSD¿¨µÄ¶à¸öblock
//unsigned char SD_WriteMultiBlock(unsigned long int sector, const unsigned char *dat, unsigned char count);
//unsigned char SD_Read_Bytes(unsigned long address,unsigned char *buf,unsigned int offset,unsigned int bytes);//ÔÚÖ¸¶¨ÉÈÇø,´Óoffset¿ªÊ¼¶Á³öbytes¸ö×Ö½Ú
//unsigned char SD_WaitReady(void)

unsigned char SD_ReadSingleBlock(unsigned long int sector, unsigned char *buffer,unsigned char side);//¶ÁSD¿¨µÄÒ»¸öblock,sideÊÇÒÔ32·Ö512µÄÎ»ÖÃ0~15
unsigned char SD_WriteSingleBlock(unsigned long int sector, const unsigned char *dat);//Ğ´ÈëSD¿¨µÄÒ»¸öblock

#endif
