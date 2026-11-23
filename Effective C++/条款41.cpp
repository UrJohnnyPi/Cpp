/*
条款41：了解隐式接口和编译期多态
*/
class Widget{
	public :
		Widget();
		virtual ~Widget();
		virtual sie_t size() const;
		virtual void normalize();
		void swap(Widget & other);
}; 

//实例分析如下： 

void doProcessing(Widget & w){
	if(w.size() > 10 && w != someNastyWidget){
		Widget temp(w);
		temp.normalize();
		temp.swap(w);
	}
}
//显式接口:
//因w被声明为Widget，所以w必须支持Widget接口。该接口由Widget源码中所有public成员的声明部分构成（若是
//函数，就是签名部分；若是变量就是声明部分）。该接口可明确从源码中找出，因此称为显式接口。 

//运行期多态：
//因Widget的一些成员函数是virtual，编译器将在运行期根据w的动态类型决定究竟调用哪个函数。 
//编译期，编译器只是去看符号表里的符号使用是否正确。比如编译到某一virtual funName时，编译器
//是去符号表里看看该funName是否存在？如何使用？等，而不去理会virtual关键字。 
 
template<typename T>
void doProcessing(T & w){
	if(w.size() > 10 && w != someNastyWidget){
		Widget temp(w);
		temp.normalize();
		temp.swap(w);
	}
}
//隐式接口：
//w必须支持的接口是由代码中w执行的操作决定的。比如这里，初次看，w执行了size()，因此
//size()应当是其隐式接口的一部分；w执行了!=，所以 operator !=似乎也应当是其隐式
//接口的一部分；同理，w的隐式接口似乎还应当包括Widget copy构造，normalize()和swap()。
//这里一直在用“初次看、似乎”这些不确定的词语，原因在于，以operator !=为例，T不需要
//支持operator !=，因为下述的情况也是可以的：“X类型的对象 operator != Y类型的对象”，而w可以
//转换为X类型，someNastyWidget可以转换为Y类型。这样，operator !=实际上就不是T类型提供
//的接口成分，而是X类型提供的接口成分，但却因为类型转换使得T类型对象能够访问X类型的接口成分。
//我们可以尝试探讨隐式接口的所有成分，但要知道的是，不论隐式接口还是显式接口，其检查都是在
//编译期完成的。

//编译期多态：
//编译期，templates参数具现化了function templates，从而导致不同的函数被调用，就称为编译期多态。
//比如，int 对象 或 double 对象 的不同输入，确定了不同的template function templates，进而导致
//不同的函数被调用。 
