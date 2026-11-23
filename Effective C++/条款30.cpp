/*
条款30：透彻了解inline的里里外外

inline：将“对此函数的每一个调用”都以函数本体替换之。
inline是对编译器的申请。分为隐式和显式。隐式就是在class中定义函数（不是声明，定义要包括函数头和函数体）。
显式是在函数前加inline关键字。
要将inline用于修饰小型、频繁使用的函数。 

并不是声明（隐式、显式）为inline的函数就一定会真按照inline规则执行。是否能够inline，取决于编译器。因为
inline操作总体上会造成程序体积的膨胀，导致内存频繁的换页行为。 
不被编译器允许内联的几种情况： 
1.包含了递归、循环等结构的函数。
2.虚函数一般不会内联，因为virtual意味着在运行期才确定调用哪个函数。
3.如果通过函数指针调用内联函数，那么该函数将不会内联而是通过call进行调用。
4.构造和析构函数一般会生成大量代码，因此一般也不适合内联。
5.如果内联函数调用了其他函数也不会被内联。
6.频繁使用、可能需要维护修改的库函数也不应当声明为inline。试想，若根据某个库函数写了
许多客户软件，这些客户软件是已编译好的exe，若该函数是non-inline的，在只要接口没变，客户
软件就不存在问题；若该函数是inline的，则所有软件代码需要重新编译，将该函数以本体的形式编到程序中。 

解释3：
inline void f(){...}
void (* pf)() = f;
...
f(); //若编译器有意愿对f内联，则f将被实际上inline 
pf(); //pf函数不允许被inline
 
解释4：
class Base{
	public :
		Base(){}
	private :
		string bm1 , bm2;
};
class Derived : public Base{
	public :
		Derived(){}
	private :
		string dm1 , dm2 , dm3;
};
看起来这里的Derived()函数体为空，似乎可以inline。但其实，构造函数和析构函数的基本功能是由编译器
对其插入代码而实现的。所以，真正的Derived()（即编译器编译过后的）应该类似于下面这样：
Derived::Derived(){
	Base::Base();
	try { dm1.string::string(); }
	catch(error){
		Base::~Base(); //若构造抛出异常，则要将已构造好的部分析构。保证程序可以回到构造前的状态。 
	}
	try{ dm2.string::string(); }
	catch(error){
		dm1.string::~string();
		Base::~Base();
	}
	try{ dm3.string::string(); }
	catch(error){
		dm2.string::~string(); //回字形。由内到外构造，由外到内析构。 
		dm1.string::~string();
		Base::~Base();
	}
}
试想若Base()和string()也被允许inline，那么这里的Base()和string()将也会以本体展开，代码
膨胀程度不可想象。
对于析构函数，和构造函数情况类似，也不建议inline。
*/
