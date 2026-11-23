/*
1.声明一个返回数组指针的函数
(1)区别
int arr[10];
int * arr[10];
int (* arr)[10] = & arr;

(2)funName(para)理解
首先要明白 funName(para) 的真正作用是什么，举例：
int fun(int para){ //a，定义处
    return x;
}

{
    use fun(para); //b，调用处
}

extern int fun(); //c，声明处
using namespace::fun;

在a处，把fun(int para)视为obj，后面的函数体{...}视为var，则一个函数可解释为：int obj = var;
在b处，use fun(para);就是use obj;
在c处，与上同理。

(3)声明一个返回数组指针的函数
int (* fun(int i))[10]{
    int (* arr)[10] = {};
    return arr;
}

{
    use fun(i);
}

int (* fun(int i))[10]理解为：int (* var)[10];

2.使用尾置返回类型（C++11新标准）
任何函数都可以使用尾置返回类型，上例可写为：
auto fun(int i) -> int(*)[10]{
    //如上
}
即：
auto fun(para) -> 类型{
}

3.使用decltype返回类型（C++11）
#include <iostream>
using namespace std;

int arr[] = {1,2,3};
decltype(arr) (* fun(int)){
    return & arr;
}
int main(){
    int (* p) [3] = fun(2);
	for(int i = 0 ; i < 3 ; ++i) cout << (* p)[i] << endl;
    return 0;
}
decltype(arr)返回类型：int [10]；但函数体返回类型：int (*)[10]，所以，decltype(arr) (* fun(int))。
此外，注意这里指针数组的使用，int * p = new int[5]，int (* p)[5] = new int [4][5];意即，p存储的是数组地址，p[i]返回数组地址，(* p)[i]返回数组元素。
*/