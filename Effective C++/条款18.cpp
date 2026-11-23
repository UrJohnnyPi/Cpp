/*
条款18：让接口容易被正确使用，不易被误用（以下所述的客户既包括界面用户，也包括API用户） 

（1）不给客户犯错的机会。能给定输入范围的，一定要给定，让客户在给定的范围内选择，不能
没有约束地自由输入。这个范围包括但不限于：每个参数类型、每个参数长度、现实意义的范围、输入格式等。
具体做法是：消除客户的资源管理责任、约束范围等。 
比如在设计Date类时：
class Date{
	public :
		Date(int month , int day , int year);
	private :
		int month;
		int day;
		int year;
};
这样的设计是极不好的。正确时，客户将这样调用：Date d(3,23,1997);，但如果输入顺序错误、输入值不符合实际
怎么办？与其在断定用户输入错误后报错，不如直接给定确定的范围，强制用户无法犯错。一个软件，用来
断定错误的语句越多越不好，反而其设计越不可能导致错误越好。比如，将Date改为附录1较好。 

 
（2）让客户的操作越简单、记忆的东西越少（或越符合主流直观）越好。具体做法有接口名一致性、类型兼容等。 
比如，STL容器接口中，
大多数的容器作用近似的方法都有着相同的名字，比如count()、find()、begin()、end()等，这就
让客户用起来就比较轻松，只要记住一种模式，其他的容器都会用。
还有就是在条款13中的代码3中有：
void startFactory(para){
	tr1::shared_ptr<Investment> i(createInvestment(para));
	//do something
}
试想，这个startFactory交由客户定义，将出现很大的问题，因为客户可能会忘记将资源第一时间
放到资源管理对象中，或甚至忘记使用资源管理对象而造成资源泄漏。因此，你设计factory时，
返回的最好是一个智能指针，而不是原始资源指针。这样，不论客户怎么定义startFactory，在被调factory
作用域的结束，资源必然被回收。 
*/

/*
附录1：

#include <iostream>
using namespace std;
//首先考虑是否要用Month定义对象？Month这里设计的是包括所有月份的类，不是包含所有月份
//共有性质的类。前者设计用于被继承、或直接提供static成员；后者设计用于定义对象。
class Month {
	public :
		static Month Jan() {
			Month jan(1);
			return jan;
			//返回一个参数为1的对象，按照构造函数的设计方式，这个参数是可以拿去的，不影响。
			//只要知道月份按照这个类的设计，不再是一个可由客户随意填写的参数了，而是一个既定
			//的、只供选择的变量。
		}
		static Month Feb() {
			Month feb(2);
			return feb;
		}
		//others
		static Month Dec() {
			Month dec(12);
			return dec;
		}
	private :
		explicit Month(int val) {}
		//不定义构造函数的话，会调用默认构造函数，不报错；写成这样的构造函数有两个作用：
		//一是为了让Month不能用于定义新对象（月份）；二是在客户定义新对象（月份）时报错。
};

class Year {
	public :
		static Year choosenYear(int val) {
			if(val >= start.getVal() && val <= end.getVal()) {
				Year cy(val);
				return cy;
			}
			Year cy(start.getVal());
			return cy;
		}
	private :
		static Year start;
		static Year end;
		static int getVal() {
			return val;
		}
		explicit Year(int val) {}
};
Year Year::start(2000);
Year Year::end(2020);

class Day{
	//要考虑年和月了。 
};

class Date {
	public :
		Date(const Month & m , const Day & d , const Year & y):month(m),day(d),year(y) {
			// do something
			cout <<
		}
	private :
		Month month;
		Day day;
		Year year;
};
int main() {
	Date date(Month::Dec() , Day::choosenDay ,Year::choosenYear(1919)); 
	return 0;
} 
*/
