#include "cuiFAT.h"
#include<usart.h>
u8  Buff0[512] = {0};

SD_valueINFO_Struct  SD_Dat;


/*******************************************************
*-��������	��
*-��������	����С������ת����intֵ
*-����		��array��С����������sizeofarrayӦ��ʹ��sizeof������鳤��
*-����ֵ	��dat Ϊת����INTֵ
*-��ע		��
*******************************************************/
u32 getvalu(u8* array,u8 sizeofarray)
{
	u8 i;
	u32 dat = 0;

	for(i = 0;i<sizeofarray;i++)
	{
		dat = dat | ((u32)array[i]<<(8*i));
	}
	return dat;
}


/*******************************************************
*-��������	��
*-��������	��//���SD�����õ���Ϣ
*-����		��
*-����ֵ	��
*-��ע		��
*******************************************************/

void Get_SD_valueINFO(void)
{
	SDMBR_Struct* SDMBR_Struct0;
	SDDBR_Struct* SDDBR_Struct0;
    SDFSINFO_Struct* FSInformation;
	unsigned int zone1_DPTFirstSector = 0;
	u8 j=0;

	//��0����
	CuiFat_ReadSingleBlock(0,Buff0);
	
	if(Buff0[0] == 0xEB)
	{
		zone1_DPTFirstSector = 0;
	}
	else
	{
		SDMBR_Struct0 = (SDMBR_Struct*)Buff0;
			/*���ҷ������� */
		while(zone1_DPTFirstSector == 0)
		{
			zone1_DPTFirstSector = getvalu(((SDMBR_Struct0->zone1_DPTFirstSector)+j),4);	
			j +=  + 16;
		}
	}

	//��BPB����
	CuiFat_ReadSingleBlock(zone1_DPTFirstSector,Buff0);														
	SDDBR_Struct0 = (SDDBR_Struct*)Buff0;
	//���FAT��1��������
	SD_Dat.FirstFATSector = zone1_DPTFirstSector + get_valu(SDDBR_Struct0->BPB_RsvdSecCnt);	
	//һ��FAT32��ռ��������	
	SD_Dat.FATSz32 = get_valu(SDDBR_Struct0->BPB_FATSz32);	
	//���FSINFO������
	SD_Dat.FSInfo = zone1_DPTFirstSector + get_valu(SDDBR_Struct0->BPB_FSInfo);
	//��ø�Ŀ¼��������
	SD_Dat.FirstDirSector = SD_Dat.FirstFATSector + get_valu(SDDBR_Struct0->BPB_NumFATs)*SD_Dat.FATSz32;
	//ÿ���ص�������	
	SD_Dat.SecPerClus = get_valu(SDDBR_Struct0->BPB_SecPerClus);
	//���0�����ڵ�������
	SD_Dat.ClusBase = SD_Dat.FirstDirSector - (SD_Dat.SecPerClus)*get_valu(SDDBR_Struct0->BPB_RootClus);
	//��ø�Ŀ¼���ڵĴغ�
	SD_Dat.FirstDirCuNum = get_valu(SDDBR_Struct0->BPB_RootClus);
	
	
    
    //��FSINFO
    CuiFat_ReadSingleBlock(SD_Dat.FSInfo,Buff0);
    FSInformation = (SDFSINFO_Struct*)Buff0;
    SD_Dat.FSINFO_secoter = get_valu(FSInformation->FSI_Nxt_free);//�¸������õĴغ�    
    
    printf("FSINFO=%d",SD_Dat.FSINFO_secoter);
}





/*******************************************************
*-��������	��
*-��������	��ƥ���ļ���
*-����		��
*-����ֵ	    ��ƥ���ļ���������0˵���ɹ���1ʧ��
*-��ע		��
*******************************************************/

u8 CompareStr(u8* filename,u8* str)
{
	u8 i=0,dat = 0;

	while(filename[i])
	{
		dat = filename[i] ^ str[i];
		i++;
		if(dat != 0) break;	//��ͬ
	}
	if(i<8)
	{
		for(;i<8;i++)
		{
			dat = str[i] ^ 0x20;//�жϺ����ǲ��ǿո�
			if(dat!= 0) break;	//��ͬ
		}	
	}

	return dat;
}


