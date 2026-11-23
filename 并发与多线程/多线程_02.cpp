#include "stdafx.h"
#include <iostream>
#include <thread>
using namespace std;
void threadPrint(){
	cout << "subThread." << endl;
}
class Test{
	public :
		Test(int x):x(x){
		}
		Test(const Test & test):x(test.x){
			cout << "copy" << endl;
		} 
		void operator()(){ //被默认调用的成员函数，不含参数，返回类型和函数体无约束 
			cout << "operator()" << endl;
			print(); 
		}
		void print(){
			cout << "test" << endl;
		}
	private :
		int x;
}; 
int main(){
	//（1）定义子线程对象，参数为函数名 
	thread subThread_1(threadPrint);
	thread subThread_2(threadPrint);
	
	//（2）阻塞主线程并等待子线程 
	subThread_1.join();
	
	//（3）join()、detach()判断 
	//（4）线程分离 
	if(subThread_2.joinable()) subThread_2.detach(); //subThread_2.joinable()返回true；故subThread_2线程分离
	//subThread_2.joinable(); //false
	
	//（5）参数为对象
	int x = 7;
	Test test(x);
	thread subThread_3(test); //默认调用obj.operator() 
	subThread_3.join();
	
	//（6）参数为lambda表达式
	auto lam = []{ cout << "lambda."; };
	thread subThread_4(lam);
	subThread_4.join();
	
	cout << "main thread." << endl;
	return 0;
}
/*
（1）thread类定义的对象即为子线程。

（2）thr.join();翻译就是：线程汇合。作用是阻塞当前进程，直到thr运行结束并返回。这时，
当前进程继续执行。如果没有join()，则线程的执行顺序是不可预期的。而且会报错，因为可能
在子线程退出之前，主线程就执行完return 0了，而主线程结束标志着进程的全部退出，所以报错。

（3）thr.detach();翻译就是：线程分离。作用是从此处起，当前线程与thr子线程便分道扬镳，互
不影响。即当前线程可以执行完成并return，而不影响thr子线程继续执行。thr执行完后，将由
C++运行时库回收资源。本例中，thr子线程自detach处便不再受当前进程的约束，main结束后，thr仍可执行。
关于detach还要注意的地方是，主线程结束后，子线程将由C++运行时库托管。可能会有这样的情况
出现：即detach后，主线程打印结束并return，而子线程未打印完毕，就不在屏幕上打印了，这并不
表示子线程不再执行，而是因为为进程分配的I/O被回收（因为进程执行结束了）。detach之前，子线程与主线程
享用了同一资源，所以子线程可以与主线程在当前的控制台上打印出信息。detach后，子线程将处于后台运行，
系统将为子线程重新分配资源。

（4）thr.joinable();作用是判断thr还能否join()或detach()，能的话返回true，否则返回false。为什么能同
时判断“thr还能否join()或detach()”？因为thr若已经join()或已经detach()，那么就表示thr已经执行完毕并返回而
无须再执行了 或 thr已经与当前进程无关而无须再管理了，所以thr经join()或detach()之后，就不能再join()和detach()了。

（5）参数为对象时，将默认调用obj.operator()。要注意类中的operator()无参数。此外，在使用thr.detach()时，该类的构造函数不能以引用、
指针为参数，因为若main传与thr(obj)中的obj指针或引用后，经detach(thr)后，main与thr便互无关系，main中的实参（或其指向的空间） 
可能随时被销毁，从而导致obj.operator()执行过程中的不确定性。 
有个问题是，那本例中的test对象也是局部变量，也会因主线程的结束而被析构，那么像这里传入一个局部对象，之后再调用
detach，难道不会出现像上面引用和指针为参数时的问题吗？答案是不会，因为这里会自动调用copy构造函数，使得
thr子进程真正调用的是一个拷贝对象的operator()。
这也说明了thread类中的构造函数：thread(funPointer para);中para不是一个&，因而才能
调用起copy构造函数。 

（6）使用Lambda表达式为参数。 
*/
