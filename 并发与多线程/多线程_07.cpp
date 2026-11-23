#include "stdafx.h"
#include <thread>
#include <iostream>
#include <vector>
#include <mutex>
using namespace std;
/*
（1）adopt_lock
class Msg {
	public:
		void putMsgInMsgQueue() {
			for (int i = 0; i < 100000; ++i) {
				lock(msgMutex_1 , msgMutex_2);
				unique_lock<mutex> ul_1(msgMutex_1 , adopt_lock);
				unique_lock<mutex> ul_2(msgMutex_2 , adopt_lock);
				msgQueue.push_back(i);
				cout << "put msg " << i << " in msgQueue" << endl;
			}
		}
		void dropMsgFromMsgQueue() {
			lock(msgMutex_1 , msgMutex_2);
			unique_lock<mutex> ul_1(msgMutex_1 , adopt_lock);
			unique_lock<mutex> ul_2(msgMutex_2 , adopt_lock);
			while (!msgQueue.empty()) {
				//do something with msgQueue.front()
				cout << "drop msg " << msgQueue.front() << " from msgQueue" << endl;
				msgQueue.pop_back();
			}
		}
	private:
		vector<int> msgQueue;
		mutex msgMutex_1 , msgMutex_2;
};
*/

/*
（2）try_to_lock
本例中，若putInThread线程被先调用，在使用lock()、guard_lock的情况下，
则dropFromThread线程将被迫等待2s+putInThread线程的执行时长。
但这里使用unique_lock配合try_to_lock参数和owns_lock()，就能让dropFromThread线程
避免额外开销。dropFromThread线程可以在不能立即取得锁时，转向执行其他操作。
将unique_lock<mutex> ul(mutex , try_to_lock);置于while中，只在ul.owns_lock()返回true
时才跳出循环。

class Msg {
	public:
		void putMsgInMsgQueue() {
			lock(msgMutex_1 , msgMutex_2);
			unique_lock<mutex> ul_1(msgMutex_1 , adopt_lock);
			unique_lock<mutex> ul_2(msgMutex_2 , adopt_lock);

			chrono::milliseconds ms(2000); //2000ms=2s
			this_thread::sleep_for(ms); //旨在该线程获得锁后，延迟20s再执行。

			for (int i = 0; i < 100000; ++i) {
				msgQueue.push_back(i);
				cout << "put msg " << i << " in msgQueue" << endl;
			}
		}
		void dropMsgFromMsgQueue() {
			while(true) {
				unique_lock<mutex> ul_1(msgMutex_1 , try_to_lock);
				unique_lock<mutex> ul_2(msgMutex_2 , try_to_lock);
				if(ul_1.owns_lock() && ul_2.owns_lock()) {
					while (!msgQueue.empty()) {
						//do something with msgQueue.front()
						cout << "drop msg " << msgQueue.front() << " from msgQueue" << endl;
						msgQueue.pop_back();
					}
					break;
				}
				else {
					//do something
				}
			}

		}
	private:
		vector<int> msgQueue;
		mutex msgMutex_1 , msgMutex_2;
};
*/

/*
（3）defer_lock
unique_lock(mutex , defer_lock);下，可以有任意对lock和unlock的配对。lock和unlock之间
是需要mutex的代码，对于对之间是不需要mutex的代码。unique_lock总能够在域结束时判断是否有
未unlock的mutex，然后为之unlock。

class Msg {
	public:
		void putMsgInMsgQueue() {
			lock(msgMutex_1 , msgMutex_2);
			unique_lock<mutex> ul_1(msgMutex_1 , adopt_lock);
			unique_lock<mutex> ul_2(msgMutex_2 , adopt_lock);

			for (int i = 0; i < 100000; ++i) {
				msgQueue.push_back(i);
				cout << "put msg " << i << " in msgQueue" << endl;
			}
		}
		void dropMsgFromMsgQueue() {
			while(true) {
				unique_lock<mutex> ul_1(msgMutex_1 , defer_lock);
				unique_lock<mutex> ul_2(msgMutex_2 , defer_lock);
				lock(msgMutex_1 , msgMutex_2);
				
				while (!msgQueue.empty()) {
					//do something with msgQueue.front()
					cout << "drop msg " << msgQueue.front() << " from msgQueue" << endl;
					msgQueue.pop_back();
				}
				
				msgMutex_1.unlock();
				msgMutex_2.unlock();
				
				//do something needn't mutex
				
				lock(msgMutex_1 , msgMutex_2);
				//do something need mutex
				
				//这里可以将msgMutex_1、msgMutex_2全部或选其一unlock，也可以不unlock。 
			} //域结束处，ul_1（ul_2）将自动判断有没有未unlock的msgMutex_1（msgMutex_2），若有则unlock后结束，若无则直接结束。 

		}
	private:
		vector<int> msgQueue;
		mutex msgMutex_1 , msgMutex_2;
}; 
*/


int main() {
	Msg msg;
	thread putInThread(&Msg::putMsgInMsgQueue, &msg);
	thread dropFromThread(&Msg::dropMsgFromMsgQueue, &msg);
	putInThread.join();
	dropFromThread.join();
	return 0;
}
/*
1.
unique_lock<mutex> ul(para_1 , para_2);可以完全取代lock_guard<mutex> gl(para_1 , para_2);
unique_lock较与lock_guard有更好的灵活性，体现在其第二个参数支持更多种类标记的输入。
但unique_lock的代价是，效率低一点、开销大一点。
unique_lock二个参数支持的标记如下：
（1）std::adopt_lock：作用是声明当前mutex已被lock()过了，无须再调用lock()。
（2）std::try_to_lock：作用是尝试去申请锁，若能申请到，有owns_lock()返回true；否则owns_lock()返回false，线程将转向其他操作，而不像普通的lock()或guard_lock一样使得申请不能及时取得的线程处于阻塞等待状态。
（3）std::defer_lock：作用是不获得mutex的所有权，即当前mutex未被lock()，可以调用lock()进行加锁，可以调用unlock()进行解锁。若如忘记解锁，unique_lock将自动解锁。

2.
lock()成员函数可以替换为try_lock()成员函数，try_lock()的作用是：尝试申请mutex，若成功则返回true，
否则返回false。申请失败时，也不不像lock()一样阻塞等待mutex，而是返回。 
如：
{
	unique_lock<mutex> ul(msgMutex , defer_lock);
	if(msgMutex.try_lock()){
		//do something need mutex 
	}else{
		//do something needn't mutex
	}
}//unique_lock将自动检查并解锁

3.
release()成员函数：返回mutex指针并解除unique_lock对mutex的所有权。例如：
{
	unique_lock<mutex> ul(msgMutex);
	mutex * ptr = msgMutex.release(); //执行此语句后，ul对msgMutex的所有权失效，所有权转交给ptr。
	//do something need mutex 
	ptr->unlock(); //因ul失效，所以得使用ptr自行解锁 
}

4.所有权问题
所有权不能共享，只可以独占并转移 
{
	//错误写法1： 
	unique_lock<mutex> ul1(msgMutex);
	unique_lock<mutex> ul2(msgMutex);
	
	//错误写法2：
	unique_lock<mutex> ul2(ul1);
	
	//错误写法3：
	unique_lock<mutex> ul2 = ul1; 
}

{
	//正确写法： 
	unique_lock<mutex> ul2(move(ul1)); //原ul1占有的mutex现转移给了ul2。ul1指向空。相当于使用了release()。 
}
*/