/*******************************************************
*-��������	��
*-��������	��//���شض�Ӧ��������
*-����		��
*-����ֵ	��
*-��ע		��
*******************************************************/
u32 ClubtoSector(unsigned long club)
{
	
	return(SD_Dat.ClusBase + SD_Dat.SecPerClus * club);
}


/*******************************************************
*-��������	��
*-��������	����FAT���еĴأ�������һ�����أ�
*-����		��
*-����ֵ	��������0XFFFFFFF
*-��ע		��
*******************************************************/

u32 FATListNEX(u32 club)
{
	u32 j;
	u8 i;
	
	FATListStruct* fatlist;
	j = club/128;
	i = club%128;	
	CuiFat_ReadSingleBlock(SD_Dat.FirstFATSector+j,Buff0);
	fatlist = (FATListStruct*)Buff0;
	return (get_valu(fatlist->FATListArray[i]));
}

/*******************************************************
*-��������	��
*-��������	��SD�������ļ���
*-����		��
*-����ֵ	�������ļ���Ϣ
*-��ע		��
*******************************************************/
FileInforStruct FineFile(u8* filename)
{
	MenuStruct* menuList;
	FileInforStruct fileInformation;
	
	u32 fatnext;
	u8 j;
	unsigned int k;
	u32 i;	//j �����ǵڼ���������i�ǵڼ�����Ϣ��k�ǵڼ�����
	
	fatnext = SD_Dat.FirstDirCuNum;//��ø�Ŀ¼�غ�
		

	//�����ɨ��
	for(k=SD_Dat.FirstDirCuNum;fatnext != 0xfffffff;k++)
	{
		//���������������ɨ��
		for(j=0;j<SD_Dat.SecPerClus;j++)
		{
			CuiFat_ReadSingleBlock(SD_Dat.ClusBase + fatnext*SD_Dat.SecPerClus + j,Buff0);
			menuList = (MenuStruct*)Buff0;
			//����������������Ϣɨ��
			for(i=0;i<16;i++)
			{
				if(CompareStr(filename,menuList->Menu[i].DirName0)==0)	//�ҵ��ļ������ļ���Ϣ
				{
					fileInformation.CuID = (get_valu(menuList->Menu[i].DIR_FstClusHI)<<16) | get_valu(menuList->Menu[i].DIR_FstClusLO);
					fileInformation.FileSize = get_valu(menuList->Menu[i].DIR_FileSize);
					//��¼�ڼ�����¼
					fileInformation.i = k*SD_Dat.SecPerClus*16+j*16+i;
					return fileInformation;
				}
			}
		}

		fatnext = FATListNEX(fatnext);//��ȡ��һ������

	};
	return fileInformation;
}


/*******************************************************
*-��������	��
*-��������	����FAT����Ѱ�ҿ��õĴ�
*-����		��
*-����ֵ	��	���صڼ��Ŵ�
*-��ע		��
*******************************************************/
u32 SearchCu()
{
	unsigned int i,i0,i1;
	u8 j;
	FATListStruct* fatlist;
    i0 = SD_Dat.FSINFO_secoter/128;
    i1 = SD_Dat.FSINFO_secoter%128;
	for(i= i0;i<SD_Dat.FATSz32;i++)
	{
		//��FAT��
		CuiFat_ReadSingleBlock(SD_Dat.FirstFATSector+i,Buff0);
		fatlist = (FATListStruct*)Buff0;
        
        if(i == i0)
        {
        	for(j=i1;j<128;j++)
            {
                if(get_valu(fatlist->FATListArray[j]) == 0)
                {
                    return (i*128+j);
                }
            }
        }else
        {
            for(j=0;j<128;j++)
            {
                if(get_valu(fatlist->FATListArray[j]) == 0)
                {
                    return (i*128+j);
                }
            }
        }

	}
	
	return 0;
}


