/*
条款31：将文件间的编译依存关系降至最低
*/

/*
代码1：

#include <string>
#include "date.h"
#include "address.h"
class Person{
	public :
		Person(const string & name , const Date & birthday , const Address & address);
		string getName() const;
		Date getBirthay() const;
		Address getAddress() const;
	private :
		string name;
		Date birthday;
		Address address;
};
//上述代码的问题出在头文件的过度引入，或说：太强的编译依存关系。
//因为代码在预编译时，会将#include <head> 命令替换为head文件含有的全部定义式的声明式。而编译器的
//操作对象就是这个替换后形成的.i文件（预处理器的产物）。例如：
//原码：

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

//本例中，如果一个头文件中的内容改变，则所有包含该头文件的程序都得重新编译，因为Person class用到了
//这些头文件声明的符号，所以，包含Person class的程序也都得重新编译。这是大项目的灾难。
*/

/*
代码2：

#include <string> //像这样的标准头文件，要保留
class PersonImpl;
class Date; //前向引用声明，可以有效解决代码1中的问题，需要什么类就声明什么类
class Address;
class Person{
	public :
		Person(const string & name , const Date & birthday , const Address & address); //前向引用声明的类型可以定义引用
		string getName() const;
		Date getBirthay() const; //前向引用声明的类型可以作为函数返回类型
		Address getAddress() const;
	private :
		tr1::shared_ptr<PersonImpl> pImpl;
		//用于指向Person类的实现类PersonImpl（Implementation class），之所以这样设计，
		//是因为“前向引用声明”要求在引用符号时，要避免涉及到内部细节。Person类中的
		//函数会涉及到大量对Date、Address的内部访问（比如构造函数的调用），因此，不得不
		//将Person类的实现放到另一个可以访问Date、Address内部细节的实现类PersonImpl中。
		//但前向引用的使用带来的好处也显而易见：Date和Address类的任何修改，都不会导致
		//Person类使用程序的重新编译。解决了代码1中的问题。
};
//Person类不能访问Date、Address的内部细节，并不代表其实现类PersonImpl也不可以。
//只要PersonImpl前包含Date、Address的定义式，则就可以访问。Date、Address的定义式
//可以通过#include将其他编译单元的内容包含进来，也可以直接实现在与PersonImpl同一
//编译单元中的PersonImpl之前，如下：
#include <iostream>
using namespace std;
class B;
class A{
	public :
		B p(B b);
	private :
		B & c;
		B * d;
};
class B{};
B A::p(B b){
	return b;
}
int main(){
	return 0;
}

关于前向引用声明：（以class B;class A{};为例）
（1）A的函数成员形参、私有数据成员、函数成员返回值等类型为B &。
（2）A的函数成员形参、私有数据成员、函数成员返回值等类型为B *。
（3）A的函数成员返回值类型为B。
*/

/*
代码3：

//PersonImpl的实现
//after identification of Date、Address
class PersonImpl{ //内部和代码1中的Person类一样 
	public :
		Person(const string & name , const Date & birthday , const Address & address); //前向引用声明的类型可以定义引用
		string getName() const;
		Date getBirthay() const;
		Address getAddress() const;
	private :
		string name;
		Date birthday;
		Address address;
};

//Person的实现过程：
#include "Person.h"
#include "PersonImpl.h"
Person::Person(const string & name , const Date & birthday , const Address & address):pImpl(new PersonImpl(name , birthday , address)){}
string Person::getName() const{
	return pImpl->name();
}

//尽管在PersonImpl实现时，Date和Address的头文件或定义式出现在PersonImpl定义式之前，但代码总体的效率还是提高了。因为
//被广泛使用的是Person类，只要Person类保证不会因Date、Address类的修改而重新编译，那么使用Person的程序也就
//不需要重新编译了。上述代码做到了这一点。这里把因Date、Address类的修改而重新编译的代价放在了PersonImpl上，而PersonImpl
//又是以前向引用声明的形式出现在Person的编译单元中。因此只要PersonImpl这一类名不变，Person也就不用重新编译。 
*/

/*
代码4：

//person.h
class Date;
class Address;
class Person{
	public :
		virtual string getName() const = 0; //pure virtual
		virtual Date getBirthay() const = 0;
		virtual Address getAddress() const = 0;
		virtual ~Person(); //virtual
		static tr1::shared_ptr<Person> create(const string & name , const Date & birthday , const Address & address);
}

//personimpl.h
//after identification of Date、Address
#include "person.h"
#include "date.h"
#include "address.h"
class PersonImpl : public Person{
	public :
		PersonImpl(const string & name , const Date & birthday , const Address & address):name(name),birthday(birthday),address(address){
		}
		string getName() const;
		Date getBirthday() const;
		Address getAddress() const;
		~PersonImpl(){
		}
	private :
		string name;
		Date birthday;
		Address address;
};

//person.cpp
//after identification of PersonImpl
#include "person.h"
#include "personimpl.h"
class Date;
class Address;
tr1::shared_ptr<Person> Person::create(const string & name , const Date & birthday , const Address & address){
	return tr1::shared_ptr<Person>(new PersonImpl(name , birthday , address));
}

//main.cpp
#include "person.h"
#include "date.h"
#include "address.h"
int main(){
	string name(str);
	Date birthday(bir);
	Address address(add);
	tr1::shared_ptr<Person> per(Person::create(name , birthday , address));
	per->getName();
	per->getBirthday();
	per->getAddress();
	return 0;
}

//上述代码同样实现了代码2+代码3中的功能。  
*/

//总结：作为头文件的.h文件中的程序应该以“完全且仅有声明式”的形式存在。 
