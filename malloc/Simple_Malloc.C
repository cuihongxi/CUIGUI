#include "Simple_Malloc.H"

// �ڴ������ʼ��
// ����һƬ�ڴ���Ϊ�ֲ��ڴ�
// ����������ڴ��ַ
void* Simple_Malloc_Init(Simple_MallocStr* mallocMesg , u32  maxlength)
{
	mallocMesg->array = SIMPLE_MALLOC_FUN(maxlength);
    mallocMesg->size = maxlength;
    mallocMesg->addr = (u8*)mallocMesg->array;
    return (void*)mallocMesg->array;
}

// �ڴ�����
void* Simple_Malloc(Simple_MallocStr* mallocMesg , u32  leng_th)
 {
	 
 	if((mallocMesg->addr + leng_th) <= ((u8*)mallocMesg->array + mallocMesg->size))
	{
		u8* addr = mallocMesg->addr;
		mallocMesg->addr += leng_th;
		return (void*)addr;
	}else
	{
		Simple_Malloc_Log("mlloc0 ������Χ���ڴ�����ʧ�ܣ�����\r\n");
		return 0;
	} 
 }
