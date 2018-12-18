#include "CUIGUI_ARRAYLIST.H"
#include "CUI_MALLCO.H"

/************************************************************************************
*-��������	������һ������
*-����	��
*����ֵ	��
*-��������	��
*-������	�������в�˶���ӿƼ�
*/

static void ArrayListAdd(ArrayList* arrayList,u8* object)
{
     if(arrayList->size<ARRAY_LIST_LENGTH)
     {
           arrayList->List[arrayList->size] = (u32)object;
           arrayList->size ++;
     }
}      

/************************************************************************************
*-��������	���Ƴ�һ������
*-����	��number ���Ƴ��ı��
*����ֵ	��
*-��������	��
*-������	�������в�˶���ӿƼ�
*/

static void ArrayListRemove(ArrayList* arrayList,u32 number)
{
      if(number<arrayList->size)
      {
            FreeCUI_Mallco((u8*)arrayList->List[number]); //�ͷŶ�Ӧ���ڴ�
            //��������ǰ�ƶ�,������size            
            for(;number<arrayList->size;number++)
            arrayList->List[number] = arrayList->List[number+1]; 
            arrayList->size --;
      }
}      


/************************************************************************************
*-��������	������һ��ArrayList����
*-����	��
*����ֵ	������ָ��
*-��������	��
*-������	�������в�˶���ӿƼ�
*/

ArrayList* NewArrayList(void)
{
      ArrayList* arrayList = (ArrayList*)CUI_Mallco(sizeof(ArrayList));
      
      arrayList->size = 0;
      arrayList->add = &ArrayListAdd;
      arrayList->remove = &ArrayListRemove;
      
      return arrayList; 
}








