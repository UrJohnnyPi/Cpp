#include "stdafx.h"
#include <thread>
#include <mutex>
#include <iostream>
#include <future>
#include <vector> 
using namespace std;
int threadPrint(int){
	cout << this_thread::get_id() << endl;
	return 7;
}
int main(){
	/*
	std::packaged_task<returnT(paraT)> pt(funName);打包线程函数 
	*/
	packaged_task<int(int)> pt1(threadPrint); //打包 
	thread thr1(ref(pt1) , 3);
	thr1.join();
	
	/*
	pt.get_future();异步获值
	*/
	future<int> res1 = pt1.get_future(); //异步获值，即thr1和主线程分离，各执行各的，于是在主线程中先用res存储一个当前还未取得的线程函数返回的值。 
	cout << res1.get() << endl; //get()取值；因thr1已经join，因此这里不再阻塞，否则get将阻塞直到线程（线程函数）返回。
	
	/*
	std::packaged_task<returnT(paraT)> pt(lambda)
	*/
	packaged_task<int(int)> pt2([](int){
		cout << this_thread::get_id() << endl;
		return 10;
	});
	thread thr2(ref(pt2) , 7);
	thr2.join();
	future<int> res2 = pt2.get_future();
	cout << res2.get() << endl; 
	
	/*
	vector<packaged_task<returnT(paraT)>> vec;即定义一个packaged_task<returnT(paraT)>容器 
	*/
	vector<packaged_task<int(int)>> vec;
	vec.push_back(move(pt1));
	vec.push_back(move(pt2)); //只能用move来移动，移动之后，pt1和pt2就为空。 
	
	auto iter = vec.begin();
	pt1 = move(* iter); //vec元素移动到pt1
	vec.erase(iter);
	thread thr3(ref(pt1) , 9);
	thr3.join();
	 
	return 0;
}
/*
std::packaged_task<returnT(paraT)> pt(funName);将线程函数打包进对象pt，这样
传入线程的参数就是pt和函数参数。
为什么要进行打包？为了获得对象pt，进而可以使用pt的成员函数，比如get_future()来异步
获值。

packaged_task与thread并无必然关系，packaged_task归根到底其实就是一个打包函数的模板类。例如：
packaged_task<int(int)> pt(threadPrint);
pt(7);
future<int> res = pt.get_future();
res.get();
这样一段不涉及thread的代码也是正确的。
*/
