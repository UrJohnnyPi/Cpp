/*
题目：在字符串中找出第一个只出现一次的字符。如输入"abaccdeff"，则输出'b'。
分析：
要使用统计+遍历的方式，但问题在于统计，如果不能采用hash方式统计，那么时间开销大。这里采用的
hash方法是：hash(ASCII-97)=count；之后遍历str，直到hash(int(str[i])-97)=1,return str[i]。
可以使用unordered_map、自建hash。
*/
#include <iostream>
#include <unordered_map>
using namespace std;

char find_char(const char * ch , int len) {
	unordered_map<char , int> fc;
	for(int i = 0 ; i < len ; ++i)
		if(fc.find(ch[i])!=fc.end())
			++fc[ch[i]];
		else fc.emplace(ch[i] , 1);
	for(int i = 0 ; i < len ; ++i)
		if(fc[ch[i]] == 1) return ch[i];
	return '\0';
}

int main() {
	const char * ch = "azbacacdeff";
	cout << find_char(ch , 10) << endl;
	return 0;
}
