/*
条款15：在资源管理类中提供对原始资源的访问

（1）资源管理类应当满足这样的设计，其定义的对象能够通过访问某一函数成员直接
返回所管理的资源。原因在于许多API都要直接指涉资源，你还不得不使用这些API，因此
要求管理对象有能力：
返回指向资源的指针（即用于初始化资源管理对象的指针）——显式转换
返回资源的内容——隐式转换 

（2）对于auto_ptr和tr1::shared_ptr等智能指针，以条款13中的Investment为例，这里的资源是Investment对象。 
显示转换：（通过get()显式转换） 
void startFactory(para){
	tr1::shared_ptr<Investment> i (createInvestment(para));
	//do something
	daysInvestment(i.get()); //智能指针类提供了一个get()函数，使得shared_ptr或auto_ptr对象obj有：obj.get()返回原始资源（指针）。 
}
void daysInvestment(const T * di){ //一个需要原始资源的API 
	//do something
}

隐式转换：（通过运算符重载转换） 
class Investment{
	public :
		bool isTaxFree() const;
		//do something
};

Investment * createInvestment(para){
	//factory function , return a specific Investment pointer
}

void startFactory(para){
	tr1::shared_ptr<Investment> i (createInvestment(para));
	//do something
	bool ifTaxFree_1 = i->isTaxFree(); //智能重载了operator ->
	bool ifTaxFree_2 = (* i).isTaxFree(); //智能重载了operator *
}

（3）自定义资源管理类中的原始资源访问函数的设计：
显式转换：
template<typename T>
class Investment{
	public :
		T * get() const { //显示转换函数 
			return resourcePtr;
		}
		//do something
	private :
		T * resourcePtr;
};

Investment * createInvestment(para){
	//factory function , return a specific Investment pointer
}

void startFactory(para){
	Investment * i (createInvestment(para));
	//do something
	daysInvestment(i->get()); //显式转换
	delete i; 
}

void daysInvestment(const T * di){ //一个需要原始资源的API 
	//do something
}

隐式转换：
template<typename T>
class Investment{
	public :
		operator T * ()(Investment *) const{ //隐式转换函数，是对T * (Investment *)的重载。 
			return resourcePtr;
		}
		//do something
	private :
		T * resourcePtr;
};

Investment * createInvestment(para){
	//factory function , return a specific Investment pointer
}

void startFactory(para){
	Investment * i (createInvestment(para));
	//do something
	
	//以下两种对operator T * ()的使用方式都是隐式转换，Investment * 转 T *： 
	
	//operator T * ()使用方式1：函数传递 
	daysInvestment(i);
	
	//operator T * ()使用方式2：typedef将T *转换为一个符号cx 
	typedef T * cx; //必须有typedef，不然会直接T * (i)报错。 
	cx(i);
	
	//隐式转换是不安全的但方便的；显示转换是安全的，但不方便的。隐式转换的写法易出错。 
	//显式转换是主流。 
	
	delete i;
}

void daysInvestment(const T * di){ //一个需要原始资源的API 
	//do something
} 
*/


/*
附录代码：
#include <iostream>
using namespace std;

template<typename T>
class Investment {
	public :
		Investment(T * ptr):resourcePtr(ptr) {
			
		}
		operator T * () const { //这里的重载符号是：T * ； 括号用于接收参数，这里无参 ； 这里无返回值，要记住，很特殊。
			return resourcePtr;
		}
	private :
		T * resourcePtr;
};

void f(int * ){
	cout << "I am here.";
}

int main() {
	int a = 123;
	int * p = &a;
	Investment<int> i(p);
	
	//operator T * ()重载的使用一： 
	f(i);
	
	//operator T * ()重载的使用二：
	typedef int * cx;
	cout << * cx(i);
	 
	return 0;
} 
*/ 
