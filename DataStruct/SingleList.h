/**
 * 
 * ������
 * 
 * ������ڵ�ʹ��SingleList_InsertBeforeʱ���൱��ջ������
 * ������ڵ�ʹ��SingleList_InsertEnd���൱�ڶ��в���
*/

#ifndef _SingleList_H_
#define _SingleList_H_

#include "uhead.h"


typedef void SingleListNode;
typedef void listType;
typedef struct 
{
	listType* next;
    unsigned int num;				// ����ڵ����
}SingleList;


typedef struct 
{	
	listType* next;
	SingleListNode* node;

}SingleListNodeStr;



SingleList* NewSingleList(void);											// ����������
unsigned int SingleList_InsertEnd(SingleList* list, SingleListNode* node);	// ��������ӽڵ�
SingleListNodeStr* SingleList_InsertBefore(SingleList* list, SingleListNode* objnode, SingleListNode* newnode);// �ڽڵ�֮ǰ�����½ڵ�,�����²���Ľڵ��ַ
SingleListNode* SingleList_DeleteNode(SingleList* list, SingleListNode* node);	// ɾ���ڵ㲢�ͷſռ�
SingleListNode* SingleList_Iterator(SingleListNode** node);					// ������
void FreeSingList(SingleList* list);										// �ͷ�����
void SingleList_MoveEndNode(SingleList* list, SingleListNode* node);		// ���ڵ��Ƶ�����ĩβ
#define		SingeListGetnode(type,nodehead)		((type*)(((SingleListNodeStr*)nodehead)->node))




#endif
