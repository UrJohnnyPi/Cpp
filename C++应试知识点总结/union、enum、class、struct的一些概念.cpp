/*
enum

#include <iostream>
using namespace std;
class A{
	public :
		void fun1(){
			cout << enu1::b << endl; //可以使用枚举名来指定，这里体现了枚举名的第一个作用：作用域分辨 
			cout << x << endl; //也可以不用枚举名指定，因为这里真正起作用的enum关键字和{elemName}中的elemName，类比为int x;int x;会被视为重复定义。要求所有enum所在的域内，不能出现两个相同的elemName。这里若将enu1中的a改为x，就会报重复定义错误。 
			cout << m << endl;
		}
		void fun2(){
			enu1 enu1obj(enu1(2)); //枚举名的第二个作用：定义对象。枚举对象通过传入elem来构造，最终具现化为一个elem。这里传入的elem是enu1(2)，之所以用enu1来强制类型转换整型值2，是因为：枚举元素不等于整型值，只是可以映射为整型值。 
			cout << enu1obj << endl; //enu1的对象enu1obj最终具现化为元素c
			if(enu1obj == a) cout << "a" << endl; 
			if(enu1obj == b) cout << "b" << endl; 
			if(enu1obj == c) cout << "c" << endl; 
			
			enu1obj = enu1(a); //定义一个临时对象，其传入参数为a，赋值给enu1obj，即enum还实现了copy assignment。可见能定义对象的enum实际上就是“一个只有public数据成员的类，且这些public数据成员已指定初值，后续可改。”因此，编译器会为enum自动生成默认构造、copy构造、copy assignment和析构。 
		} 
	private :
		enum enu1{
			a , b , c //倘若不指定值，则默认从0起升序 
		};
		enum enu2{
			x = 5 , y = 9 , z //elem所对应的整型值可被重新指定，这里的z将按照前面的指定值自动升序，为10 
		};
		enum { //不定义对象 或 不使用分辨命名空间时，有没有enumName效果都一样，重要的是enum关键字和elemName的域内唯一性 
			k , m , n
		};
		int arr[n]; //这个技术叫做“the enum hack”。
		
		//“the enum hack”解决了这样的一个问题：
		//倘若在类中声明了一个符号，那该符号在初始化前是不能获得值的，比如在private
		//下声明一个static const int n;然后需要在类外实现其定义式，所以这里的int arr[n];
		//就会因为用到一个目前只有声明式可见的符号n而报错。 
};

int main(){
	A a;
	a.fun1();
	a.fun2();
	return 0;
}
*/

