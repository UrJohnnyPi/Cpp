/*
题目：在数组中的两个数字如果前面一个数字大于后面的数字，则这两个数字组成一个逆序对。
输入一个数组，求出这个数组中的逆序对的总数。
*/
#include <iostream>
using namespace std;

template<class ElemType>
void Merge(ElemType A[] , ElemType B[] , int low , int mid , int high , int & Reverse_order){
	for(int i = low ; i <= high ; ++i) B[i] = A[i];
	
	int i = low , j = mid + 1 , k = low;
	for( ; i <= mid && j <= high ; ++k)
	if(B[i] <= B[j]) A[k] = B[i++];
	else {
		Reverse_order += (mid - i + 1);
		A[k] = B[j++];
	}
	while(i <= mid) A[k++] = B[i++];
	while(j <= high) A[k++] = B[j++];
}

template<class ElemType>
void MergeSort(ElemType A[] , ElemType B[] , int low , int high , int & Reverse_order){
	if(low < high){
		int mid = (low + high) / 2;
		MergeSort(A , B , low , mid , Reverse_order);
		MergeSort(A , B , mid + 1 , high , Reverse_order);
		Merge(A , B , low , mid , high , Reverse_order);
	}
}

int main(){
	int A[6] = {7,5,6,4,3,1}; // 逆序数=14 
	int B[6];
	int Reverse_order = 0;
	MergeSort(A , B , 0 , 5 , Reverse_order);
	cout << Reverse_order;
	return 0;
}
