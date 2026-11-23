#include <iostream>
using namespace std;
class base1 {
public :
	virtual void d() {
		cout << "base1::d"<<endl;
	}
};
class base2:public base1 {
public :
	void d() {
		cout << "base2::d"<<endl;
	}
};
int main() {
	//基类强转为派生类_1
	cout << "(1)" << endl;
	base1 b;
	base1 *pb = &b;
	pb->d(); //base1::d 
	static_cast<base2 *>(pb)->base1::d(); //base1::d 注意 
	dynamic_cast<base2 *>(pb)->base2::d(); //base2::d 注意 

	//基类强转为派生类_2
	cout << "(2)" << endl;
	base2 c;
	base1 *pc = &c;
	pc->d(); //base2::d 注意
	pc->base1::d();
	dynamic_cast<base2 *>(pc)->d(); //base2::d 
	dynamic_cast<base2 *>(pc)->base1::d(); //base1::d 

	//派生类强转为基类
	cout << "(3)" << endl;
	base2 d;
	base2 *pd = &d;
	pd->d(); //base2::d 
	dynamic_cast<base1 *>(pb)->d(); //base1::d，因为static_cast返回的是pd指向空间的base1部分的副本，这个副本空间不包含外部的base2部分，因此虚函数不存在。
	static_cast<base1 *>(pb)->d(); //base1::d
	//static_cast<base1 *>(pb)->base2::d();错误 
	
	//先下后上_1
	cout << "(4)" << endl;
	base2 e;
	base1 * f = &e;
	base2 * g = static_cast<base2 *>(f);
	g->d(); 
	static_cast<base1 *>(g)->d();
	
	//先下后上_2
	base1 dd;
	base1 * pdd_1 = &dd;
	base2 * pdd_2 = static_cast<base2 *>(pdd_1); //下行转换
	static_cast<base1 *>(pdd_2)->d();
	
	return 0; 
}
/*
1.你被强制转换为的类型，决定了你使用什么范围内的成员是合法或不合法的。

2.你实际具有的值，决定了你将默认使用哪一个虚函数。

3.
下行转换：基类转派生类
 
若使用dynamic_cast()强制转换一个 具有派生类值的基类指针 为派生类指针，对于基类和派生类中的同名虚函数， 
默认虚函数是派生类中的虚函数。
因为指针是派生类指针，因此可以通过作用域分辨符访问基类和派生类中的虚函数。
此时dynamic_cast()=static_cast() 

若使用dynamic_cast()强制转换一个 具有基类值的基类指针 为派生类指针，对于基类和派生类中的同名虚函数， 
它没有默认虚函数。
必须使用作用于分辨符决定使用是哪个虚函数。
此时，dynamic_cast()!=static_cast()，前者若不使用::则报错。后者若不使用::，默认虚函数为基类中虚函数。
这是不安全的。

上行转换：派生类转基类

强制转换一个 具有派生类值的派生类指针 为基类。没有默认的概念，唯一虚函数就是基类中的虚函数。
此时使用dynamic_cast()和使用static_cast()等效。
 
你无法把一个基类值赋值给派生类，直接报错。因为不符合类型兼容规则。
你若将一个 具有派生类值（或基类）的 基类指针 强制转换为一个 派生类指针，然后赋值给一个
派生类指针。之后再强制转换该派生类指针为一个基类指针，则唯一虚函数就是基类中的虚函数。
即先下后上，ptr能使用的范围只是基类这一空间。 

如果去掉虚函数这个限制，即基类中没有虚函数，那么下行转换只要是使用dynamic_cast()就会出错。
只能使用static_cast()。 

4.
C style cast：
type(var);
(type)var;

C++ style cast：
const_cast(var); //一、常量指针被转化成非常量的指针，并且仍然指向原来的对象；二、常量引用被转换成非常量的引用，并且仍然指向原来的对象。
dynamic_cast(var); //实现安全的下行转换，但开销大，速度慢。因此在注重效率的代码中要尽量不使用dynamic_cast。 
static_cast(var); //强迫隐式转换 
reinterpret_cast(var);

5.
强迫隐式转换static_cast的问题所在：设Derived继承自Base，* this指代当前的Derived对象，则有：
static_cast<Base>(* this);
并不返回* this的base部分，而返回一个* this的base部分的副本对象。即通过static_cast<Base>(* this);只能改变* this的base部分的副本对象，而不能改变* this的base部分。
原理在于强制类型转换返回的是输入对象的转换副本，如A ——cast——>B，这里的B将在一个临时空间中被建立，cast返回的也正是这个B，而cast操作 以及 之后的对B的操作都不对
A造成任何影响。就像按照一幅水墨画 画出一张 临时的 油画 一样。
一旦离开cast语句，这个临时空间就会被回收，因此，要通过赋值的方式保留这个转换副本。

《Effective C++》条款27
*/ 
