#### 1.代码示例
```javascript
//server.cpp

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main(){
    //1.创建套接字，参数含义为：IPV4,SOCK_STREAM,TCP
    int serv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    //2.将套接字和IP、端口绑定
    struct sockaddr_in serv_addr; //sockaddr_in是一个struct类型，内含的成员如下述代码所示；其作用是存储IP和port；该语句是定义一个sockaddr_in对象。
    memset(&serv_addr, 0, sizeof(serv_addr));  //每个字节都用0填充
    serv_addr.sin_family = AF_INET;  //使用IPv4地址
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");  //具体的IP地址
    serv_addr.sin_port = htons(1234);  //端口
    bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    //3.进入监听状态，等待用户发起请求
    listen(serv_sock, 20);

    //4.接收客户端请求
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size = sizeof(clnt_addr);
    int clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size); //程序一旦执行到 accept() 就会被阻塞（暂停运行），直到客户端发起请求。

    //5.向客户端发送数据
    char str[] = "http://c.biancheng.net/socket/";
    write(clnt_sock, str, sizeof(str)); //写操作完成后，立即回传
       
    //6.关闭套接字
    close(clnt_sock);
    close(serv_sock);

    return 0;
}

//client.cpp

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main(){
    //1.创建套接字
    int sock = socket(AF_INET, SOCK_STREAM, 0);

    //2.向服务器（特定的IP和端口）发起请求
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));  //每个字节都用0填充
    serv_addr.sin_family = AF_INET;  //使用IPv4地址
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");  //具体的IP地址
    serv_addr.sin_port = htons(1234);  //端口
    connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
       
    //3.读取服务器传回的数据
    char buffer[40];
    read(sock, buffer, sizeof(buffer)-1); //读操作完成后，立即反馈。
       
    printf("Message form server: %s\n", buffer);
       
    //4.关闭套接字
    close(sock);

    return 0;
}
```

#### 2.socket()
* 头文件：`#include <sys/socket.h>`
* 原型声明如：`int socket(int af, int type, int protocol);`
* `af`为地址族`（Address Family）`，也就是`IP`地址类型，常用的有`AF_INET`和`AF_INET6`，分别表示`IPV4`和`IPV6`。
* `type`为数据传输方式/套接字类型，常用的有`SOCK_STREAM`（流格式套接字/面向连接的套接字）和`SOCK_DGRAM`（数据报套接字/无连接的套接字）。
* `protocol`表示传输协议，常用的有`IPPROTO_TCP`和`IPPTOTO_UDP`，分别表示`TCP`传输协议和`UDP`传输协议；将 protocol 的值设为 0，系统会自动推演出应该使用什么协议。

