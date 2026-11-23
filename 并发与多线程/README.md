#### 1.多线程_01
* 进程和线程的一些基本概念、同步和互斥的一些知识。
* 基于`Windows API`的线程函数，不具备可移植性，即如果换做`Linux`平台，则还需要换为`Linux`提供的`API`。

#### 2.多线程_02
* `C++11`中定义的线程函数，可跨平台。
* `#include <thread>`下的线程操作：线程对象定义（函数为参数、对象为参数）、`join()、joinable()、detach()`。

#### 3.多线程_03（重要）
* 作为线程参数的函数，其要求的形参为引用时引出的讨论：什么时候才是调用detach的合适时期？

#### 4.多线程_04
* `std::this_thread::get_id();`返回线程`id`。
* `std::ref(obj in main thread);`将阻止`obj`的`copy`构造函数自动调用，而使引用类型的形参发挥作用，真正绑定到`obj`而非其`copy`对象上。
* 作为线程参数的函数，其要求的形参为智能指针。
* 类的成员函数作为线程函数。
* 类成员函数`operator()(para)`作为线程函数。

#### 5.多线程_05
* 多线程交由容器托管；多线程情形下线程的执行顺序问题。
* `#include<mutex>`下的`mutex`定义互斥锁、`lock()`和`unlock()`。
* `#include<mutex>`下的`lock_guard<mutex> mut(mutexObj);`。

#### 6.多线程_06
* 死锁解决方案1：将线程对mutex的请求顺序统一。
* 死锁解决方案2：使用`std::lock(mutex_1 , mutex_2);`是实现：要么两个互斥量都锁住，要么两个互斥量都没锁住。 
* 死锁解决方案2：使用`std::lock(mutex_1 , mutex_2);`和`std::lock_guard<std::mutex> guard_1(mutex_1 , std::adopt_lock);`及`std::lock_guard<std::mutex> guard_2(mutex_2 , std::adopt_lock);`实现：要么两个互斥量都锁住，要么两个互斥量都没锁住。其中：`adopt_lock`是一个结构体对象，起一个标记作用，作用是表示`mutex`参数已经`lock()`过了，无须在`lock_guard`里对`mutex`再次`lock()`了。

#### 7.多线程_07
* `std::unique_lock<mutex> ul(mutexObj , para_2);`具有更强的功能。
* `para_2`为`adopt_lock、try_to_lock、defer_lock`时具有的不同功能。
* `try_lock()`和`release()`成员函数。
* `unique_lock`是所有权问题。

#### 8.多线程_08
* 一个典型的单例模式类。
* 单例模式多线程设计：核心是双重判定式。
* `call_once`写法对双重判定式的取代。

#### 9.多线程_09
* `condition_variable convar;`
* `convar.wait(mutex , lambda);`
* `convar.notify_one();`
* `convar.notify_all();`

#### 10.多线程_10
* 异步操作：`future<T> result = async(para);`
* 给`async`传递类成员函数。
* 给`async`传递`launch::deferred`
* 给`async`传递`launch::async`

#### 11.多线程_11
* 代码过程：
```javascript
std::packaged_task<returnT(paraT)> pt(funName);
or
std::packaged_task<returnT(paraT)> pt(lambda);

thread thr(ref(pt) , para);
thr.join();

future<returnT> res = pt.get_future();
res.get();
```

#### 12.多线程_12
* `promise<T> pro;`类模板。

#### 13.多线程_13
* `future_status`
* `shared_future<returnT> res = pt.get_future();`或`shared_future<returnT> res(pt.get_future());`以及`shared_future<returnT> res(resOfFuture.get());`或`shared_future<returnT> res(move(resOfFuture));`或`shared_future<returnT> res(resOfFuture.share());`

#### 14.多线程_14
* 同写、有读有写这两种操作，如若不用某些机制限制，结果一定出错。
* 使用加锁方式，速度会慢一点，但不出错。
* 原子操作：`atomic<T> var = val;`

#### 15.多线程_15
* `recursive_mutex`：递归的独占互斥量。
* `timed_mutex`带超时功能的独占互斥量。`timed_mutex::try_lock_for(s);`和`timed_mutex::try_lock_until(s);`

#### 16.多线程_16
* 线程池