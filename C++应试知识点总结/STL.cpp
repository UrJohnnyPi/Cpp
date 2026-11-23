/*
常见STL容器函数整理：

1.STL序列式容器：
（1）vector：（内部原理是动态数组，连续内存）（头文件：#include <vector>）
begin(),end(),size(),empty(),front(),back(),push_back(),pop_back(),clear(),erase(),emplace()

vector内部原理：
为降低二次分配时的成本，为vector实际分配的 堆空间 大小可能比客户需求的更大一些。
为vector分配的堆空间是capacity，客户需求的堆空间大小是size，capacity >= size。
当vector的大小和容量相等（size==capacity），也就是满载时，如果再向其添加元素，那么vector就需要扩容。vector容器扩容的过程需要经历以下 3 步：
a.完全弃用现有的内存空间，重新申请更大的内存空间；（申请的新空间，GCC下是原capacity的2倍，VS下是1.5倍）
b.将旧内存空间中的数据，按原有顺序移动到新的内存空间中；
c.最后将旧的内存空间释放。
这也就解释了，为什么 vector 容器在进行扩容后，与其相关的指针、引用以及迭代器可能会失效的原因。（重点）
四个相关的函数：
v.size(); //返回v的size大小
v.resize(n , val); //将v的size值置为n；若n>原size()，将unsed的capacity中的n-size()大小的空间纳入size部分，并填充val值；若n<原size()，将size中的后size()-n销毁。
v.capacity(); //返回v的capacity大小
v.reserve(n); //分配capacity空间，若n>原capacity()，则按照上述规则扩容；否则，什么也不做。

#include <iostream>
#include <vector>

int main ()
{
  std::vector<int> myvector;

  // set some initial content:
  for (int i=1;i<10;i++) myvector.push_back(i);

  myvector.resize(5);
  myvector.resize(8,100);
  myvector.resize(12);

  std::cout << "myvector contains:";
  for (int i=0;i<myvector.size();i++)
    std::cout << ' ' << myvector[i];
  std::cout << '\n';

  return 0;
}
输出：myvector contains: 1 2 3 4 5 100 100 100 0 0 0 0

emplace_back() 和 push_back()：
二者实现的都是向v后添加元素。但emplace_back()效率更高，原因是：底层实现的机制不同。
push_back() 向容器尾部添加元素时，首先会创建这个元素，然后再将这个元素拷贝或者移动到容器中（如果是拷贝的话，事后会自行销毁先前创建的这个元素）；
而 emplace_back() 在实现时，则是直接在容器尾部创建这个元素，省去了拷贝或移动元素的过程。
如下：
#include <vector> 
#include <iostream> 
using namespace std;
class testDemo
{
public:
    testDemo(int num):num(num){
        std::cout << "调用构造函数" << endl;
    }
    testDemo(const testDemo& other) :num(other.num) {
        std::cout << "调用拷贝构造函数" << endl;
    }
    testDemo(testDemo&& other) :num(other.num) {
        std::cout << "调用移动构造函数" << endl;
    }
private:
    int num;
};

int main()
{
    cout << "emplace_back:" << endl;
    std::vector<testDemo> demo1;
    demo1.emplace_back(2);  

    cout << "push_back:" << endl;
    std::vector<testDemo> demo2;
    demo2.push_back(2);
}
运行结果为：
  emplace_back:
  调用构造函数
  push_back:
  调用构造函数
  调用移动构造函数
在此基础上，读者可尝试将 testDemo 类中的移动构造函数注释掉，再运行程序会发现，运行结果变为：
  emplace_back:
  调用构造函数
  push_back:
  调用构造函数
  调用拷贝构造函数
由此可以看出，push_back() 在底层实现时，会优先选择调用移动构造函数，如果没有才会调用拷贝构造函数。

emplace() 和 insert():
通过 insert() 函数向 vector 容器中插入类对象，若是隐式转换或临时对象则先需要调用类的构造函数，之后必要调用移动构造函数（或拷贝构造函数）
emplace() 在插入元素时，若是是隐式转换或临时对象，则在容器的指定位置直接构造元素，而不是先单独生成，再将其复制（或移动）到容器中，所以此种情况只调用构造函数；但若传入一个非临时对象（即左值），调用拷贝或移动（有move时）。

（2）list：（内部原理是双向链表，非连续内存）（头文件：#include <list>）
begin(),end(),empty(),size(),front(),back(),push_front(),pop_front(),push_back(),pop_back(),emplace(),swap(),clear(),sort(),erase()

（3）string：（头文件：#include <string>）
str.size()
str.length()
str.push_back(char)：str后添加字符
str.compare(string)：str-string
str.find(string/char)：返回string/char的下标地址
str.erase(start , len)：删除str[start~start+len]
str.c_str()：返回const char * ，即C风格的字符串
str.substr(start , len)：返回str[start~start+len]
to_string(int/long/float/double)：将数值转换为字符串并返回
stoi(string)：stoi("0001234.555")返回1234；stoi(".1234")抛出异常。
//在C-style中还有一个和stoi()相同功能的函数是atoi(const char *)。
//string->c-num：string str; int num = atoi(str.c_str());

2.STL关联式容器：
（1）map: （内部原理是红黑树）（头文件：#include <map>）
begin(),end(),find(),empty(),size(),erase(),swap(),clear(),count()
iter->first,iter->second

（2）multimap：（排序的，允许有多个key值相等的map）（内部原理是红黑树）（头文件：#include <map>）

（3）set: （内部原理是红黑树）（头文件：#include <set>）
begin(),end(),find(),empty(),size(),erase(),emplace(),swap(),clear(),count()

（4）multiset：（排序的，允许有多个value值相等的set）（内部原理是红黑树）（头文件：#include <set>）

3.STL无序关联容器：
unordered_map：（无序）（内部原理是hash）（头文件：#include <unordered_map>）
unordered_multimap：（无序，允许多个key值相等的map）（内部原理是hash）（头文件：#include <unordered_map>）

unordered_set：（无序）（内部原理是hash）（头文件：#include <unordered_set>）
unordered_multiset：（无序，允许有多个value值相等的set）（内部原理是hash）（头文件：#include <unordered_set>）

4.STL容器适配器：
stack：（头文件：#include <stack>）
top(),push(),pop(),empty(),size()

queue：（头文件：#include <queue>）
push(),pop(),size(),empty(),front(),back();
*/