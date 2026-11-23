/*
条款49：了解new-handler的行为

new-handler的定义式：
namespace std{
	typedef void (* new_handler)();
	new_handler set_new_handler(new_handler p) throw();
}
分析：new_handler是一个函数指针（指向函数的指针）的别名，该函数不接收参数，无返回值。set_new_handler
则是接收一个new_handler型参数，返回一个new_handler型值的、nothrow函数。nothrow表示该函数保证无异常抛出
（条款29）。

new-handler的用途：
当使用new（或称operator new）开辟空间，出现错误时，C++指出可以先调用客户指定的异常处理函数，接口就是
set_new_handler()。

应用举例：
void outOfMem(){
	std::cerr << "Unable to satisfy request for memory.";
	std::abort();
}
int main(){
	std::set_new_handler(outOfMem); //定义new_handler，并实现set_new_handler。 
	int * pMem = new int[1000000000];
	return 0;
}
当本例中的operator new发生错误时，set_new_handler就会被自动调用。
若operator new频繁发生错误，set_new_handler就会被频繁调用，直到new语句申请到足够内存。

上面的发生错误 不同于 抛出异常，抛出异常就是执行内置的或自定义的异常处理函数。这里的发生错误，
但还没有处理错误，而处理错误，既可以用抛出异常的方法，也可以用本条款的new_handler方法。

设计一个良好的new_handler函数要做的事情：
（1）new_handler要能释放已申请到的无用内存，这能提高之后operator new的成功率。
（2）如果当前的new_handler无法获取到更多可用内存，它有能力通过set_new_handler重新安装一个更有效的new_handler，然后把职能托付于它。
子new_handler函数内部可以修改一些能够对父new_handler产生影响的数据（static、global、namespace），以影响父new_handler进程。
（3）如：set_new_handler(nullptr);，向set_new_handler传递null，是对new_handler的卸载，表示客户允许在operator new产生错误时，使用抛出异常
的方式。 
*/
class NewHandlerHolder{
	public :
		explicit NewHandlerHolder(new_handler nh):handler(nh){}
		~NewHandlerHolder(){
			set_new_handler(handler);
		}
	private :
		new_handler handler;
		NewHandlerHolder(const NewHandlerHolder &); //阻止copy 
		NewHandlerHolder & operator=(const NewHandlerHolder &);
};

template<typename T>
class NewHandlerSupport{
	public :
		static new_handler set_new_handler(new_handler p) throw(){ //实体化的set_new_handler 
			new_handler oldHandler = currentHandler;
			currentHandler = p;
			return oldHandler;
		}
		static void * operator new(size_t size) throw(bad_alloc){ //重写的operator new，若执行出错，则抛出bad_alloc异常 
			NewHandlerHolder h(set_new_handler(currentHandler));
			return ::operator new(size); //表示返回标准new的处理结果 
		}
	private :
		static new_handler currentHandler;
};
template<typename T>
new_handler NewHandlerSupport<T>::currentHandler = 0;

void outOfMem(){
	//new_handler函数 
}

class Widget : public NewHandlerSupport{
	//do something
};

Widget::set_new_handler(outOfMem); //设定Widget的专属new_handler为outOfMem 
Widget * pw1 = new Widget; //如果内存分配失败，调用outOfMem
Widget::set_new_handler(0); //设定Widget的专属new_handler为null 
Widget * pw2 = new Widget; //如果内存分配失败，抛出异常