/*
class 

重点概念
	（1）class 和 struct 的区别：
		c++语境下，允许在结构体当中定义函数，它的用法和类的用法很像，不过与类的区别在于，struct中定义的函数和变量都是默认为public的，而class是默认为private的：
		******************
		#include <iostream>
		using namespace std;
		struct A{
			A(int x); //可有可无的构造函数
			void fun();
			int x;
			A(const A & a):x(a.x){} //拷贝构造函数
			const A & operator = (const A & a){ //copy assignment函数
				x = a.x;
				return * this;
			}
			~A(){} //析构函数
		}; 
		A::A(int x):x(x){
		}
		void A::fun(){
			cout << x << endl;
		}
		int main(){
			A a(7);
			a.fun();
			cout << a.x << endl;
			return 0;
		}
		C语言中没有class。
		
		注意，C++中的struct能被继承、可以实现多态（即内部可以定义虚函数）、可写构造、析构、copy等函数。

	（2）类中的初始化操作次序：
		 虚基类
		 基类：声明继承时的顺序。
		 私有数据成员：声明为数据成员时的顺序。若数据成员中有const和&，则必须为类定义一个构造函数，且必须在初始化列表中为const成员和&成员初始化。
	（3）编译器编译时，能自动创建的有四个函数：默认构造函数、non-vartual析构函数、copy构造函数 和 copy assignment。
		 这些自动创建函数都是public和inline的。此外要注意，编译器不是无条件地创建，实际上编译器并不预先创建，只有这些函数被需要时才创建。自己写了的，也不被创建。
	（4）构造函数 和 拷贝构造函数都是构造函数，意即其初始化操作都要在初始化列表中完成，构造函数的一些性质也适合拷贝构造函数，比如，不能声明为virtual，也不能调用
		 virtual；在数据成员中含有const和&的情况下，必须得为class写一个构造函数/拷贝构造函数。拷贝构造函数的性质和构造函数相仿（因为都是构造函数，用于从无到有构造实例），
		 作用和copy assignment相仿（因为都是为了拷贝）。
	（5）当copy构造函数和copy assignment中有大量重复代码时，不能用其中一个去调用另一个，因为：若copy构造函数调用copy assignment，意味着一个尚未构建完成的对象竟然
		 调用了成员函数operator=；同理，若用copy assignment调用copy构造函数，意味着一个构建完成的对象竟调用了用来构造对象的函数。所以，解决办法是：在private下写一个
		 共享的init函数用来存放重复代码。
	（6）若逻辑上不应该为class自定义copy assignment，且为了阻止编译器自生成一个copy assignment，则要让class继承自uncopyable class：
		 uncopyable class{
			 private :
				uncopyable & operator = (const uncopyable &); //不用写实现
				uncopyable & operator = (const Uncopyable &);
		 }
		 derived class : private uncopyable{}
		 将base class的copy assignment声明为private，编译器将不会为derived class自动生成copy assignment。这样derived class在编译器，若遇到本不应该出现的 obj1 = obj2;就会报编译错误。
	（7）所有的STL中的容器和标准string的析构函数都是non-virtual，因此都不适合被继承。
	（8）类分为以下几种：
			不用作base class的：如STL中的容器和标准string。
			用作base class但不用作多态：如uncopyable。
			用作多态：析构函数声明为virtual。
	（9）类的析构函数都必须是public下的，否则：
			若该类不被继承，则其对象因无权调用private成员而无法被析构，造成内存泄漏。
			若该类被继承，则其派生类因无权调起基类中的private成员而导致派生类对象的基类部分无法被析构，造成资源泄漏。
		类的构造函数声明为public，是单例模式的核心所在。
	（10）别让异常逃离析构函数，否则就会因析构函数的执行中断而造成资源泄漏。为此要在可能出现异常的析构函数中使用try catch语句。或者将析构函数用空白异常明细（throw()）声明，声明后必为nothrow函数。例如：int fun() throw();就表示fun()不抛出任何异常。 
	（11）C++中的explicit关键字 只能 用于修饰 只有一个参数的构造函数（包括构造函数 和 copy构造）,它的作用是表明该构造函数参数传递方式是显式的, 而非隐式的；跟它相对应的另一个
		  是implicit,implicit不是关键字，是一个语义，表明该构造函数参数传递方式是隐式的,构造函数默认情况下是implicit(隐式)：
		  ******************
		  class A{
			public :
				A(T para){} //只有一个参数的构造函数，默认的参数传递方式是implicit。 
		  };
		  A a(para); //OK，显式 
		  A a=para; //OK，隐式 
		  ******************
		  class A{
			public :
				explicit A(T para){} //只有一个参数的构造函数，声明其参数传递方式是explicit。 
		  };
		  A a(para); //OK，显式 
		  A a=para; //wrong，禁止隐式
		  ******************
		  
		  实例：
		  ******************
		  class Rational { //有理数类
			public :
				Rational(int num = 0 , int den = 1) : num(num) , den(den) {}
				int getNum() const {
					return num;
				}
				int getDen() const {
					return den;
				}
			private :
				int num , den; //num是分子，den是分母
		  };
		  
		  const Rational operator * (const Rational & lhs , const Rational & rhs) { //注意：lhs和rhs都得是const，否则报错。隐式类型转换要求形参对象必须为常对象。
			return Rational(lhs.getNum() * rhs.getNum() , lhs.getDen() * rhs.getDen());
		  }
		  
		  Rational d = a * 2; //没问题，发生了隐式类型转换，理解：Rational d = operator * (a , 2); 
		  Rational e = 2 * a; //没问题，发生了隐式类型转换，理解：Rational e = a.operator * (2 , a);
		  ******************
	（12）class的设计理念之一：
			不给客户犯错的机会，具体做法：消除客户的资源管理责任、约束范围等；
			让客户的操作越简单、记忆的东西越少（或越符合主流直观）越好，具体做法：接口名一致性、类型兼容等。
			设计class犹如设计type。
			将可能出现的、不能消除的转型写在函数中提供给客户，避免客户自己使用转型语句。 
			详见《Effective C++》条款18、条款19、条款27
	（13）封装性保证：（越少的波及面，越好的封装性）
			将数据成员声明为private。
			越少的成员函数 和 越少的friend函数。
			深复制取代浅复制。
	（14）若所有参数皆需类型转换，请为此采用non-member函数。因为non-member函数只将对象视为参数，如：const Rational operator * (const Rational & lhs , const Rational & rhs); operator * (obj1 , obj2);
		  而member函数还要视一个对象为调用者，如：const Rational operator * (const Rational & rhs) const; obj1.operator * (obj2);
		  这就使得non-member函数允许lhs和rhs中任何一个或全部为隐式类型转换；而member函数只允许rhs为隐式类型转换，* this不能隐式类型转换。详见《Effective C++》条款24
		  
		  对于类模板，若所有参数皆需类型转换，请在class template内声明 并 “定义”friend非成员函数：
		  ******************
		  template<typename T>
		  class Rational {
			public :
				Rational(const T & num = 0 , const T & den = 1) : num(num) , den(den) {
				}
				friend const Rational<T> operator * (const Rational<T> & rhs , const Rational<T> & lhs){
					return Multiply(rhs , lhs);
				} 
			private :
				T num , den;
		  };
		  
		  template<typename T>
		  const Rational<T> Multiply(const Rational<T> & rhs , const Rational<T> & lhs){
			return  Rational<T>(lhs.getNum() * rhs.getNum() , lhs.getDen() * rhs.getDen());
		  }
		  ******************
		  原因是：
		  （a）operator *();不能是member函数，因为member函数只允许rhs为隐式类型转换，* this不能隐式类型转换，即 a * 2;正确，而 2 * a;错误。且封装性要求尽可能使用non-member函数。
		  （b）不能是non-member non-friend函数，因为non-friend函数如：
			   template<typename T>
			   const Rational<T> operator * (const Rational<T> & rhs , const Rational<T> & lhs){
					//do something 
			   } 
			   为 类模板 写的 函数 必是 函数模板。而函数模板有这样的要求：若传入的参数不能“直接”推导出T，则拒绝接受该参数。于是，a * 2; 2 * a;都不正确，
			   因为无法通过2“直接”推断T是什么类型，正确的输入只能是 a * b;。根源是：对于template class/function而言，具现化之前，调用是无法完成的。（而
			   template fun的具现化恰是通过传递两个Rational<T>类型的参数实现的）。因为一般化的template function和特化版本的template function可能同时存在，
			   如果具现化前就能调用，则产生歧义。
		  （c）若将operator * 声明（不定义）为class template中的friend函数，代码将导致连接器报错。因为编译器具现化的是声明在Rational中的operator *，编译器
			   针对oneHalf * 2，调用的也是该具现化的friend声明式。但其定义式不受影响，依旧是template function。
		  （d）将operator * 声明并“定义”为class template中的friend函数，通过调用Multiply（或自写）就能实现目的。原理是：总要定义一个a：
			   Rational<int>a;，不论是a * 2;还是2 * a;都是正确的，因为在定义a时，T就被具现化了：
			   ******************
			   #include <iostream>
			   #include <typeinfo>
			   using namespace std;

			   template<typename T>
			   class Rational {
				public :
					Rational(const T & num = 0 , const T & den = 1) : num(num) , den(den) {
					}
					friend const Rational<T> operator * (const Rational<T> & rhs , const Rational<T> & lhs) {
						return Multiply(rhs , lhs);
					}
					const T getNum() const {
						return num;
					}
					const T getDen() const {
						return den;
					}
				private :
					T num , den;
			   };

			   template<typename T>
			   const Rational<T> Multiply(const Rational<T> & rhs , const Rational<T> & lhs) {
					return  Rational<T>(lhs.getNum() * rhs.getNum() , lhs.getDen() * rhs.getDen());
			   }

			   int main() {
					Rational<int> a(2,6);
					a * 2;
					2 * a;
					return 0;
			   }
			   ******************
	（15）public继承建立的“is-a”的关系。如：
		  class Base{}; 
		  class Derived : public Base{};
		  表示Derived 对象也是一种Base 对象。为了实现“是一种”的关系，我们要使Base类的设计和Derived类的设计在逻辑上正确，而不仅只在编译时正确。
		  要把编译过程和实际的逻辑相匹配，即逻辑上如何如何的事物，其对应代码应在编译期、连接期还是运行期应该如何。
		  既然Derived 对象也是一种Base 对象，那么就不该对从Base中继承而来的non-virtual符号进行重写。
		  Derived class中的成员函数将隐藏继承自Base class的所有同名函数，意即，即使继承自Base class的fun函数与Derived class中新定义的fun函数在参数列表、是否为const等方面表现不同，也不会发生重载，Derived obj对fun的调用只能是Derived::fun，试图对Base::fun重载调用将导致错误。
		  对Base::fun的调用有两种方式：
		  ******************
		  派生类函数中使用作用域分辨符：
		  
		  Derived class : public Base{
			  public :
				T fun(){
					Base::fun();
				}
		  };
		  ******************
		  using：
		  
		  Derived class : public Base{
			  public :
				using Base::fun;
				T fun(){
					Base::fun();
				}
		  };
		  ******************
		  若是在派生类函数中使用作用域分辨符这一方式，则通过Derived::fun实现对Base::fun的调用；若使用using方式，则将Base::fun的作用域暴露至Derived class中，此时，Derived obj就能通过重载调用Derived中可见的Base::fun。
		  在多重继承中，有：
		  ******************
		  class A{
			  public :
				T fun();
		  };
		  class B : public A{
			  public :
				T fun();
		  };
		  
		  B b;
		  b.A::fun(); //obj.zoon::fun;
		  ******************
		  即对象通过作用域分辨符访问基类中的被派生类同名隐藏的函数，但这样做极不建议，对基类中被派生类同名隐藏的函数的访问，要按照上述的using和派生类函数实现。
	（16）组合类塑模的是“has-a” 或 “根据某物实现出”的关系。
		  “hsa-a”关系体现在：需要通过程序中的组合类对象对现实对象进行模拟时。比如Person这个类，has a 内嵌对象，叫做name；has a 内嵌对象，叫做address；等等。
		  “根据某物实现出”体现在：在编程实现时需要某个类对象来实现另一个类时，具有抽象意义。比如：set对象可根据一个list对象实现出。
	（17）private继承的含义是“根据某物实现出”。意即，如果D以private形式继承B，则D对象是“根据B对象实现出来的”。
		  组合类 和 private继承能塑模出相同的关系，基于下面这个原因，要尽量选用 组合类 而非 private继承：组合类能有效降低编译依存性：
		  ******************
		  //使用private继承，必然要使用#include，因此编译依存性增大
		  base.h
		  
		  class Base{};
		  ******************
		  derived.h
		  
		  #include "base.h"
		  class Derived : private Base{};
		  
		  $$$$$$$$$$$$$$$$$$
		  //使用组合类，可以使用extern 或 前向引用声明的方式降低编译依存性
		  base.h
		  
		  class Base{};
		  extern Base & Getb(){
			  static Base base;
			  return base;
		  };
		  ******************
		  derived.h
		  
		  extern Base & Getb();
		  class Derived{
			  public :
				Derived():b(Getb()){}
			  private :
				Base b;
		  };
		  ******************
	（18）菱形继承时若只想维护一个间接基类副本，就可以考虑虚继承。如下： 
		  class File{
			  File(para){}
		  };
		  class InputFile : virtual public File{ 
			 InputFile(para):File(para){}
		  }; 
		  class OutputFile : virtual public File{
			  OutputFile(para):File(para){}
		  }; 
		  class IOFile : public InputFile , public OutputFile{
			  IOFile(para):File(para){}
		  }; 
		  但代价是空间开销和时间开销变大。
		  虚继承将只维护一个File副本，而非从两条继承路径继承而来的两个副本。意即IOFile的构造函数调用起的 InputFile构造函数 和 OutputFile构造函数 将不调起
		  各自的虚基类File的构造函数，而是将File的构造权交给IOFile。这也体现了virtual具有“可被派生类重写”之意：IOFile对File的继承 “重写了（隐藏了）” InputFile和OutputFile对File的继承。
	（19）嵌套类
		  ******************
		  #include <iostream>
		  using namespace std;

		  template <class T , typename U>
		  class A{
			public :
			
				A(T){
				}
				
				void display(){
					cout << "A" << endl;
					U u;
					B b(u);
					b.display();
				}
		  
				class C{ //A的public嵌套类 
					public :
						C(double){
						}
						display(){
							cout << "C" << endl;
						}
				};
				
			private :
				
				class B{ //A的private嵌套类 
					public :
						B(U){
						}
				
						void display(){
							cout << "B" << endl;
						}
				};
		  };

		  class D : public A<int , char>::C{ //继承自A的嵌套类C 
			public :
			
				D():A<int , char>::C(9){
				}
				
				void display(){
					A<int , char>::C::display();
				}
		  };
		  
		  class E : public A<int , char>{ //继承A 
			public :
			
				E():A<int , char>(9){
				}
				
				void display(){
					A<int , char>::C c(8);
					c.display();
				}
		  };
		  
		  int main() {
			A<int , char> a(7);
			a.display(); //输出：A B 
	
			A<int , char>::C c(10);
			c.display(); //输出：C 
	
			D d;
			d.display(); //输出：C 
	
			E e;
			e.display(); //输出：C 
			return 0;
		  }
		  ******************
		  其中A称为外部类，B和C称为嵌套类（内部类）。
		  
《Effective C++》条款05、条款07、条款08、条款14、条款18、条款22、条款23、条款24、条款29、条款32、条款33、条款38、条款46
*/

/*
union

（1）联合体的全部数据成员共享同一组内存单元。
（2）其内存单元中每一时刻只能存在某一成员。
（3）内存的字节为所占字节最大的成员的字节。
（4）对于无名联合体，函数或类或其他联合体或结构体可以直接对其成员进行操作。
（5）将无名联合体应用于类中的private中，为我们提供了这样的一种可能：在联合体中创造了多个“虚成员”，以该类定义了对象A后，A所具有的参数可以为联合体中的某一成员，而不含其它成员。
（6）实例：
	 #include <iostream>
	 #include <string>
	 using namespace std;

	 class A{
		public :
			A(int a):a(a){}
			A(char b):b(b){}
			A(string c):c(c){}
		
			~A(){}
		
		private :
			union{
				int a;
				char b;
				string c;
			};
	 };

	 int main(){
		A a(7);
		A b('x');
		A c("str");
		return 0;
	 }
*/