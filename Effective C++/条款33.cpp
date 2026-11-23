/*
条款33：避免遮掩继承而来的名称
*/

/*
代码1：

#include <iostream>
using namespace std;
class Base{
	public :
		virtual void mf1() = 0;
		virtual void mf2(){
			cout << "Base::mf2" << endl;
		}
		void mf3(){
			cout << "Base::mf3" << endl;
		}
	private :
		int x;
};
class Derived : public Base{
	public :
		void mf1(){
			cout << "Derived::mf1" << endl;
		}
		void mf4(){
			mf2();
			cout << "Derived::mf4" << endl;
		}
};
int main(){
	Derived der;
	der.mf4();
	return 0;
}
//在der调用mf4时，其运行过程是这样的：当编译器看到mf4()中的mf2符号时，编译器将查找
//local作用域（mf4()的作用域），未找到；于是查找外围空间，即Derived作用域（仅派生空间），
//未找到；再查找Derived作用域中的继承空间，找到，停止查找。
//如果Derived作用域（包括派生空间和继承空间）没有mf2符号，编译器将查找其外围的namespace或
//global空间。
*/

/*
代码2：

#include <iostream>
using namespace std;
class Base{
	public :
		virtual void mf1() = 0;
		virtual void mf1(int){
			cout << "Base::mf1(int)" << endl;
		}
		virtual void mf2(){
			cout << "Base::mf2" << endl;
		}
		void mf3(){
			cout << "Base::mf3" << endl;
		}
		void mf3(double){
			cout << "Base::mf3(int)" << endl;
		}
	private :
		int x;
};
class Derived : public Base{
	public :
		void mf1(){
			cout << "Derived::mf1" << endl;
		}
		void mf3(){
			cout << "Derived::mf3" << endl;
		}
		void mf4(){
			cout << "Derived::mf4" << endl;
		}
};
int main(){
	int x = 0;
	Derived der;

	der.mf1(); //Derived::mf1
	//der.mf1(x); //错误
	der.mf2(); //Base::mf2
	der.mf3(); //Derived::mf3
	//der.mf3(x); //错误
	der.mf4(); //Derived::mf4

	return 0;
}
//该代码的两个错误之处在于：Derived中的mf1将隐藏继承自Base的所用mf1的重载形式（即不论
//Base中的mf1是怎么写的，参数列表、返回值类型如何，只要同名就会被隐藏）。mf4亦然。
//解决同名隐藏的办法是在派生空间中利用：
//（1）作用域分辨符：例：
class Derived : public Base{
	public :
		void mf1(int x){
			Base::mf1(x);
		}
		void mf3(double x){
			Base::mf4(x);
		}
		//do something
};
//作用域分辨符完成的是一次调用。

//（2）using +作用域分辨符：例：
class Derived : public Base{
	public :
		using Base::mf1;
		using Base::mf3;
		void mf1(){
			cout << "Derived::mf1" << endl;
		}
		void mf3(){
			cout << "Derived::mf3" << endl;
		}
		void mf4(){
			cout << "Derived::mf4" << endl;
		}
};
//using语句是将符号暴露在指定空间中。
//此时：
der.mf1(x);
der.mf3(x);
//均正确
*/

/*
代码3：

//对于多重继承，避免派生类通过作用域分辨符访问间接基类中的成员，若有访问的必要，
//则：
class A {
	public :
		void display(); //不论有没有virtual都会被隐藏 
		void display(int); 
};
class B : public A {
	public :
		using A::display; //法1：将display(int);的作用域扩展至B（display()仍被隐藏）。于是可以在B/C中利用重载访问A成员。 
		void display(){ //法2：转交函数，C的成员可以通过访问此函数实现对A成员的访问。 
			A::display();
		} 
};
class C : public B {
	public :
		cdisplay(){
			B::display(x); 
			B::display();
		} 
}; 
*/
