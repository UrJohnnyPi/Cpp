/*
static 和 const 
*/ 

/*
static：

1.static分类：
	local static：（有static修饰符显式声明）
	函数内的、使用static关键字修饰的对象（即变量，实际上变量即对象）称为local static。
	local static 变量的生命周期从定义语句第一次出现始到代码全部执行完毕后结束；local static 变量的作用域从函数中定义的那行起到函数结尾。 
	（所谓“到代码全部执行完毕后结束”，就是指所在源文件执行结束）
	
	non-local static：（可以没有static修饰符显式声明）
	global对象、namespace内定义的对象、class内定义的对象、file内定义的对象称为non-local static。
	non-local static 的生命周期和作用域都是从定义的那行起到所在域的域内代码执行完毕后结束。
	
	重点：static变量 和 全局变量的区别：
	全局变量本身就是静态存储方式， 静态全局变量当然也是静态存储方式。 这两者在存储方式上并无不同。这两者的区别在于非静态全局变量的作用域是整个源程序，
	当一个源程序由多个源文件组成时，非静态的全局变量在各个源文件中都是有效的。 而静态全局变量则限制了其作用域， 即只在定义该变量的源文件内有效，
	在同一源程序的其它源文件中不能使用它。由于静态全局变量的作用域局限于一个源文件内，只能为该源文件内的函数公用，因此可以避免在其它源文件中引起错误。
	
2.注意点
	（1）企图使用extern将变量的作用域扩展到外部模块，要注意的是，要将意欲扩展到外部使用的non-local static 转换为 local static。具体来说就是：
	****************
	extern.cpp
	
	int x = 5;
	****************
	main.cpp
	
	#include <iostream>
	using namespace std;
	extern int x;
	
	int main
	{
		cout << x;
		return 0;
	}
	****************
	
	改为：
	****************
	extern.cpp
	
	int x = 5;
	extern int & fun(){ //注意这里通过将extern var/obj 改为 extern T & fun()来实现将non-local static 转换为 local static。
		static int X = x;
		return X;
	}
	****************
	main.cpp
	
	#include <iostream>
	using namespace std;
	extern int & fun();
	
	int main
	{
		cout << fun();
		return 0;
	}
	****************
	这种修改方法是因为：C++不保证不同编译单元中的non-local static的初始化顺序。
	对于第一个例子，可能存在这样的情况：即编译器首先编译的是main.cpp，这时就会因首次遇到符号extern x而去其他编译单元中寻找x定义式。当找到x的定义式时（可以有extern 也可以没有，因为non-const和non-static函数和变量本身就具有外部链接属性），
	仅 执行x的定义式，这就使得若x是一个obj，编译器将只知道x是一个类对象，而不能获值其中的成员函数等内部细节。
	
	基于上述这个原因，只要将extern local var/obj改为extern & fun(){ return static var/obj}就一定能获得不会出错，且同等效用的符号。
	
	（2）static函数成员不能引用普通函数成员，因为若在定义对象前，使用类名来访问static函数成员，那static函数成员中的普通成员函数将属于哪一对象？
		 static函数成员可以被其他普通函数成员引用，但不建议，因为static函数成员是属于类的性质，而不应当交由对象（及其普通成员函数）调用。
		 static数据成员可被普通成员函数调用。在定义对象前，只能通过static函数成员来访问static数据成员。
	
	《Effective C++》条款04
*/

/*
const：

1.定义
	声明一个变量、对象、函数为常量。

2.应用
	（1）
	char p [] = "love";
	const char * ptr = "love"; //non-const ptr , const data
	char * const ptr = p; //const ptr , non-const data
	const char * const ptr = "love"; //const ptr , const data
	
	（2）STL相关的const迭代器
	const vector<int>::iterator iter = v.begin(); //const iter , non-const data
	* iter = 5; //正确
	++ iter = 5; //错误
	
	vector<int>::const_iterator iter = v.begin(); //non-const iter , const data
	* iter = 5; //错误
	++ iter = 5; //正确
	
	（3）const fun域内，当前对象也成为常对象，this成为常指针。常成员函数中不支持对常指针指向的数据成员直接修改，因此若想在常成员函数中修改数据成员，要么使用const_cast来转换this，要么使用mutable修饰数据成员。
	#include <iostream>
	using namespace std;
	class A{
		public :
			void setX(int x){
				this->x = x;
			}
			void printX() const{
				//this->x --;报错：x readonly 
				const_cast<A *>(this)->x --;
				cout << x;
			}
		private :
			int x;
	};
	int main(){
		A a;
		a.setX(7);
		a.printX();
		return 0;
	}

3.注意点
	（1）const修饰的常变量，必须初始化，后期不可修改其值。
	（2）若常对象初始化时，需要传入一个对象（即常对象的类是一个组合类），则该对象必须是const。如：const B b(1,2); const A a(1,2,b);。
		 这么做是为了保证常对象不可被更新或赋值，也不可存在被更新的可能。试想若b为non-const，则赋值后，a的b数据成员可以调用B中non-const
		 函数成员。而int b = 2; const A a(1,2,b);中，b就不用是const，因为b作为一个基本类型，没有member function。
	（3）const对象只能调用const member function；类中的所有成员函数都能调用const member function 和 const member data；
		 const member function不能调用non-const member function，因为普通成员函数存在修改数据成员的可能。
		 const member function能调用non-const数据成员，不论调不调用，所有数据成员在常成员函数域内，均不可修改，按照const操作。
	（4）类的const数据成员，必须在类的构造函数的初始化列表中完成初始化动作。即，不能依赖类自动生成的默认构造函数，也不能在构造函数体内用赋值操作为const数据成员
		 赋值。此外，数据成员中的T & var，也必须在类的构造函数的初始化列表中完成初始化动作。
		 有static const T var;即static和const同时修饰数据成员时，var按照static的属性进行，即在类外完成初始化操作。
	（5）尽可能使用const，即若能将成员函数声明为const，就要声明为const（原因是：1.声明为const意即该函数不再调用non-const成员函数，即不会被non-const成员函数的修改
		 影响，这是封装性声明；2.因为（7）中说的重载：试想，我们无法预测客户定义的对象是否为const，倘若我们实现的类中没有可重载的const函数，那么客户定义的const对象将在调用成员函数时报错）。
		 若声明某一成员函数为const，则看看其参数是否也可声明为const（参数的不修改保证）。看看其返回值是否也能声明为const，这一点在《Effective C++》条款3中谈到，
		 是为了保证logic const而非bitwise const，即倘若常成员函数返回member data且该常成员函数返回值不为const，则通过该const成员函数，对象可以修改member data，是bitwise const。
	（6）如果确实需要在const函数中修改成员变量，则声明变量为mutable，则变量总可以被修改。或 const_cast<T>(* this);
	（7）如果函数重载中仅以const作为函数区分，则普通对象，重载的const function 和 non-const function这两个成员函数都可以调用，但调用时编译器选择最靠前的那个函数。常对象只可调用常成员函数。
	（8）试图在non-const member function中调用重载的同名const member function：const_cast<T &>(static_cast<const T &> (* this).function());
	（9）写法习惯：
			#define VARWORD //宏定义全部大写
			const VarWord //const定义的变量每一单词首元大写
			class VarWord //类名每一单词首元大写
			struct VarWord //struct名每一单词首元大写
			T varWord(); //方法名首单词小写，之后的每一单词首元大写
	（10）《extern、namespace、#include.cpp》中谈到的extern const T var = value;
	
	《Effective C++》条款03
*/