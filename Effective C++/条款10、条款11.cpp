/*
条款10：令operator=返回一个reference to * this
条款11：在operator=中处理“自我赋值”
*/

/*
条款10：

class Widget{
	public :
		//do something
		Widget & operator = (const Widget & rhs){
			//do something
			return * this;
		}
		Widget & operator += (const Widget & rhs){ //任何赋值相关的重载均满足条款10
			//do something
			return * this;
		}
	private :
		char * ptr;
};
*/

/*
条款11：
在“C++基本算法/运算符重载_2”中，operator=是这样的形式：
Widget & operator = (const Widget & rhs) {
	if(this == & rhs) return * this; //为了解决自我赋值的问题，即若this == & rhs，则delete this->ptr等于delete rhs.ptr。 
	delete this->ptr;
	this->ptr = new char(* rhs.ptr);
	return * this;
}
这样做有一个缺点，就是若new产生异常，那么该代码直接退出，而this->ptr=nullptr。

于是修改为：
Widget & operator = (const Widget & rhs) {
	char * chPtr = this->ptr;
	this->ptr = new char(* rhs.ptr);
	delete chPtr;
	return * this;
}
分析这段代码：
（1）自我赋值，没问题。
（2）若new异常，代码退出后，this->ptr保持原值。 
*/

