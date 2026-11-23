/*
条款46：需要类型转换时请为模板定义非成员函数 
*/

/*
代码1：

template<typename T>
class Rational { //有理数类，是对条款24中例子的template版本的讨论 
	public :
		Rational(const T & num = 0 , const T & den = 1) : num(num) , den(den) {
		}
		const T getNum() const {
			return num;
		}
		const T getDen() const {
			return den;
		}
	private :
		T num , den; //num是分子，den是分母
};
//为Rational写一个non-member的乘法函数如：
template<typename T>
const Rational<T> operator * (const Rational<T> & rhs , const Rational<T> & lhs){
	//do something 
} 
//实例代码：
Rational<int> oneHalf(1 , 2);
Rational<int> result = oneHalf * 2; //错误，无法通过编译
//分析：在条款24中，类似的乘法函数可以通过隐式类型转换使得oneHalf * 2正确。但对于template
//而言，这种的隐式转换是不存在的。对于oneHalf，其值传给rhs，便能根据oneHalf为Rational<int>
//推导出rhs的T为int。但对于2，将2传给lhs，这不会导致像条款24中的例子一样的过程，即编译器不会
//去调用Rational<T>的构造函数，进而推导出T并返回tempObj。实际上，只有T确定了，template class
//具现化了（此时class就变成了一个普通的class），这时，对于2的操作才会如条款24中一样。 
//为什么这里一直在谈论推导T？因为对于template class/function而言，具现化之前，调用是无法完成的。
//因为一般化的template function和特化版本的template function同时存在，如果具现化前就能调用，则产生
//歧义。所以本例中，oneHalf * 2可以调用这里的operator *。  
*/

/*
代码2：

template<typename T>
class Rational {
	public :
		//do something
		friend const Rational<T> operator * (const Rational<T> & rhs , const Rational<T> & lhs);
		//注意，它的等效形式如下：
		//friend const Rational operator * (const Rational & rhs , const Rational & lhs);
		//意即，在class template中“template”=“template<T>”。 
	private :
		T num , den;
};
template<typename T>
const Rational<T> operator * (const Rational<T> & rhs , const Rational<T> & lhs){
	//do something 
}
Rational<int> oneHalf(1 , 2);
Rational<int> result = oneHalf * 2;
//该代码就可以通过编译了。因为当oneHalf被定义时，就有T=int，而operator * 被声明在Rational<T>中，
//因此，operator * 的T也就被确定为int了。所以现在，一个Rational<T>的具现化就导致了位于Rational<T>中operator *<T>的
//的具现化。Rational、Rational中的operator *都成为了一般性的class和function，也就能够使oneHalf * 2通过编译了。
//实际上，就是oneHalf调用了自己内部的那个friend，而不是对象外部的operator *。所以会发生错误。

//但是该代码将导致连接器报错。因为编译器具现化的是声明在Rational中的operator *，编译器针对oneHalf * 2，调用的
//也是该具现化的friend声明式。但其定义式不受影响，依旧是template function。
//为了让函数运行成功，完善如下：
template<typename T>
class Rational {
	public :
		//do something
		friend const Rational<T> operator * (const Rational<T> & rhs , const Rational<T> & lhs){
			return Multiply(rhs , lhs);
		} 
	private :
		T num , den;
};
template<typename T>
const Rational<T> Multiply(const Rational<T> & rhs , const Rational<T> & lhs){
	return  Rational(lhs.getNum() * rhs.getNum() , lhs.getDen() * rhs.getDen());
}
//即把operator *函数的函数声明和定义完成与class内。
//尽管如此，Rational<int> result = 2 * oneHalf；也是错误的。
*/

//总结该条款：需要隐式类型转换时，请在class template内声明并定义friend非成员函数。 
//而这里完全可以用成员函数，反正不论是成员函数还是friend，实现的都是oneHalf * 2而非2 * oneHalf。
//只不过考虑到要尽可能地将函数放到类外这一封装要求，所以才如此。