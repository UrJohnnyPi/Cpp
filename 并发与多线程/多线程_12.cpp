#include "stdafx.h"
#include <thread>
#include <future>
#include <iostream>
using namespace std;
int threadPrint(promise<int> & pro , char){
	//经过长时间的计算，终于得到了result。
	int result = 7; 
	pro.set_value(result);
	return 8;
}
int main(){
	promise<int> pro;
	thread thr1(threadPrint , ref(pro) , 'k');
	future<int> res = pro.get_future(); //获取线程函数返回值 
	thr1.join();
	auto val = res.get(); //这样就在主线程中获得了子线程函数中的某一值。把主线程换成其他子线程一样适用。 
	
	return 0;
}
/*
promise<T> pro;类模板，可以在A线程中获值，然后在B线程中使用值。 
*/ 
