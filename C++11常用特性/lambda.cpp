/*
1.lambda表达式结构：
[capture list](parameters list) -> return type {function body}
lambda采用尾置返回类型。
lambda是一个未命名的内联函数。
lambda可以定义在函数内部。
lambda可省去(parameters list) -> return type部分，最简结构是[]{...}，如：auto f = []{return 42;}; cout << f() << endl; 注意调用方式不变。
lambda不能设置缺省参数。
lambda中的capture list（捕获列表），用于放置要在lambda中使用到的父函数中定义的变量。
当lambda不指定返回类型时，如果lambda体内含有除return之外的任何语句，编译器假定该lambda返回void；若该lambda只含return，则编译器据return val中val的值来推断返回类型。如：
stable_sort(words.begin() , words.end() , [](const string & a , const string & b){ return a.size() < b.size();}); //推断返回bool
stable_sort(words.begin() , words.end() , 
    [](const string & a , const string & b){ 
        if(a.size() < b.size())
            return true;
        else return false;
    }); //与上例效果一致，但编译器将假定为void
修改为：[](const string & a , const string & b) -> bool {...}，即使用尾置返回类型。

2.实例
stable_sort(words.begin() , words.end() , [](const string & a , const string & b){ return a.size() < b.size();});

auto wc = find_if(words.begin() , words.end() , [sz](const string & a){ return a.size() >= sz;});

3.for_each算法：对范围内的每一元素执行指定函数
原型：
template<typename InputIterator, typename Function>
Function for_each(InputIterator beg, InputIterator end, Function f) {
  while(beg != end) 
    f(*beg++);
}
实例：
for_each(v.begin() , v.end() , fun(para)/lambda); //注意这里要写全fun(para)

4.lambda捕获
值捕获实例：
void fun(){
    int v = 5;
    auto f = [v]{return v;}; //值捕获
    v = 0;
    auto j = f(); //j为5而非0
}
值捕获要做这么一件事，readonly v = v;前一个v是lambda函数体中的v，后一个v是fun中定义的v（也是[]中的v），所以有：
（1）lambda中无法对捕获来的值做修改。即这里[v]{return --v;}错误
（2）赋值操作是在lambda创建时完成的，后续调用不会重新赋值。
（3）若想在lambda中对捕获的值作出修改，有：auto f = [v]() mutable{return --v;};注意，()不能少。

引用捕获实例：
void fun(){
    int v = 5;
    auto f = [&v]{return --v;}; //值捕获
    auto i = f(); //i为4
    v = 0;
    auto j = f(); //j为0
}
引用捕获可以让lambda实现对v的修改。

隐式捕获：让编译器根据lambda体中的代码自动推断其捕获的是父函数中的哪个变量，[&]表示引用捕获，[=]表示值捕获
find_if(words.begin() , words.end() , [=](const string & a){ return a.size() >= sz;})； //lambda体内所有未知变量推断为值捕获
find_if(words.begin() , words.end() , [&](const string & a){ return a.size() >= sz;})； //lambda体内所有未知变量推断为引用捕获
混合使用隐式捕获 和 显式捕获：[]的第一个参数必须是 = 或 &符号，显式参数置于后面
find_if(words.begin() , words.end() , [= ， &sz](const string & a){ return a.size() >= sz;})； //lambda体内的sz采用引用捕获，其他所有未知变量推断为值捕获
find_if(words.begin() , words.end() , [& ， sz](const string & a){ return a.size() >= sz;})； //lambda体内的sz采用值捕获，其他所有未知变量推断为引用捕获

参考：https://blog.csdn.net/weixin_43055404/article/details/103299156
*/