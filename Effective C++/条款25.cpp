/*
条款25：考虑写出一个不抛异常的swap函数
*/

/*
代码1：

#include <iostream>
using namespace std;
int main(){
	int a = 0 , b = 1;
	swap(a , b); //来自STL、std中定义好的swap()
	cout << a << b;
	return 0;
}

//std中定义的swap()的代码实现：
namespace std {
	template <typename T>
	void swap(T & a , T & b){
		T temp(a); //copy构造函数
		a = b; //copy assignment
		b = temp; //copy assignment
	}
}

//如果有以下这样的类，swap()仍旧要执行三次对象值的交换，而不是直接交换数据成员ptr，因此缺乏效率：
template<typename T>
class Widget{
	public :
		Widget(const Widget & rhs):ptr(rhs.ptr){
		}
		Widget & operator = (const Widget & rhs){ //条款10、条款11
			T * temPtr = ptr;
			ptr = new T (* rhs.ptr);
			delete temPtr;
			return * this;
		}
	private :
		T * ptr;
};
*/

/*
代码2：

#include <iostream>
using namespace std;
class Widget{
	public :
		//std::swap调用的copy函数部分
		Widget(int * ptr):ptr(ptr){
		}
		Widget(const Widget & rhs):ptr(rhs.ptr){
		}
		Widget & operator = (const Widget & rhs){
			int * temPtr = ptr;
			ptr = new int (* rhs.ptr);
			delete temPtr;
			return * this;
		}

		//std::swap的全特化版本使用的成员函数
		void swap(Widget & b){
			using std::swap;
			swap(ptr , b.ptr);
		}

		//验证是否交换
		void getVal(){
			cout << * ptr;
		}
	private :
		int * ptr;
};

//在std中定义了std::swap的全特化版本，全特化就是说：在std空间中，一般情况下，swap还是原swap，
//一旦用到交换两个Widget身上，就会调用这里的全特化版本。
namespace std{
	//全特化代码起始
	template<>
	void swap<Widget>(Widget & a , Widget & b){
		a.swap(b);
	}
	//全特化代码结束
}

int main(){
	int av = 1;
	int bv = 2;
	Widget a(&av);
	Widget b(&bv);
	swap(a , b);
	a.getVal(); //2
	b.getVal(); //1
	return 0;
}
*/

/*
代码3：

//代码2中，若是对ptr进行泛化。将Widget改为template<typename T> Widget{ private : T * ptr};
//C++对泛化模板的特化称为偏特化。且只允许对class template 偏特化，不能对function template偏特化。
//意即，若对Widget泛化，这里的swap便不再能够被特化（全特化、偏特化），而只能通过重载解决问题。如下：
namespace WidgetStuff{
	template<typename T>
	void swap(Widget<T> & a , Widget<T> & b){
		a.swap(b);
	}
};
//swap的重载也不能定义在std空间中。因为C++不允许添加新的template到std中。所以这里就定义在
//WidgetStuff中。总的代码如下： 

#include <iostream>
using namespace std;
namespace WidgetStuff {
	template <typename T>
	class Widget {
		public :
			Widget(T * ptr):ptr(ptr) {
			}
			Widget(const Widget & rhs):ptr(rhs.ptr) {
			}
			Widget & operator = (const Widget & rhs) {
				T * temPtr = ptr;
				ptr = new int (* rhs.ptr);
				delete temPtr;
				return * this;
			}

			//std::swap的重载版本使用的成员函数
			void swap(Widget & b) {
				using std::swap;
				swap(ptr , b.ptr);
			}

			//验证是否交换
			void getVal() {
				cout << * ptr;
			}
		private :
			T * ptr;
	};

	template<typename T>
	void swap(Widget<T> & a , Widget<T> & b) {
		a.swap(b);
	}
};
using namespace WidgetStuff;

int main(){
	int av = 1;
	int bv = 2;
	Widget<int> a(&av);
	Widget<int> b(&bv);
	swap(a , b);
	a.getVal(); //2
	b.getVal(); //1
	return 0;
}
*/

/*
模板的全特化和偏特化：
网站：https://zhuanlan.zhihu.com/p/72356532

模板为什么要特化，因为编译器认为，对于特定的类型，如果你能对某一功能更好的实现，那么就该听你的。

模板分为类模板与函数模板，特化分为全特化与偏特化。全特化就是限定死模板实现的具体类型，偏特化就是如果这个模板有多个类型，那么只限定其中的一部分。

先看类模板：

template<typename T1, typename T2>
class Test
{
public:
	Test(T1 i,T2 j):a(i),b(j){cout<<"模板类"<<endl;}
private:
	T1 a;
	T2 b;
};
 
template<>
class Test<int , char>
{
public:
	Test(int i, char j):a(i),b(j){cout<<"全特化"<<endl;}
private:
	int a;
	char b;
};
 
template <typename T2>
class Test<char, T2>
{
public:
	Test(char i, T2 j):a(i),b(j){cout<<"偏特化"<<endl;}
private:
	char a;
	T2 b;
};

那么下面3句依次调用类模板、全特化与偏特化：

Test<double , double> t1(0.1,0.2);
Test<int , char> t2(1,'A');
Test<char, bool> t3('A',true);
而对于函数模板，却只有全特化，不能偏特化：

//模板函数
template<typename T1, typename T2>
void fun(T1 a , T2 b)
{
	cout<<"模板函数"<<endl;
}
 
//全特化
template<>
void fun<int ,char >(int a, char b)
{
	cout<<"全特化"<<endl;
}
 
//函数不存在偏特化：下面的代码是错误的

template<typename T2>
void fun<char,T2>(char a, T2 b)
{
	cout<<"偏特化"<<endl;
}
至于为什么函数不能偏特化，似乎不是因为语言实现不了，而是因为偏特化的功能可以通过函数的重载完成。
*/