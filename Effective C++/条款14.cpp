/*
条款14：在资源管理类中小心copying行为

（1）条款13中提出了将智能指针对象作为资源管理对象处理资源的方法。在
实际运用中，我们可能需要自己写资源管理类来管理资源，这个类要满足以下
两个守则：
a.资源在该类对象构造期间获得。
b.资源在该类对象析构期间释放。

简单模拟tr1::shared_ptr：
#include <unordered_map>
using namespace std;
template<typename T> 
class shared_ptr{
	public :
		explicit shared_ptr(T * ptr):resorcePtr(ptr){
			if(resorceCount.find(ptr) == resorceCount.end()) resorceCount.emplace(resorcePtr , 0);
			++resorceCount[resorcePtr];
		}
		shared_ptr(const shared_ptr & rhs):resorcePtr(ptr){
			++resorceCount[resorcePtr];
		}
		shared_ptr & operator = (const shared_ptr & rhs){
			resorcePtr = ptr;
			++resorceCount[resorcePtr];
		}
		~shared_ptr(){
			--resorceCount[resorcePtr];
			if(!resorceCount[resorcePtr]) {
				resorceCount.erase(resorcePtr);
				delete resorcePtr;
			}
		}
	private :
		T * resorcePtr;
		static unordered_map<T * , int> resorceCount;
};
unordered_map<T * , int> shared_ptr::resorceCount;

（2）C++中的explicit关键字只能用于修饰只有一个参数的构造函数, 
它的作用是表明该构造函数参数传递方式是显式的, 而非隐式的；跟它相对应的另一个
关键字是implicit, 表明该构造函数参数传递方式是隐式的,构造函数默认情况下是implicit(隐式)。
例如：
class A{
	public :
		A(T para){} //只有一个参数的构造函数，默认的参数传递方式是implicit。 
};
A a(para); //OK，显式 
A a=para; //OK，隐式 

class A{
	public :
		explicit A(T para){} //只有一个参数的构造函数，声明其参数传递方式是explicit。 
};
A a(para); //OK，显式 
A a=para; //wrong，禁止隐式

（3）自写的一个资源管理类：
class Lock{
	public :
		explicit Lock(Mutex * ptr):mutexPtr(ptr){ lock(mutexPtr); }
		~Lock(){ unlock(mutexPtr); } 
		//这里不同于智能指针的地方是每个资源管理对象在作用域结束时unlock资源，
		//而不是让系统回收资源，即使没有资源对象指向该资源。 
	private :
		Mutex * mutexPtr;
};
该资源管理类的问题在于没考虑copy的问题。对于copy问题，有以下四种处理方式：
a.禁止复制：
class Lock : private Uncopyable{
	//do something
};

b.资源引用计数，即类似于tr1::shared_ptr：
class Lock{
	public :
		explicit Lock(Mutex * ptr):mutexPtr(ptr , unlock){}
	private :
		tr1::shared_ptr<Mutex> mutexPtr;
};
当Lock构造函数被调用时，对象成员mutexPtr调用shared_ptr构造函数，赋值ptr和unlock。
ptr指向资源，unlock声明资源回收方式为unlock（shared_ptr的第二个对象传入一个函数名，
如若不传，则按照默认的delete方式回收）。
当Lock析构函数被调用时，对象成员mutexPtr调用shared_ptr析构函数，shared_ptr析构函数
根据内置的资源引用计数器来决定是否回收资源。

c.深复制（资源拷贝）

d.资源转移，类似于auto_ptr机制 
*/ 

