#include <iostream>
#include <list> //STL中的双向链表 
using namespace std;

int main(){
	list<int> lis_1; //（1）初始化
	list<int> lis_2(10); //定义10个元素
	list<int> lis_3(10 , 5); //定义10个元素，初始化为5。
	
	/*
	lis.empty();
	lis.begin();
	lis.end();
	lis.size();
	lis.front();
	lis.back();
	lis.push_front();
	lis.pop_front();
	lis.push_back();
	lis.pop_back();
	lis.insert();
	lis.erase(start_ptr , len);
	lis.clear();
	lis.swap(lis_1);
	lis.merge(lis_1);//合并两个事先已排好序的 list 容器，并且合并之后的 list 容器依然是有序的。
	lis.sort(); //通过更改容器中元素的位置，将它们进行排序。
	lis.reverse(); //反转 
	*/ 
	
	return 0;
} 
