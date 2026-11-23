#include "stdafx.h"
#include <thread>
#include <iostream>
#include <vector>
#include <mutex> 
using namespace std;
/*
void threadPrint(int subThreadID){
	cout << subThreadID << endl; 
}
*/
class Msg{
	public :
		void putMsgInMsgQueue(){ //将收到的消息放到消息队列中，这里的消息是i 
			/*
			使用lock()和unlock() 
			*/ 
			for (int i = 0; i < 100000; ++i) {
				msgMutex.lock();
				msgQueue.push_back(i);
				cout << "put msg " << i << " in msgQueue" << endl;
				msgMutex.unlock();
			}
		}
		void dropMsgFromMsgQueue(){ //将消息取出消息队列 
		    /*
			使用lock_guard<mutex> mut(msgMutex);
			*/
			lock_guard<mutex> mut(msgMutex);
			while(!msgQueue.empty()) {
				//do something with msgQueue.back()
				cout << "drop msg " << msgQueue.back() << " from msgQueue" << endl;
				msgQueue.pop_back();
			}
		} 
	private :
		vector<int> msgQueue; //消息队列
		mutex msgMutex; 
};
int main(){
	/*
	1.多线程的容器托管
	vector<thread> thr1;
	for(int i = 0 ; i < 5 ; ++i) thr1.push_back(thread(threadPrint , i));
	for(auto iter = thr1.begin() ; iter != thr1.end() ; ++iter) iter->join();
	*/
	
	/*
	2.互斥锁 
	*/
	Msg msg;
	thread putInThread(&Msg::putMsgInMsgQueue , & msg); //不加&报错。 
	thread dropFromThread(&Msg::dropMsgFromMsgQueue , & msg);
	putInThread.join();
	dropFromThread.join();
	return 0;
}
/*
（1）多线程最好用容器管理。
多线程情形下，子线的实际程执行顺序不由其定义位置相对于其他子线程的前后与否、也不由其join()/detach()的相对位置
相对于其他子线程的前后与否来决定。例如本例中，thr1[0]线程的定义和join()均早于其他子线程，但实际上的执行顺序
却是乱的。原因在于，操作系统线程调度是按照操作系统开发人员设计的一套算法实现的，和你怎么定义子线程顺序无关。
但要注意的是，主线程的执行却可以通过join()来阻塞延迟的。
即：thr1[0]线程可能被最先放入就绪队列，或最先被调入内存，但因为调度算法 或 该线程
所要求的资源未被满足等原因，thr1[0]线程并不一定是最先执行的。 
（2）对于只读数据，多线程情形下不存在什么问题。
（3）对于可读可写的数据，多线程情形下就是：先同步后互斥。即读时不写，写时不读。另外，多个线程读时，无须控制，
多个线程写时就需要互斥。典型的生产者消费者问题。
（4）#include<mutex>下的mutex类用于定义互斥锁，使用lock()和unlock()锁定代码。
或使用类似于智能指针的lock_guard<mutex> mut(msgMutex);来锁定代码，当定义mut的
域终结时，mut将被自动析构。 
*/ 
