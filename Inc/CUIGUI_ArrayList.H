#ifndef  __CUIGUI_ARRAYLIST_H   
#define  __CUIGUI_ARRAYLIST_H

#include "CUIGUI_UDATA.H"

#define    ARRAY_LIST_LENGTH       50   //����һ������ĳ���


typedef struct ArrayList_struct
{
     u32 List[ARRAY_LIST_LENGTH];         //��ŵ�ַ
     u32 size;                            //�����ڵĸ���
     void(*add)(struct ArrayList_struct*,u8*);                     //����һ��
     void(*remove)(struct ArrayList_struct*,u32);                  //�Ƴ�һ��
}ArrayList;


ArrayList* NewArrayList(void);

#endif
