/*
条款02：尽量以const,enum,inline替换#define
（1）宁可以编译器替换预处理器。
（2）取代#define要注意的点：
A.
宏变量全部大写，const常量则是每一单词的首元大写。 

B.
常量指针：const char * ptr = "I love you"; 指向常量的指针。 
指针常量：const char * const ptr = "I love you";指针类型的常量。 
以const std::string ptr("I love you");替代char * const ptr = "I love you";更妥。

C.
static const相关：
写法一（不推荐）： 
class A {
	private :
		static const int a = 5; //声明式，a在声明时获得初值。不合情理，可能出问题。 
};
const int A::a; //不能缺少的定义式，因声明式中已赋值，这里就不能再次赋值。

写法二（推荐）：
class A {
	private :
		static const int a; //声明式，用于声明a是A的专属（static）常量（const）。 
};
const int A::a = 5; //定义式，定义式获得初值称为初始化，合情理。

由此，类的建议写法：
class A {
	public :
		A();
		~A();
};
A::A(){}
A::~A(){}
即定义式out class，声明式in class。

注意：编译器在遇到定义式之前是不知道其内部细节的，声明只是告知编译器它是一个什么类型的变量。
比如：
class A {
	private :
		static const int a;
		int b[a]; //错误，编译器此时只知道a是static const，因而只能进行一些针对
				  //a的static const约束操作，而不能利用其除static const以外的性质，比如它的值。 
};
const int A::a = 5; 
此种情形下，使用“the enum hack”：
class A {
	private :
		enum {a = 5}; 
		int b[a];
};

D.
使用template inline函数替代宏定义函数。
比如：
用
template<typename T>
inline T callWithMax(const T & a , const T & b){
	return (a > b ? a : b);
}
替代
#define CALL_WITH_MAX(a,b)((a)>(b)?(a):(b))
template inline具有和#define一样强大的泛化、效率能力。 
*/
#include <iostream>
#define CALL_WITH_MAX(a,b)((a)>(b)?(a):(b))

using namespace std;

int main(){
	char a = 'a' , b = 'b';
	cout << CALL_WITH_MAX(a,b);
	return 0;
} 
