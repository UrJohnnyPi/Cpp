/*
条款04：确定对象被使用前已被初始化
（1）自定义类的初始化操作发生在初始化列表中，而函数体内的叫做赋值操作。
像const数据成员、references数据成员虚基类构造函数、基类构造函数都必须
在初始化列表中完成初始化操作。

（2）类中的初始化操作次序：
虚基类
基类：声明继承时的顺序。
私有数据成员：声明为数据成员时的顺序。

（3）static分为两类：
local static：（有static修饰符显式声明）
函数内的、使用static关键字修饰的对象（即变量，实际上变量即对象）称为local static。
non-local static：（可以没有static修饰符显式声明）
global对象、namespace内定义的对象、class内定义的对象、file内定义的对象称为non-local static。

（4）编译单元：能产生单一目标文件的源码。.cpp+.h构成的能生成单一完整目标文件的源码。
*/

/*
代码1：

#include <iostream>
#include <string>
#include <list>
using namespace std;
class ABEntry{
	public :
		ABEntry();
		ABEntry(const string & name , const list<int> & phoneNum , const int & age);
	private :
		string name;
		list<int> phoneNum;
		int age;
};
ABEntry::ABEntry():name(),phoneNum(),age(0){} //STL容器会调用默认构造函数，内置基本类型需显示初始化。
ABEntry::ABEntry(const string & name , const list<int> & phoneNum , const int & age):name(name),phoneNum(phoneNum),age(age){}
int main(){
	ABEntry abe; //若对象A调用无参构造函数，则main函数中A对象后连括号也没有。
	return 0;
}
*/

/*
代码2：

//file 1
class FileSystem {
		//do something
		int numDisks() const{ return 0; }
};
extern FileSystem tfs; 
//extern用于两个cpp文件，使得该变量的作用域扩展到其他cpp中，该变量不能是static,const。 
//.h文件通过#include ""来将内部定义扩展到其他域，这时变量就不设限了。
 

//file 2
class Directory {
	public :
		Directory():disks(tfs.numDisks()){} 
		//这里可能会报错。 
		//因为C++不保证不同编译单元中的non-local static的初始化顺序。 
		//因此，执行该语句时，tfs可能还没有被初始化。
		//比如，当编译器先编译file 2时，当它编译到该行时，因为当前头文件中不包含tfs符合， 
		//所以，它会试图从其他cpp文件中寻找extern符号以匹配。编译器在file 1中
		//找到了tfs符号，但问题是file 1中的tfs处于未编译状态，即状态未知，这时
		//编译器可能不会说进而去编译fille 1文件，而是可能直接报错。而错误的原因就是
		//编译器此时知道tfs是一个extern FileSystem，因此只能使用extern这个性质，而不能
		//涉及FileSystem的内部细节，因为file 1未编译，所以这里tfs.numDisks()就必然报错。 
		//倘若编译器找到的file 1中的这个tfs是一个函数，那编译器就一定不会报错。 
		//可能是因为函数名是一个pointer，其状态一直确定，为待执行的extern FileSystem & 。 
		
		//综上，extern只能保证由此开始的一个域内的行为被执行。若这个域内的行为是声明，如：
		//extern FileSystem tfs;，那么file 2中就只能从这个声明中获取所有关于tfs的信息；若
		//这个域内的行为是函数，那么这个函数返回值就是其能提供的信息，而函数本身又可以通过
		//定义对象来使编译器编译相应的类型，进而得到一个定义式而非声明式，也因此能够提供类中
		//的信息。 
	private :
		int disks;
};
extern Directory tempDir(); //同理，在file 3中使用tempDir对象也可能会出错。 
*/

/*
代码3：

//file 1
class FileSystem {
		//do something
		int numDisks() const{ return 0; }
};
extern FileSystem & tfs(){ 
	static FileSystem fs;
	//将tfs写成函数，当被调用时，必然会执行对fs的初始化。因而不会出错。解决了代码2中的问题。 
	//即non-local static -> local static，使得变量的生命周期从定义语句第一次出现始
	//到代码全部执行完毕后结束；变量的作用域从函数中定义的那行起到函数结尾。 
	//而non-local static 的生命周期和作用域全从定义的那行起到代码执行完毕后结束。 
	 
	return fs;
}

//file 2
class Directory {
	public :
		Directory():disks(tfs.numDisks()){} 
	private :
		int disks;
};
extern Directory & tempDir(){ //同理 
	static Directory td;
	return td;
}
*/

