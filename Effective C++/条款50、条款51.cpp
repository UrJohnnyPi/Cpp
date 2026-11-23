/*
条款50：了解new和delete的合理替换时机
条款51：编写new和delete时需固守常规
*/

//operator new的一般写法
void * operator new(size_t size) throw(bad_alloc) {
	if(size == 0) size = 1; //将0-byte申请视为1-byte申请
	while(true) { //这个循环在条款49中提及，即：如果内存分配失败，则一直调用new_handler，直至分配成功。 
		//于此分配size bytes
		if(/*分配成功*/) return ptr; //ptr指向分配得来的内存

		new_handler globalHandler = set_new_handler(0); //要铭记：new_handler是一个函数指针（指向函数的指针）的别名
		set_new_handler(globalHandler); //设定new_handler函数是：set_new_handler(0)，即抛出异常

		if(globalHandler) (* globalHandler)(); //内存分配失败，且new_handler函数装载成功，则激活globalHandler
		else throw bad_alloc(); //存分配失败，但new_handler函数装载失败，抛出异常。
	}
}
//operator new的class写法
class Base {
	public :
		static void * operator new(size_t size) throw(bad_alloc) {
			if(size != sizeof(Base)) return ::operator new(size);
			//这条语句是为了同时解决“继承问题”和“申请byte为0”而提出的。
			//（1）该语句被继承到Derived中，意即Derived * pD = new Derived;将调用这个
			//继承而来的函数。所以Base中定义的这个new应当也能为Derived服务。该语句译为：
			//如若new的类型不是Base，则调用标准的operator new开辟size大小的空间并返回指针。
			//（2）该语句之所以能解决“申请byte为0”的问题，是因为如果size=0，则size != sizeof(Base)
			//一定成立，因为Base一定不为0byte。

			//do others
		}
};
class Derived : public Base {
		//do something
};

//operator delete的一般写法
void operator delete(void * rawMemory) throw() {
	if(rawMemory == 0) return; //被删的是一个nullptr，则什么也不做
	//于此释放内存
}
//operator delete的class写法
class Base {
	public :
		static void * operator new(size_t size) throw(bad_alloc);
		static void operator delete(void * rawMemory , size_t size) throw() {
			if(rawMemory == 0) return;
			if(size != sizeof(Base)){
				::operator delete(rawMemory); //等价于标准的：delete rawMemory;
				return;
			}
			//于此释放内存
			return; 
		}
};
