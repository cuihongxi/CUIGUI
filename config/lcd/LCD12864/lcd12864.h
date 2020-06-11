
#ifndef __LCD12864_H__
#define __LCD12864_H__

#include <uhead.h>


#define Parallel_Mode   1
#define Serial_Mode     0


//���Ŷ���
#define     GPIO_RS     RS_GPIO_Port,     RS_Pin
#define     GPIO_RW     RW_GPIO_Port,     RW_Pin
#define     GPIO_E      E_GPIO_Port,      E_Pin


#define     GPIO_BK     BCK_CRL_GPIO_Port,BCK_CRL_Pin    

#define     GPIO_D0     LCD_D0_GPIO_Port, LCD_D0_Pin
#define     GPIO_D1     LCD_D1_GPIO_Port, LCD_D1_Pin
#define     GPIO_D2     LCD_D2_GPIO_Port, LCD_D2_Pin
#define     GPIO_D3     LCD_D3_GPIO_Port, LCD_D3_Pin
#define     GPIO_D4     LCD_D4_GPIO_Port, LCD_D4_Pin
#define     GPIO_D5     LCD_D5_GPIO_Port, LCD_D5_Pin
#define     GPIO_D6     LCD_D6_GPIO_Port, LCD_D6_Pin
#define     GPIO_D7     LCD_D7_GPIO_Port, LCD_D7_Pin

#define     Set_RS()    GPIO_SetBits(GPIO_RS) 
#define     Set_RW()    GPIO_SetBits(GPIO_RW) 
#define     Set_E()     GPIO_SetBits(GPIO_E) 

#define     Set(D0)    GPIO_SetBits(GPIO_##D0) 
//#define     Set_D1()    GPIO_SetBits(GPIO_D1) 
//#define     Set_D2()    GPIO_SetBits(GPIO_D2) 
//#define     Set_D3()    GPIO_SetBits(GPIO_D3) 
//#define     Set_D4()    GPIO_SetBits(GPIO_D4) 
//#define     Set_D5()    GPIO_SetBits(GPIO_D5) 
//#define     Set_D6()    GPIO_SetBits(GPIO_D6) 
//#define     Set_D7()    GPIO_SetBits(GPIO_D7) 

#define     Clr_RS()    GPIO_ResetBits(GPIO_RS) 
#define     Clr_RW()    GPIO_ResetBits(GPIO_RW) 
#define     Clr_E()     GPIO_ResetBits(GPIO_E) 

#define     Clr_D0()    GPIO_ResetBits(GPIO_D0) 
#define     Clr_D1()    GPIO_ResetBits(GPIO_D1) 
#define     Clr_D2()    GPIO_ResetBits(GPIO_D2) 
#define     Clr_D3()    GPIO_ResetBits(GPIO_D3) 
#define     Clr_D4()    GPIO_ResetBits(GPIO_D4) 
#define     Clr_D5()    GPIO_ResetBits(GPIO_D5) 
#define     Clr_D6()    GPIO_ResetBits(GPIO_D6) 
#define     Clr_D7()    GPIO_ResetBits(GPIO_D7) 
    
#define     Open_BK()   GPIO_SetBits(GPIO_BK)         //������
#define     Close_BK()  GPIO_ResetBits(GPIO_BK)       //�ر���

#define     Read(D0)   GPIO_ReadInputDataBit(LCD_##D0##_GPIO_Port,LCD_##D0##_Pin)
//#define     Read_D1()   GPIO_ReadInputDataBit(GPIO_D1)
//#define     Read_D2()   GPIO_ReadInputDataBit(GPIO_D2)
//#define     Read_D3()   GPIO_ReadInputDataBit(GPIO_D3)
//#define     Read_D4()   GPIO_ReadInputDataBit(GPIO_D4)
//#define     Read_D5()   GPIO_ReadInputDataBit(GPIO_D5)
//#define     Read_D6()   GPIO_ReadInputDataBit(GPIO_D6)
//#define     Read_D7()   GPIO_ReadInputDataBit(GPIO_D7)

#define lcd_delay_ms(a) HAL_Delay(a)
#define lcd_delay_us(a) //delay_us(a)  

//LCD12864���е�ַ
#define LINE0 0X80
#define LINE1 0X90
#define LINE2 0X88
#define LINE3 0X98
#define LINE0_N 0XA0
#define LINE1_N 0XB0
#define LINE2_N 0XA8
#define LINE3_N 0XB8

#define DISPLAY 1
#define NOTDISPLAY 0


//����
extern void LCD12864_Init(void);            //��ʼ��
extern void LCD12864_String(unsigned char x,unsigned char y,unsigned char* str);//�����꣺��x��y����д��str��������

unsigned char LCD12864_WaitIdle(void);
extern void LCD12864_NoWaitIdle_COM_Write(unsigned char com_da);
extern void LCD12864_COM_Write( unsigned char com_da);
extern void LCD12864_Data_Write(unsigned char da);
void LCD12864_Setposition(unsigned char xpos,unsigned char ypos);

extern void LCD12864_write_word(unsigned char *s);

//LCD12864 �ӵ�ַ�ж�����
u16 LCD12864_ReadDAT(unsigned char addr);
//ʹ����չָ��
void setRE(void);

//��ʹ����չָ��
void unsetRE(void);
//��0x88~0x8f,0x98~0x9f,�����ݿ�����0xA0~0XA8,0XB0~0XB8��
void copySTRtoNex(unsigned char start,unsigned char end);
//�������
//void Rullplay(u16 time);

//void rollto(unsigned char line);
void LCD12864_clearGDR(void);
void LCD12864_Drawpoint(unsigned char x,unsigned char y,unsigned char displayornot);//x��Χ��0~127��y��Χ0~63//���ã�����
void LCD12864_DrawPicture(unsigned char position_x,unsigned char position_y,\
                          unsigned char datH,unsigned char datL,unsigned char* picture,unsigned char color);//��һ��ָ����С��ͼƬ
unsigned char LCD12864_Readpoint(unsigned char x,unsigned char y);
void LCD12864_Clear(void);
void LCD12864_CGRAM_ON(void);
void LCD12864_CGRAM_OFF(void);
void LCD12864_DrawRectangle(u16 position_x,u16 position_y, u16 datH,u16 datL);//��ʵ�ľ���
unsigned char LCD12864_Readpoint(unsigned char x,unsigned char y);//����
void LCD12864_ClearPicture(unsigned char position_x,unsigned char position_y,unsigned char datH,unsigned char datL);
//;���ó�����ģʽ
void LCD12864_IO_OUT(void);
//;���ó�����ģʽ
void LCD12864_IO_IN(void);
void LCD12864_GPIO_INIT(void);
void LCD12864_DrawPicture2(unsigned char position_x,unsigned char position_y,\
                          unsigned char datH,unsigned char datL,unsigned char* picture);
#endif



