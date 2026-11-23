/*
智能指针
*/

/*
shared_ptr

1.
shared_ptr<list<int>> sp(new list<int>); //将 list<int> 资源托管到sp中，调用shared_ptr的构造函数
shared_ptr<list<int>> ssp = sp; //调用shared_ptr的copy assignment
shared_ptr<list<int>> sssp(ssp); //调用shared_ptr的拷贝构造函数

2.
make_shared<T> (args);
功能：返回一个shared_ptr，指向一个动态分配的类型为T的对象。使用args初始化此i对象。
举例：
shared_ptr<int> p1 = make_shared<int>(42);
shared_ptr<string> p2 = make_shared<string>(10 , '9'); //p2指向"9999999999"
shared_ptr<int> p3 = make_shared<int>(); //p3指向0
auto p4 = make_shared<int>(6);

3.shared_ptr类中应该有一个static数据成员，叫做“计数器”，用来统计指向同一资源对象的shared_ptr指针数。
shared_ptr的析构函数具有这样的能力：
--计数器；
if(计数器 == 0) destory 资源对象;

4.因为智能指针内部原理是new和delete（详见C++应试知识点总结/智能指针），所以要明白，智能指针作用的存储空间
是堆（自由存储区），因此满足自由存储区数据的特性，比如：
shared_ptr<Foo> factory(T arg){
    return make_shared<Foo> (arg);
}

void use_factory(T arg){
    shared_ptr<Foo> p = factory(arg);
}

但不能：
void factory(T arg){
    shared_ptr<Foo> p(arg);
}

void use_factory(T arg){
    use p;
}
因为在作用域的结束处，p被析构，资源对象被回收。

5.关于类封闭性的讨论
（1）深复制 浅复制 问题，旨在让实例对象实现封闭，copy行为会导致新的空间/资源的开辟，新的实例对象的数据成员指针指向该新的空间/资源。
（2）拒绝copy，比如uncopyable的子类。
（3）copy行为具有move语义，即资源转移，比如unique_ptr。
（4）共用+计数器，如这里的shared_ptr。

6.有如下：
int * ptr = new int();
shared_ptr<int> sp(ptr);
shared_ptr<int> ssp(ptr);
与：
shared_ptr<int> sp(ptr);
shared_ptr<int> ssp(sp);
二者是不同的，前者调用了两次构造函数，因此，sp和ssp维护的不是同一个计数器（因为+1操作发生在copy中，而非构造函数中），因此会出现这样的情况：若sp的作用域结束，sp
管理的资源被回收，那么ssp将悬空。
此外，如下的也是不合法的：
int * ptr = new int();
shared_ptr<int> sp(ptr);
int p = sp.get();
delete p;

7.unique()和reset()
if(!sp.unique()) sp.reset(new string(* p)); //若sp不是管理其所管理的资源对象的唯一shared_ptr，则reset
* p = += newVal; //否则，直接重置资源对象的值
*/

/*
auto_ptr

auto_ptr被C++11废除的原因：
因为有：
auto_ptr<T> ap(new T);

auto_ptr<T> aap(ap); 或 auto_ptr<T> aap = ap;

cout << * ap << ends << * aap;

auto_ptr中的copy函数的存在是其被废除的原因所在。上述这段代码在运行期出错，报错：ap悬空。
对比unique_ptr，unique_ptr中就没有copy函数，所以如：unique_ptr<T> uup(up);这样的
写法会在编译期就报错。
尽管auto_ptr的copy函数也实现了资源转移的功能，但问题就在于，copy函数的存在让如cout << * ap;这样
错误的语句在编译期不能尽早发现。
*/

/*
unique_ptr

1.不正确的资源转移写法
unique_ptr<int> up(new int(5));
unique_ptr<int> uup(up); //错误
uup = up; //错误

正确的资源转移写法
unique_ptr<int> up(new int(5));
unique_ptr<int> uup(up.release()); //正确
uup.reset(uup.release()); //正确

2.重载删除器
unique_ptr<objT , delT> p(new objT , fun); //其中，delT是fun的返回值类型
*/