/*
auto

1.auto让编译器通过 值的类型 来推理 其声明变量的类型，如：
auto item = val1 + val2; //若val1 + val2的类型是double，那么item就是double；若val1 + val2的类型是int，那么item就是int。

2.使用auto也能在一条语句中声明多个变量
类比：
int a = 5 , b = 6; 
int a = 5 , * p = &a; //之所以能在一条语句中声明多个变量，是因为一条语句中只有一个基本数据类型。

auto a = 4 , b = 5; //正确，可以推断出auto是int
auto i = 0 , * p = & i; //正确，可以推断出auto是int
auto i = 9 ， b = 3.14; //错误，无法推断auto是int还是double/float

3.顶层const 和 底层const
（1）顶层const：用于声明变量为const。如：int * const ptr、const int a
（2）底层const：用于声明指针指向的数据、引用绑定到的数据为const。如：const int * ptr、const int &a

（3）auto忽略顶层const，保留底层const
const int a = 5;
auto b = a; //忽略表达式的顶层const属性，b推出为int
auto c = & a; //推出c为const int *，保留表达式的底层const属性
auto & d = a; //推出d为const int &

若想保留顶层const属性：
const auto e = a; //推出e为int，加上const为const int

此外
auto & f = 42; //错误，不能绑定到字面值
const auto & f = 42; //正确
*/

/*
decltype

1.decltype(表达式)将返回表达式运算结果的类型，但并不实际进行表达式的运算
decltype(f()) a = b; //若f()返回值是int，则a是int，但编译器并不调用f()

const int a = 0 , & b = a;
decltype(a) c = 6; //const int
decltype(b) d = a; //const int &

2.引用
int a = 7 , * b = & a , & c = a;
decltype(c) //返回 int &
decltype(c + 0) //返回 int，因为该表达式的值将存储在另一变量而非c中，而另一变量是int而非 int &
decltype(* b) //返回int &而非int，因为实际上是：int * b = & a;而b是指向a空间的指针，* b是a空间的引用。
decltype(a) //返回int
decltype((a)) //返回int &，双括号((variable))必然返回variable的引用。而单括号，只有在variable是引用时才返回引用。
*/