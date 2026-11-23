/*
条款16：成对使用new和delete时要采取相同形式

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

（3）看以下代码：
typedef string add[4]; //定义了一个4元数组add，每个元素都表示string类型。
string * strPtr = new add; //定义了一个strPtr指向这么一个空间：共四块，每块都是一个string类型所占的内存。等效于：string * strPtr = new string[4]; 
delete [] add; //回收这四个空间，这里看上去不太符合（2）中所言，new中没有[]，但delete中确有[]。
//因此，为避免出现这种易出错的情形，有结论：不对数组执行typedef。 
 
*/
