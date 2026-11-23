#include "stdafx.h"
#include <thread>
#include <future>
#include <iostream>
using namespace std;

int threadPrint(int){
	chrono::milliseconds ms(5000); //该线程至少要执行5s+ 
	this_thread::sleep_for(ms);
	return 7;
}
int main(){
	/*
	（1）
	future_status //枚举类型 
	res.wait_for(time)：返回time时长后res的状态，res对象存储着线程 
	chrono::seconds(1000)
	future_status::timeout
	future_status::ready
	future_status::deferred
	*/
	future<int> res1 = async(threadPrint , 7);
	future_status status = res1.wait_for(chrono::seconds(1000)); //主线程只等待1s 
	if(status == future_status::timeout) {
		//线程超时时要执行的操作 
	} 
	else if(status == future_status::ready) {
		//线程准时时要执行的操作 
	} 
	else if(status == future_status::deferred){
		//通过res1.get();激活当前线程执行线程函数。 
	}
	
	/*
	（2）
	shared_future
	res.valid()
	shared_future::get()
	*/
	shared_future<int> res2(move(res1)); 
	if(res2.valid()){
		//若res2有值，则执行此处
	}	
	auto res3 = res2.get();
	
	/*
	（3）shared_future实践 
	*/
	packaged_task<int(int)> pt(threadPrint);
	thread thr1(ref(pt) , 7);
	thr1.join();
	shared_future<int> res4(pt.get_future());
	auto res5 = res4.get();
	return 0;
}
/*
由多线程10知，future::get()具有移动语义，效果等同于move。
而shared_future::get()具有复制语义，即shared_future<T> res2(res1);不会
置res1为空。 

注意：shared_future<T> res2 = res1.get();或shared_future<T> res2(res1.get());错误，
因为res2是一个类模板，其复制构造函数接收一个future<T>，而其构造函数显然不接受一个T obj。 
常见的赋值形式：
shared_future<T> res2 = move(res1); //res1是一个future<T>；若res1是一个shared_future<T>，则不加move()。
shared_future<T> res2(move(res1));
shared_future<T> res2 = async(launch::para , fun , para); //多线程10 
shared_future<T> res2 = packaged_task_obj.get_future(); //多线程11 
shared_future<T> res2 = promise_obj.get_future(); //多线程12 
*/
