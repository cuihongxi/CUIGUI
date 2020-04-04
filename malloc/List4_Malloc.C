#include "List4_Malloc.H"

list4Message list4mess = {0};     // 保存内存池信息
u8  list4mallocArray[MAX_MALLOCSIZE]  = {0} ;//__attribute__((at(0x20000000)))

/**
 *  初始化
 *  给一片内存作为动态申请区，堆
 */
void List4_Malloc_Init(list4Message* mess,u8* mallocAddr,List4Type length)
{
    mess->startAddr = mallocAddr;
    mess->endAddr = mallocAddr + length;
    mess->miniAddr = mallocAddr;
    mess->sizeStr = sizeof(list4Str);                  
    ((list4Str*)mallocAddr)->offset = length;
    ((list4Str*)mallocAddr)->blocksize = length;
//    debug("mess->startAddr = %#X\r\n",(u32)mess->startAddr);
//    debug("mess->endAddr = %#X\r\n",(u32)mess->endAddr);
//    debug("mess->endAddr - mess->startAddr = %d\r\n",(u32)(mess->endAddr - mess->startAddr));
}


//内存链表中按照大小顺序，插入一个块
void List4_InsertBlock(list4Message* mess,u8* addr)
{
    list4Str* current = (list4Str*)mess->miniAddr;  // 当前指向的内存区，该变量在后面会被迭代
    list4Str* thisaddr = (list4Str*)addr;           // 指向所要插入的内存
    list4Str* back = 0;                             // 迭代过程中的内存地址备份
    if(mess->miniAddr == mess->endAddr)
    {
        mess->miniAddr = addr;
        ((list4Str*)addr)->offset = mess->endAddr - mess->startAddr;
        return;
    }
    while(thisaddr->blocksize > current->blocksize)    // 寻找比这个块大的块地址
    {
        if((mess->startAddr + current->offset) == mess->endAddr) // 插入的块是最大的
        {
            current->offset = addr - mess->startAddr;
            thisaddr->offset =  mess->endAddr - mess->startAddr ;
            return;
        }
        back = current;
        current = (list4Str*)(mess->startAddr + current->offset);      
    }
    if((u8*)current == mess->miniAddr)
    {
        mess->miniAddr = (u8*)thisaddr;
        thisaddr->offset = (u8*)current - mess->startAddr;
    }else
    {
        thisaddr->offset = (u8*)current - mess->startAddr;
        back->offset = (u8*)thisaddr - mess->startAddr;
    }
}

/**
 *  内存申请
 */
void* List4_Malloc(list4Message* mess,List4Type length)
{
    list4Str* current = (list4Str*)mess->miniAddr;      // 当前指向的内存区，该变量在后面会被迭代
    list4Str* back = current;                           // 迭代过程中的内存地址备份
    
    if(mess->miniAddr == mess->endAddr) 
    {
        List4_Malloc_Log("内存满,无内存可以分配\r\n");
        return 0;                                       // 无内存块可申请
    }
    while(current->blocksize + mess->sizeStr < length)
    {
        back = current;
        current = (list4Str*)(mess->startAddr + current->offset);       // 找下一个内存块
        if((u8*)current == mess->endAddr)
        {
            List4_Malloc_Log("无内存可以分配\r\n");
            return 0;                                   // 找到最后
        } 
    } 
    back->offset =  current->offset;                    // current地址将被分配出去，back记录下一个地址，相当于将current指向的块从链表中取下   
    if(current->blocksize > length + mess->sizeStr)     // 该块被拆分成两部分，一部分分配掉，另一部分变成新块插入到链表中
    {
        //bakc做中间存储，填充新的块信息，并将其按照大小插入到链表
        back = (list4Str*)((u8*)current + mess->sizeStr + length);
        back->blocksize = current->blocksize - mess->sizeStr - length;
        if(mess->miniAddr == (u8*)current) 
        {
            mess->miniAddr = (u8*)back;
            back->offset = current->offset;
        }else  List4_InsertBlock(mess,(u8*)back); 
        current->blocksize = length;                    //记录该内存的大小
    }else   // 该块只够分配的
    {
        current->blocksize = length;                    //记录该内存的大小
        if(mess->miniAddr == (u8*)current) mess->miniAddr = mess->startAddr + current->offset;
    }
    return (void*)((u8*)current + mess->sizeStr);
}


/**
 *  内存释放
 */
void List4_Free(list4Message* mess,void* addr)
{
    list4Str* current = (list4Str*)mess->miniAddr;                  // 当前指向的内存区，该变量在后面会被迭代
    list4Str* thisaddr = (list4Str*)((u8*)addr - mess->sizeStr);    // 指向所要插入的内存 
    list4Str* back = current;                                       // 迭代过程中的内存地址备份    
    list4Str* pbefor = 0;                                           // 保存前块地址
    list4Str* pback = 0;                                            // 保存后块地址
    
    // 范围判断
    if((u8*)addr < (mess->startAddr + mess->sizeStr) || ((u8*)addr + thisaddr->blocksize) > mess->endAddr)
    {
        List4_Malloc_Log("释放错误，不在该内存池内,addr= %#X,thisaddr->blocksize = %d\r\n",(u32)addr,thisaddr->blocksize);
        return ;                   
    }
    while((u8*)current != mess->endAddr)        // 寻找在它之前有没有一个块
    {
        if(GETSTR_NEXTADDR(current) == (u8*)thisaddr)
        {
            //从链表摘下
            pbefor = current;
            if((list4Str*)mess->miniAddr == current) mess->miniAddr = mess->startAddr + current->offset;
            if(back != current) back->offset = current->offset;
            break;
        }
        back = current;
        current = (list4Str*)(mess->startAddr + current->offset);
    }

    // 指针复位
    current = (list4Str*)mess->miniAddr;
    back = current;
    while((u8*)current != mess->endAddr)        // 寻找在它之后有没有一个块
    {
        if(GETSTR_NEXTADDR(thisaddr) == (u8*)current)
        {
            //从链表摘下
            pback = current;
            if((list4Str*)mess->miniAddr == current) mess->miniAddr = mess->startAddr + current->offset;
            if(back != current) back->offset = current->offset;
            break;
        }
        back = current;
        current = (list4Str*)(mess->startAddr + current->offset); 
    }

    if(pbefor || pback)// 如果有,则合成新块
    {
        if(pback)
        {
            thisaddr->blocksize += pback->blocksize + mess->sizeStr;
        }
        if(pbefor)
        {
            pbefor->blocksize += thisaddr->blocksize + mess->sizeStr;
            thisaddr = pbefor;
        }
    }
    List4_InsertBlock(mess,(u8*)thisaddr);// 插入到链表中
}
