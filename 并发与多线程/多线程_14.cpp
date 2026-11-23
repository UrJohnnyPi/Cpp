#include "stdafx.h"
#include <iostream>
#include <thread>
#include <future>
#include <mutex>
#include <atomic>
using namespace std;
/*
void plu(int &a){
	for(int i = 0 ; i < 100000000 ; ++i) ++a;
}
int main(){
	int a = 0;
	thread thr1(plu , ref(a));
	thread thr2(plu , ref(a));
	thr1.join();
	thr2.join();
	cout << a << endl;
	return 0;
}
//上述代码的输出结果每次都是不固定的，而不是200000000。这种错误，是因为
//cpp代码在编译期，将从C++源文件转为汇编文件。而这里的一句，在汇编中就是
//好几个语句。又因为操作系统自有的一套线程调度算法，导致可能出现这样的情况： 
//假如++a转换成的目标语句有3条，当A线程执行了前两条后，线程调度发生，B线程执行
//且执行了全部3条，那么由于某些原因，B线程就修改了A线程放置在&a中的中间量，
//因此，当A线程再度执行时，因中间量已经改变，所以最后值就改变。

//总而言之，同写、有读有写这两种操作，如若不用某些机制限制，结果一定出错。
*/

/*
mutex mu;

void plu(int &a){
	for(int i = 0 ; i < 100000000 ; ++i){
		mu.lock();
		++a;
		mu.unlock();
	}
}
int main(){
	int a = 0;
	thread thr1(plu , ref(a));
	thread thr2(plu , ref(a));
	thr1.join();
	thr2.join();
	cout << a << endl;
	return 0;
}
//使用加锁方式，速度会慢一点，但不出错。
//因为有两千万次的加锁解锁操作。 
*/

/*
原子操作：atomic<T> var = val;

原子操作拥有比mutex方式更高的效率，但锁可以用来锁一段共享数据，而原子操作
是针对一个变量而言的。

atomic<T> var = val;对var的++、--、-=、+=等增强运算符起作用，对其他操作可能不支持。
如本例中，若令：a = a + 1;，则结果也会出错。

原子对象初始化：
atomic<T> atom = val; //T型值val赋给atom 
atomic<T> atom2(atom.load()); //将一个原子对象赋值给一个原子对象 

修改原子对象值：
atom2.store(val2);
atom2 = val2

代码： 
void plu(atomic<int> &a){
	for(int i = 0 ; i < 100000000 ; ++i) ++a;
}
int main(){
	atomic<int> a(0);
	thread thr1(plu , ref(a));
	thread thr2(plu , ref(a));
	thr1.join();
	thr2.join();
	cout << a << endl;
	return 0;
} 
*/
