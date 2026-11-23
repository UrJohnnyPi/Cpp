/*
1.移动构造函数
不同与拷贝构造函数，移动构造函数不是拷贝源对象的资源，而是接管源对象掌管的资源。
class Test{
    public :
        Test(int * ptr1 , int * ptr2) : ptr1(ptr1) , ptr2(ptr2){
        }
        Test(const Test & t) : ptr1(new int(* t.ptr1)) , tr2(new int(* t.ptr2)){
        }
        Test(Test && t) noexcept : ptr1(t.ptr1) , ptr2(t.ptr2){
            t.ptr1 = nullptr;
            t.ptr2 = nullptr;
        }
    private :
        int * ptr1 , * ptr2;
};
对上述移动构造函数有几点说明：
a.注意是non-const，因为要改变t的内容。
b.注意是&&，因为意在让形参t绑定到源对象的值上，这样就可以管理“值”资源，实现接管“值”。
c.注意noexcept，noexcept是C++11引入的，用于声明该函数不抛出异常。而移动构造函数应声明为不抛出异常，原因如下：
首先，移动构造函数可能也允许抛出异常，但若编译器无法认定其不抛出异常，编译器就不会调用移动构造函数而转而使用拷贝构造函数，
因为若移动构造函数抛出异常，则表示一部分资源的控制权完成了转移，一部分没有，这就致使源对象和* this各掌管一部分，出错。
但若拷贝构造函数抛出异常，源对象不变，直接回收* this的拷贝空间就行。所以，若能保证移动构造函数不抛出异常，就要声明为
noexcept，保证其可以被调用。
*/

/*
2.移动赋值运算符
class Test{
    public :
        Test & operator = (Test && t) noexcept{
            if(t != * this){
                ptr1 = t.ptr1;
                ptr2 = t.ptr2;
                t.ptr1 = t.ptr2 = nullptr; 
            }
            return * this;
        }
    private :
        int * ptr1 , * ptr2;
};
分析：移动赋值运算符 类比 copy assignment，即：能处理自我移动，把源对象的资源转移给* this。
又类比移动构造函数，通过右值引用来操作资源。
*/

/*
3.注意
（1）源对象的资源转移后，实际上其最好被继而析构，而非再次调用。
（2）只有当一个类没有自定义copy函数（copy构造、copy assignment），且它的所有数据成员都可以通过移动函数（移动构造、移动赋值）构造或赋值时，编译器才会为它自生成移动函数。
分析：若一个类自定义了copy函数而未定义移动函数（或移动函数未声明为noexcept时），那么编译器总试图去使用copy函数而非移动函数，因为copy函数更少犯错（尽管开销更大）。所以
编译器也就不会自生成移动函数了；第二句话要注意的是，内置类型的数据成员一定能被移动，string类型也有自己的移动操作，如果数据成员是类类型，那么只有该类有自己的移动操作时，这个
数据成员才能被移动。
//例子1
struct X{
    int x;
    string s;
};
struct Y{
    X y;
};
X x1 , x2 = move(x1);
Y y1 , y2 = move(y1);

//例子2
struct X{
    //定义copy构造，不定义移动函数
};
struct Y{
    Y() = default; // construct = default旨在说明：该类中需要默认的构造函数，即使类中自定义了构造函数，也不该覆盖默认构造。
    Y(Y &&) = default;
    X x;
};
Y y1 , y2 = move(y1); //错误，因为Y的数据成员x无法移动，所以y1无法move。尽管这里的Y声明要使用默认的移动构造，但实际上Y是没有默认移动构造的，原因如（2）。

（3）定义了移动函数的类也要自定义copy函数，否则不自动生成。

（4）
class Foo{
    public :
        Foo = default;
        Foo (const Foo &);
};
Foo x;
Foo y(x); //调用拷贝构造
Foo z(move(x)); //调用拷贝构造
该类定义了copy函数，未自定义移动函数，所以编译器也不会自生成移动函数。move(x)返回一个x的右值引用Foo &&，但因为没有移动函数，所以Foo &&转换为
const Foo &，转而调用拷贝函数。而（2）中的例子2之所以move报错，就在于Y既没有移动函数，也没有copy函数。

（5）
class HasPtr{
    public :
        HasPtr(const HasPtr & p): ps(p.ps){
        }
        HasPtr(HasPtr && p) noexcept : ps(p.ps){
            p.ps = nullptr;
        }
        HasPtr & operator = (HasPtr rhs){
            swap(* this , rhs);
            return * this;
        }
};
注意这里的赋值运算符，它既实现了拷贝赋值运算的功能，也实现了移动赋值运算的功能。如：
HasPtr hp1 , hp2;
hp1 = hp2; //赋值运算充当赋值运算
hp1 = move(hp2); //赋值运算充当移动赋值运算
原理就在于参数既不是 const HasPtr & rhs，也非 HasPtr && rhs，而是 HasPtr rhs。这就发生：
hp1 = hp2;调用赋值函数，即 hp1.operator = (hp2);hp2传值给rhs，调用拷贝构造，rhs获值，p也未被修改，最后swap(*this , rhs)。
hp1 = move(hp2);调用赋值函数，move(hp2)传临时值给rhs，调用移动构造，rhs获值，p被置空，最后swap(*this , rhs)。

所以自己写类时，不用写6个函数，只写5个函数就行：构造、copy构造、移动构造、赋值运算、析构。

（6）其他成员函数若能同时考虑到copy和移动两个版本，效率会更高：
class StrVec{
    public :
        void push_back(const string &);
        void push_back(string &&);
};
StrVec vec;
string s = "I love you";
vec.push_back(s); //调用第一个push_back
vec.push_back("I love you"); //调用第二个push_back

（7）
class Foo{
    public :
        Foo sorted() && { //* this在本函数中充当右值，意味着其没有其他用户，即可以原址排序
            sort(data.begin() , data.end());
            return * this;
        }
        Foo sorted & { //* this在本函数中充当左值，意味着其可能有其他用户，不能原址排序
            Foo ret(* this); //不能原址排序，只能copy
            sort(ret.begin() ,ret.end());
            return ret;
        }
        Foo sorted const && { //* this在本函数中是const，不能原址排序
            Foo ret(* this); //不能原址排序，只能copy
            sort(ret.begin() ,ret.end());
            return ret;
        }
};
Foo a;
Foo b = move(a);
b.sorted(); //调用第一个sorted()
Foo c = a;
c.sorted(); //调用第二个sorted()

此外：
Foo{
    public :
        Foo sorted() &&;
        Foo sorted() &;
        Foo sorted() const; //错误
};
第三个sorted()错误的原因是：如果一个成员函数有引用限定符，则具有相同参数列表的所有版本都必须有引用限定符。

（8）编译器可默认生成的函数有6个，但不是一定会生成。
*/