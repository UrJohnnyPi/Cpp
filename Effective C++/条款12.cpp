/*
条款12：赋值对象时勿忘其每一个成分

（1）copy构造函数用于初始化对象，构建一个本不存在的对象，A b(a);调用copy构造函数，
而A b;b(a);报错。
	copy assignment用于为一个已初始化的对象赋值。A b;b=a;。
（2）copy构造函数与copy assignment代码有相似的地方，但并不是说为了避免代码重复而
可以允许copy构造函数调用copy assignment，因为一个对象尚未创建完成，就对其进行赋值
这类本应作用于已初始化对象上的操作，是不适宜的；同理，copy assignment不应调用
copy构造函数，一个已初始化的对象去调用用来创建新对象的函数，同样不适宜。
（3）为避免代码膨胀，可以在private下定义一个init()来存放copy中重复的代码。 
*/

class PriorityCustomer : public Customer{
	public :
		//do something 
		PriorityCustomer(const PriorityCustomer & rhs);
		PriorityCustomer & operator = (const PriorityCustomer & rhs);
	private :
		int priority;
};
PriorityCustomer::PriorityCustomer(const PriorityCustomer & rhs) : Customer(rhs) , priority(rhs.priority){ //注意对base copy构造函数的调用。 
	//do something
}
PriorityCustomer & PriorityCustomer::operator = (const PriorityCustomer & rhs) {
	Customer::operator = (rhs); //注意对base operator=的调用；copy函数和构造函数一样符合回字形从内到外的赋值（构造）。 
	priority = rhs.priority;
	return * this; 
} 