#### 3.bind()
* 原型声明为：`int bind(int sock, struct sockaddr *addr, socklen_t addrlen);`
* `sock` 为 `socket` 文件描述符，`addr` 为 `sockaddr` 结构体变量的指针，`addrlen` 为 `addr` 变量的大小，可由 `sizeof()` 计算得出。
* `server.cpp`中的`bind`：
```javascript
struct sockaddr_in serv_addr;
...
bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
```
* `sockaddr_in`结构体——专门用来保存`IPv4`地址的结构体。
```javascript
struct sockaddr_in{ //共16B
    sa_family_t     sin_family;   //2B；地址族（Address Family），也就是地址类型
    uint16_t        sin_port;     //2B；位的端口号。上例中，使用htons()转换端口号
    struct in_addr  sin_addr;     //4B；32位IP地址
    char            sin_zero[8];  //8B；不使用，一般用0填充。上例中，先用memset()将结构体的全部字节填充为0，再给前3个成员赋值，剩下的sin_zero自然就是0了。
};

struct in_addr{
    in_addr_t  s_addr;  //32位的IP地址
};

//in_addr_t 在头文件 <netinet/in.h> 中定义，等价于 unsigned long，长度为4个字节。也就是说，s_addr 是一个整数，而IP地址是一个字符串，所以需要 inet_addr() 函数进行转换，例如：
unsigned long ip = inet_addr("127.0.0.1");
printf("%ld\n", ip); //输出16777343
```
* `sockaddr_in6`用来保存`IPv6`地址。
```javascript
struct sockaddr_in6 { //共16B
    sa_family_t sin6_family;  //2B；地址类型，取值为AF_INET6
    in_port_t sin6_port;  //2B；16位端口号
    uint32_t sin6_flowinfo;  //4B；IPv6流信息
    struct in6_addr sin6_addr;  //4B；具体的IPv6地址
    uint32_t sin6_scope_id;  //4B；接口范围ID
};
```
* `bind()` 第二个参数的类型为 `sockaddr`，而代码中却使用 `sockaddr_in`，然后再强制转换为 `sockaddr`，这是为什么呢？先看`sockaddr`原型：
```javascript
struct sockaddr{
    sa_family_t  sin_family;   //2B；地址族（Address Family），也就是地址类型
    char         sa_data[14];  //14B；IP地址和端口号
};
//可见，强制类型转换完成的是将sockaddr_in的后14字节（sin_port成员、sin_addr成员、sin_zero[8]成员）转换为sockaddr的sa_data[14]成员的过程。
//因为sa_data[14]成员在被赋值时，需要同时获知IP和port，例如”127.0.0.1:80“。但没有相关函数将这个字符串转换成需要的形式（这个形式就是由sockaddr_in被转换三成员的类型决定），也就很难给sockaddr类型的变量赋值，所以使用sockaddr_in来代替。
```
#### 4.listen()
* `listen()`函数原型：`int listen(int sock, int backlog);`。
* `listen()`函数可以让套接字进入被动监听状态。所谓被动监听，是指当没有客户端请求时，套接字处于“睡眠”状态，只有当接收到客户端请求时，套接字才会被“唤醒”来响应请求。
* `sock`为需要进入监听状态的套接字，`backlog`为请求队列的最大长度。
* 当套接字正在处理客户端请求时，如果有新的请求进来，套接字是没法处理的，只能把它放进缓冲区，待当前请求处理完毕后，再从缓冲区中读取出来处理。如果不断有新的请求进来，它们就按照先后顺序在缓冲区中排队，直到缓冲区满。这个缓冲区，就称为请求队列`（Request Queue）`。缓冲区的长度（能存放多少个客户端请求）可以通过`listen()`函数的`backlog`参数指定。并发量小的话可以设定为`10`或者`20`。如果将`backlog`的值设置为`SOMAXCONN`，就由系统来决定请求队列长度，这个值一般比较大，可能是几百，或者更多。当请求队列满时，就不再接收新的请求，对于 `Linux`，客户端会收到 `ECONNREFUSED` 错误。
* 注意：`listen()`只是让套接字处于监听状态，并没有接收请求。接收请求需要使用`accept()`函数。

#### 5.accept()
* `accept()` 函数接收客户端请求。
* `accept()`原型为：`int accept(int sock, struct sockaddr *addr, socklen_t *addrlen);`。
* `sock` 为 `socket` 文件描述符，`addr` 为 `sockaddr` 结构体变量的指针，`addrlen` 为 `addr` 变量的大小，可由 `sizeof()` 计算得出。

#### 6.write()
* 两台计算机之间的通信相当于两个套接字之间的通信，在服务器端用 `write()` 向套接字写入数据，客户端就能收到，然后再使用 `read()` 从套接字中读取出来，就完成了一次通信。
* `write()`原型：`ssize_t write(int fd, const void *buf, size_t nbytes);`。
* `fd` 为要写入的文件的描述符，`buf` 为要写入的数据的缓冲区地址，`nbytes` 为要写入的数据的字节数。`size_t` 是通过 `typedef` 声明的 `unsigned int` 类型；`ssize_t` 在 `"size_t"` 前面加了一个`"s"`，代表 `signed`，即 `ssize_t` 是通过 `typedef` 声明的 `signed int` 类型。
* `write()` 函数会将缓冲区 `buf` 中的 `nbytes` 个字节写入文件 `fd`，成功则返回写入的字节数，失败则返回 `-1`。

#### 7.connect()
* `connect()`原型为：`int connect(int sock, struct sockaddr *serv_addr, socklen_t addrlen);`

#### 8.read()
* `read()`函数原型：`ssize_t read(int fd, void *buf, size_t nbytes);`。
* `read()` 函数会从 `fd` 文件中读取 `nbytes` 个字节并保存到缓冲区 `buf`，成功则返回读取到的字节数（但遇到文件结尾则返回`0`），失败则返回 `-1`。