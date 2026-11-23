#include <thread>
#include <iostream>
#include <vector>
#include <mutex>
using namespace std;
/*
死锁解决方案1：将线程对mutex的请求顺序统一

class Msg {
public:
	void putMsgInMsgQueue() {
		for (int i = 0; i < 100000; ++i) {
			msgMutex_1.lock();
			msgMutex_2.lock();
			msgQueue.push_back(i);
			cout << "put msg " << i << " in msgQueue" << endl;
			msgMutex_2.unlock();
			msgMutex_1.unlock();
		}
	}
	void dropMsgFromMsgQueue() {
		lock_guard<mutex> m1(msgMutex_1);
		lock_guard<mutex> m2(msgMutex_2);
		while (!msgQueue.empty()) {
			//do something with msgQueue.back()
			cout << "drop msg " << msgQueue.back() << " from msgQueue" << endl;
			msgQueue.pop_back();
		}
	}
private:
	vector<int> msgQueue;
	mutex msgMutex_1 , msgMutex_2;
};
*/

/*
死锁解决方案2：使用std::lock(mutex_1 , mutex_2);是实现：要么两个互斥量都锁住，要么两个互斥量都没锁住

class Msg {
public:
	void putMsgInMsgQueue() {
		for (int i = 0; i < 100000; ++i) {
			lock(msgMutex_1 , msgMutex_2);
			msgQueue.push_back(i);
			cout << "put msg " << i << " in msgQueue" << endl;
			msgMutex_2.unlock();
			msgMutex_1.unlock();
		}
	}
	void dropMsgFromMsgQueue() {
		lock(msgMutex_1 , msgMutex_2);
		while (!msgQueue.empty()) {
			//do something with msgQueue.front()
			cout << "drop msg " << msgQueue.front() << " from msgQueue" << endl;
			msgQueue.pop_back();
		}
		msgMutex_2.unlock();
		msgMutex_1.unlock();
	}
private:
	vector<int> msgQueue;
	mutex msgMutex_1 , msgMutex_2;
};
*/

/*
死锁解决方案2：使用std::lock(mutex_1 , mutex_2);和std::lock_guard<std::mutex> guard_1(mutex_1 , std::adopt_lock);
及std::lock_guard<std::mutex> guard_2(mutex_2 , std::adopt_lock);实现：要么两个互斥量都锁住，要么两个互斥量都没锁住
其中：adopt_lock是一个结构体对象，起一个标记作用，作用是表示mutex参数已经lock()过了，无须在
lock_guard里对mutex再次lock()了。

该法显然比上法好，因为连续的语句，比分离的unlock()好。

class Msg {
public:
	void putMsgInMsgQueue() {
		for (int i = 0; i < 100000; ++i) {
			lock(msgMutex_1 , msgMutex_2);
			lock_guard<std::mutex> guard_1(mutex_1 , std::adopt_lock);
			lock_guard<std::mutex> guard_2(mutex_2 , std::adopt_lock);
			msgQueue.push_back(i);
			cout << "put msg " << i << " in msgQueue" << endl;
			//注意：无须unlock()
		}
	}
	void dropMsgFromMsgQueue() {
		lock(msgMutex_1 , msgMutex_2);
		lock_guard<std::mutex> guard_1(mutex_1 , std::adopt_lock);
		lock_guard<std::mutex> guard_2(mutex_2 , std::adopt_lock);
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

int main() {
	Msg msg;
	thread putInThread(&Msg::putMsgInMsgQueue, &msg);
	thread dropFromThread(&Msg::dropMsgFromMsgQueue, &msg);
	putInThread.join();
	dropFromThread.join();
	return 0;
}
