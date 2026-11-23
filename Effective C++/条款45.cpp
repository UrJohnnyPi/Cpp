/*
条款45：运用成员函数模板接受所有兼容类型
*/

/*
代码1： 

template<typename T>
class SmartPtr{
	public :
		template<typename U>
		SmartPtr(const SmartPtr<U> & other); //泛化copy构造函数 
}; 
SmartPtr<Y> spu;
SmartPtr<X> spt(spu); //调用copy构造函数
*/ 

/*
代码2：

//对于： 
class Base{}; 
class Derived : public Base{};
//我们希望一个Derived指针可以转换为一个Base指针，也就是说，一个Derived指针/引用可以赋值给一个
//Base指针而不是相反。
//所以，我们可以基于这样的考虑完善代码1：
template<typename T>
class SmartPtr{
	public :
		template<typename U>
		SmartPtr(const SmartPtr<U> & other) : heldPtr(other.get()){ //该代码做到了在类型兼容时才能通过编译 
		}
		
		template<typename U>
		SmartPtr & operator=(const SmartPtr<U> & other){
			heldPtr = other.get();
		}
		
		T * get(){
			return heldPtr;
		}
	private :
		T * heldPtr;
}; 
*/

/*
代码3：

//在class中声明了template copy构造函数 和 template copy assignment，并不意味着class中
//就自动获得了“正常的”non-template copy函数。而编译器正是要根据class中是否有non-template
//copy函数来自动为之生成default copy函数。意即，如若想阻止默认行为，就要在声明template copy
//的同时，声明non-template copy。
//基于此，修改代码如下：
 
template<typename T>
class SmartPtr{
	public :
		SmartPtr(const SmartPtr & other); //non-template copy构造函数，注意没有<>。 
		template<typename U> //template copy构造函数 
		SmartPtr(const SmartPtr<U> & other) : heldPtr(other.get()){ 
		}
		
		SmartPtr & operator=(const SmartPtr & other);
		template<typename U>
		SmartPtr & operator=(const SmartPtr<U> & other){
			heldPtr = other.get();
		}
		
		T * get(){
			return heldPtr;
		}
	private :
		T * heldPtr;
}; 
*/
