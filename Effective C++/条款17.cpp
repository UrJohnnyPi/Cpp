/*
条款17：以独立语句将newd对象置入智能指针


*/
//设有以下两个函数 
int priority(); //返回程序的优先权 
void processWidget(tr1::shared_ptr<Widget> pw , int priority); //根据不同的优先权，在获得的Widget资源上执行不同的操作。 

//错误的调用方式：
processWidget(new Widget , priority()); //错在了第一个参数类型不匹配。形参：Widget *；实参是shared_ptr<Widget>。

//正确的调用方式（但可能造成资源泄漏）：
processWidget(tr1::shared_ptr<Widget> (new Widget) , priority());
//正确的调用方式也可能出现资源泄漏的问题，原因在于C++对同一表达式内不相干的
//几个操作没有执行顺序上的规定。比如该例中，可能的执行顺序是：
//一：执行new Widget->调用tr1::shared_ptr<Widget>的构造函数->调用priority()->最终初始化形参
//二：执行new Widget->调用priority()->调用tr1::shared_ptr<Widget>的构造函数->最终初始化形参
//三：调用priority()->执行new Widget->调用tr1::shared_ptr<Widget>的构造函数->最终初始化形参
//这三种执行顺序中，只有两点是可以确定的：“执行new Widget”必然在“调用tr1::shared_ptr<Widget>的构造函数”之前；
//“最终初始化形参”一定在所有操作之后。
//C++无法保证编译器将选择上述三种执行顺序中的哪种。各编译器对此的设计也没有统一。因此
//若当前的编译器选择执行顺序二，则可能会造成资源泄漏。顺序二中，当“调用priority()”出现
//异常时，将导致程序结束，但已获得的Widget资源却无法被释放（因为此时的Widget尚未交到资源管理对象手中）。
//造成资源泄漏。为解决这一问题，根据此条款，代码如下：

tr1::shared_ptr<Widget> pw(new Widget);
processWidget(pw , priority());
//这样分离的语句是最正确的方式。因为编译器对语句的执行顺序没有随意安排的权利，必须从头到尾一条一条。 
