/*
1.使用template和inline的组合替代#define函数，如：
	用
	template<typename T>
	inline T callWithMax(const T & a , const T & b){
		return (a > b ? a : b);
	}
	替代
	#define CALL_WITH_MAX(a,b)((a)>(b)?(a):(b))
	因为template inline具有和#define一样强大的泛化、效率（即同样能实现预编译）能力。

《Effective C++》条款02

2.模板的全特化和偏特化：
（1）网站：https://zhuanlan.zhihu.com/p/72356532

（2）模板为什么要特化，因为编译器认为，对于特定的类型，如果你能对某一功能更好的实现，那么就该听你的。
	 模板分为类模板与函数模板，特化分为全特化与偏特化。全特化就是限定死模板实现的具体类型，偏特化就是如果这个模板有多个类型，那么只限定其中的一部分。

（3）先看类模板：
	 //模板类
	 template<typename T1, typename T2>
	 class Test
	 {
		public:
			Test(T1 i,T2 j):a(i),b(j){cout<<"模板类"<<endl;}
		private:
			T1 a;
			T2 b;
	 };
 
	 //全特化
	 template<>
	 class Test<int , char>
	 {
		public:
			Test(int i, char j):a(i),b(j){cout<<"全特化"<<endl;}
		private:
			int a;
			char b;
	 };
 
	 //偏特化
	 template <typename T2>
	 class Test<char, T2>
	 {
		public:
			Test(char i, T2 j):a(i),b(j){cout<<"偏特化"<<endl;}
		private:
			char a;
			T2 b;
	 };
	 
	 那么下面3句依次调用类模板、全特化与偏特化：
	 Test<double , double> t1(0.1,0.2);
	 Test<int , char> t2(1,'A');
	 Test<char, bool> t3('A',true);

（4）而对于函数模板，却只有全特化，不能偏特化：
	 //模板函数
	 template<typename T1, typename T2>
	 void fun(T1 a , T2 b)
	 {
		cout<<"模板函数"<<endl;
	 }
 
	 //全特化
	 template<>
	 void fun<int ,char >(int a, char b)
	 {
		cout<<"全特化"<<endl;
	 }
 
	 //函数不存在偏特化：下面的代码是错误的
	 template<typename T2>
	 void fun<char,T2>(char a, T2 b)
	 {
		cout<<"偏特化"<<endl;
	 }
	 至于为什么函数不能偏特化，似乎不是因为语言实现不了，而是因为偏特化的功能可以通过函数的重载完成。

《Effective C++》条款25

3.类模板的继承
（1）
	//类模板 继承 普通类
	class A{
		public :
			A(para):data(para){}
	};
	
	template<typename T>
	class B : public A{
		public :
			B(para):A(para){}
	};
	
	//普通类继承类模板
	template<typename T>
	class A{
		public :
			A(para):data(para){}
	};
	
	template<typename T>
	class B : public A<T>{
		public :
			B(para):A<T>(para){}
	};
	
	或对T具现化：
	
	template<typename T>
	class B : public A<char>{
		public :
			B(para):A<char>(para){}
	};
	
	//类模板 继承 类模板
	template<typename T>
	class A{
		public :
			A(para):data(para){}
	};
	
	template<typename T , typename U>
	class B : public A<T>{
		public :
			B(para):A<T>(para) , var(para){}
		private :
			U var;
	};
	
	或对T具现化：
	
	template<typename U>
	class B : public A<int>{
		public :
			B(para):A<int>(para) , var(para){}
		private :
			U var;
	};

4.typename
（1）template <class T> 和 template <typename T>中，typename 和 class 没有区别，可以互相取代。
（2）编译器在编译之前，由解析器先行解析，解析器的作用旨在语法（文法）分析，即进行语法上的正确性判别。如下的代码是无法通过解析器解析环节的：
	 **********************
	 #include <iostream>
	 #include <vector>
	 using namespace std;

	 template <typename T>
	 void fun(const T & container){
		T::const_iterator iter = container.begin(); //const_iterator称为：嵌套从属名称
	 }

	 int main() {
		vector<int> v;
		fun(v);
		return 0;
	 }
	 **********************
	 原因在于：在设计之初，就假设fun要接收一个STL容器，于是，fun的内部代码都是按照这个STL容器展开的，但问题是，你没有相关的代码让编译器（解析器）也知道fun的参数
	 是一个STL容器，而解析器之所以不知道container是一个STL容器，归咎于template接收的T表示泛化的类型。如若解析器不知道T是什么，它也就无法诊断T::const_iterator iter
	 是什么语法（解析器既可以认为const_iterator是T作用域下的一个类型，也可以认为const_iterator是T的一个static等），因为语法的分析，不接受歧义。解析器针对这种
	 不确定将直接报错。而container.begin();的使用是不会导致错误的，因为container.begin();这段代码没有歧义，就是传入参数的成员函数：
	 **********************
	 template <typename T>
	 void fun(const T & container){
		container.begin(); //解析正常、（若container为STL容器，则）编译正常、（若container为STL容器，则）正确执行
	 }
	 **********************
	 要说明的是，一个注意点是：const_iterator是嵌套从属名称。所谓嵌套从属名称，就是依赖于泛化类型T的类型。嵌套从属，就是说，一环套一环、一个从属另一个的意思。
	 解决之道：
	 **********************
	 #include <iostream>
	 #include <vector>
	 using namespace std;

	 template <typename T>
	 void fun(const T & container){
		typename T::const_iterator iter = container.begin(); //加上typename
	 }

	 int main() {
		vector<int> v;
		fun(v);
		return 0;
	 }
	 **********************
	 在嵌套从属名称前加上typename，告诉解析器：这里的T::const_iterator是一个类型。
	 不使用typename的两个地方：
	 **********************
	 template<typename T>
	 class Derived : public Base<T>::Nested{ //1.继承处不能用typename声明Base<T>::Nested是类型 
		public :
			explicit Derived(int x) : Base<T>::Nested(x){ //2.初始化列表中不能用typename声明Base<T>::Nested是类型
				typename Base<T>::Nested temp; //嵌套从属名称，要使用typename声明类型。 
			}
	 }; 
	 **********************
	 这里的Base<T>::Nested中Nested是一个嵌套类，详见《union、enum、class、struct的一些概念》class部分，注意（19）。

5.类模板中的同名隐藏举例
	 **********************
	 #include <iostream>
	 using namespace std;
	 template<typename T , typename U>
	 class A {
		public :
			void test() {}
	 };

	 template<typename T>
	 class A<T , int> {
		public :
			void test() {}
	 };

	 template<>
	 class A<char , char> {
		public :
			void test() {}
	 };

	 template<typename T , typename U>
	 class Derived : public A<T , U>{
		public :
			//法一：using A<T , U>::test;
			void Test(){
				//法二：A<T , U>::test();
			}
	 };

	 int main() {
		Derived<int , char> d;
		d.Test();
		return 0;
	 }
	 **********************

6.template接收的参数既可以是类型，也可以是数据，如：
	 **********************
	 #include <iostream>
	 using namespace std;

	 template<class T , int x> //同时接收类型T和数据x
	 class A {
		public :
			A(T) {
				cout << x;
			}
	 };
	 
	 int main() {
		A<int , 7> a(9);
		return 0;
	 }
	 **********************
	 但极力反对这样的写法，而要求template只接收类型参数。因为：
	 （a）对仅以类型为参数的class/function template的具现化不会导致代码膨胀。即对于template<class , class> class A，只要传入的类型、顺序不变比如（char , int），则所有的A<char,int>对象只需要一个A<char,int>具现化副本（就是说，新定义A<char,int>对象时，不需要重新开辟内存生成A<cahr,int>类了）。
	 （b）对含非类型参数的class/function template的具现化导致代码膨胀。即对于template<class , int> class A，即使每次传入（char , int），每传入一个不同的int值，就开辟新空间具现化一个新类型，代码膨胀。 

《Effective C++》条款42
*/