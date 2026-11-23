#include "stdafx.h"
#include <thread>
#include <iostream>
#include <vector>
#include <mutex>
using namespace std;
class Msg {
	public:
		void putMsgInMsgQueue() { 
			for (int i = 0; i < 100000; ++i) {
				unique_lock<mutex> m(msgMutex);
				msgQueue.push_back(i);
				cout << "put msg " << i << " in msgQueue" << endl;
				convar.notify_one(); //putInThread线程执行至此，企图唤醒dropFromThread线程中的wait()。
			}
		}
		void dropMsgFromMsgQueue() {
			unique_lock<mutex> m(msgMutex);
			convar.wait(m , [this] { //（2）convar.wait(mutex , lambda)
				if(!msgQueue.empty()) return true;
				else return false;
			});  
			cout << "drop msg " << msgQueue.front() << " from msgQueue" << endl;
			msgQueue.pop_back();
		}
	private:
		vector<int> msgQueue;
		mutex msgMutex;
		condition_variable convar; //（1）声明一个条件变量
		/*
		mutex就是互斥信号量 
		condition_variable就是同步信号量 
		*/ 
};
int main() {
	Msg msg;
	thread putInThread(&Msg::putMsgInMsgQueue, &msg);
	thread dropFromThread(&Msg::dropMsgFromMsgQueue, &msg);
	putInThread.join();
	dropFromThread.join();
	return 0;
}
//该代码的问题之处在于，倘若在putInThread线程执行完毕，dropFromThread线程都没有
//执行到wait处，也就不存在什么唤醒不唤醒了。dropFromThread在wait处若被阻塞，本
//代码中也没有其他机制可以解决。 

/*
（1）condition_variable convar;声明/定义convar是一个条件变量。
（2）convar.wait(mutex , lambda);第二个参数lambda表达式可有可无。lambda中用于
判断某个条件是否成立来返回true或false。
a.有lambda的情况下：
若lambda返回true，则什么也不做，出了wait语句继续执行；
若lambda返回false，则解锁当前线程针对本段代码的mutex，并使当前线程阻塞在wait语句。
直到其他线程执行了notify_one()为止。本例中，就是若return false，则m将失去msgMutex
所有权，当前线程阻塞在wait语句。
b.无lambda的情况下：按返回false处理。即解锁、阻塞。

当lambda返回false或无lambda时，wait将阻塞当前线程并释放当前线程的mutex所有权。
当另一线程调用notify_one时，表示那个线程已经用完了锁，所以notify_one将同时让
那个线程释放mutex、激活wait，wait将进而执行的工作是：
a.不断尝试让当前线程重新获得mutex所有权。如果获取不到，就像lock()一样，当前线程一直阻塞至此。
如果获取到锁，当前线程继续执行。
b.
b.1)若wait有第二个参数lambda，lambda返回false，则继续解锁阻塞。
b.2)若wait有第二个参数lambda，lambda返回true，则wait返回，当前线程继续执行。
b.3)若wait没有第二个参数，则wait返回，当前线程继续执行。
（3）notify_one一次只能激活一个wait，意即若有多个线程处于wait阻塞态，则调用一次
notify_one将随机激活一个线程中的wait。
（4）若有多个线程处于wait阻塞态，采用notify_all()。notify_all()将激活所有因wait
而阻塞的线程。 
*/
