/*
1.
C++中变量定义/声明格式为：存储类型 修饰符类型 数据类型 变量名;
存储类型：register（寄存器）、static（全局/静态变量存储区）、const（常量存储区）
修饰符类型：volatile（不稳定声明）、unsigned、signed、long、short
数据类型：int、char、float、double、bool
变量名：

2.编译器优化
例如：int x = 5;编译器为节省开销，采用优化的做法：若上次读x到i后，到目前，代码没有对x进行修改，
则本次操作仍从i中获取x的值。问题是，倘若i是一个寄存器，那么，因为i可能在这一间隔期内被不可控地
改变（如被操作系统、其他进程改变），则本次读到脏值。

3.
volatile修饰符会告知编译器，其修饰的变量可能会被不可控的改变。所以，每次读取该变量都从原始地址访问，
阻止编译器优化，提供对特殊地址的稳定访问。例如：
volatile int x = 5;

4.
(1) 可以把一个非volatile int赋给volatile int，但是不能把非volatile对象赋给一个volatile对象。
//正确
int y = 6;
volatile int x = y;

//错误
volatile int x = 6;
int y = x;

(2) 除了基本类型外，对用户定义类型也可以用volatile类型进行修饰。 
(3)
volatile int * ptr; //volatile data;
int * volatile ptr; //volatile ptr;
*/
