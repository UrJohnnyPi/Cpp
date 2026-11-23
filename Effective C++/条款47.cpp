/*
条款47：请使用traits class表现类型信息

traits：特点、特征
 
STL共有5中迭代器：
（1）Input迭代器：只能向前移动，一次一步，客户只可读取（不能涂写）它们所指的东西，而且只能读取一次。代表：istream_iterators。
（2）Output迭代器：只能向前移动，一次一步，客户只可涂写（不能读取）它们所指的东西，而且只能涂写一次。代表：ostream_iterators。
（3）forward迭代器：能做上述两种迭代器的每一件事，且可以读或写其所指物一次以上。
（4）Bidirectional迭代器：在forward迭代器功能的基础上，还可以向后移动。代表：list、set、multiset、map、multimap的迭代器。 
（5）random access迭代器：在Bidirectional迭代器功能的基础上，还可以在O(1)时间内向前/后跳跃任意距离。代表：vector、deque、string的迭代器。 

以上这些迭代器之间的继承关系：
struct input_iterator_tag{};
struct output_iterator_tag{};
struct forward_iterator_tag : public input_iterator_tag{};
struct bidirectional_iterator_tag : public forward_iterator_tag{};
struct random_access_iterator_tag : public bidirectional_iterator_tag{};

STL提供了一个template advance函数，用来将某个迭代器移动某个给定的距离。下面，我们就来
实现这一函数。约定advance签名：
template<typename IterT , typename DistT>
void advance(IterT & iter , DistT & d); 

实现advance之前，要理解traits这一技术：一段代码符合traits，那么该代码对内置类型 和 用户自定义类型
表现的必须一样好。意即，一段代码，不仅要能处理约定类型的数据，还能处理可能出现的内置类型的数据。 
*/

/*
代码1：

//我们想要的advance的设计实现：
template<typename IterT , typename DistT>
void advance(IterT & iter , DistT d){
	if(typeid(typename iterator_traits<IterT>::iterator_category) == typeid(random_access_iterator_tag)) iter += d;
	else {
		if(d >= 0) while(d--) ++iter;
		else while(d++) --iter;
	}
}
//但这个代码（typeid-based）的效率比代码2中traits class的要低。因为：
//编译期可以获知IterT，所以iterator_traits<IterT>::iterator_category也可以在编译期具现化，但是if语句只有在运行期才会执行，
//所以这在时间上效率不高。 
*/

/*
代码2：

//traits class的设计实现：（这里的struct就被视为class）
template<typename IterT>
struct iterator_traits{
	typedef typename IterT::iterator_category iterator_category; 
	//迭代器都内含iterator_category类型，用于返回迭代器类，所以这里有IterT::iterator_category；我们自己写时，也要将将接收到的迭代器类统一重命名为iterator_category，以符合标准。 
	//do something 
};
template<typename IterT>
struct iterator_traits<IterT *>{ //template偏特化，针对迭代器指针 
	typedef random_access_iterator_tag iterator_category; 
	//因为指针的行为与random access迭代器类似，所以指定IterT *属于random_access_iterator_tag类。 
	//do something
};
//STL中deque、list的迭代器设计如附录所示。 

//traits class的使用： 
template<typename IterT , typename DistT> //这份实现用于random access迭代器 
void doAdvance(IterT & iter , DistT d , random_access_iterator_tag){
	iter += d;
}
template<typename IterT , typename DistT> //这份实现用于Bidirectional迭代器 
void doAdvance(IterT & iter , DistT d , bidirectional_iterator_tag){
	if(d >= 0){
		while(d--) ++iter;
	} else while(d++) --iter; 
}
template<typename IterT , typename DistT> //这份实现用于input迭代器 
void doAdvance(IterT & iter , DistT d , input_iterator_tag){
	if(d < 0){
		throw out_of_range("Negative distance");
	}
	while(d--) ++iter;
}

template<typename IterT , typename DistT>
void advance(IterT & iter , DistT d){
	doAdvance(iter , d , typename iterator_traits<IterT>::iterator_category()); //iterator_traits<IterT>::iterator_category()返回一个无名对象 
}

//附录
//deque
template<...>
class deque{
	public :
		class iterator{
			public :
				typedef random_access_iterator_tag iterator_category; //可随机访问 
				... 
		};
		...
}; 
//list 
template<...>
class list{
	public :
		class iterator{
			public :
				typedef bidirectional_iterator_tag iterator_category; //可双向行进 
				... 
		};
		...
};
*/
 
//该条款的含义是：
//（1）要如上述结构设计类来从该类中提供某些类型，如：deque的iterator可提供bidirectional_iterator_tag类，而这个类就是deaue<T>::iterator的类。
//（2）要统一标准，比如这里的迭代器类以iterator_category为统一的重定义类名。 
