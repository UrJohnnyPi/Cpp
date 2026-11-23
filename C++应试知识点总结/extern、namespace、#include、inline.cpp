/*
变量的内部链接属性：

变量的外部链接属性：
*/

/*
extern

1.定义
	extern可以置于变量或者函数前，extern只能用于声明变量或函数，以表示变量或者函数的定义 在当前文件中extern语句下方的某处 或 在别的文件中，提示编译器遇到此变量和函数时在 当前文件中extern语句下方 或 其他模块（.cpp文件 或 .h文件中）中寻找其定义。
	extern修饰符既可以出现在.h文件中，也可以出现在.cpp文件中。放置在.h文件中时，是在告诉其他模块：若试图使用本.h文件中的全部声明，则使用#include；若试图使用本.h文件中的某一声明，使用extern。

2.作用
	（1）当它与"C"一起连用时，如: extern "C" void fun(int a, int b);告诉编译器在编译fun这个函数名时按着C的规则去翻译相应的函数名而不是C++的，C++的规则在翻译这个函数名时会把fun这个名字变得面目全非，可能是fun@aBc_int_int#%$也可能是别的（不同编译器不同），因为C++支持函数的重载。
	（2）声明某一变量或函数可被外部模块引用。

3.应用
	***************
	global.cpp //这里不能是.h，因为.h文件中从来都只放声明式；若.h中放置定义式，采用#include方式载入时，可能出现重复定义错误。
			   //若换做.h文件，则在global.cpp文件中定义：int test = 5;  在global.h文件中声明：extern int test; 或 int test; ，加不加extern都行。
	int test = 5; //这个可以加extern，也可以不加。加：extern int test = 5;，即显式声明变量的链接属性为外部链接。即使不加，像这样的普通变量/函数也都是extern（外部链接属性）。
	***************
	main.cpp
	
	#include <stdio.h>
	extern int test;
	
	int main(int argc , char * argv[])
	{
		printf("%d\n", test);
		return 0;
	}
	***************

4.注意
	（1）在一个源文件中定义了一个数组char arr[2] = {'a' , 'b'};。在另一个源文件中若使用extern char * arr;这样的声明就会报错。因为 数组变量!=指针变量。因此正确的声明方式为：extern char arr [];
	（3）static修饰的全局变量的作用域只在本文件中（即static变量的链接属性是内部链接），所以extern和static不能同时修饰一个变量。
	（4）const默认是内部链接
		像下述这样，会报错为无法解析，即无法获得定义式的变量test：
		***************
		global.cpp
		
		const int test = 5;
		***************
		main.cpp
	
		#include <stdio.h>
		extern const int test; //extern 报错
	
		int main(int argc , char * argv[])
		{
			printf("%d\n", test);
			return 0;
		}
		***************
		
		使用extern的正确写法：
		***************
		global.cpp
		
		extern const int test = 5; //定义时就声明为extern，旨在将const变量的默认链接属性从“内部链接”改为“外部链接”。
		***************
		main.cpp
	
		#include <stdio.h>
		extern const int test;
	
		int main(int argc , char * argv[])
		{
			printf("%d\n", test);
			return 0;
		}
		***************
		
		也可以：
		***************
		global.h
		
		extern const int test; 
		***************
		global.cpp
		
		#include "global.h" //若没有这个，就错了。因为const默认的链接属性是内部链接，因此，你要保证.cpp中的test定义对.h可见。要么加#include，要么extern。但因为若extern后，就没必要在.h中声明了，因此这里加#include。
		const int test = 5;
		***************
		main.cpp
	
		#include <stdio.h>
		extern const int test;
	
		int main(int argc , char * argv[])
		{
			printf("%d\n", test);
			return 0;
		}
		***************
		
	（5）在上述关于extern在变量上的用法规则，同样可以迁移到函数上，要注意的是，在使用模块中有：extern T fun();如：
		***************
		extern.cpp
		
		void fun(){} //这里可以加extern 也可以不加 extern，默认为extern；在.h文件中，注意放置的是fun的声明式。
		***************
		main.cpp
	
		#include <stdio.h>
		extern void fun();
	
		int main(int argc , char * argv[])
		{
			fun();
			return 0;
		}
		***************

	（6）要分外注意的是，在“3.应用”和“4.注意（5）”中提到的：普通变量、函数在.cpp定义时可以加extern也可以不加extern，因为普通变量、函数默认为extern，
		 所以只需要在引用时用extern声明就可以了；const var、const fun在.cpp中定义时必须加extern，为了将内部链接属性改为外部链接属性；而static var、
		 static fun因为static就是显式声明为静态链接属性的意思，所以，绝不能使得static和extern一起修饰变量名或函数名。
		 此外，这样的写法也是正确的：
		 const int test = 5;
		 extern const int test;
		 即先定义后声明。
	（7）《static 和 const.cpp》中第一个注意点：企图使用extern将变量的作用域扩展到外部模块，要注意的是，要将意欲扩展到外部使用的non-local static 转换为 local static。
*/

