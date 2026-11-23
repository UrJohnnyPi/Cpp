/*
题目：假设把某股票的价格按照时间先后顺序存储在数组中，请问买卖交易该股
票可能获得的利润是多少？例如一只股票在某些时间节点的价格为{9, 11, 8, 5,
7, 12, 16, 14}。如果我们能在价格为5的时候买入并在价格为16时卖出，则能
收获最大的利润11。
分析：利用选择排序的思路最好。
sale_ptr指向9时，min指向9。存储profit=0。 
sale_ptr指向11时，9<11，min仍指向9。存储profit=2。 
sale_ptr指向8时，min指向8。存储profit=0。
以此类推。 
*/
#include <iostream>
#include <set>
using namespace std;

template<typename T>
int max_profit(T arr[] , int length){
	multiset<T> profit;
	for(int i = 0 , min = 0 ; i < length ; ++i){
		if(arr[min] > arr[i]) min = i;
		profit.insert(arr[i] - arr[min]);
	}
	auto it = profit.end();
	return * (--it);
}

int main(){
	int arr[] = {9, 11, 8, 5, 7, 12, 16, 14};
	cout << max_profit(arr , 8);
	return 0;
}
