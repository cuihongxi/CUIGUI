/**
	cuiFAT������
	2019��5��3��12:40:22
*/

#ifndef	__CUIFAT_CONFIG_H
#define	__CUIFAT_CONFIG_H

#define	CuiFat_molloc(x)											CUI_Mallco(x)												//��̬�ڴ����
#define	CuiFat_Free(p)												FreeCUI_Mallco(p)										//�ͷ��ڴ�

#define	CuiFat_EndianValu(parray,type)				*((type*)parray)										//���С�˵�ֵ

#define	CuiFat_ReadSingleBlock(sector,pbuff)	SD_ReadBlock(pbuff,sector,512);			//��һ������
#define	CuiFat_WriteSingleBlock(sector,pbuff)	SD_WriteBlock(pbuff,sector,512);		//дһ������
    
//�����̷�


#endif
