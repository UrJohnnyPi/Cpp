/*
条款29：为“异常安全”而努力是值得的

//以下例展开叙述
class PrettyMenu{ //菜单背景背景图类
	public :
		void changeBackground(T & imgSrc);
		// do something
	private :
		Mutex mutex; //互斥器
		Image * bgImage; //目前的背景图
		int imageChanges; //背景图像被改变的次数
};
void PrettyMenu::changeBackground(T & imgSrc){
	Lock ml(&mutex); //来自于条款14，省去了调用者unlock()。
	delete bgImage;
	++imageChanges;
	bgImage = new Image(imgSrc);
}

“异常安全函数”应满足以下要求之一：
1.如果程序抛出异常，程序内的任何事物应当保持有效，即不能因异常而造成其他事物的败坏。所谓败坏，就是像
上述代码中，若new Image抛出异常，bgImage就空悬，imageChanges也多加了1。
2.如果域内抛出异常，程序内的事物能够回复到“域被执行前”的状态。
3.不抛掷异常保证：带有空白异常明细（throw()）者必为nothrow函数。例如：
int fun() throw();就表示fun()不抛出任何异常。 

“异常安全函数”调用的函数也应当是“异常安全函数”。 
*/

/*
代码1：（满足“异常安全函数”要求1） 

class PrettyMenu{
	public :
		void changeBackground(T & imgSrc);
		// do something
	private :
		Mutex mutex;
		tr1::shared_ptr<Image> bgImage;
		int imageChanges;
};
void PrettyMenu::changeBackground(T & imgSrc){
	Lock ml(&mutex);
	bgImage.reset(new Image(imgSrc)); //这个修改应用到了条款13的论点。
	++imageChanges;
}
//智能指针保证：只有new Image(imgSrc)执行成功后，才会执行delete bgImage，最后才是bgImage的重赋值。
//这样的改动使得changeBackground成为异常安全函数。
*/


/*
代码2：（满足“异常安全函数”要求2） 

//copy and swap策略：为你想要修改的原件做一个副本（copy），在副本上做修改，若修改抛出异常，
//则原件仍可保证处于修改前的状态；若所有修改没有抛出异常，则通过一个不抛出异常的操作，将原件
//和副本swap。

struct PMImpl {
	tr1::shared_ptr<Image> bgImage;
	int imageChanges;
};
class PrettyMenu {
	public :
		void changeBackground(T & imgSrc);
		// do something
	private :
		Mutex mutex;
		tr1::shared_ptr<PMImpl> pImpl;
};
void PrettyMenu::changeBackground(T & imgSrc){
	using std::swap;
	Lock ml(& mutex);
	//创建副本 
	tr1::shared_ptr<PMImpl> pNew(new PMImpl(* pImpl));
	//修改副本 
	pNew->bgImage.reset(new Image(imgSrc));
	++pNew->imageChanges;
	//执行到此说明上述过程未抛出异常，遂交换原件和副本 
	swap(pImpl , pNew); 
}

//copy and swap策略的问题显而易见，空间和时间的开销。 
*/
