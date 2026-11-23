/*
条款21：必须返回对象时，别妄想返回其reference
*/
class Rational{ //有理数类 
	public :
		Rational(int num = 0 , int den = 1) : num(num) , den(den){ 
		}
	private :
		int num , den; //num是分子，den是分母 
		friend const Rational operator * (const Rational & lhs , const Rational & rhs); //声明为友元函数的有理数乘法重载函数 
};
const Rational operator * (const Rational & lhs , const Rational & rhs){ //根据条款03：尽可能使用const，来返回一个const 
	Rational result(lhs.num * rhs.num , lhs.den * rhs.den);
	return result;
	//直接写成 return Rational(lhs.num * rhs.num , lhs.den * rhs.den);就可以，即省去定义对象名，返回一个无名对象。 
}
//以上是正确的代码。但若为了使用引用以减少开销，就可能出现下面这样错误的函数：
const Rational & operator * (const Rational & lhs , const Rational & rhs){
	Rational result(lhs.num * rhs.num , lhs.den * rhs.den);
	return result; 
}
//错在了result是local对象，作用域结束便被析构，返回的是一个废弃对象的引用。
//那是不是写成如下形式就可以了？ 
const Rational & operator * (const Rational & lhs , const Rational & rhs){ 
	Rational * result = new Rational(lhs.num * rhs.num , lhs.den * rhs.den);
	return * result; 
}
//更差的代码，理由如下： 
Rational  w , x , y , z;
w = x * y * z; 
//y * z返回一个heap空间代称为r_1，x * r_1返回第二个heap空间为r_2。最后
//将r_2赋值给w。但问题是，却无法对两个heap空间回收。
//于是，有如下思考： 
const Rational & operator * (const Rational & lhs , const Rational & rhs){
	static Rational result(lhs.num * rhs.num , lhs.den * rhs.den);
	return result;
}
//同样烂，因为如果有下述需求：
bool operator == (const Rational & lhs , const Rational & rhs);
Rational a , b , c , d;
if((a * b) == (c * d)){
	// do something 
} else {
	// do other things
}
//else永远不会被执行。因为在执行a * b时，就已经为result开启了生命周期，在执行c * d时
//operator * ()被调用，但只是为result重赋值，因此if中的左值和右值本就是同一个static对象，
//所以if永远成立。
 
