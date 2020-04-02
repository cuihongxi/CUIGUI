#include "SingleList.h"

// ��������
SingleList*  NewSingleList(void)
{
	 SingleList* sl = (SingleList*)malloc(sizeof(SingleList));
	 sl->next = 0;
	 sl->num = 0;
	 return sl;
}

// ����β������ڵ�,�����²���Ľڵ��ַ
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

// �ڽڵ�֮ǰ�����½ڵ�,�����²���Ľڵ��ַ
SingleListNodeStr* SingleList_InsertBefore(SingleList* list, SingleListNode* objnode, SingleListNode* newnode)
{
	SingleList* current = list;
	SingleListNodeStr* new = (SingleListNodeStr*)malloc(sizeof(SingleListNodeStr));
	new->node = newnode;
    for(; current->next != 0;)
    {
		if(current->next == objnode)	// �ҵ��ڵ�
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


//�����Ƴ��ڵ����һ���ڵ�,���ͷŽڵ�
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
	free(back);
	list->num --;
    return (SingleListNode*)current;
}

//������
//�������һ���򷵻���һ��ָ�룬���򷵻� 0
SingleListNode* SingleList_Iterator(SingleListNode** node)
{
	if(*node) *node = ((SingleListNodeStr*)*node)->next;
	return ((SingleListNodeStr*)*node);
}

// �ͷ�����
void FreeSingList(SingleList* list)
{

	SingleListNode* node = (SingleListNode*)list;
	while(SingleList_Iterator(&node))
	{
		 SingleList_DeleteNode(list,node);
	}
	free(list);
}

//���ڵ��Ƶ������
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

