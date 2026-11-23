/*
条款05：了解C++默默编写并调用哪些函数

要注意：public不是函数成员的私人领域，private也不是数据成员的私人领域。
public下写的数据成员，如若是const，也必须要在构造函数中初始化，即类中的
数据成员仅因位于public、protected或private下而产生继承性的区别，其他性质
相同。
同理，private也可写的函数成员。
*/

/*
代码1：

//你定义的空类
class Empty {};

//编译器为你实际生成的，其中所有的函数都是public和inline的。
//并不预先创建，被需要时才创建。
//自己写了的，也不被创建。
class Empty {
	public :
		Empty() { //default 构造函数
			//若数据成员中有const或references，则必须重写构造函数以初始化该const成员。
			//若数据成员中有类的对象，则调用其构造函数。
			//若该成员是内置基本类型或static，则不予初始化。
		}
		Empty(const Empty & rhs) { //copy 构造函数
			//对于rhs的每一non-static数据成员：
			//若该成员是类的对象（或STL容器对象），则调用其copy 构造函数；
			//若该成员是内置基本类型，则复制。
		}
		~Empty() { //析构函数，这里注意编译器产生的是non-virtual析构
			//maybe something
		}
		Empty & operator = (const Empty & rhs) { //copy assignment操作符
			//maybe something
		}
};
*/

/*
代码2：

class Empty {
	public :
		Empty(string & nameValue , const int & objectValue);
	private :
		string & nameValue;
		const int objectValue;
};
string str_1("jack");
string str_1("herry");
Empty a(str_1 , 1);
Empty b(str_2 , 2);
a = b;
//这里会直接报错，因为你无法将一个引用赋值给另一个引用；无法将一个const赋值给另一个const。
//因此，编译器不会为你自动生成一个copy assignment函数。
//编译器自动生成copy assignment函数的要求是：未自定义copy assignment函数；（包括public、protected、private）数据成员中无const和references。
//注意：int a = 4; int & b = a; int & c = b;这是正确的，之所以说一个引用无法赋值给另一个引用，是因为这样做不满足封闭性，即深复制和浅复制的问题。 

//还有，若base class将copy assignment声明为private，编译器将不会为derived class自动生成
//copy assignment。因为例如：
class Base {
	private :
		Base & operator = (const Base & rhs) {
			//do something
		}
};
class Derived : public Base{
	//do something
};
Derived der_1;
Derived der_2;
der_1 = der_2;
//这里会报错，因为编译器没有为Derived生成copy assignment函数。
//假设编译器生成了operator = ，当这里调用=时，Derived赋值完
//派生部分（即回字构造法外围部分）的成员后，是无法调用base中的
//private operator = 来为内围部分成员赋值的。  
*/
