
#include "SingleList.h"
#include "malloc.h"


// 创建链表
SingleList*  NewSingleList(void)
{
	 SingleList* sl = (SingleList*)malloc(sizeof(SingleList));
	 sl->next = 0;
	 sl->num = 0;
	 return sl;
}

// 链表尾部插入节点,返回新插入的节点地址
unsigned int  SingleList_InsertEnd(SingleList* list, SingleListNode* node)
{ 
    SingleList* current = list;

    SingleListNodeStr* newnode = (SingleListNodeStr*)malloc(sizeof(SingleListNodeStr));
	newnode->next = 0;
	newnode->node = node;

    for(; current->next != 0;)
    {
        current = current->next;
    }
	current->next = newnode;     
    list->num ++;
	//debug("newnode addr = %#X\r\n",(u32)newnode);
    return (unsigned int)newnode;
}

unsigned int  SingleList_InsertEnd0(SingleList* list, SingleListNode* node)
{ 
    SingleList* current = list;

    SingleListNodeStr* newnode = (SingleListNodeStr*)malloc(sizeof(SingleListNodeStr));
	newnode->next = 0;
	newnode->node = node;

    for(; current->next != 0;)
    {
        current = current->next;
    }
	current->next = newnode;     
    list->num ++;
	debug("newnode addr = %#X\r\n",(u32)newnode);
    return (unsigned int)newnode;
}

// 在节点之前插入新节点,返回新插入的节点地址
SingleListNodeStr* SingleList_InsertBefore(SingleList* list, SingleListNode* objnode, SingleListNode* newnode)
{
	SingleList* current = list;
	SingleListNodeStr* new = (SingleListNodeStr*)malloc(sizeof(SingleListNodeStr));
	new->node = newnode;
    for(; current->next != 0;)
    {
		if(current->next == objnode)	// 找到节点
		{
			current->next = new;
			new->next = objnode;
			break;	
		}
        current = current->next;
		list->num ++;
    }
	return new;	
}


//返回移除节点的上一个节点,并释放节点
SingleListNode* SingleList_DeleteNode(SingleList* list, SingleListNode* node)
{
	SingleList* current = list;
	void* back;
    for(;current->next != 0; )
    {
        if( ((SingleListNodeStr*)(current->next))->node == node ) break;
        current = current->next;
    }
	if(current->next == 0) return (SingleListNode*)current;
	back =  current->next;
    current->next = ((SingleListNodeStr*)(current->next))->next;
	//debug("back addr = %#X\r\n",(u32)back);
	//free(back);
	list->num --;
    return (SingleListNode*)current;
}
SingleListNode* SingleList_DeleteNode0(SingleList* list, SingleListNode* node)
{
	SingleList* current = list;
	void* back;
    for(;current->next != 0; )
    {
        if( ((SingleListNodeStr*)(current->next))->node == node ) break;
        current = current->next;
    }
	if(current->next == 0) return (SingleListNode*)current;
	back =  current->next;
    current->next = ((SingleListNodeStr*)(current->next))->next;
	debug("back addr = %#X\r\n",(u32)back);
	//free(back);
	list->num --;
    return (SingleListNode*)current;
}
//迭代器
//如果有下一个则返回下一个指针，否则返回 0
SingleListNode* SingleList_Iterator(SingleListNode** node)
{
	if(*node) *node = ((SingleListNodeStr*)*node)->next;
	return ((SingleListNodeStr*)*node);
}

// 释放链表
void FreeSingList(SingleList* list)
{

	SingleListNode* node = (SingleListNode*)list;
	while(SingleList_Iterator(&node))
	{
		 SingleList_DeleteNode(list,node);
	}
	free(list);
}

//将节点移到最后面
void SingleList_MoveEndNode(SingleList* list, SingleListNode* node)
{

    SingleList* current = list;
	void* back;
	unsigned char i = 0;
    for(;current->next != 0; )
    {
		i++;
		debug("i = %d\r\n",i);
        if( ((SingleListNodeStr*)(current->next))->node == node )
        	{
        		break;
        	}
        current = current->next;
    }
	 if(current->next == 0)return;
	if(((SingleListNodeStr*)(current->next))->next)
	{
	
		back = current->next;

		current->next = ((SingleListNodeStr*)(current->next))->next;
	    for(; current->next != 0;)
	    {
	        current = current->next;
	    }

	    current->next = back;
	    ((SingleList*)back)->next = 0;
	}
}

