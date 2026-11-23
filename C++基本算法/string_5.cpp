#include <iostream>
#include <string>
using namespace std;
int test_1(const char * p) {
	return sizeof(p);
}
int test_2(const char p []) {
	return sizeof(p);
}
int test_3(string p) {
	return sizeof(p);
}
int main() {
	char * p_1 = "I love you.";
	char p_2 [] = "I hate you.";
	string p_3 = "I forgive you."; 
	
	cout << sizeof(p_1) << ends << sizeof(p_2) << ends << sizeof(p_3) << endl;

	cout << test_1(p_1) << ends << test_1(p_2) << endl;

	cout << test_2(p_1) << ends << test_2(p_2) << endl;

	cout << test_3(p_1) << ends << test_3(p_2) << ends << test_3(p_3) << endl;

	/*
	输出结果：
	8 12 8
	8 8
	8 8
	8 8 8
	分析：已知因为指针的存储类型是unsigned long int，故在32位系统上，sizeof(指针)=4；
	在64位系统上，sizeof(指针)=8。sizeof()的度量是字节。 
	上述结果表明：
	（1）char * p_1和string p_3中p_1和p_3均是指针，这里还不能说是什么类型的指针，但不是数组指针；
	char p_2 []中p_2是数组指针，和p_1与p_3是有区别的。这里sizeof(p_2)输出12个字节，11个字符所占，
	1个'\0'。 
	（2）当数组作为函数的参数进行传递时，数组就自动退化为同类型的指针。 
	*/
	return 0;
}

/*
这里要注意的一个点还有string对象的赋值方式：
string str(const str); //const字符串赋值
string str(const char *); //一个C风格的const char *字符串的指针赋值
string str(char []); //一个C风格的char []字符串的数组名赋值

以第二种赋值方式举例：
const char * p = "UUUUU";
string str(p);
str[0] = 'a'; 
//这一步是正确的，因为你写的又不是const string str(p);，因此这里的str不是const。由string_6可知"UUUUU"是const，但str不是一个const ptr，一旦有str[0]='a';，则str指针将指向新的常量空间。
//不同于char str [] = "UUUUU";，据string_6可知char []指向的不是const空间。
//所谓const空间就是BSS段。BSS段通常是指用来存放程序中未初始化的或者初始化为0的全局变量和静态变量的一块内存区域。特点是可读写的，在程序执行之前BSS段会自动清0。
//可执行程序包括BSS段、数据段、代码段（也称文本段）。
//有一种写法：char * p = "UUUUU";，然后又有：* p = 'a'；这是不正确的。首先char * p = "UUUUU";本身就是不对的写法（但可以编译通过），因为字符串的三种正确表现方式：
string str(str);
const char * str = str;
char str [] = str;
//若用指针，必用const修饰。因为"UUUUU"要么视为一个const量（存储在const char * var/string var），要么视为一个数组量（存储在char var []）。而视为
//const量，就不能改动其值，只可以改变指针所指。
*/