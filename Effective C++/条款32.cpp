/*
条款32：确定你的public继承塑模出is-a关系

条款32解释为：public继承意味is-a（是一种）的关系。如：
class Base{}; 
class Derived : public Base{};
表示Derived 对象也是一种Base 对象。为了实现“是一种”的关系，我们要使Base类的设计
和Derived类的设计在逻辑上正确，而不仅只在编译时正确。
要把编译过程和实际的逻辑相匹配，即逻辑上如何如何的事物，其对应代码应在编译期、连接期还是运行期报错。 
*/ 

/*
代码1：

class Bird{
	public :
		virtual void fly();
		//do something 
};
class Penguin : public Bird{ //企鹅也是鸟 
	//do something
};
//企鹅也是鸟，但这样的继承方式使得编译过程和实际的逻辑不匹配，因为Penguin继承了来自Bird的fly()，这
//使得不论fly()如何被重写，Penguin对象对fly()的调用总能在编译期通过。而我们想要的是这样的局面：
//既然企鹅不能飞，我们就不应该允许其编译通过。 
*/

/*
代码2：

class Bird{ 
	//不声明fly函数 
	//do something 
};
class FlyBird : public Bird{
	virtual void fly();
	//do something 
}; 
class Penguin : public Bird{
	//不声明fly函数
	//do something
};
//该代码相较于代码1有进步，比如，这里的Penguin对象如果试图调用fly()，则会在编译期报错。
//但随之而来的问题是，我们把关注点都放在了“一只鸟能不能飞”的话题上，并基于此构造了类，
//但实际上，我们既可以按“能不能飞”分，也可以按“有没有喙”分。也就是说，上面的写法不
//符合泛化的思想。 
*/

/*
代码3：

class Bird{
	public :
		virtual void fly();
		//do something 
};
class Penguin : public Bird{
	public :
		void fly(){
			throw error("wrong");
		} 
		//do something
};
//该代码的问题甚至不如代码2，因为它对“企鹅不能飞”的反应在运行期发生。 
*/

/*
代码4：

class Bird{
	//不声明fly函数
	//do something
};
class Penguin : public Bird{
	//不声明fly函数
	//do something
};
//绝佳代码，注意它和代码2是不同的，该代码并没有按“fly属性”划分出FlyBird类。该代码只是
//将Bird必含的一些属性写入Bird类，不去触及那些可从不同角度划分Bird的属性。对于“飞不飞”，
//“有没有喙”这些区分Bird的属性，我们可以按照项目实际来构造（这就回到了代码2）。如果只是
//要求泛化的继承体系，不提供实际要求，那我们既不可以将Bird衍生出FlyBird和non-FlyBird，也不可以
//衍生出HasMouth和HasNoMouth。我们只可以如该代码所述，为每一个鸟的种类写自己的独特的派生。 
*/
