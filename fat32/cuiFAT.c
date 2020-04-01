#include "cuiFAT.h"
#include<usart.h>
u8  Buff0[512] = {0};

SD_valueINFO_Struct  SD_Dat;


/*******************************************************
*-函数名称	：
*-函数作用	：将小端数组转换成int值
*-参数		：array，小端数组名，sizeofarray应该使用sizeof获得数组长度
*-返回值	：dat 为转换的INT值
*-备注		：
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
*-函数名称	：
*-函数作用	：//获得SD卡有用的信息
*-参数		：
*-返回值	：
*-备注		：
*******************************************************/

void Get_SD_valueINFO(void)
{
	SDMBR_Struct* SDMBR_Struct0;
	SDDBR_Struct* SDDBR_Struct0;
    SDFSINFO_Struct* FSInformation;
	unsigned int zone1_DPTFirstSector = 0;
	u8 j=0;

	//读0扇区
	CuiFat_ReadSingleBlock(0,Buff0);
	
	if(Buff0[0] == 0xEB)
	{
		zone1_DPTFirstSector = 0;
	}
	else
	{
		SDMBR_Struct0 = (SDMBR_Struct*)Buff0;
			/*查找分区表项 */
		while(zone1_DPTFirstSector == 0)
		{
			zone1_DPTFirstSector = getvalu(((SDMBR_Struct0->zone1_DPTFirstSector)+j),4);	
			j +=  + 16;
		}
	}

	//读BPB扇区
	CuiFat_ReadSingleBlock(zone1_DPTFirstSector,Buff0);														
	SDDBR_Struct0 = (SDDBR_Struct*)Buff0;
	//获得FAT表1的扇区号
	SD_Dat.FirstFATSector = zone1_DPTFirstSector + get_valu(SDDBR_Struct0->BPB_RsvdSecCnt);	
	//一个FAT32表占的扇区数	
	SD_Dat.FATSz32 = get_valu(SDDBR_Struct0->BPB_FATSz32);	
	//获得FSINFO扇区号
	SD_Dat.FSInfo = zone1_DPTFirstSector + get_valu(SDDBR_Struct0->BPB_FSInfo);
	//获得根目录的扇区号
	SD_Dat.FirstDirSector = SD_Dat.FirstFATSector + get_valu(SDDBR_Struct0->BPB_NumFATs)*SD_Dat.FATSz32;
	//每个簇的扇区数	
	SD_Dat.SecPerClus = get_valu(SDDBR_Struct0->BPB_SecPerClus);
	//获得0簇所在的扇区号
	SD_Dat.ClusBase = SD_Dat.FirstDirSector - (SD_Dat.SecPerClus)*get_valu(SDDBR_Struct0->BPB_RootClus);
	//获得根目录所在的簇号
	SD_Dat.FirstDirCuNum = get_valu(SDDBR_Struct0->BPB_RootClus);
	
	
    
    //读FSINFO
    CuiFat_ReadSingleBlock(SD_Dat.FSInfo,Buff0);
    FSInformation = (SDFSINFO_Struct*)Buff0;
    SD_Dat.FSINFO_secoter = get_valu(FSInformation->FSI_Nxt_free);//下个可以用的簇号    
    
    printf("FSINFO=%d",SD_Dat.FSINFO_secoter);
}





/*******************************************************
*-函数名称	：
*-函数作用	：匹配文件名
*-参数		：
*-返回值	    ：匹配文件名，返回0说明成功，1失败
*-备注		：
*******************************************************/

u8 CompareStr(u8* filename,u8* str)
{
	u8 i=0,dat = 0;

	while(filename[i])
	{
		dat = filename[i] ^ str[i];
		i++;
		if(dat != 0) break;	//不同
	}
	if(i<8)
	{
		for(;i<8;i++)
		{
			dat = str[i] ^ 0x20;//判断后面是不是空格
			if(dat!= 0) break;	//不同
		}	
	}

	return dat;
}


/*******************************************************
*-函数名称	：
*-函数作用	：//返回簇对应的扇区号
*-参数		：
*-返回值	：
*-备注		：
*******************************************************/
u32 ClubtoSector(unsigned long club)
{
	
	return(SD_Dat.ClusBase + SD_Dat.SecPerClus * club);
}


