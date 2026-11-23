/*
条款38：通过复合塑模出has-a或“根据某物实现出”

复合 = 内嵌 = 组合
组合（复合）类：
class Address {}; 
class PhoneNumber {};
class Person{
	public :
		//do something
	private :
		string name; //内嵌对象 
		Address address;
		PhoneNumcer mobileNumber; 
};

根据条款32，public继承是“is-a”的含义。
复合则是“has-a”（有一个）的含义 或  “根据某物实现出”的含义。

1）hsa-a关系体现在：需要通过程序中的组合类对象对现实对象进行模拟时。比如Person这个类，
has a 内嵌对象，叫做name；has a 内嵌对象，叫做address；等等。这些内嵌对象和其他成员构成 
组合类，完成了对现实的模拟。组合类、内嵌对象都是现实的映射。

2）“根据某物实现出”的关系，通过下例说明：
//我们试图通过STL中的list来自己写一个set：
template<typename T>
class Set : public list<T>{
	//do something 
}; 
//这是错误的，如果是public继承，就要满足set is a list的关系。但实际上，list的机制要保证
//list可以存在重复元素，而set应该避免重复元素，因此set和list不是is a的关系。
template<typename T>
class Set {
	public :
		bool member(const T & item) const;
		void insert(const T & item);
		void remove(const T & item);
		//others 
	private :
		list<T> rep;
};
//这就可以了，这个含义就是set对象可根据一个list对象实现出。 
*/
