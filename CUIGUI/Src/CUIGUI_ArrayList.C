#include "CUIGUI_ARRAYLIST.H"
#include "CUI_MALLCO.H"

/************************************************************************************
*-函数名称	：增加一个内容
*-参数	：
*返回值	：
*-函数功能	：
*-创建者	：蓬莱市博硕电子科技
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
*-函数名称	：移除一个内容
*-参数	：number 被移除的编号
*返回值	：
*-函数功能	：
*-创建者	：蓬莱市博硕电子科技
*/

static void ArrayListRemove(ArrayList* arrayList,u32 number)
{
      if(number<arrayList->size)
      {
            FreeCUI_Mallco((u8*)arrayList->List[number]); //释放对应的内存
            //将数组往前移动,并减少size            
            for(;number<arrayList->size;number++)
            arrayList->List[number] = arrayList->List[number+1]; 
            arrayList->size --;
      }
}      


/************************************************************************************
*-函数名称	：创建一个ArrayList数组
*-参数	：
*返回值	：数组指针
*-函数功能	：
*-创建者	：蓬莱市博硕电子科技
*/

ArrayList* NewArrayList(void)
{
      ArrayList* arrayList = (ArrayList*)CUI_Mallco(sizeof(ArrayList));
      
      arrayList->size = 0;
      arrayList->add = &ArrayListAdd;
      arrayList->remove = &ArrayListRemove;
      
      return arrayList; 
}









