//代码1：
//new和malloc开辟的空间，必须用delete和free释放，否则这段内存不会随着域的终结而回收。
//这里之所以没体现出来这段未被回收的空间，完全是因为str被初始化为nullptr。str传值给p后，p也为nullptr，但p随后获得了100Byte的空间，
//而因为str和p没有联系的锚点，因此str仍是nullptr。
#include <iostream>
#include <cstring>
using namespace std;
void GetMemory(char * p){
	p = (char *)malloc(100); //若：p = (char *)malloc(100);，也是锚点分离的
}
void Test(void){
	char * str = nullptr; 
	GetMemory(str);
	if(str == nullptr) cout << "***";  //str为nullptr，输出：***
	else {
		strcpy(str , "hello , world");
		printf(str);
	} 
}
int main(){
	Test();
	return 0;
}

//代码2：
//使用realloc(start , Bytes)建立 稳固/不稳固 的锚点。
#include <iostream>
#include <cstring>
using namespace std;
void GetMemory(char * p){
	p = (char *)realloc(p , 100); //建立的不稳固锚。因为realloc是这样工作的：若从start开始，之后有足够的空间用来开辟Bytes，那就从start开始；否则，从另一内存开始开辟Bytes大小的空间，修改start指向该段空间的起始地址。
								  //因此，这样写有如下问题：（1）main传过来的是一个char型空间始址，而这里是先从这个始址开始，试图开辟100B的空间，即realloc执行成功，会将main传来的这段空间重新格式化。（2）若realloc
								  //执行失败，p将不再指向main传来的char型空间始址，而是另一段堆空间，p就与str再次分离，p与str指向两段不同的内存。等GetMemory返回时，实质上，已经造成了内存泄漏。

  //* p = (char *)realloc(* p , 100); //若GetMemory(char * * p);则建立的是稳固锚点。
}
void Test(void){
	char * str = new char; 
	GetMemory(str);
	if(str == nullptr) cout << "***"; 
	else {
		strcpy(str , "hello , world"); 
		printf(str); //输出：hello , world
	} 
}
int main(){
	Test();
	return 0;
}

//代码3：
//malloc建立稳固锚点，可以通过回收str来回收malloc开辟的空间
#include <iostream>
#include <cstring>
using namespace std;
void GetMemory(char ** p) {
	* p = (char *)malloc(100);
}
void Test(void) {
	char * str = nullptr;
	GetMemory(&str);
	if(str == nullptr) cout << "***";
	else {
		strcpy(str , "hello , world");
		printf(str); //有输出，正确
	}
}
int main() {
	Test();
	return 0;
}

//代码4：
//对于new同理，这里返回local variable的引用，但正确，因为这个local variable始终存在
#include<iostream>
using namespace std;
struct A{
	int i;
};
const A & clone(A & a) {
	A * p = new A(a);
	return * p;
}
int main() {
	A a;
	a.i = 7;
	cout << clone(a).i; //正确输出 
	return 0;
}

//代码5：
//这里返回一个local mem的地址，但正确。
#include<iostream>
using namespace std;
struct A{
	int i;
};
A * clone(const A & a) {
	A * p = new A(a);
	return p;
}
int main() {
	A a;
	a.i = 7;
	cout << clone(a)->i; //正确输出 
	return 0;
}

//代码6：
//无法对一个非new/malloc开辟的空间做引用，即无法对non-new/non-malloc local variable 做引用、取地址
#include<iostream>
using namespace std;
struct A{
	int i;
};
const A & clone(A & a) {
	A p(a);
	return p; //报错：对local variable的引用 
}
int main() {
	A a;
	a.i = 7;
	cout << clone(a).i;
	return 0;
}

//代码7：
//同理无法对一个non-new/non-malloc local variable 做引用、取地址
#include <iostream>
#include <cstring>
using namespace std;
char * GetMemory(void){
	char p [] = "hello , world";
	return p;
}
void Test(void){
	char * str = nullptr;
	str = GetMemory(); //str拿到一个已回收的空间的地址 
	if(str == nullptr) cout << "***";
	else printf(str); //str不为nullptr，但所指内存不定，故输出脏值 
}
int main(){
	Test();
	return 0;
}

//代码8：
//空间回收后，原有指针将指向不确定，在某些编译器中也可能被置为nullptr。
#include <iostream>
#include <cstring>
using namespace std;
void Test(void) {
	char * str = (char *)malloc(100);
	strcpy(str , "hello");
	cout << strlen(str) << endl;
	free(str);
	cout << strlen(str) << endl; //未初始化的str，即str不为nullptr，但也未指定值，是一个随机的、未知其所指内存边界的值。 
	if(str == nullptr) cout << "***";
	else {
		strcpy(str , "world");
		printf(str); //输出正确，但覆盖了后值；或直接报错：未初始化的str 
		cout << endl << strlen(str) << endl; 
	}
}
int main() {
	Test();
	return 0;
}

