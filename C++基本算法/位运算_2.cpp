/*
题目：一个整型数组里除了两个数字之外，其他的数字都出现了两次。请写程序
找出这两个只出现一次的数字。要求时间复杂度是O(n)，空间复杂度是O(1)。
分析：采用位运算的方法。以{2,4,3,6,3,2,5,5}为例：
（1）2^4^3^6^3^2^5^5=4^6，^为异或运算。即连续异或得到的是那两个不同数字的异或。
（2）4^6=0010，对64位机而言，int占32bit，故这里一个0表示8bit。
（3）按照第三字节处为0还是为1，可以把{2,4,3,6,3,2,5,5}分为两个子数组。可以理解的是
0010中0的部分表示两个不同的数的二进制中相同的部分，1的部分表示个不同的数的二进制中
相异的部分。
（4）该例可得到{2,3,6,3,2}和{4,5,5}。即将两个单一的数分离到两个子数组中。此外，每个
数组中其余的部分都是成对的元素，因为两个相同的数字的位值相等，因而也就被分到了同一
数组中。
（5）每一子数组内部取异或：
2^3^6^3^2=6
4^5^5=4
这样就得到了这两个单一数字。
*/
#include <iostream>
using namespace std;
int * find_single(int arr[] , int len) {
	if(len < 4) return nullptr;

	int total_xor = arr[0];
	for(int i = 1 ; i < len ; ++i) total_xor ^= arr[i];

	int count = 0; //因为total_xor可能不止有一个1，所以要使得total_xor只含一个1。	cout << (total_xor & 1);
	for( ; (total_xor & 1) == 0 ; total_xor = total_xor >> 1) ++count;

	total_xor &= 1;
	while(count--) total_xor = total_xor << 1;

	int swi_fir = 0;
	int swi_sec = 0;
	int fir_xor; //fir_xor是i=0的数字的异或值
	int sec_xor; //sec_xor是i=1的数字的异或值
	for(int i = 0 ; i < len ; ++i)
		if((total_xor & arr[i]) == 0) { //格外注意：位运算符优先级特别低，不加括号就会先算==。所以判断语句中的位运算必须加括号。 
			if(swi_fir == 0) {
				fir_xor = arr[i];
				swi_fir = 1;
			} else fir_xor ^= arr[i];
		} else {
			if(swi_sec == 0) {
				sec_xor = arr[i];
				swi_sec = 1;
			} else sec_xor ^= arr[i];
		}

	int arr_single[] = {fir_xor , sec_xor};
	return arr_single;
}

int main() {
	int arr[] = {2,4,3,6,3,2,5,5};
	int * arr_single = find_single(arr , 8);
	cout << arr_single[0] << ends << arr_single[1];
	return 0;
}

/*
位运算出错，很大几率在于：
1.在判断语等句中，没有给位运算加括号。忽略了位运算符的低优先级。 
2.使用了不存在的增强符，如>>=。 
*/
