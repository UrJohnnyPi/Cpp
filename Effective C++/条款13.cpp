/*
条款13：以对象管理资源
*/

/*
代码1：

//先看一个典型的工厂模式结构： 
class Investment {
	//do something
	virtual int howMuch();
};
StockInvestment : public Investment {
	//do something
	int howMuch(); 
};
FundInvestment : public Investment {
	//do something
	int howMuch(); 
};
Investment * createInvestment(para){ //factory function 
	if(a){
		StockInvestment si(para);
		Investment * i = & si;
		return i;
	}
	if(b){
		FundInvestment fi(para);
		Investment * i = & fi;
		return i;
	} 
}
void startFactory(para){
	Investment * i = createInvestment(para);
	//do something
	delete i; 
}
//问题出在startFactory中的do something，如果这一部分有异常、return、exit，或是
//createInvestment和delete同在一个循环内，而二者间又可能出现continue、break等，
//则极易造成资源泄漏。即使你足够认真不出错，他人也会在后期的维护中出错，因此
//要对此有一个好的设计模式。这个设计模式要做到：在i离开当前作用域时被回收。 
*/ 

/*
代码2：

//auto_ptr对象是资源管理对象。因此可以解决代码1中提及的问题。 
void startFactory(para){
	auto_ptr<Investment> i(createInvestment(para));
	//do something
} 
//使用智能指针auto_ptr，可以在其作用域结束时自动调用auto_ptr对象的析构函数确保i所指的资源被系统回收。
//auto_ptr的问题在于，绝不可能出现多个auto_ptr对象指向同一资源。因为若某一auto_ptr对象在被析构时，系统就会回收
//其所指资源，系统一旦回收，因为所有auto_ptr对象所指为同一资源，所以其他auto_ptr对象也失去了资源。
//因为这一缘故，auto_ptr的copy函数设计原理没有了原始意义：
auto_ptr<Investment> i(createInvestment(para));
auto_ptr<Investment> g(i); //g拥有了createInvestment(para)返回的资源，i=null（即资源被剥夺）。 
auto_ptr<Investment> k;
k = g; //k拥有了createInvestment(para)返回的资源，g=null。
*/ 

/*
代码3：

void startFactory(para){
	tr1::shared_ptr<Investment> i(createInvestment(para));
	//do something
}
//std::tr1::shared_ptr和std::auto_ptr的区别在于，shared_ptr可以计数有多少个
//对象指向同一资源，当指向同一资源的对象为0时，允许系统回收该资源。
//分析：
void startFactory(para){
	//do something
	tr1::shared_ptr<Investment> i(createInvestment(para));
	tr1::shared_ptr<Investment> g(i); //g和i指向同一笔资源 
	tr1::shared_ptr<Investment> k;  
	k = g; //i,g,k指向同一笔资源 
} //作用域结束，k,g,i的析构函数被依次调用，shared_ptr内置资源计数器值为0，允许系统回收该资源。 
*/

//注意，auto_ptr和shared_ptr的析构函数内部使用的是delete而不是delete[]，因此
//不能使用数组名（或数组指针）给这两种智能指针对象初始化/赋值。 

/*
代码4：

智能指针头文件：#include<memory>

auto_ptr<T> aptr(new T (para));
（1）不支持共享，即唯一占有，copy操作（copy构造 或 copy assignment）都会导致被复制对象释放对资源的控制权。
auto_ptr<T> aptr1(new T (para));
auto_ptr<T> aptr2 = aptr1; 或 auto_ptr<T> aptr2(aptr1);都使得aptr1为空。
cout << * aptr1; //错误
cout << * aptr2; //正确
但实际上，对auto_ptr对象使用move()语法来转移所有权更为妥当：aptr2(move(aptr1)); 或 aptr2 = move(aptr1);
（2）参数不能为new T []，因为其析构函数内部是delete aptr;而非delete [] aptr;
（3）辅助函数：
release()和reset()：aptr2.reset(aptr1.release()); //aptr1释放并返回原始指针，reset()实现aptr1所指资源的重置。
get()：aptr1.get(); //返回aptr1所管理的原始指针，但注意，aptr1只是返回指针，并不是释放所有权。

unique_ptr<T> uptr(new T (para));
（1）不支持共享，唯一占有：
unique_ptr<T> uptr1(new T (para));
unique_ptr<T> uptr2(move(uptr1)); 或 unique_ptr<T> uptr2 = move(uptr1); //转出控制权给uptr2
（2）参数不能为new T []。
（3）辅助函数：
release()和reset()：uptr2.reset(uptr1.release());
get()：uptr2.get();
swap()：uptr2.swap(uptr1); //交换被管资源

shared_ptr<T> sptr(new T (para));
（1）支持共享，不使用move()语法，不含release()成员函数：
shared_ptr<T> sptr1(new T (para));
shared_ptr<T> sptr2(sptr1); //sptr1和sptr2管理同一资源。
（2）参数不能为new T []。
（3）辅助函数：
swap()
get()
reset()
*/ 