/*
总结：
1.malloc与new所定义的堆空间，在对应的free和delete被调用之前，是不会被释放的。即使malloc和new所在的域终结，这段空间也不会被回收。
malloc/new-heap的回收仅由free/delete实现。

2.non-new/non-malloc local variable是无法通过引用和取地址的方式被传递到域外的，因为域结束时，这些non-new/non-malloc local variable的空间便
被回收。而new/malloc local variable则可以通过引用和取地址的方式被传递到域外。

总结1和2，结合《C++内存分配》可知，栈空间总要在域结束时回收，栈中的数据（局部变量、函数参数、返回值）同时被消除；堆空间仅在使用delete/free 或 所有程序结束时才会被回收。

3.在函数实参与形参均为指针时，若试图在被调函数中使用new/malloc为该指针开辟空间，则要保证实参指针 要与 形参指针建立锚点。建立锚点的管用伎俩是* * p。

4.对指针delete/free后（或不为指针初始化），指针所指向的内存空间不确定（即起始地址、空间边界均不定），在某些编译器中也可能被置为nullptr。此时
若试图对该指针所指内存读写，则读写到脏值，或者直接报错：未初始化的指针。

5.
（1）
	new的动作：
	分配内存->针对此内存调用构造函数（new type：调用一次构造函数；new type [n]：调用n次构造函数）
	delete的动作：
	针对此内存调用一次或多次析构函数->释放内存
（2）
	常规：
	int * p = new int(); //空括号表示以0赋值，可以写其他值。如new int(5);。 
	
	一维数组：
	int * p = new int [表达式];
	delete [] p;
	int * p = new int [表达式](); //括号内不能自定义值，只能空括号，表示初始化为0。 
	delete [] p;
	
	多维数组：
	int (* p)[常量1][常量2];
	p = new int [表达式][常量1][常量2];
	delete [] p;
	注释：不加括号时：类可以看做是创建了一个隐含的对象，该隐含的对象调用默认构造函数。
	加括号时：在调用默认构造函数的基础上，对该隐含对象私有数据成员中的基本类型和指针类型的成员以0赋值。
	总之，调用new时使用[]，则调用delete时也使用[]。调用new时没使用[]，则调用delete时也不该使用[]。 
	
	数组的内存分布图解：https://blog.csdn.net/weixin_44319931/article/details/104703869
（3）
	看以下代码：
	typedef string add[4]; //定义了一个4元数组add，每个元素都表示string类型。
	string * strPtr = new add; //定义了一个strPtr指向这么一个空间：共四块，每块都是一个string类型所占的内存。等效于：string * strPtr = new string[4]; 
	delete [] add; //回收这四个空间，这里看上去不太符合（2）中所言，new中没有[]，但delete中却有[]。
	//因此，为避免出现这种易出错的情形，有结论：不对数组执行typedef。 

6.new和malloc语句是极有可能抛出异常的，分配空间失败时，new抛出bad_alloc异常，malloc返回NULL（而非nullptr，nullptr是C++11标准）。

7.要以独立语句将newd对象置入智能指针。
即：
将
processWidget(tr1::shared_ptr<Widget> (new Widget) , priority());
换为
tr1::shared_ptr<Widget> pw(new Widget);
processWidget(pw , priority());
首先要知道，同一语句段内，C++不保证执行顺序，即（1）new Widget（2）shared_ptr<Widget> (ptr)（3）priority()三个操作，只能保证（1）在（2）前，而不保证（3）执行完的时间点。
事实上，（3）的执行完成时间点和（1）、（2）没有任何关系。
因次第一种写法，若完成的时间点顺序为（1）（3）（2），则若priority()抛出异常（此时new可能已经开辟好内存，但还没来得及向shared_ptr赋值），程序因priority()异常而退出，但new
开辟的空间尚未得以回收，最终造成内存泄漏。

8.new-handler的用途：当使用new（或称operator new）开辟空间，出现错误时，C++指出可以先调用客户指定的异常处理函数，接口就是set_new_handler(new_handler)。
应用举例：
void outOfMem(){
	std::cerr << "Unable to satisfy request for memory.";
	std::abort();
}
int main(){
	std::set_new_handler(outOfMem); //定义new_handler = outOfMem，并实现set_new_handler。 
	int * pMem = new int[1000000000];
	return 0;
}
当本例中的operator new发生错误时，set_new_handler就会被自动调用。若operator new频繁发生错误，set_new_handler就会被频繁调用，直到new语句申请到足够内存。
set_new_handler(nullptr);，向set_new_handler传递null，是对new_handler的卸载，表示客户允许在operator new产生错误时，使用抛出异常的方式。 

设计一个良好的new_handler函数要做的事情：
（1）new_handler要能释放已申请到的无用内存，这能提高之后operator new的成功率。
（2）如果当前的new_handler无法获取到更多可用内存，它有能力通过set_new_handler重新安装一个更有效的new_handler，然后把职能托付于它。

9. new operator 和 operator new
（1）new operator：C++中定义的，无法改变其行为。如：T * ptr = new T;。

（2）operator new：用以实现自己设定的内存分配策略，可重载。
	重载版本：
	void *operator new(size_t);
	void *operator new[](size_t);
	void *operator delete(void*);
	void *operatordelete[](void*);
	注意： 
	只分配所要求的空间，不调用相关对象的构造函数。（不同于new operator）
	重载时，返回类型必须声明为void*。（原因在于上）
	重载时，第一个参数类型必须为表达要求分配空间的大小（字节），类型为size_t。
	重载时，可以带其它参数。
	举例：
	class Foo { 
		public:
        	void * operator new( size_t ){
				cout<<"in Foo's operator new"<<endl;
				return ::operator new(t);
			}
			Foo(){
				cout<<"in Foo's ctor"<<endl;
			}
	};
	
	Foo * foo = new Foo();  
输出：
in Foo's operator new
in Foo's ctor
分析：先调用operator new，再调用构造函数。不同与new operator，因为new operator是C++内建的，所以既可分配空间，又能调用构造函数。

总之，operator new是用来重载的，与类型无关，即不需要知道开辟的是哪个类型的实例空间，它只要求传入总字节大小，然后开辟之，初始化操作
交由其他函数（如构造函数）完成。

《Effective C++》条款16、条款17、条款49
*/