/*******************************************************
*-函数名称	：
*-函数作用	：读FAT表中的簇，给出下一个链簇，
*-参数		：
*-返回值	：结束是0XFFFFFFF
*-备注		：
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
*-函数名称	：
*-函数作用	：SD卡中找文件名
*-参数		：
*-返回值	：返回文件信息
*-备注		：
*******************************************************/
FileInforStruct FineFile(u8* filename)
{
	MenuStruct* menuList;
	FileInforStruct fileInformation;
	
	u32 fatnext;
	u8 j;
	unsigned int k;
	u32 i;	//j 决定是第几个扇区，i是第几条信息，k是第几个簇
	
	fatnext = SD_Dat.FirstDirCuNum;//获得根目录簇号
		

	//逐个簇扫描
	for(k=SD_Dat.FirstDirCuNum;fatnext != 0xfffffff;k++)
	{
		//在整个簇逐个扇区扫描
		for(j=0;j<SD_Dat.SecPerClus;j++)
		{
			CuiFat_ReadSingleBlock(SD_Dat.ClusBase + fatnext*SD_Dat.SecPerClus + j,Buff0);
			menuList = (MenuStruct*)Buff0;
			//在整个扇区逐条信息扫描
			for(i=0;i<16;i++)
			{
				if(CompareStr(filename,menuList->Menu[i].DirName0)==0)	//找到文件返回文件信息
				{
					fileInformation.CuID = (get_valu(menuList->Menu[i].DIR_FstClusHI)<<16) | get_valu(menuList->Menu[i].DIR_FstClusLO);
					fileInformation.FileSize = get_valu(menuList->Menu[i].DIR_FileSize);
					//记录第几条记录
					fileInformation.i = k*SD_Dat.SecPerClus*16+j*16+i;
					return fileInformation;
				}
			}
		}

		fatnext = FATListNEX(fatnext);//读取下一个链簇

	};
	return fileInformation;
}


