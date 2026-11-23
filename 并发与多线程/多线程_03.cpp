#include "stdafx.h"
#include <iostream>
#include <thread>
using namespace std;
void threadPrint(int x , char y){
	cout << "subThread." << ends << x << ends << y << endl;
}
class Test{
	public :
		Test(int x):x(x){
			cout << "constructor fun of " << this << endl;
		}
		Test(const Test & test):x(x){
			cout << "copy constructor fun of " << this << endl;
		} 
		~Test(){
			cout << "destroy constructor fun of " << this << endl;
		}
	private :
		int x;
};
void useTest(const Test & test){
	cout << "useTest" << endl;
}
int main(){
	//（1）向线程对象传入threadPrint(int x , char y)
	thread subThr_1(threadPrint , 7 , 'x');
	subThr_1.join();
	
	//（2）当形参必须是引用时的注意点
	/*
	Test test(1);
	thread subThr_2(useTest , test);
	subThr_2.detach(); 
	错误的写法，和多线程_1中提及的问题一样，test作为参数传给useTest，但useTest要求
	一个引用，因此，当test被主线程销毁时，useTest将陷入不可预知态。 
	*/
	
	/* 
	thread subThr_2(useTest , 1);
	错误的写法。本意是想通过传1来隐式类型转换，从而调用Test的构造函数。但问题在于，
	这个1占用的仍是主线程的空间，只有在调用构造函数成功后，以这个1为构造参数的临时对象
	才能产生出来，之后再绑定到useTest中的& test上。意即临时对象产生之时，才是main与thr
	真正可以分道扬镳之时。但主线程中的detach才不等你，所以也就可能出现这样的情况：临时
	对象还没被构造完成，1所占的空间就已经被主线程释放。 
	这个语句单元只做：把useTest交到subThr_2中，把1传出去。1传给构造函数，该语句单元就完成传出去的任务了，
	即使构造函数还没执行完毕。
	而把对象交给拷贝构造函数性质就不一样了，因为拷贝构造函数的签名时T(const T & obj)，即必须等obj完成赋值操作后，该语句单元才能结束。
	*/
	
	/*
	上述问题都指出了一个更深的问题：什么时候线程分离才比较合适？或说，什么时候才是调用detach
	的合适时期？
	
	答案是：当子线程确定自己获得了不再由主线程修改的参数时。具体说，就是，当主线程传递的参数
	能调用起拷贝函数时。比如传一个临时对象、当函数参数不为引用时传一个类对象（见多线程_1）
	当函数参数不为引用时传一个内置类型变量（将调用内置的copy函数）等。 
	
	为什么这么说？因为thread thr(para);这个主线程中不被中断的完整语句对应的是：para传到形参、形参获值，
	如下例中，Test(1)调用构造函数并创建临时对象的所有操作都是在
	thread subThr_2(useTest , Test(1));这一语句的生命期中要完成的。将创建的临时对象成功
	赋值给形参后，该语句的生命期才算结束。而这里的成功赋值就是指拷贝构造函数的调用成功并执行完成。 
	*/
	thread subThr_2(useTest , Test(1)); //创建临时对象 
	subThr_2.detach();

	return 0;
}
/*
输出：
subThread. 7 x
constructor fun of 0x70fe00
copy constructor fun of 0x70fd90
copy constructor fun of 0xa41c38
destroy constructor fun of 0x70fd90
destroy constructor fun of 0x70fe00
useTest
destroy constructor fun of 0xa41c38

可见产生了两个临时对象，地址是0x70fe00和0x70fd90。前者是Test(1)产生的，后者是调用的拷贝构造
以复制Test(1)产生的临时对象而产生的。 
*/ 
