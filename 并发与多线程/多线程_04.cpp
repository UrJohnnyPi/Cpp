#include "stdafx.h"
#include <thread>
#include <iostream>
using namespace std;
void threadPrint(){
	cout << "threadPrint of " << this_thread::get_id() << endl;
}
class Test{
	public :
		Test(int x):x(x){
			cout << "constructor fun of " << this_thread::get_id() << endl;
		}
		Test(const Test & test):x(x){
			cout << "copy constructor fun of " << this_thread::get_id() << endl;
		} 
		~Test(){
			cout << "destroy constructor fun of " << this_thread::get_id() << endl;
		}
		void display(int){
			cout << "display" << endl;
		} 
		void operator()(int){
		} 
	private :
		int x;
};
void useTest(const Test & test){
	cout << "useTest" << endl;
}
void smartPtr(unique_ptr<int> uptr){
	//do something
} 
int main(){
	/*
	1.this_thread::get_id() 
	*/ 
	
	//thread thr1(threadPrint);
	//thr1.join();
	
	//thread thr2(useTest , 1);
	//thr2.detach();
	
	thread thr3(useTest , Test(1));
	thr3.detach();
	
	//cout << "main of " << this_thread::get_id() << endl;
	
	/*
	2.std::ref()
	*/ 
	Test test(1); 
	thread thr4(useTest , ref(test)); //形参将接收到test而非其copy对象。 
	thr4.join();
	/*
	据上例可总结：
	形参为引用时，若实参为临时对象Test(1)，则可以使得子线程的运行
	不再受主线程中数据的影响，也因此，子线程可用detach()操作；
	若实参为ref(test)，则将传递该对象本身，意即线程函数中的&将发挥作用。
	也因此，子线程只能用join()。 
	*/ 
	
	/*
	3.smart ptr
	*/ 
	unique_ptr<int> up(new int(0));
	thread thr5(smartPtr , move(up));
	thr5.join();
	return 0;
	
	/*
	4.类的成员函数作为线程函数 
	*/
	Test t(1);
	thread thr6(&Test::display , t , 2); 
	thr6.join();
	
	/*
	5.operator()(para)
	*/ 
	Test to(2);
	thread thr7(to , 7);
	thr7.join();
}
/*
输出：
constructor fun of 1
copy constructor fun of 1
copy constructor fun of 1
destroy constructor fun of 1
destroy constructor fun of 1
useTest
destroy constructor fun of 2 

（1）std::this_thread::get_id();返回当前函数在哪一线程下工作。
由输出可见，Test(1)在主线程（线程id=1）中产生临时对象。在主线程中完成拷贝构造函数。
与多线程_3中总结的一致。
（2）std::ref(obj)将传出真正的obj而不调用copy构造，因此这时形参中的引用才发挥作用，真正
绑定到主线程传出的obj而非临时对象上。
（3）当传递智能指针时，也最好用join()而要斟酌detach()。因为智能指针有不同的机制，
比如传一个shared_ptr，用detach就可能出错，因为在子线程执行完前，主线程可能就已经执行完毕
并回收空间了。但若换成unique_ptr，那使用detach就是无错误的。
（4）类的成员函数作为线程函数，thr构造形式：thread thr(&className::memberFun , obj , paraOfMemberFun);
既可以使用join也可以使用detach，因为原理还和传递一个类对象性质一样，将先默认调用拷贝构造函数，形参实际
是一个copy Obj。
（5）类成员函数operator()(para)作为线程函数
*/ 
