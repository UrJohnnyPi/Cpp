/*
题目：输入一个递增排序的数组和一个数字s，在数组中查找两个数，使得它们
的和正好是s。如果有多对数字的和等于s，输出任意一对即可。
分析：以{1,2,4,7,11,15}为例：
（1）因为任何一个数都是大数+小数。故可设置大数指针big_ptr指向有序序列的最大值15。
设置小数指针lit_ptr指向有序序列的最小值1。
（2）比对arr[big_ptr]+arr[lit_ptr]与key。
若arr[big_ptr]+arr[lit_ptr]=key，则返回{big_ptr，lit_ptr} 。
若arr[big_ptr]+arr[lit_ptr]>key，则--big_ptr。 
若arr[big_ptr]+arr[lit_ptr]<key，则++lit_ptr。
若big_ptr==lit_ptr，返回error。 
*/
#include <iostream>
using namespace std;

int * find_pair(int arr[] , int length , int key){
	int lit_ptr = 0 , big_ptr = length - 1;
	for( ; lit_ptr != big_ptr && arr[lit_ptr] + arr[big_ptr] != key ; (arr[lit_ptr] + arr[big_ptr] < key ? ++lit_ptr : --big_ptr));
	if(lit_ptr == big_ptr) return nullptr;
	else {
		int pair[] = {lit_ptr , big_ptr};
		return pair;
	}
}

int main(){
	int arr[] = {1,2,4,7,11,15};
	int * pair = find_pair(arr , 6 , 15);
	if(pair) cout << pair[0] << ends << pair[1];
	else cout << "none";
	return 0;
} 
