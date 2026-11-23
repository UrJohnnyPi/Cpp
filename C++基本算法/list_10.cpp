/*
题目：输入两个链表，找出它们的第一个公共结点。
分析：例如 
a->b->c
       \
         f->g->h
       /
   d->e
list_1.length - list_2.length = d； 
令ptr_1先在list_1上走过d；
令ptr_1和ptr_2同时走，直到二者指向同一元素，即为f。 
*/
#include <iostream>
using namespace std;

typedef struct ListNode {
	int data;
	ListNode *nextPtr;
} LNode , *ListPtr;

//尾插法 
void addToTail(ListPtr &L , int data) { //没有头结点
	ListPtr nodePtr = new LNode();
	nodePtr->data = data;
	nodePtr->nextPtr = nullptr;
	if(!L) L = nodePtr;
	else {
		ListPtr ptr = L;
		for( ; ptr->nextPtr ; ptr = ptr->nextPtr);
		ptr->nextPtr = nodePtr;
	}
}

//找公共节点 
void find_public_node(ListPtr & L1 , ListPtr & L2){
	ListPtr L1Ptr = L1 , L2Ptr = L2;
	int L1Len = 1 , L2Len = 1;
	for( ; L1Ptr->nextPtr ; L1Ptr = L1Ptr->nextPtr , ++L1Len);
	for( ; L2Ptr->nextPtr ; L2Ptr = L2Ptr->nextPtr , ++L2Len);
	if(L1Len > L2Len) for(L1Ptr = L1 , L2Ptr = L2 ; L1Ptr->nextPtr && L1Len > L2Len; L1Ptr = L1Ptr->nextPtr , --L1Len);
	if(L1Len < L2Len) for(L2Ptr = L2 , L1Ptr = L1 ; L2Ptr->nextPtr && L1Len < L2Len; L2Ptr = L2Ptr->nextPtr , --L2Len);
	for( ; L1Ptr->nextPtr && L2Ptr->nextPtr && L1Ptr != L2Ptr ; L1Ptr = L1Ptr->nextPtr , L2Ptr = L2Ptr->nextPtr);
	cout << L1Ptr->data;
}

int main(){
	ListPtr L1 = nullptr;

	addToTail(L1,7);
	addToTail(L1,9);
	addToTail(L1,17);
	addToTail(L1,69); //交叉处 
	addToTail(L1,18);
	addToTail(L1,39);
	
	ListPtr L2 = nullptr;
	
	addToTail(L2,7);
	addToTail(L2,9);
	L2->nextPtr->nextPtr = L1->nextPtr->nextPtr->nextPtr;
	
	find_public_node(L1 , L2);
} 
