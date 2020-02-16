/**
 * 
 * 单链表
*/

#ifndef _SingleList_H_
#define _SingleList_H_

#include "uhead.h"


typedef void SingleListNode;
typedef void listType;
typedef struct 
{
	listType* next;
    unsigned int num;				// 链表节点个数
}SingleList;


typedef struct 
{	
	listType* next;
	SingleListNode* node;

}SingleListNodeStr;

SingleListNode* SingleList_DeleteNode0(SingleList* list, SingleListNode* node);
SingleList* NewSingleList(void);											// 创建新链表
unsigned int SingleList_InsertEnd(SingleList* list, SingleListNode* node);	// 向链表添加节点
unsigned int SingleList_InsertEnd0(SingleList* list, SingleListNode* node);	// 向链表添加节点
SingleListNodeStr* SingleList_InsertBefore(SingleList* list, SingleListNode* objnode, SingleListNode* newnode);// 在节点之前插入新节点,返回新插入的节点地址
SingleListNode* SingleList_DeleteNode(SingleList* list, SingleListNode* node);	// 删除节点并释放空间
SingleListNode* SingleList_Iterator(SingleListNode** node);					// 迭代器
void FreeSingList(SingleList* list);										// 释放链表
void SingleList_MoveEndNode(SingleList* list, SingleListNode* node);		// 将节点移到链表末尾
#define		SingeListGetnode(type,nodehead)		((type*)(((SingleListNodeStr*)nodehead)->node))





#endif
