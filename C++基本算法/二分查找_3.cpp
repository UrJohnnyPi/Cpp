/*
题目：统计一个数字在排序数组中出现的次数。例如输入排序数组{1, 2, 3, 3,
3, 3, 4, 5}和数字3，由于3在这个数组中出现了4次，因此输出4。
分析：有序序列中找某数，就是二分查找的范畴。 
*/
#include <iostream>
using namespace std;

int binSearch(const int * arr , int start , int end , int key){
	int low = start , high = end , mid , count = 0;
	while(low <= high){
		mid = low + (high - low) / 2;
		if(key == arr[mid]){
			++count;			
			for(int i = mid - 1 ; i >= low && arr[i] == key ; --i) ++count;
			for(int i = mid + 1 ; i <= high && arr[i] == key ; ++i) ++count;
			return count;
		}
		if(key > arr[mid]) low = mid + 1;
		if(key < arr[mid]) high = mid - 1;
	}
	return 0;
} 

int main(){
	int arr[] = {1, 2, 3, 3, 3, 3, 4, 5};
	cout << binSearch(arr , 0 , 7 , 5);
	return 0;
}
/*
题目：一个长度为n-1的递增排序数组中的所有数字都是唯一的，并且每个数字
都在范围0到n-1之内。在范围0到n-1的n个数字中有且只有一个数字不在该数组
中，请找出这个数字。

题目：假设一个单调递增的数组里的每个元素都是整数并且是唯一的。请编程实
现一个函数找出数组中任意一个数值等于其下标的元素。例如，在数组{-3, -1,
1, 3, 5}中，数字3和它的下标相等。
*/
