#include "Simple_Malloc.H"

// 内存申请初始化
// 申请一片内存作为局部内存
// 返回申请的内存地址
void* Simple_Malloc_Init(Simple_MallocStr* mallocMesg , u32  maxlength)
{
	mallocMesg->array = SIMPLE_MALLOC_FUN(maxlength);
    mallocMesg->size = maxlength;
    mallocMesg->addr = (u8*)mallocMesg->array;
    return (void*)mallocMesg->array;
}

// 内存申请
void* Simple_Malloc(Simple_MallocStr* mallocMesg , u32  leng_th)
 {
	 
 	if((mallocMesg->addr + leng_th) <= ((u8*)mallocMesg->array + mallocMesg->size))
	{
		u8* addr = mallocMesg->addr;
		mallocMesg->addr += leng_th;
		return (void*)addr;
	}else
	{
		Simple_Malloc_Log("mlloc0 超出范围，内存申请失败！！！\r\n");
		return 0;
	} 
 }
