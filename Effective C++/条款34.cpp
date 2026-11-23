/*
条款34：区分接口继承和实现继承

继承分为：
1.只继承成员函数的接口：Derived只继承Base成员函数的接口（即Base的函数声明）。对应pure virtual。 
2.继承成员函数的接口，并希望能重写实现。对应impure virtual。
3.继承成员函数的接口和实现，不希望重写。对应non virtual。
*/
 
/*
代码1：

class Shape {
	public :
		virtual void draw() const = 0;
		virtual void error(string & msg);
		int objectID() const;
};
class Rectangle : public Shape {
		//orverride draw()
		//orverride error()
		//完全继承objectID()
};
class Ellipse : public Shape {
		//orverride draw()
		//orverride error()
		//完全继承objectID()
};
//1.声明draw()为pure virtual属于第一种继承方式。而之所以为Shape定义draw()为pure virtual，
//是因为draw()对于Shape而言，是应该存在的，因为任何Shape都可以draw（因此要声明）；但Shape
//是抽象的，无法具体draw下来（因此为pure virtual）。可以这样调用：
void fun(Shape * sha){
	sha.draw();
	sha.error();
	//do something 
}
int main(){
	Rectangle rec;
	fun(&rec);
	return 0;
} 
//2.声明error()是impure virtual，属于第二种继承方式，意在告诉Derived，Base提供了Derived一个缺省的error()，
//Derived有权重写，若不想重写，也可以调用缺省error()。
//3.objectID()显然是第一种继承方式。
*/

/*
代码2：（用于说明impure virtual提供缺省行为可能遇到的困境及解决办法） 

(1)
class Airport{};
class Airplane{
	public :
		virtual void fly(const Airport & des){
			//缺省代码，以缺省的飞行模式飞至目的地 
		}
}; 
class ModelA : public Airplane{}; //ModelA和ModelB缺省状态下，都具有同一飞行模式，缺省飞行模式正如继承而来的fly所述。 
class ModelB : public Airplane{}; 
//上述代码的正确性建立在“ModelA和ModelB缺省状态下，都具有同一飞行模式”这一条件上。
//如果来不及为ModelA重写fly()，则该代码完全可以先投入使用。

(2)
class Airport{};
class Airplane{
	public :
		virtual void fly(const Airport & des){
			//缺省代码，以缺省的飞行模式飞至目的地 
		}
}; 
class ModelA : public Airplane{}; //ModelA和ModelB缺省状态下，都具有同一飞行模式，缺省飞行模式正如继承而来的fly所述。 
class ModelB : public Airplane{}; 
class ModelC : public Airplane{}; //新加入的ModelC，但其缺省状态下，飞行模式不同于继承而来的fly所述。 
//此时若来不及为ModelC重写fly()，就会导致灾难性后果。

(3)
//鉴于（1）（2），我们可以得知问题出在了设计时，没有做到“缺省”和“接口”的分离。
class Airport{};
class Airplane{
	public :
		virtual void fly(const Airport & des) = 0; 
}; 
void Airplane::fly(const Airport & des){ 
	//缺省代码，以缺省的飞行模式飞至目的地 
}
class ModelA : public Airplane{
	virtual void fly(const Airport & des) {
		Airplane::fly(des); //来不及重写fly，则使用缺省fly()。 
	}
}; 
class ModelB : public Airplane{
	//do something 
}; 
class ModelC : public Airplane{
	virtual void fly(const Airport & des){
		//因ModelC飞行模式不同于继承而来的fly所述，因此必须重写fly。 
	}
};
//上述代码的关键在fly被分割为两个基本要素：其声明部分表现的是接口，其定义部分表现为缺省。
//这种pure virtual的构造方式在条款07中也出现过。
//通过继承关系，继承的基类成员的声明式；通过作用域分辨符访问的是基类成员的定义式。
//这里的ModelA的fly中Airplane::fly(des)就调用Airplane中fly的定义式； 
//而class ModelC : public Airplane表明ModelC只是继承了Airplane中声明的符号，因此对
//ModelC而言，fly必须重写，因为它继承的来自Airplane的fly是一个pure virtual。
*/