/*
#include 

重要概念：
		（1）
		要记住，总在.h文件中放置声明而非定义：
		***************
		global.h
		
		int test = 5; //这里放置了test的定义而非声明
		***************
		extern.h
		
		void fun();
		***************
		extern.cpp
		
		#include <stdio.h>
		#include "global.h"
		#include "extern.h"

		void fun()
		{
			printf("%d\n", test);
		}
		***************
		main.cpp
		
		#include <stdio.h>
		#include "extern.h"
		#include "global.h"

		int main(int argc , char * argv[])
		{
			fun();
			printf("%d\n", test);
			return 0;
		}
		***************
		extern.cpp和main.cpp都要用test，因此同时引入#include "global.h"，这样在extern.cpp和main.cpp中各有一个int test = 5;定义式。当连接extern.cpp和main.cpp时，最后的源文件中将出现两个int test = 5;定义式。
		重复定义错误。
		也可以这样想：有两个.h文件，分别为A.h和B.h，其中A.h中含有一个定义式f，B.h则需要#include <A.h>，那么当一个cpp文件同时包含A.h和B.h时，就必然会出现两个f，重复定义。
		因此，前global.h文件和global.cpp文件应改为：
		***************
		global.h
		
		int test; //放置声明
		***************
		global.cpp
		
		int test = 5; //放置定义
		***************
		
		（2）
		代码在预编译时，会将#include <head.h> 命令替换为head.h文件含有的全部声明式。而编译器的操作对象就是这个替换后形成的.i文件（预处理器的产物）。例如：
		***************
		//源码：
		//lib.h文件
		int add(int x, int y);
		
		//lib.cpp文件
		#include"lib.h"
		int add(int x,int y) {
			return x+y;
		}
		
		//main.cpp文件
		#include"lib.h"
		using namespace std;
		int main() {
			int sum = add(1, 2);
			return 0;
		}
		
		//预处理后：
		//lib.i文件
		int add(int x, int y);
		int add(int x,int y) {
			return x+y;
		}
		
		//main.i文件
		int add(int x, int y);
		using namespace std;
		int main() {
			int sum = add(1, 2);
			return 0;
		}
		***************
		
		（3）降低编译依存。我们知道，将某个类放到一个.h文件中是常有的情况，倘若该.h文件被广泛包含，那么对该.h文件中的这个类的频繁修改将是灾难性的——所有包含该.h文件的
		模块均不得不随之而重新编译。不考虑对这个类本身的修改，最易被忽视的是该.h文件中包含的头文件中的内容的变化，即它所需要的代码的变化导致了它的变化。如：
		***************
		Person.h
		
		#include <string>
		#include "date.h"
		#include "address.h"
		class Person{};
		***************
		Person.h被广泛包含，因此#include <string>、#include "date.h"和#include "address.h"的频繁变化是灾难性的。
		#include<head.h>的问题根源在于：预编译阶段要以本体替换之。所以，如#include "date.h"本体发生了变化，就要在预编译时重新替换Person.h中的#include "date.h"语句。
		这一替换举动导致了#include "Person.h"本体的变化，因此所有包含#include "Person.h"的模块就需要重新预编译、编译、链接。
		
		解决之道是：
		（a）尽量以extern obj替换#include "head.h"，因为如：
		***************
		Person.h
		
		#include <string>
		extern Date dateObj;
		extern Address addressobj;
		class Person{};
		***************
		这样的代码将data.h和address.h的变化隐藏到了对象内部。dateObj和addressobj均定并声明为extern与于另一个模块，这里是对两个obj的声明，得以在Person中使用。
		
		（b）尽量以前向引用声明，因为如：
		***************
		Person.h
		
		#include <string>
		class Date;
		class Address;
		class Person{};
		***************
		因为前向引用声明无法提供使用者对类内部的访问权限，因此也就隐藏了内部的变化。Person.cpp中在实现Person时，再#include "date.h"和#include "address.h"以引入内部细节，按照（2）中代码预编译过程可以得知，Person.cpp中引入的头文件的改变不会影响Person.h。
		
		关于前向引用声明：（以class B;class A{};为例）
		（i）A的函数成员形参、私有数据成员、函数成员返回值等类型为B &。
		（ii）A的函数成员形参、私有数据成员、函数成员返回值等类型为B *。
		（iii）A的函数成员返回值类型为B。
		
		（4）#include <> 和 #include ""的区别：
		#include <>：表明引用的头文件来自编译器的类库，遂仅从编译器的类库路径中搜索该头文件。
		#include ""：先从本文件的相对路径中搜索该头文件，若找不到，再从编译器的类库路径中搜索该头文件。
*/

/*
namespace

重要概念：C++不允许添加新的template到std中。《Effective C++》条款25
*/

/*
文件命名

文件的命名符合一定的规范，其中A为文件中的类名（变量名）：

类（变量）的声明文件必是A.h
类（变量）的定义（实现）文件必是A.cpp
程序执行文件最好是main.cpp
*/

/*
透彻了解inline的里里外外

（1）inline：将“对此函数的每一个调用”都以函数本体替换之。
（2）inline是对编译器的申请。分为隐式和显式。隐式就是在class中定义函数（不是声明，定义要包括函数头和函数体）。显式是在函数前加inline关键字。
（3）要将inline用于修饰小型、频繁使用的函数。 
（4）并不是声明（隐式、显式）为inline的函数就一定会真按照inline规则执行。是否能够inline，取决于编译器。因为inline操作总体上会造成程序体积的膨胀，导致内存频繁的换页行为。 
（5）不被编译器允许内联的几种情况： 
		1.包含了递归、循环等结构的函数。
		2.虚函数一般不会内联，因为virtual意味着在运行期才确定调用哪个函数。
		3.如果通过函数指针调用内联函数，那么该函数将不会内联而是通过call进行调用。
		4.构造和析构函数一般会生成大量代码，因此一般也不适合内联。
		5.如果内联函数调用了其他函数也不会被内联。
		6.频繁使用、可能需要维护修改的库函数也不应当声明为inline。试想，若根据某个库函数写了
		许多客户软件，这些客户软件是已编译好的exe，若该函数是non-inline的，只要接口没变，客户
		软件就不存在问题；若该函数是inline的，则所有软件代码需要重新编译，将该函数以本体的形式编到程序中。
		
《Effective C++》条款30
*/