/*
条款20：宁以pass-by-reference-to-const替换pass-by-value
*/

/*
代码1：

class Person{
	public :
		Person(para);
		Person(const Person & rhs);
		virtual ~Person();
	private :
		string name;
		string address;
};
class Student : public Person{
	public :
		Student(para);
		Student(const Person & rhs);
		virtual ~Student();
	private :
		string schoolName;
		string schoolAddress;
};

void fun(Student stu);
Student plato(para);

fun(plato(para));
//分析：当fun以pass-by-value（传值方式）调用时，整个代码的开销于此：
//Student plato(para);调用了Student的构造函数，进而调用了Person的构造函数+两次string的构造函数（name和address）。
//又调用了初始化自身私有成员（schoolName和schoolAddress）所需的两次string构造函数。
//fun(plato);调用了stu的复制构造函数，进而调用了stu的Person部分的复制构造函数+两次string的复制构造函数（name和address）。
//又调用了为stu私有成员赋值（schoolName和schoolAddress）所需的两次string复制构造函数。
//仅fun(plato);语句的开销就是6次构造函数；同理析构时也要执行6次。  
*/

/*
代码2：

void fun(const Student & stu);
Student plato(para);

fun(plato(para));
//用pass-by-reference-to-const替换pass-by-value后，继承了pass-by-value的优点，即
//para在fun内的不可变动性；同时具备了pass-by-value不具有的高效，因为fun(plato);不会
//任何构造函数和析构函数，因此时间和空间上都是很高效的。
//此外，pass-by-reference-to-const能够实现多态，而pass-by-value不能：
#include <iostream>
using namespace std;
class A{
	public :
		virtual void display() const{
			cout << "A";
		}
};
class B : public A{
	public :
		void display() const{
			cout << "B";
		}
};
void fun_val(A a){
	a.display();
}
void fun_ref(const A & a){ //此时a是一个const对象，因此不要忘记要将display声明为const。 
	a.display();
}
int main(){
	B b;
	fun_val(b); //输出A
	fun_ref(b); //输出B
	return 0;
}

//原因是：&是别名的意思，这里就是a获得了b中base部分的值，这就是多态，即：定义你的类型决定了你可以调用哪个类中的函数，而你实际具有的类型
//决定了你将使用哪个被重写的函数。这里的性质和“将一个派生类引用赋值给一个基类指针”一样，基类指针和派生类引用指向的是同一基类空间。
//而形参为对象则表明会调用copy构造函数，而a和b最终是两个不同的、毫无关联的空间，b的意义仅限于给a赋值。所以因为不指向同一基类空间，所以就
//不存在多态。

//多态的定义，前面说是：通过一个基类接口可以访问不同派生类的一种结构。
//在内存上定义多态，可以是：基类指针（引用）和派生类对象 二者 共享派生类对象的基类空间。
//要义：两个类必须是继承关系；基类享用了派生类对象的基类空间（这就表明，要用派生类对象赋值给基类引用 或 派生类地址赋值给基类指针）。
*/

/*
若传递的值是内置类型时，如int、char等，或是STL迭代器和函数对象，pass-by-value反而效率更高。
*/
