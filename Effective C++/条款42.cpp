/*
条款42：了解typename的双重意义
*/

/*
代码1：

template<class T> class Widget; 
template<typename T> class Widget;
//template< >中，使用class和使用typename没有什么不同。 
*/

/*
代码2：

//旨在设计一个函数，可以接受STL容器，输出第二个元素。其中，该容器存储int值。 
template<typename C>
void print2nd(const C & container){
	if(container.size() >= 2){
		C::const_iterator iter(container.begin());
		++iter;
		int value = * iter;
		cout << value;
	}
} 
//此函数提出的两个概念：
//（1）template function/class 内出现的名称如果依赖于某一template参数，则称该名称为“从属名称”。
//若从属名称在template function/class作用域内的域内，则称为“嵌套从属名称”。这里的iter就是嵌套
//从属名称。嵌套从属名称可能导致编译器解析出错。 
//（2）这里的value是int型，不依赖于任何template参数，故称该名称为“非从属名称”。
 
template<typename C>
void print2nd(const C & container){
	if(container.size() >= 2){
		C::const_iterator iter(container.begin());
		//编译器总在编译前先使用解析器解析。编译器在解析到这里时，是不知道const_iterator是什么的，
		//因为，你这里是预想的要传入STL容器，但没有代码保证解析器也可以预想到这样的输入。于是，C++
		//默认的规则是：认定嵌套从属名称不是一个类型。 也就是说，解析器默认不认为const_iterator是C（STL） 
		//容器的常迭代器类型，而只认为它是一个普通的符号。这时，该句的含义完全变了，解析器根据作用域分辨符
		//判断const_iterator是一个C类中的static，但是C又未知，其static怎么可以提前获知？所以报错。
	}
}

template<typename C>
void print2nd(const C & container){
	if(container.size() >= 2){
		typename C::const_iterator iter(container.begin()); 
		//这样修改了，解析器就知道C::const_iterator是个类型了。 
	}
} 
*/

/*
代码3：（该代码将指出两种不能用typename声明为类型的情况） 

template<typename T>
class Base{
	public :
		//将Nested定义为一个类 
};
template<typename T>
class Derived : public Base<T>::Nested{ //1.继承处不能用typename声明Base<T>::Nested是类型 
	public :
		explicit Derived(int x) : Base<T>::Nested(x){ //2.初始化列表中不能用typename声明Base<T>::Nested是类型
			typename Base<T>::Nested temp; //嵌套从属名称，要使用typename声明类型。 
		}
}; 
*/

/*
代码4：

//企图输入一个STL容器的iterator 
template<typename IterT>
void workWithIterator(IterT iter){
	typename iterator_traits<IterT iter>::value_type temp(* iter);
	//这个语法的用法是，如果IterT = vector<int>::iterator，则temp就是int型。
	//可以通过：typedef typename iterator_traits<IterT iter>::value_type value_type;来使用短符号value_type指代该类型。 
}  
*/

