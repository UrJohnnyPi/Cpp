/*
常量表达式：值不会改变；值能在编译期获得 的表达式。

const int a = 5; //是
const int b = a + a; //是
const int c = fun(); //不是，值在运行期获得

引入constexpr，解决这样的问题：const修饰的是编译期常量 和 运行期常量，所以如上，编译期 和 运行期能初始化的变量
都可以用const修饰。
而constexpr特指编译期的常量。如下：

constexpr int a = 5; //正确，字面值就是constexpr
constexpr int b = a + a; //正确，仅含constexpr而无普通变量的表达式的值是constexpr
constexpr int c = fun(); //仅当fun()也是constexpr时，表达式才正确。

当constexpr修饰指针时，constexpr把它所修饰的表达式置为顶层const，即仅对指针有效，而与指针所指向的对象无关：
const int * p = nullptr; //p指向的值是常量
constexpr int * p = nullptr; //p是常量
*/

/*
constexpr函数：用constexpr修饰；形参 和 返回值类型都是字面值类型；返回值是constexpr；必有一个return。
如：
constexpr int fun(int x){
    return 7 * x;
}

fun(8); //正确

{
    int i = 8;
    fun(i); //错误，non-constexpr 参与的表达式就是non-constexpr
}

constexpr函数是隐式内联的，因此欲实现内联效果，需要满足inline的写法。
*/

/*

构造函数不能是const，但却可以是constexpr。
constexpr构造函数用来生成constexprn对象。

*/
