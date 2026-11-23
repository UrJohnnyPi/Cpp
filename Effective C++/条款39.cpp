/*
条款39：明智而审慎地使用private继承

public继承的含义是is a。
private继承的含义是“根据某物实现出”，这和条款38谈到的复合的第二个含义相同。意即，
如果D以private形式继承B，则D对象是“根据B对象实现出来的”。

既然private继承 和 复合具有相同的含义，那如何取舍？答案是：尽可能使用复合，必要时再使用
private继承。 
*/

/*
代码1：

//举例，假设要定义一个Widget（小器物），我们要为这个Widget引入一个已有的Timer（计时器），
//使得Widget内某个函数可以实现这样的功能：该函数能周期性的访问某些数据。
//private继承写法： 
class Timer{
	public :
		explicit Timer(int tickFrequence);
		virtual void onTick() const; //定时器每滴答一次，该函数被调用一次。
		//do something 
};
class Widget : private Timer{
	private :
		virtual void onTick() const;
		//do something
}; 
//复合写法：
class Widget{
	private :
		class WidgetTimer : public Timer{
			public :
				virtual void onTick() const;
		}; 
		WidgetTimer timer;
		//do something
}; 
//尽量用 复合 替代 private继承 的原因一：
//若想为Widget写一个Derived class，但又不想让Derived class重写onTick()，即onTick()的virtual继承性
//停止于此，则private继承写法（事实上，只要是继承写法）都无法做到这点。只能用复合。
//原因二：降低Widget的编译依存性。若使用private继承，Timer的定义式必须可见，因此得使用#include "Timer.h"； 
//但若使用复合，可以在另一个编译单元中定义WidgetTimer，在此编译单元中定义WidgetTimer & timer;，即
//把对Timer的相关修改操作放到另一个编译单元。这样，就降低了Widget编译依存性。 
*/ 

/*
代码2：

//private继承主要用于：
//（1）A类不得不访问B类的Protected成员的情况下。此时A类只能作为Derived of B。 
//（2）为了重写virtual函数。 
*/