/*******************************************************
*-��������	��
*-��������	�����ļ�Ŀ¼����д�����Ϣ���ļ���С
*-����		��
*-����ֵ	��
*-��ע		��
*******************************************************/
void FstWriteData(u32 fileInfor_i,u32 filesize,u32 club)
{
	u8 i;
	u32 j;
	MenuStruct* menuList;

	j = (fileInfor_i/16)+SD_Dat.ClusBase;//�������
	i = fileInfor_i%16;//���λ��
	//��
	CuiFat_ReadSingleBlock(j,Buff0);
	menuList = (MenuStruct*)Buff0;
//	//д���
//	menuList->Menu[i].DIR_FstClusHI[0] = club%16777216/65536;
//	menuList->Menu[i].DIR_FstClusHI[1] = club/16777216;
//	menuList->Menu[i].DIR_FstClusLO[0] = club%256;
//	menuList->Menu[i].DIR_FstClusLO[1] = club%65536/256;
//	//д���С
//	menuList->Menu[i].DIR_FileSize[0] = filesize%256;	
//	menuList->Menu[i].DIR_FileSize[1] = filesize%65536/256;
//	menuList->Menu[i].DIR_FileSize[2] = filesize%16777216/65536;
//	menuList->Menu[i].DIR_FileSize[3] = filesize/16777216;
	//д���
	menuList->Menu[i].DIR_FstClusHI[0] = (club&0x00ff0000)>>16;
	menuList->Menu[i].DIR_FstClusHI[1] = (club&0xff000000)>>24;
	menuList->Menu[i].DIR_FstClusLO[0] = club&0x000000ff;
	menuList->Menu[i].DIR_FstClusLO[1] = (club&0x0000ff00)>>8;
	//д���С
	menuList->Menu[i].DIR_FileSize[0] = filesize&0x000000ff;	
	menuList->Menu[i].DIR_FileSize[1] = (filesize&0x0000ff00)>>8;
	menuList->Menu[i].DIR_FileSize[2] = (filesize&0x00ff0000)>>16;
	menuList->Menu[i].DIR_FileSize[3] = (filesize&0xff000000)>>24;
	//д����
	CuiFat_WriteSingleBlock(j,Buff0);
}
/*******************************************************
*-��������	��
*-��������	����дĳһ���ص�ֵ
*-����		��
*-����ֵ	��
*-��ע		��
*******************************************************/
void CuWriteData(u32 club,u32 dat)
{
	u32 j;//����
	u8 i;	
	FATListStruct* fatlist;
	j = club/128;
	i = club%128;
	//��
	CuiFat_ReadSingleBlock(j+SD_Dat.FirstFATSector,Buff0);
	fatlist = (FATListStruct*)Buff0;
	
	fatlist->FATListArray[i][0] = dat&0x000000ff;
	fatlist->FATListArray[i][1] = (dat&0x0000ff00)>>8;
	fatlist->FATListArray[i][2] = (dat&0x00ff0000)>>16;
	fatlist->FATListArray[i][3] = (dat&0xff000000)>>24;
	CuiFat_WriteSingleBlock(j+SD_Dat.FirstFATSector,Buff0);
}
/*******************************************************
*-��������	��
*-��������	����һ���ļ���д����
*-����		��
*-����ֵ	��LENTH��STR�ĳ���
*-��ע		��
*******************************************************/
void File_Write(u8* filename,u8* str,u32 lenth)	
{
	u32 cuNum,cuNum2;
	unsigned int lenth_i;
	u8 lenth_j;
	unsigned int i;
	FileInforStruct fileInformation;
	
	SDFSINFO_Struct* FSInformation;
//    u32 ClusNum ;
    
    //�����Ҫ��������
//	ClusNum =1 + (lenth/(SD_Dat.SecPerClus*512));
    
    
    
    
	//ƥ���ļ���������ļ���Ϣ
	fileInformation = FineFile(filename);
	cuNum = fileInformation.CuID;
		
	lenth_j = (fileInformation.FileSize%(SD_Dat.SecPerClus*512))/512;	//���еĵڼ�������
	lenth_i = (fileInformation.FileSize%(SD_Dat.SecPerClus*512))%512;	//�����е��ֽ�λ��
	if(cuNum == 0)
	{
    
		//�������õĴ�
		cuNum= SearchCu();
		
        //Ԥд����
        
        
		//���ô�д0XFFFFFFF
		CuWriteData(cuNum,0XFFFFFFF);
		
		//��ôض�Ӧ��������д����
		CuiFat_WriteSingleBlock(ClubtoSector(cuNum),str);
		//���ļ�Ŀ¼��д��ôأ����ļ���С
		FstWriteData(fileInformation.i,lenth,cuNum);
        
        //��FSINFO
        CuiFat_ReadSingleBlock(SD_Dat.FSInfo,Buff0);
        FSInformation = (SDFSINFO_Struct*)Buff0;
       
        //дFSINFO
        FSInformation->FSI_Nxt_free[0] = cuNum&0x000000ff;
        FSInformation->FSI_Nxt_free[1] = (cuNum&0x0000ff00)>>8;
        FSInformation->FSI_Nxt_free[2] = (cuNum&0x00ff0000)>>16;
        FSInformation->FSI_Nxt_free[3] = (cuNum&0xff000000)>>24;
        CuiFat_WriteSingleBlock(SD_Dat.FSInfo,Buff0);
	}
	else 
	{
		//���FAT��ôص�ֵ,�������β�ؾͲ�����һ������
		
		do
		{
			cuNum2 = cuNum;				//cuNum2��¼��β�صĴغ�
			cuNum = FATListNEX(cuNum);
		}while(cuNum!=0xfffffff);

		cuNum = cuNum2;
		
		//��ôض�Ӧ��������������
		CuiFat_ReadSingleBlock(ClubtoSector(cuNum)+lenth_j,Buff0);
		
		//д����.�����������ಿ��дstr,���������������д����һ���������������8����д��һ���أ�����FAT���д
		
		if(lenth<=(512-lenth_i))//δ��������
		{
			//���������д��
			for(i=0;i<lenth;i++)
			{
				Buff0[lenth_i+i] = str[i];
				
			}
			CuiFat_WriteSingleBlock(ClubtoSector(cuNum)+lenth_j,Buff0);
            
		}
		else//��������
		{

			//���������д��
			for(i=0;i<(512-lenth_i);i++)
			{
				Buff0[lenth_i+i] = str[i];
				
			}
			CuiFat_WriteSingleBlock(ClubtoSector(cuNum)+lenth_j,Buff0);
			
			//�������һ����
			if(lenth_j == (SD_Dat.SecPerClus-1))
			{
				//������һ�����õĴأ�������һ��FAT�����ӵ��ô�
								
				cuNum2 = SearchCu();		//cuNum2��¼��β�صĴغ�
				CuWriteData(cuNum,cuNum2);	
				CuWriteData(cuNum2,0xfffffff);
				cuNum = cuNum2;
				for(i=0;i<(lenth-(512-lenth_i));i++)
				{
					Buff0[i] = str[512-lenth_i+i];
				}
				CuiFat_WriteSingleBlock(ClubtoSector(cuNum),Buff0);
                
                //��FSINFO
                CuiFat_ReadSingleBlock(SD_Dat.FSInfo,Buff0);
                FSInformation = (SDFSINFO_Struct*)Buff0;
                //дFSINFO
                FSInformation->FSI_Nxt_free[0] = cuNum&0x000000ff;
                FSInformation->FSI_Nxt_free[1] = (cuNum&0x0000ff00)>>8;
                FSInformation->FSI_Nxt_free[2] = (cuNum&0x00ff0000)>>16;
                FSInformation->FSI_Nxt_free[3] = (cuNum&0xff000000)>>24;
                CuiFat_WriteSingleBlock(SD_Dat.FSInfo,Buff0);
			}
			//���δ����һ����
			else
				{
					for(i=0;i<(lenth-(512-lenth_i));i++)
					{
						Buff0[i] = str[512-lenth_i+i];
					}
					CuiFat_WriteSingleBlock(ClubtoSector(cuNum)+lenth_j+1,Buff0);
				}
		}

		
		//д��Ŀ¼����Ϣ

		FstWriteData(fileInformation.i,lenth+fileInformation.FileSize,fileInformation.CuID);

	}
}


























