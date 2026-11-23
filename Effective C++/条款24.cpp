/*
条款24：若所有参数皆需类型转换，请为此采用non-member函数
*/

//以下是有理数类，在条款21中，我们实现的乘法运算是一个non-member且friend函数，符合条款22、23中
//关于封装的要求。以下是通过忽略条款22、23，暂将乘法写为member函数，来解释条款24：
class Rational { //有理数类
	public :
		Rational(int num = 0 , int den = 1) : num(num) , den(den) {
		}
		int getNum() const {
			return num;
		}
		int getDen() const {
			return den;
		}
		const Rational operator * (const Rational & rhs) const;
	private :
		int num , den; //num是分子，den是分母
};

Rational a(1 , 8);
Rational b(1 , 2);
Rational c = a * b; //没问题，常规操作。可以理解如：Rational c = a.operator * (b);

Rational d = a * 2; //没问题，发生了隐式类型转换，理解：Rational d = a.operator * (2);
Rational e = 2 * a; //错误！理解：Rational e = 2.operator * (a);
//分析：对于Rational d = a * 2;语句的隐式类型准换，编译器的实际操作是：
Rational tempObj(2);
Rational d = a * tempObj;
destroy tempObj;
//编译器之所以能自觉地将本来传入另一member函数的参数用于初始化Rational构造函数，进而
//构造出一个临时对象，这是隐式类型转换的功劳。隐式类型转换要求，如果某一member函数需要本
//类对象作为传入参数（有const修饰），若传入一个其他类型的变量，那编译器就会试图调用本类
//构造函数（implicit），调用成功，则返回一个本类的临时对象供该member函数使用。例子见附录代码。
//注意，讨论隐式类型转换时，用于接收传值的构造函数参数数必须为1。比如这里的构造函数 
//Rational(int num = 0 , int den = 1);列表里声明了两个，传值时可以只传一个，因而explicit和implicit
//可应用修饰这个函数，进而隐式类型转换也可以应用于此。 
//上述代码中，若将构造函数声明为explicit，则可以保证a*2和2*a的一致性，即均报错。

//正确的代码如下：
const Rational operator * (const Rational & lhs , const Rational & rhs) {
	return Rational(lhs.getNum() * rhs.getNum() , lhs.getDen() * rhs.getDen());
}
Rational d = a * 2; //没问题，发生了隐式类型转换，理解：Rational d = operator * (a , 2); 
Rational e = 2 * a; //没问题，发生了隐式类型转换，理解：Rational e = a.operator * (2 , a);
//注意点一：
//该non-member函数仍运用了隐式类型转换。
//注意点二：
//在条款21中将该函数声明为friend，但实际上如条款23所言，为了保证封闭性，能避免friend就避免。使用public get()同样可以满足。 

/*
附录：
#include <iostream>
using namespace std;
class A{
	public :
		A(int x):x(x){ //将构造函数声明为explicit报错类型不匹配
		}
		void show(const A & a) const{ //如果把参数列表中的const去掉，换成A & a，则代码提示类型错误int不与A &匹配。
			cout << a.x;
		}
	private :
		int x;
};
int main(){
	A a(0);
	a.show(1); //输出1
	return 0;
}
*/
