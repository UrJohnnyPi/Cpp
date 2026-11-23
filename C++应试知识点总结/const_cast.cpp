#include <iostream>
using namespace std;

int main(){
	const int a = 7; //注意这里的const
	const int * b = & a;
	int * c = const_cast<int *>(b); //const * -> *
	* c = 6;
	cout << * b << endl; //输出6 
	cout << a << endl; //输出7
	//这样的输出表明：b不再指向a这个常量，转而指向值为6的常量；通过const_cast得到的c能够修改b的指向常量。 
	
	const int & d = a;
	int & e = const_cast<int &>(d); //const & -> &
	e = 5;
	cout << d << endl; //输出5 
	cout << a << endl; //输出7
	//这样的输出表明：d不再绑定到a这个常量，转而绑定到值为6的常量；通过const_cast得到的e能够修改d的绑定到的常量。
	
	return 0;
}

/*
一、常量指针被转化成非常量的指针，并且仍然指向原来的对象；
二、常量引用被转换成非常量的引用，并且仍然指向原来的对象。 

综述：const_cast只接受pointer 和 references值，不接受obj本身。其作用不是消除pointer 和 references 所指向/绑定的const obj的const属性，而是消除pointer 和 references 
的const属性。pointer 和 references 不再指向/绑定原来的const obj，而是指向/绑定一个non-const但同值的obj。

即：消除pointer 和 references 的 const属性，并使之指向/绑定non-const同值对象。
*/

