/*
条款23：宁以non-member、non-friend替换member函数
*/
class WebBrowser { //网页浏览器 
	public :
		void clearCache();
		void clearHistory();
		void removeCookies();
		//do something
};
//现在出现的一个问题是，如果要添加一个clearEverything();函数，该函数是
//设置为WebBrowser的一个函数成员好，还是设置为一个non-member且non-friend的函数好？
//设置为member：
class WebBrowser { 
	public :
		void clearEverything();
		//do something
};
//设置为non-member且non-friend：
void clearEverything(WebBrowser wb){
	wb.clearCache();
	wb.clearHistory();
	wb.removeCookies();
	//do something
}
//二者的区别在于设计为non-member且non-friend函数使WebBrowser的封装性大大增强。
//对封装性的误解是：越多函数、数据放到类中，封装性越强。
//封装性的真正含义是：变动波及的客户越少，或说能看到内部变化的函数越少，封装性越强。
//在条款22中，提出要将类的数据成员全部放到private下，以避免放到public下波及客户函数，
//以避免放到protected下波及derived函数。现在就要考虑member function和non-member且non-friend function
//的封装性了。很明显，member function可以看到来自public、private和protected的变化，而non-member且non-friend function
//只能看到public的变化，因此封装性大大增强。 之所以要说non-member且non-friend function，是因为friend函数像goto语句
//一样，具有和member函数一样的访问权限。
//常有的WebBrowser设计模式：

//webbrowsercore.h
namespace WebBrowserStuff{
	class WebBrowser{
		//do something
	};
	void clearEverything();
	//这里写核心机能，即所有客户都需要的non-member函数。
	//意即将浏览器的核心机能（class WebBrowser + core non-member fun）放到 webbrowsercore.h文件里。 
}

//webbrowserbookmarks.h
namespace WebBrowserStuff{
	//与书签相关的non-member函数 
}

//webbrowsercookies.h
namespace WebBrowserStuff{
	//与cookie相关的non-member函数 
}

//others

//这样的设计模式也是namespace std的设计模式，想一想，其实在std中定义了string、vector、list和cout等，
//使用string时加上#include<string>而不用加#include<iostream>，使用vector时加上#include<vector>而不用加#include<list>，
//这就减少了编译依赖，即编译器不用预处理、编译没加入的符号。
//namespace 可以跨越多个源文件，而其他符号不能，只能通过#include、extern、using等方式出现。 