/*******************************************************
*-函数名称	：
*-函数作用	：在FAT表中寻找可用的簇
*-参数		：
*-返回值	：	返回第几号簇
*-备注		：
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
		//读FAT表
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
*-函数名称	：
*-函数作用	：在文件目录项中写入簇信息和文件大小
*-参数		：
*-返回值	：
*-备注		：
*******************************************************/
void FstWriteData(u32 fileInfor_i,u32 filesize,u32 club)
{
	u8 i;
	u32 j;
	MenuStruct* menuList;

	j = (fileInfor_i/16)+SD_Dat.ClusBase;//获得扇区
	i = fileInfor_i%16;//获得位置
	//读
	CuiFat_ReadSingleBlock(j,Buff0);
	menuList = (MenuStruct*)Buff0;
//	//写入簇
//	menuList->Menu[i].DIR_FstClusHI[0] = club%16777216/65536;
//	menuList->Menu[i].DIR_FstClusHI[1] = club/16777216;
//	menuList->Menu[i].DIR_FstClusLO[0] = club%256;
//	menuList->Menu[i].DIR_FstClusLO[1] = club%65536/256;
//	//写入大小
//	menuList->Menu[i].DIR_FileSize[0] = filesize%256;	
//	menuList->Menu[i].DIR_FileSize[1] = filesize%65536/256;
//	menuList->Menu[i].DIR_FileSize[2] = filesize%16777216/65536;
//	menuList->Menu[i].DIR_FileSize[3] = filesize/16777216;
	//写入簇
	menuList->Menu[i].DIR_FstClusHI[0] = (club&0x00ff0000)>>16;
	menuList->Menu[i].DIR_FstClusHI[1] = (club&0xff000000)>>24;
	menuList->Menu[i].DIR_FstClusLO[0] = club&0x000000ff;
	menuList->Menu[i].DIR_FstClusLO[1] = (club&0x0000ff00)>>8;
	//写入大小
	menuList->Menu[i].DIR_FileSize[0] = filesize&0x000000ff;	
	menuList->Menu[i].DIR_FileSize[1] = (filesize&0x0000ff00)>>8;
	menuList->Menu[i].DIR_FileSize[2] = (filesize&0x00ff0000)>>16;
	menuList->Menu[i].DIR_FileSize[3] = (filesize&0xff000000)>>24;
	//写扇区
	CuiFat_WriteSingleBlock(j,Buff0);
}
/*******************************************************
*-函数名称	：
*-函数作用	：改写某一个簇的值
*-参数		：
*-返回值	：
*-备注		：
*******************************************************/
void CuWriteData(u32 club,u32 dat)
{
	u32 j;//扇区
	u8 i;	
	FATListStruct* fatlist;
	j = club/128;
	i = club%128;
	//读
	CuiFat_ReadSingleBlock(j+SD_Dat.FirstFATSector,Buff0);
	fatlist = (FATListStruct*)Buff0;
	
	fatlist->FATListArray[i][0] = dat&0x000000ff;
	fatlist->FATListArray[i][1] = (dat&0x0000ff00)>>8;
	fatlist->FATListArray[i][2] = (dat&0x00ff0000)>>16;
	fatlist->FATListArray[i][3] = (dat&0xff000000)>>24;
	CuiFat_WriteSingleBlock(j+SD_Dat.FirstFATSector,Buff0);
}
/*******************************************************
*-函数名称	：
*-函数作用	：向一个文件中写内容
*-参数		：
*-返回值	：LENTH是STR的长度
*-备注		：
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
    
    //获得需要的扇区数
//	ClusNum =1 + (lenth/(SD_Dat.SecPerClus*512));
    
    
    
    
	//匹配文件名并获得文件信息
	fileInformation = FineFile(filename);
	cuNum = fileInformation.CuID;
		
	lenth_j = (fileInformation.FileSize%(SD_Dat.SecPerClus*512))/512;	//簇中的第几个扇区
	lenth_i = (fileInformation.FileSize%(SD_Dat.SecPerClus*512))%512;	//扇区中的字节位置
	if(cuNum == 0)
	{
    
		//搜索可用的簇
		cuNum= SearchCu();
		
        //预写簇链
        
        
		//将该簇写0XFFFFFFF
		CuWriteData(cuNum,0XFFFFFFF);
		
		//获得簇对应的扇区，写扇区
		CuiFat_WriteSingleBlock(ClubtoSector(cuNum),str);
		//将文件目录项写入该簇，和文件大小
		FstWriteData(fileInformation.i,lenth,cuNum);
        
        //读FSINFO
        CuiFat_ReadSingleBlock(SD_Dat.FSInfo,Buff0);
        FSInformation = (SDFSINFO_Struct*)Buff0;
       
        //写FSINFO
        FSInformation->FSI_Nxt_free[0] = cuNum&0x000000ff;
        FSInformation->FSI_Nxt_free[1] = (cuNum&0x0000ff00)>>8;
        FSInformation->FSI_Nxt_free[2] = (cuNum&0x00ff0000)>>16;
        FSInformation->FSI_Nxt_free[3] = (cuNum&0xff000000)>>24;
        CuiFat_WriteSingleBlock(SD_Dat.FSInfo,Buff0);
	}
	else 
	{
		//获得FAT表该簇的值,如果不是尾簇就查找下一个链簇
		
		do
		{
			cuNum2 = cuNum;				//cuNum2记录了尾簇的簇号
			cuNum = FATListNEX(cuNum);
		}while(cuNum!=0xfffffff);

		cuNum = cuNum2;
		
		//获得簇对应的扇区，读扇区
		CuiFat_ReadSingleBlock(ClubtoSector(cuNum)+lenth_j,Buff0);
		
		//写扇区.接着扇区空余部分写str,如果超过扇区，就写到下一个，如果扇区超过8，就写下一个簇，并将FAT表改写
		
		if(lenth<=(512-lenth_i))//未超过扇区
		{
			//将这个扇区写满
			for(i=0;i<lenth;i++)
			{
				Buff0[lenth_i+i] = str[i];
				
			}
			CuiFat_WriteSingleBlock(ClubtoSector(cuNum)+lenth_j,Buff0);
            
		}
		else//超过扇区
		{

			//将这个扇区写满
			for(i=0;i<(512-lenth_i);i++)
			{
				Buff0[lenth_i+i] = str[i];
				
			}
			CuiFat_WriteSingleBlock(ClubtoSector(cuNum)+lenth_j,Buff0);
			
			//如果超过一个簇
			if(lenth_j == (SD_Dat.SecPerClus-1))
			{
				//搜索下一个可用的簇，并将上一个FAT表链接到该簇
								
				cuNum2 = SearchCu();		//cuNum2记录了尾簇的簇号
				CuWriteData(cuNum,cuNum2);	
				CuWriteData(cuNum2,0xfffffff);
				cuNum = cuNum2;
				for(i=0;i<(lenth-(512-lenth_i));i++)
				{
					Buff0[i] = str[512-lenth_i+i];
				}
				CuiFat_WriteSingleBlock(ClubtoSector(cuNum),Buff0);
                
                //读FSINFO
                CuiFat_ReadSingleBlock(SD_Dat.FSInfo,Buff0);
                FSInformation = (SDFSINFO_Struct*)Buff0;
                //写FSINFO
                FSInformation->FSI_Nxt_free[0] = cuNum&0x000000ff;
                FSInformation->FSI_Nxt_free[1] = (cuNum&0x0000ff00)>>8;
                FSInformation->FSI_Nxt_free[2] = (cuNum&0x00ff0000)>>16;
                FSInformation->FSI_Nxt_free[3] = (cuNum&0xff000000)>>24;
                CuiFat_WriteSingleBlock(SD_Dat.FSInfo,Buff0);
			}
			//如果未超过一个簇
			else
				{
					for(i=0;i<(lenth-(512-lenth_i));i++)
					{
						Buff0[i] = str[512-lenth_i+i];
					}
					CuiFat_WriteSingleBlock(ClubtoSector(cuNum)+lenth_j+1,Buff0);
				}
		}

		
		//写入目录项信息

		FstWriteData(fileInformation.i,lenth+fileInformation.FileSize,fileInformation.CuID);

	}
}


























