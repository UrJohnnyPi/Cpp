/*
1.举例：
bool a(const string & , const string &); //a是函数名
bool (* pa)(const string & , const string &); //pa是函数指针

上述的这两个都是变量的定义语句，第一个定义的变量是a，没有实现，完整的定义实现是：
bool a(const string & , const string &){
    //。。。
}
第二个定义的变量是pa，完整的定义实现是：
bool (* pa)(const string & , const string &) = a;

2.当把函数名作为一个值使用时，该函数名自动转换为函数指针：
pa = a; //将a赋值给pa，就把a看做了一个指针
pa = &a; //等效
注意，为函数指针赋值时，要保证所赋值的参数列表类型、数量 和 返回值类型匹配。如：
int a(const string & , const string &);
就不能：pa = a;

但可以：
pa = 0;
pa = nullptr;

此外：
bool b = a("str1" , "str2");
bool c = pa("str1" , "str2");
bool c = (* pa)("str1" , "str2"); //三个等效

要注意的是，尽管把函数名作为一个值使用时，该函数名自动转换为函数指针，但函数名 和 函数指针仍有着本质区别。
就如同指针 和 数组名一样。

3.实现重载
void a(int *);
void a(unsigned int);

void (* pa)(int *) = a; //实现重载，pa指向第一个a。

4.函数指针做参数
形参：
void c(int x , bool y(const string & , const string &));
void c(int x , bool (* y)(const string & , const string &));
实参：
void c(x , pa);
void c(x , a); //传递函数名，若按值使用，则作为函数指针

5.
typedef bool fun(const string & , const string &); //定义fun变量为：bool (const string & , const string &)类型
typedef decltype(a) * fun; //等效，因为decltype返回的是a函数的类型bool (const string & , const string &)，所以要加上*才能得到一个函数指针。
typedef bool (* fun)(const string & , const string &); //等效

6.返回函数指针的函数（这里就体现出了函数名 和 函数指针 的区别）
using f = int(int * , int *); //f是函数名，而非函数指针
using pf = int(*)(int * , int *); //f是函数指针

f fun(); //错误，f不是函数指针。
pf fun(); //正确
f * fun(); //正确
*/

/*
类型别名

定义类型别名的传统方式：
typedef int x;
#define x int;

定义类型别名的C++11方式：
using x = int;
*/