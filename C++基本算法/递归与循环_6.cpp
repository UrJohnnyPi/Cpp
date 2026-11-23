/*
雀魂启动！
https://www.nowcoder.com/questionTerminal/448127caa21e462f9c9755589a8f2416?orderByHotValue=1&page=1&onlyReference=false
*/
#include <iostream>
using namespace std;

bool if_success(int arr[] , int start , int length) {
	if(arr[start] >= 3) { //顺子操作
		arr[start] -= 3;

		if(if_success(arr , start , length)) {
			arr[start] += 3;
			return true;
		}

		arr[start] += 3;
	}

	if(start + 2 < 9 && arr[start] && arr[start + 1] && arr[start + 2]) { //刻子操作
		--arr[start];
		--arr[start + 1];
		--arr[start + 2];

		if(if_success(arr , start , length)) {
			++arr[start + 2];
			++arr[start + 1];
			++arr[start];
			return true;
		}

		++arr[start + 2];
		++arr[start + 1];
		++arr[start];
	}

	if(!arr[start] && start < length - 1)
		if(if_success(arr , start + 1 , length)) return true;

	if(!arr[start] && start == length - 1) return true;

	return false;
}

//启动函数
void bird_soul(int arr[]) {
	int result[9] = {};
	for(int i = 0 ; i < 9 ; ++i) {
		++arr[i]; //抽到的那张1~9

		if(arr[i] <= 4) {
			for(int j = 0 ; j < 9 ; ++j) { //去雀头
				if(arr[j] >= 2) {
					arr[j] -= 2;

					if(if_success(arr , 0 , 9)) result[i] = i + 1;

					arr[j] += 2;
				}
			}
		}

		--arr[i];
	}
	for(int i = 0 ; i < 9 ; ++i) arr[i] = result[i];
}

int main() {
	int arr_1[13];
	int arr_2[9] = {};
	for(int i = 0 ; i < 13 ; ++i) {
		cin >> arr_1[i];
		if(arr_1[i] > 9 || arr_1[i] < 1) return 0;
		++arr_2[arr_1[i] - 1];
	}
	bird_soul(arr_2);
	for(int i = 0 , count = arr_2[0] ; i < 9 ; count += arr_2[++i]) {
		if(arr_2[i]) cout << arr_2[i] << " ";
		if(i == 8 && count == 0) cout << 0;
	}

	return 0;
}
/*
（1）能写进启动函数的，就不写入递归体。比如这里的去雀头操作。 
（2）递归最最重要的就是抽象节点，写节点操作，然后传递关系给其他节点。这里抽象节点时
有困难，问题就在要视谁为节点。
（3）谨记递归的三明治结构。这里犯的错误就是：
bool fun(int v[]){
	++v[i];
	if(fun(v)) return true;
	--v[i]; 
}
因为操作的指针，这里在return前没有--v[i]，这里就导致返回的v[]已被修改。这是不正确的。
不论启动函数里是否还要使用v，都要把它改回原值。正确的写法：
bool fun(int v[]){
	++v[i];
	if(fun(v)){
		--v[i]; 
		return true;
	}
	--v[i]; 
}
*/
