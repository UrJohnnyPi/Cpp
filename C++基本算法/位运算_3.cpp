/*
题目：在一个数组中除了一个数字只出现一次之外，其他数字都出现了三次。
请找出那个只出现一次的数字。
分析：以{1,1,1,2,2,2,4,4,4,6}为例：
像1,2,4这些出现三次的的数字，相同位相加，和必能整除3。即所有这些数的相同位的和%3=0。
所有数的相同位和%3，若等于0，则只出现一次的数字的该位为0，反之为1。
这就要求要构建一个arr[32]用于存放序列的每位和。 
*/
#include <iostream>
#include <math.h>
using namespace std;

int find_once(int arr[] , int length){
	int bit[32] = {};
	for(int i = 0 ; i < length ; ++i){
		int elem = arr[i];
		for(int j = 0 ; j < 32 ; ++j , elem = elem >> 1) bit[j] += (elem & 1); //elem低位对应bit[]低位 
	}
	for(int i = 0 ; i < 32 ; ++i) bit[i] = bit[i] % 3;
	
	//二进制转十进制
	int dec = 0;
	for(int i = 0 ; i < 32 ; ++i) dec += bit[i] * pow(2 , i);
	return dec;
}

int main(){
	int arr[] = {1,4,1,6,2,1,2,4,6,2,6,4,5};
	cout << find_once(arr , 13);
	return 0;
} 
