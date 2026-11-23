/*
条款27：尽量少做转型动作

C style cast：
type(var);
(type)var;

C++ style cast：
const_cast(var); //一、常量指针被转化成非常量的指针，并且仍然指向原来的对象；二、常量引用被转换成非常量的引用，并且仍然指向原来的对象。
dynamic_cast(var); //实现安全的下行转换，但开销大，速度慢。因此在注重效率的代码中要尽量不使用dynamic_cast。 
static_cast(var); //强迫隐式转换 
reinterpret_cast(var);
*/

/*
代码1：
 
static_cast强迫隐式转换的例子：
class Widget{
	public :
		explicit Widget(int size);
		//do something 
};
void doSomeWork(const Widget & w);

doSomeWork(static_cast<Widget>(15));
*/

/*
代码2：

#include <iostream>
using namespace std;
class Window{
	public :
		Window(){
			test = 0;
		}
		virtual void display(){
			++test; 
			cout << "Window" << ends <<test <<endl;
		}
		void getTestVal(){
			cout << test << endl;
		}
		virtual ~Window(){
		}
	private :
		int test;
};
class SpecialWindow : public Window{
	public :
		void display(){
			static_cast<Window>(* this).display(); //输出：Window 1
			Window::getTestVal(); //输出：0
			
			//由此可见强迫隐式转换static_cast的问题所在：static_cast<Window>(* this)这个类模板语句 
			//并不返回* this的base部分，而返回一个* this的base部分的副本对象。即通过static_cast<Window>(* this)
			//只能改变* this的base部分的副本对象，而不能改变* this的base部分。 
			
			Window::display(); //输出：Window 1
			
			//欲修改* this的base部分，必须通过作用域分辨符，不能使用类型转换。 
			
			cout << "SpecialWindow" << endl; //输出：SpecialWindow
		}
};
int main(){
	SpecialWindow sw;
	sw.display();
	return 0;
}
*/

//将可能出现的、不能消除的转型写在函数中提供给客户，避免客户自己使用转型语句。 
