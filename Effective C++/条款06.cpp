/*
条款06：若不想使用编译器自动生成的函数，就该明确拒绝
*/
/*
代码1：

class HomeForSale{
	//do something
};
HomeForSale h1;
HomeForSale h2(h1);
HomeForSale h3 = h1;
//实际场景中常会出现这样的场景，对象是唯一的，比如房屋、人等。
//因此，像上面这样的调用copy 构造函数和copy assignment的情况是不该出现的。
//这就要求HomeForSale中既不该自定义copy 构造函数和copy assignment，也应当
//阻止编译器自动生成这两种函数。但这是很矛盾的，因为不自定义便会编译器自动
//生成，若自定义又有违这种类的设计理念。
*/

/*
代码2：

class HomeForSale {
	public :
		//do something
	private :
		HomeForSale(const HomeForSale &);
		HomeForSale & operator = (const HomeForSale &);
		//为解决代码2出现的问题而出现这样的设计。
		//这样的设计使得h2(h1)和h3 = h1出错，因为对象无权访问private下成员。
		//但成员函数和friend函数仍然可以访问这两个copy函数，只是在连接时因
		//无法获得函数体而报错。

		//这里的函数参数列表中没有参数名，实际上也本不需要，只是因为大多数时候
		//函数体内要使用参数，所以要写出参数名。这里没有函数体，也就不需要写了。

		//代码3将讨论试图把连接期时的报错提前到编译期，理念是：错误越早发现越好。
};
HomeForSale h1;
HomeForSale h2(h1);
HomeForSale h3 = h1;
*/

/*
代码3：

class Uncopyable{
	private :
		Uncopyable(const Uncopyable &);
		Uncopyable & operator = (const Uncopyable &);
};
class HomeForSale : private Uncopyable{
	//do something

	//这样的设计使得HomeForSale没有自定义的copy函数，编译器也不会为其生成默认copy函数。
	//因为如条款05中代码2中的例子所言，base类的copy函数若声明为private，则derived类中
	//不会自动生成copy函数（包括copy 构造函数和copy assignment）。

	//这样的设计也使得在编译h2(h1)和h3 = h1时，因无法获得HomeForSale中的copy函数
	//而在编译器报错。若成员函数或friend函数试图调用copy函数，也会因HomeForSale中
	//本不存在copy函数而在编译器报错。
};
HomeForSale h1;
HomeForSale h2(h1);
HomeForSale h3 = h1;
*/

