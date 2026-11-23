/*
1.智能指针头文件：#include<memory>

2.auto_ptr<T> aptr(new T (para));
（1）不支持共享，即唯一占有，copy操作（copy构造 或 copy assignment）都会导致被复制对象释放对资源的控制权。
auto_ptr<T> aptr1(new T (para));
auto_ptr<T> aptr2 = aptr1; 或 auto_ptr<T> aptr2(aptr1);都使得aptr1为空。
cout << * aptr1; //错误
cout << * aptr2; //正确
但实际上，对auto_ptr对象使用move()语法来转移所有权更为妥当：aptr2(move(aptr1)); 或 aptr2 = move(aptr1);
（2）参数不能为new T []，因为其析构函数内部是delete aptr;而非delete [] aptr;
（3）辅助函数：
release()和reset()：aptr2.reset(aptr1.release()); //aptr1释放并返回原始指针，reset()实现aptr1所指资源的重置。
get()：aptr1.get(); //返回aptr1所管理的原始指针，但注意，aptr1只是返回指针，并不是释放所有权。

3.unique_ptr<T> uptr(new T (para));
（1）不支持共享，唯一占有：
unique_ptr<T> uptr1(new T (para));
unique_ptr<T> uptr2(move(uptr1)); 或 unique_ptr<T> uptr2 = move(uptr1); //转出控制权给uptr2
（2）参数不能为new T []。
（3）辅助函数：
release()和reset()：uptr2.reset(uptr1.release());
get()：uptr2.get();
swap()：uptr2.swap(uptr1); //交换被管资源

4.shared_ptr<T> sptr(new T (para));
（1）支持共享，不使用move()语法，不含release()成员函数：
shared_ptr<T> sptr1(new T (para));
shared_ptr<T> sptr2(sptr1); //sptr1和sptr2管理同一资源。
（2）参数不能为new T []。
（3）辅助函数：
swap()
get()
reset()

《Effective C++》条款13
*/

/*
注意：总试图用智能指针取代ptr，其中ptr = new T (para); 或 ptr = (T *)malloc(sizeof(T));。
*/