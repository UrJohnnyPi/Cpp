/*
条款07：为多态基类声明virtual析构函数

（1）如果class不含virtual函数，通常表示它并不意图被用作一个base class。
（2）
虚函数表：
欲实现出 virtual 函数，对象必须携带某些信息，主要用来在运行期决定哪一个virtual函数该被调用。
这份信息通常是由“虚函数表指针”vptr(virtual table pointer)指出。vptr指向一个由
函数指针构成的数组，称为vtbl(virtual table)即虚函数表：每一个带有virtual函数的class都有一个
相应的vtbl。当对象调用某一virtual函数，实际被调用的函数取决于该对象的vptr所指的那个
vtbl——编译器在其中寻找适当的函数指针。
（3）
如下代码：
class Point{
	public :
		Point(int x , int y);
		virtual ~Point();
	private :
		int x , y;
};
假设不加virtual关键字，Point对象在64位机中占4B + 4B = 64bit，即两个int数据成员的空间。
加上virtual后，Point对象在64位机中占4B + 4B + 8B = 128bit，后一个8B是vptr的空间。

因此，并不是将所有class的析构函数一味地声明为virtual，只有当其被继承时才声明。
（4）所有的STL中的容器和标准string的析构函数都是non-virtual，因此都不适合被继承。
（5）类分为以下几种：
不用作base class的：如STL中的容器和标准string。
用作base class但不用作多态：如条款06代码3中的Uncopyable。
用作多态：析构函数声明为virtual。

多态的核心：通过base class接口处理derived对象，即base指针指向derived对象的引用。
其现实意义是：客户给你某些参数，你要根据这些参数去选定原子钟、水钟还是普通钟为
客户返回时间。对所有客户来说，他们都操作的是TimeKeeper类，TimeKeeper却实际上处理
了各种不同的derived对象。如下：
class TimeKeeper{
	//do something
	virtual string time();
};
class AtomicClock : public TimeKeeper{
	string time(); //重写了基类中的time() 
};
class WaterClock : public TimeKeeper{
	string time();
};
class WristClock : public TimeKeeper{
	string time();
};

TimeKeeper * createClock(para){
	//这是很重要的一个函数，称之为factory function，之所以重要是因为
	//所有用户都可以通过这个函数获得被不同derived引用赋值的TimeKeeper指针。
	//即所有用户均可通过一个接口，获取不同的derived的virtual服务。
	switch(para){
		case about AtomicClock :
			AtomicClock ac(para);
			TimeKeeper * c = & ac;
			return c;
		case about WaterClock :
			WaterClock wc(para);
			TimeKeeper * c = & wc;
			return c;
		//others 
	}
}

void startFactory(para){ //Factory函数仅用于提供不同的TimeKeeper对象，因此要有这个startFactory来实现对取得对象的具体操作。（当然也不是绝对的） 
	TimeKeeper * cptr = createClock(para);
	//do something
	delete cptr; 
}
（6）
class AWOV{
	public :
		virtual ~AWOV() = 0;
};
AWOV::~AWOV(){}
当你想得到一个纯虚函数，为什么？因为抽象类不能实例化，只能被继承并实现。
而你需要一个抽象类作为泛化接口。 
此时在没有其他纯虚函数的情况下，可以如上把析构函数声明为纯虚函数，但在类外还要
给出析构函数的定义式，不然derived类析构到AWOV部分时，编译器会创建一个对
~AWOV的调用动作，而因没有函数体导致连接器报错。

这种设计其实还体现出了一个有趣的事实：类中的虚函数声明式只是告知编译器该类是一个
抽象类，无法实例化，只能被继承，且这种抽象特性也会被继承。但“类中放置的只是函数的
声明式”这一事实也告知了编译器~AWOV()的定义式在类外，而一个函数的定义式必然是包括函数体
在内的。所以编译器同时也知道~AWOV()是一个完整的函数。至于连接器，其只管将符号和定义
连接起来。所以，基于此，这里必然也要有函数体。

注意，类的虚函数都必须是public下的，否则：
若该类不被继承，则其对象因无权调用private成员而无法被析构，造成内存泄漏。
若该类被继承，则其派生类因无权调起基类中的private成员而导致派生类对象的基类部分无法被析构，造成资源泄漏。
*/


