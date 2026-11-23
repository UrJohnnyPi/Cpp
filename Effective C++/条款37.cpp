/*
条款37：绝不重新定义继承而来的缺省参数值

（1）简单回忆静态类型 和 动态类型 
class Shape{
	public :
		enum ShapeColor{
			Red , Green , Blue
		};
		virtual void draw(ShapeColor color = Red) const = 0;
};
class Rectangle : public Shape{
	public :
		virtual void draw(ShapeColor color) const;
};
class Circle : public Shape{
	public :
		virtual void draw(ShapeColor color) const;
		//以对象调用此函数，一定要指定参数值，因为静态绑定下这个函数不从其
		//base继承缺省参数值。
		//但若以指针（或reference）调用此函数，可以不指定参数值，因为动态绑定下
		//该函数会从base继承缺省参数值。
};
Shape * ps;
Shape * pc = new Circle;
Shape * pr = new Rectangle;
//静态类型就是声明的类型，动态类型是实际的类型。
//静态类型决定了指针（或reference）调用什么类中的成员函数合法；动态类型决定了运行期间，指针（或reference）将指向哪个函数。如： 
pc->draw(Shape::Red); 
pr->draw(Shape::Red);
//上例中ps、pc、pr的静态类型是Shape *，动态类型分别是：无，Circle * 和 Rectangle *。
//通过赋值动作，实现指针（或reference）动态类型的改变，如：
ps = pc; //动态类型是Circle *
ps = pr; //动态类型是Rectangle *

（2） 
静态绑定 又名 前期绑定。
动态绑定 又名 后期绑定。
virtual函数是动态绑定，缺省参数值是静态绑定。
考虑带有缺省参数值的virtual函数（如上例中的draw()），有“具有派生类引用值（指针值）的基类指针在调用一个定义于Derived内的virtual函数
的同时，却使用了来自Base的virtual函数指定的缺省参数值”。 如：
pr->draw(); //调用Rectangle::draw(Shape::Red) 

静态绑定 由 静态类型 规定。动态绑定 由 动态类型 规定。如：
将上例Rectangle中的virtual void draw(ShapeColor color) const;改为：virtual void draw(ShapeColor color = Green) const;，
pr->draw();调用的仍是Rectangle::draw(Shape::Red)，而不是Shape::Green，因为缺省参数值是静态绑定的，由pr的静态类型规定，
为Shape *，Shape * 指出缺省参数值应取自Shape::draw(default)。 

对指针成立的，对reference同样成立。
*/
