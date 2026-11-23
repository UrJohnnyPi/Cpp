#include "stdafx.h" 
#include <thread>
#include <iostream>
#include <vector>
#include <mutex> 
using namespace std;

/*
recursive_mutex：递归的独占互斥量 

class Msg {
public:
	void putMsgInMsgQueue() {
		msgMutex_1.lock();
		msgMutex_2.lock();
		msgMutex_2.lock();
		
		for (int i = 0; i < 100000; ++i) {
			msgQueue.push_back(i);
			cout << "put msg " << i << " in msgQueue" << endl;
		}
		
		msgMutex_2.unlock();
		msgMutex_2.unlock();
		msgMutex_1.unlock();
	}
	void dropMsgFromMsgQueue() {
		lock_guard<recursive_mutex> m1(msgMutex_1);
		lock_guard<recursive_mutex> m2(msgMutex_2);
		while (!msgQueue.empty()) {
			//do something with msgQueue.front()
			cout << "drop msg " << msgQueue.front() << " from msgQueue" << endl;
			msgQueue.pop_back();
		}
	}
private:
	vector<int> msgQueue;
	recursive_mutex msgMutex_1 , msgMutex_2;
};

int main() {
	Msg msg;
	thread putInThread(&Msg::putMsgInMsgQueue, &msg);
	thread dropFromThread(&Msg::dropMsgFromMsgQueue, &msg);
	putInThread.join();
	dropFromThread.join();
}
*/ 

/*
timed_mutex::try_lock_for(s);

timed_mutex mu;
void threadPrint(){
	chrono::milliseconds ms(1000); //设置最多等1s 
	if(mu.try_lock_for(ms)){
		//1s内获取到锁时的业务代码
		cout << this_thread::get_id() << endl;
		mu.unlock(); //解锁 
	}
	else{
		//超时业务代码 
	} 
} 
*/ 

/*
timed_mutex::try_lock_until(s);

timed_mutex mu;
void threadPrint(){
	chrono::milliseconds ms(1000); //设置最多等1s 
	if(mu.try_lock_until(chrono::steady_clock::now() + ms)){ //当前之后的1s 
		//1s内获取到锁时的业务代码
		cout << this_thread::get_id() << endl;
		mu.unlock(); //解锁 
	}
	else{
		//超时业务代码 
	} 
}
int main(){
	thread thr1(threadPrint);
	thread thr2(threadPrint);
	thr1.join();
	thr2.join(); 
	return 0;	
} 
*/

/*
（1）recursive_mutex 
mutex：独占互斥量；同一个线程对该mutex不能两次lock()和unlock()。
recursive_mutex：递归的独占互斥量；允许同一个线程对该锁多次lock()和unlock()，但注意
lock()和unlock()得匹配。 
（2）带超时功能的互斥量
timed_mutex：带超时功能的独占互斥量。
recursive_timed_mutex：带超时功能的递归独占互斥量。
（3）
timed_mutex mu;
mu.try_lock_for(ms);若当前线程阻塞等待ms后，当前线程获得mu，则返回true，否则返回false。

mu.try_lock_until(ms); 
*/
