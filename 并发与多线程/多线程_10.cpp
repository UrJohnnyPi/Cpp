#include "stdafx.h"
#include <thread>
#include <mutex>
#include <future>
#include <iostream>
using namespace std;
class A {
	public :
		int test(int) {
			cout << this_thread::get_id() << endl;
			return 7;
		}
};
int threadPrint() {
	cout << this_thread::get_id() << endl;
	chrono::milliseconds ms(2000);
	this_thread::sleep_for(ms);
	cout << this_thread::get_id() << endl;
	return 1;
}
int main() {
	/*
	给async传递函数
	*/
	future<int> res1 = async(threadPrint); //定义异步操作
	//long time
	cout << res1.get() << endl;

	/*
	给async传递类成员函数
	*/
	A a;
	future<int> res2 = async(&A::test , &a , 1);
	res2.get();

	/*
	给async传递std::launch::deferred，停止创建线程，转由当前线程接管。get被调时，当前线程执行test() 
	*/
	future<int> res3 = async(launch::deferred , &A::test , &a , 1);
	res3.get();
	return 0;
}
/*
（1）异步操作
async(funName)是一个函数模板，future<T> 是一个类模板。async(funName)将返回
一个future<T>对象，其中T是funName的返回值类型。
async(funName)的作用是：自动创建一个线程，并执行线程入口函数fun。async(funName)返回
的future<T>对象result保存着这样的一个值：该值是线程入口函数fun执行的结果，但当前
无法拿到，只能在未来fun执行完成并返回后拿到。意即：future<int> result = async(threadPrint);
语句不会等待async(threadPrint);返回结果，而是直接执行下一步操作。等结果返回时，result
将捕获。捕获语句：result.get()。
如果在执行result.get()时，threadPrint仍没有返回，则主线程就会阻塞在result.get()语句，
直到threadPrint返回。
总结：
future<T> result = async(funName);
//do something
result.get(); //若函数有返回值，必须用get()
result.wait(); //若函数无返回值，用wait()和get()都行。wait()是等子线程运行结束（即函数执行完毕）。

get()只能被调用一次！！多次调用报错。因为get是移动语义，即若有var = res.get()，即将即等同var = move(res)，var等于空。

（2）给async传递类成员函数：async(&className::funName , &obj , funPara);
注意这里的第二个参数是&obj，这才能保证线程使用的是同一个obj，若不是引用，则传递
的是一个obj的copy对象。

这里集中说一下当参数为对象时，传递&obj的好处：实参为&obj，那试想形参是什么？必然是一个指针。意即，子线程操作 和 主线程操作的是同一个对象。这就避免了对
深复制和浅复制情况的判断。但要求是，若为thread(&obj)，必须要用join()而非detach()。

（3）通过给async传递std::launch参数，使得async拥有更多的操作选项。
给async传递launch::deferred，使得async停止创建线程，并将线程函数的执行延迟到
result.get()或result.wait()被调用时，线程函数将交由当前线程执行。本例中test
将由主线程接管。

（4）给async传递launch::async参数，async将立即创建新线程并执行线程函数。

（5） 
future<T> res = async(launch::async | launch::deferred , fun);等效于
future<T> res = async(fun);
而launch::async | launch::deferred表示，二选一。若系统资源充足，能分配出额外
的线程则可能选择launch::async参数；否则，可能就会选择launch::deferred。即
选择异步方式或同步方式。

（6）thread()方式创建线程和async()方式创建线程的区别在于：
thread()：若系统资源紧张，则可能创建失败，然后报异常。
async()：可以创建新线程，也可不创建新线程。尤其是无launch参数的情况下，默认的
launch::async | launch::deferred将根据系统资源情况而被自动选择。

（7）但有种情况下，你必须得知道async()无launch参数下到底选择了哪种方式，于是有：
future<T> res = async(fun);
future_status status = res.wait_for(chrono::seconds(0)); //注意是0 
if(status == future_status::deferred){
	//此时async选择的就是launch::deferred，系统资源紧张，不开辟新线程 
} else{ //不是deferred就是async 
	if(status == future_status::ready){
		//新线程创建好了 
	}
	else if(status == future_status::timeout){
		//新线程超时了 
	}
} 
*/
