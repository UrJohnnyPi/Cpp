#include "stdafx.h"
#include <iostream>
#include <thread>
#include <mutex>
using namespace std;
/*
//一个典型的单例模式类

class A {
	public :
		static A * instanceFun() {
			if(!instancePtr) {
				instancePtr = new A;
				return instancePtr;
			}
			return nullptr;
		}
	private :
		A() {}
		static A * instancePtr;
};
A * A::instancePtr = nullptr;
*/

/*
单例模式多线程设计：
核心是双重判定式，双重判定式用于保证多个线程，只有一个线程能执行某一函数，即使该线程执行完毕，其他线程也不能再去执行：
funB(){
}
funA(){ 
	if(条件){
		mutex.lock();
		if(条件){
			funB(); 
		}
		mutex.unlock();
	}
}
这里两个if中的条件是一样的。第一个if是为了过滤那些已经有线程执行完funB了，但还没来得及
获取mutex的那些线程，这么做，是为了提高效率，避免那些慢得出奇的线程也进入mutex等待队列。
第二个if是为了保证funB只能被一个函数执行。

class A {
public:
	static A* instanceFun() {
		if (instancePtr == nullptr) {
			//cout << this_thread::get_id() << endl;
			mut.lock();
			//cout << this_thread::get_id() << endl;
			if (instancePtr == nullptr) {
				instancePtr = new A;
				//cout << this_thread::get_id() << endl;
			}
			mut.unlock();
		}
		return instancePtr;
	}
private:
	A() {}
	static A* instancePtr;
	static mutex mut;
}; 
A * A::instancePtr = nullptr;
mutex A::mut;

void createThread() {
	A * ptr = A::instanceFun();
}

总的来看，只有一个线程能够执行最内层if的函数体一次，其他线程，有的只能执行最外层if的外部，
有的能执行最内层if的外部。
单例模式就是通过将唯一实例的创建放到最内层if中，确保只有一个线程能执行最内层if的函数体一次。 
*/

/*
call_once写法：call_once(flag , instanceFun);若多个线程执行该语句，call_once可以保证
只有一个线程能执行instanceFun()。原理是，call_once可以通过设置标记flag而确保最先执行该
语句的线程获得对instanceFun()的唯一所有权，即使该线程执行instanceFun()完毕并退出，由于
flag标记instanceFun()为已执行，其他线程也无法再次执行instanceFun()。 
其中，std::once_flag flag; 

实际上用：
funB(){
}
funA(){
	call_once(flag , instanceFun);
}
取代了双重判定式： 
funB(){
}
funA(){ 
	if(条件){
		mutex.lock();
		if(条件){
			funB(); 
		}
		mutex.unlock();
	}
}
而call_once形式效率更高。
 
//代码： 
class A {
	public:
		static A * getInstance(){
			//cout << this_thread::get_id() << endl;
			call_once(flag , instanceFun);
			return ptr;
		}
	private:
		A() {}
		static void instanceFun() {
			ptr = new A;
		}
		static A* instancePtr;
		static mutex mut;
		static once_flag flag;
};
A * A::instancePtr = nullptr;
mutex A::mut;
once_flag A::flag;

void createThread() {
	A::getInstance();
}
*/ 

int main() {
	thread thr_1(createThread);
	thread thr_2(createThread);
	thr_1.join();
	thr_2.join();
	return 0;
}
