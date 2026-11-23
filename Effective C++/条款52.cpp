/*
条款52：写了placement new也要写placement delete

正常的operator new：（一般写法、class写法） 
void * operator new(size_t size) throw(bad_alloc);
正常的operator delete：
void operator delete(void * rawMemory) throw(); //一般写法 
void operator delete(void * rawMemory , size_t size) throw(); //class写法 

当都是用正常的new和delete签名时，系统就总能将new和delete对应起来。

一个典型的placement new（不正常的）：
void * operator new(size_t size , void * pMemory) throw(bad_alloc); //含有了专属的参数pMemory

考虑：
Widget * pw = new Widget;
该语句的执行使得两个函数被调用：operator new和Widget构造函数（在new中提供sizeof(Widget)，
甚至允许新开辟的空间初始化）。如果构造函数执行错误，则要求new函数有能力将程序恢复到进入
new前的状态。这一任务不由用户自行使用delete完成，因为构造函数执行错误发生在运行期，且pw
尚未赋值完成，所以这一任务就由运行期系统自动调用与new相对应的delete完成。如果系统找不到
相对应的delete，则会造成内存泄漏（因为在构造函数错误前，已分配的空间无法释放）。 

相对应的placement new和placement delete具有相同的额外参数。placement delete只在构造函数
异常时（或准确的说，是运行期出错时）被运行期系统自动调用。对一个没有异常的operator new和
构造函数，使用的delete是正常的delete。例如：
class Widget{
	public :
		static void * operator new(size_t size , ostream & logStream) throw(bad_alloc); //placement new
		static void operator delete(void * rawMemory) throw(); //正常的new，无错误时，用于释放空间
		static void operator delete(void * rawMemory , ostream & logStream) throw(); //placement new，只有运行期错误时才会被系统自动调用 		 
};
Widget * pw = new (cerr) Widget; //ostream &参数为cerr
delete pw; //内存分配成功，pw已指向分配的内存，调用正常的delete 

对于class而言，类中的operator new/delete会隐藏C++提供的global operator new/delete；Derived中的
operator new/delete会隐藏继承自Base的operator new/delete。于是，倘若Widget中只定义了placement new/delete，
那么如：Widget * pw = new Widget;和delete pw;就会报错。 
所以除非你真的想隐藏，为了使用自定义的new/delete，否则应该在class中声明两种版本：
class Base{
	public :
		//global operator new/delete 或称 正常的operator new/delete 
		static void * operator new(size_t size) throw(bad_alloc){
			return ::operator new(size); 
		}
		static void operator delete(void * rawMemory) throw(){
			::operator delete(rawMemory); 
		}
		//placement new/delete 
		static void * operator new(size_t size , ostream & logStream) throw(bad_alloc);
		static void operator delete(void * rawMemory , ostream & logStream) throw();
};
class Derived : public Base{
	public :
		using Base::operator new;
		using Base::operator delete;
		static void * operator new(size_t size , int n) throw(bad_alloc);
		static void operator delete(void * rawMemory , int d) throw();
}; 
*/
