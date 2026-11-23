#include <iostream>
using namespace std;

template<class ElemType>
void Merge(ElemType A[] , ElemType B[] , int low , int mid , int high) {
	for(int i = low ; i <= high ; ++i) B[i] = A[i];
	
	int i = low , j = mid + 1 , k = low; //i、j是中间数组B[]的指针；k是A[]的指针 
	for( ; i <= mid && j <= high ; ++k)
		if(B[i] <= B[j]) A[k] = B[i++];
		else  A[k] = B[j++];
	while(i <= mid) A[k++] = B[i++];
	while(j <= high) A[k++] = B[j++];
}

template<class ElemType>
void MergeSort(ElemType A[] , ElemType B[] , int low , int high) {
	if(low < high) {
		int mid = (low + high) / 2;
		MergeSort(A , B , low , mid); //归并mid左侧 
		MergeSort(A , B , mid + 1 , high); //归并mid右侧 
		Merge(A , B , low , mid , high); //将左右排序合并 
	}
}

int main() {
	int A[] = {49 , 38 , 65 , 97 , 76 , 13 , 27}; //待排数组 
	int * B = new int[7]; 
	MergeSort(A , B , 0 , 6);
	for(int i = 0 ; i < 7 ; ++i) cout << A[i] << ends;
	return 0;
}
