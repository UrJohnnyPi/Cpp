#### 1.什么是socket
* `socket`的原意是“插座”，在计算机通信领域，`socket`被翻译为“套接字”，它是计算机之间进行通信的一种约定或一种方式。通过`socket`这种约定，一台计算机可以接收其他计算机的数据，也可以向其他计算机发送数据。

#### 2.socket有哪些类型
| 属性 | 流格式套接字`（SOCK_STREAM）` | 数据报格式套接字`（SOCK_DGRAM）` |
| :-: | :-: | :-: |
| 可靠性 | `SOCK_STREAM` 是一种可靠的、双向的通信数据流，数据可以准确无误地到达另一台计算机，如果损坏或丢失，可以重新发送。 | 计算机只管传输数据，不作数据校验，如果数据在传输中损坏，或者没有到达另一台计算机，是没有办法补救的。也就是说，数据错了就错了，无法重传。 |
| 传输效率 | 较低 | 因为数据报套接字所做的校验工作少，所以在传输效率方面比流格式套接字要高。 |
| 特点 | （1）数据是按照顺序传输的；（2）不存在数据边界。流格式套接字的内部有一个缓冲区（也就是字符数组），通过 `socket` 传输的数据将保存到这个缓冲区。接收端在收到数据后并不一定立即读取，只要数据不超过缓冲区的容量，接收端有可能在缓冲区被填满以后一次性地读取，也可能分成好几次读取。 | （1）强调快速传输而非传输顺序；（2）传输的数据可能丢失也可能损毁；（3）限制每次传输的数据大小；（4）数据的发送和接收是同步的（即“存在数据边界”）。 |
| 协议 | `TCP` | `UDP` |

#### 3.OSI网络模型
![OSI](http://c.biancheng.net/uploads/allimg/190124/1-1Z1241445324H.jpg "OSI")
* 我们所说的`socket`编程，是站在传输层的基础上，所以可以使用`TCP/UDP`协议，但是不能干「访问网页」这样的事情，因为访问网页所需要的`http`协议位于应用层。

#### 4.TCP/IP协议族
* 目前实际使用的网络模型是`TCP/IP`模型，它对`OSI`模型进行了简化，只包含了四层，从上到下分别是应用层、传输层、网络层和链路层（网络接口层），每一层都包含了若干协议。
* `TCP/IP`模型包含了`TCP、IP、UDP、Telnet、FTP、SMTP`等上百个互为关联的协议，其中`TCP`和`IP`是最常用的两种底层协议，所以把它们统称为“TCP/IP 协议族”。

#### 5.IP、MAC和端口号——网络通信中确认身份信息的三要素

#### 6.Windows下的socket演示程序
```javascript
服务器端代码 server.cpp：

#include <stdio.h>
#include <winsock2.h>
#pragma comment (lib, "ws2_32.lib")  //加载 ws2_32.dll

int main(){
    //初始化 DLL
    WSADATA wsaData;
    WSAStartup( MAKEWORD(2, 2), &wsaData);

    //创建套接字
    SOCKET servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP); //IPV4,SOCK_STREAM,TCP

    //将套接字和IP、端口绑定
    sockaddr_in sockAddr; //sockaddr_in是一个struct，内含的成员如代码所示；其作用是存储IP和port。
    memset(&sockAddr, 0, sizeof(sockAddr));  //每个字节都用0填充
    sockAddr.sin_family = PF_INET;  //使用IPv4地址
    sockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");  //具体的IP地址
    sockAddr.sin_port = htons(1234);  //端口
    bind(servSock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));

    //进入监听状态
    listen(servSock, 20);

    //接收客户端请求
    SOCKADDR clntAddr;
    int nSize = sizeof(SOCKADDR);
    SOCKET clntSock = accept(servSock, (SOCKADDR*)&clntAddr, &nSize);

    //向客户端发送数据
    char *str = "Hello World!";
    send(clntSock, str, strlen(str)+sizeof(char), NULL);

    //关闭套接字
    closesocket(clntSock);
    closesocket(servSock);

    //终止 DLL 的使用
    WSACleanup();

    return 0;
}

客户端代码 client.cpp：

#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")  //加载 ws2_32.dll

int main(){
    //初始化DLL
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    //创建套接字
    SOCKET sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    //向服务器发起请求
    sockaddr_in sockAddr;
    memset(&sockAddr, 0, sizeof(sockAddr));  //每个字节都用0填充
    sockAddr.sin_family = PF_INET;
    sockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    sockAddr.sin_port = htons(1234);
    connect(sock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));

    //接收服务器传回的数据
    char szBuffer[MAXBYTE] = {0};
    recv(sock, szBuffer, MAXBYTE, NULL);

    //输出接收到的数据
    printf("Message form server: %s\n", szBuffer);

    //关闭套接字
    closesocket(sock);

    //终止使用 DLL
    WSACleanup();

    system("pause");
    return 0;
}
```

#### 7.WSAStartup()
* 原型声明如：`int WSAStartup(WORD wVersionRequired , LPWSADATA lpWSAData);`
* 第一个参数`wVersionRequired`要求一个socket版本号，如这里的`MAKEWORD(2, 2)`将传入一个`2.2`版的`socket`。
* 第二个参数`lpWSAData`要求一个指向`WSADATA`数据结构的指针。`WSADATA`数据结构中包含`Windows Sockets`的实现细节。
* 本函数必须是应用程序或`DLL`调用的第一个`Windows Sockets`函数。它允许应用程序或`DLL`指明`Windows Sockets API`的版本号及获得特定版本`Windows Sockets`实现的细节。应用程序或`DLL`只能在一次成功的`WSAStartup()`调用之后才能调用进一步的`Windows Sockets API`函数。
* 使用`WSACleanup();`终止使用`DLL`。

#### 8.socket()
* 原型声明如：`SOCKET socket(int af, int type, int protocol);`
* `af`为地址族`（Address Family）`，也就是`IP`地址类型，常用的有`AF_INET`和`AF_INET6`，分别表示`IPV4`和`IPV6`。
* `type`为数据传输方式/套接字类型，常用的有`SOCK_STREAM`（流格式套接字/面向连接的套接字）和`SOCK_DGRAM`（数据报套接字/无连接的套接字）。
* `protocol`表示传输协议，常用的有`IPPROTO_TCP`和`IPPTOTO_UDP`，分别表示`TCP`传输协议和`UDP`传输协议。
* 函数返回`SOCKET`类型的句柄。

#### 9.bind()和connect()
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
* `bind()`原型为：`int bind(SOCKET sock, const struct sockaddr *addr, int addrlen); //Windows`
* 上例中，有`bind(servSock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));`。已知`bind()`第二个参数为`SOCKADDR *`，那为什么不直接传一个`SOCKADDR *`，而是先定义`sockaddr_in`，然后再强制类型转化？先看`SOCKADDR`原型：
```javascript
struct sockaddr{
    sa_family_t  sin_family;   //2B；地址族（Address Family），也就是地址类型
    char         sa_data[14];  //14B；IP地址和端口号
};
//可见，强制类型转换完成的是将sockaddr_in的后14字节（sin_port成员、sin_addr成员、sin_zero[8]成员）转换为sockaddr的sa_data[14]成员的过程。
//因为sa_data[14]成员在被赋值时，需要同时获知IP和port，例如”127.0.0.1:80“。但没有相关函数将这个字符串转换成需要的形式（这个形式就是由sockaddr_in被转换三成员的类型决定），也就很难给sockaddr类型的变量赋值，所以使用sockaddr_in来代替。
```
* `connect()`原型为：`int connect(SOCKET sock, const struct sockaddr *serv_addr, int addrlen); //Windows`

#### 10.listen()和accept()
* `listen()`函数可以让套接字进入被动监听状态。所谓被动监听，是指当没有客户端请求时，套接字处于“睡眠”状态，只有当接收到客户端请求时，套接字才会被“唤醒”来响应请求。
* `listen()`原型：`int listen(SOCKET sock, int backlog); //Windows`
* `sock`为需要进入监听状态的套接字，`backlog`为请求队列的最大长度。
* 当套接字正在处理客户端请求时，如果有新的请求进来，套接字是没法处理的，只能把它放进缓冲区，待当前请求处理完毕后，再从缓冲区中读取出来处理。如果不断有新的请求进来，它们就按照先后顺序在缓冲区中排队，直到缓冲区满。这个缓冲区，就称为请求队列`（Request Queue）`。缓冲区的长度（能存放多少个客户端请求）可以通过`listen()`函数的`backlog`参数指定。并发量小的话可以设定为`10`或者`20`。如果将`backlog`的值设置为`SOMAXCONN`，就由系统来决定请求队列长度，这个值一般比较大，可能是几百，或者更多。当请求队列满时，就不再接收新的请求，对于`Windows`，客户端会收到`WSAECONNREFUSED`错误。
* 注意：`listen()`只是让套接字处于监听状态，并没有接收请求。接收请求需要使用`accept()`函数。
* `accept()`原型为：`SOCKET accept(SOCKET sock, struct sockaddr *addr, int *addrlen); //Windows`
* `accept()`返回一个新的套接字来和客户端通信，上例中有：`SOCKET clntSock = accept(servSock, (SOCKADDR*)&clntAddr, &nSize);`，和客户端通信的是`clntSock`，而不是`servSock`。

#### 11.send()和recv()
* `send()`原型：`int send(SOCKET sock, const char *buf, int len, int flags);`。
* `sock`为要发送数据的套接字，`buf`为要发送的数据的缓冲区地址，`len`为要发送的数据的字节数，`flags`为发送数据时的选项。
* `recv()`原型：`int recv(SOCKET sock, char *buf, int len, int flags);`。

#### 12.socket缓冲区以及阻塞模式
* 每个`socket`被创建后，都会分配两个缓冲区，输入缓冲区和输出缓冲区。
* `send()`并不立即向网络中传输数据，而是先将数据写入缓冲区中，再由`TCP`协议将数据从缓冲区发送到目标机器。一旦将数据写入到缓冲区，函数就可以成功返回，不管它们有没有到达目标机器，也不管它们何时被发送到网络，这些都是`TCP`协议负责的事情。
* `TCP`协议独立于`send()`函数，数据有可能刚被写入缓冲区就发送到网络，也可能在缓冲区中不断积压，多次写入的数据被一次性发送到网络，这取决于当时的网络情况、当前线程是否空闲等诸多因素，不由程序员控制。
* `recv()`函数也是如此，也从输入缓冲区中读取数据，而不是直接从网络中读取。
* 对于`TCP`套接字（默认情况下）:

| 当使用`send()`发送数据时 | 当使用`recv()`读取数据时 |
| :-: | :-: |
| （1）首先会检查缓冲区，如果缓冲区的可用空间长度小于要发送的数据，那么`send()`会被阻塞（暂停执行），直到缓冲区中的数据被发送到目标机器，腾出足够的空间，才唤醒`send()`函数继续写入数据。（2）如果`TCP`协议正在向网络发送数据，那么输出缓冲区会被锁定，不允许写入，`send()`也会被阻塞，直到数据发送完毕缓冲区解锁，`send()`才会被唤醒。（3）如果要写入的数据大于缓冲区的最大长度，那么将分批写入。（4）直到所有数据被写入缓冲区`send()`才能返回。 | （1）首先会检查缓冲区，如果缓冲区中有数据，那么就读取，否则函数会被阻塞，直到网络上有数据到来。（2）如果要读取的数据长度小于缓冲区中的数据长度，那么就不能一次性将缓冲区中的所有数据读出，每次读一部分使得剩余数据不断积压，直到`recv()`函数再次读取。（3）直到读取到数据后`recv()`函数才会返回，否则就一直被阻塞。 |

#### 13.TCP协议的粘包问题（数据的无边界性）
* 数据的接收和发送是无关的。
* 例如，`send()`重复执行三次，每次都发送字符串`"abc"`，那么目标机器上的`recv()`可能分三次接收，每次都接收`"abc"`；也可能分两次接收，第一次接收`"abcab"`，第二次接收`"cabc"`；也可能一次就接收到字符串`"abcabcabc"`。
* 假设我们希望客户端每次发送一位学生的学号，让服务器端返回该学生的姓名、住址、成绩等信息，这时候可能就会出现问题，服务器端不能区分学生的学号。例如第一次发送`1`，第二次发送`3`，服务器可能当成`13`来处理，返回的信息显然是错误的。
* 这就是数据的“粘包”问题，客户端发送的多个数据包被当做一个数据包接收。也称数据的无边界性。`recv()`函数不知道数据包的开始或结束标志（实际上也没有任何开始或结束标志），只把它们当做连续的数据流来处理。
```javascript
服务器端代码 server.cpp：

#include <stdio.h>
#include <windows.h>
#pragma comment (lib, "ws2_32.lib")  //加载 ws2_32.dll

#define BUF_SIZE 100

int main(){
    WSADATA wsaData;
    WSAStartup( MAKEWORD(2, 2), &wsaData);

    //创建套接字
    SOCKET servSock = socket(AF_INET, SOCK_STREAM, 0);

    //绑定套接字
    sockaddr_in sockAddr;
    memset(&sockAddr, 0, sizeof(sockAddr));  //每个字节都用0填充
    sockAddr.sin_family = PF_INET;  //使用IPv4地址
    sockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");  //具体的IP地址
    sockAddr.sin_port = htons(1234);  //端口
    bind(servSock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));

    //进入监听状态
    listen(servSock, 20);

    //接收客户端请求
    SOCKADDR clntAddr;
    int nSize = sizeof(SOCKADDR);
    char buffer[BUF_SIZE] = {0};  //缓冲区
    SOCKET clntSock = accept(servSock, (SOCKADDR*)&clntAddr, &nSize);

    Sleep(10000);  //注意这里，让程序暂停10秒

    //接收客户端发来的数据，并原样返回
    int recvLen = recv(clntSock, buffer, BUF_SIZE, 0);
    send(clntSock, buffer, recvLen, 0);

    //关闭套接字并终止DLL的使用
    closesocket(clntSock);
    closesocket(servSock);
    WSACleanup();

    return 0;
}

客户端代码 client.cpp：

#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>
#include <windows.h>
#pragma comment(lib, "ws2_32.lib")  //加载 ws2_32.dll

#define BUF_SIZE 100

int main(){
    //初始化DLL
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    //向服务器发起请求
    sockaddr_in sockAddr;
    memset(&sockAddr, 0, sizeof(sockAddr));  //每个字节都用0填充
    sockAddr.sin_family = PF_INET;
    sockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    sockAddr.sin_port = htons(1234);

    //创建套接字
    SOCKET sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    connect(sock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));

    //获取用户输入的字符串并发送给服务器
    char bufSend[BUF_SIZE] = {0};
    printf("Input a string: ");
    gets(bufSend);
    for(int i=0; i<3; i++){
        send(sock, bufSend, strlen(bufSend), 0);
    }
    //接收服务器传回的数据
    char bufRecv[BUF_SIZE] = {0};
    recv(sock, bufRecv, BUF_SIZE, 0);
    //输出接收到的数据
    printf("Message form server: %s\n", bufRecv);

    closesocket(sock);  //关闭套接字
    WSACleanup();  //终止使用 DLL

    system("pause");
    return 0;
}

//先运行 server，再运行 client，并在10秒内输入字符串"abc"，再等数秒，服务器就会返回数据。运行结果如下：
//Input a string: abc
//Message form server: abcabcabc
//即客户端企图传出3个分离的abc，也企图获得来自服务器端三个分离的abc作为回复，但事实上只能收到abcabcabc。
//TCP协议的粘包问题完全是因为TCP是面向字节流的，即：虽然程序和TCP的交互是一次一个数据块（大小不等），但TCP把应用程序交下来的数据仅仅看成是一连串的无结构字节流。接收方的应用程序必须有能力识别收到的字节流，把它还原成有意义的应用层数据。（详见《计算机网络第七版》P211）
```

#### 14.基于UDP的服务器端和客户端（欲读此处，需先回顾《计算机网络第七版》运输层章节）
* `UDP`是无连接的，因此，不论是服务器端还是客户端都没有`connect()、listen()`和`accept()`。
* `TCP`中，套接字是一对一的关系。如要向`10`个客户端提供服务，那么除了负责监听的套接字外，还需要创建`10`套接字。但在`UDP`中，不管是服务器端还是客户端都只需要`1`个套接字。
* `UDP`是无连接的，因此，每次传输数据都要添加目标地址信息。使用`sendto()`：

`int sendto(SOCKET sock, const char *buf, int nbytes, int flags, const struct sockadr *to, int addrlen);  //Windows`

| 参数 | 解释 |
| :-: | :-: |
| `sock` | 用于传输`UDP`数据的套接字 |
| `buf` | 保存待传输数据的缓冲区地址 |
| `nbytes` | 带传输数据的长度（以字节计） |
| `flags` | 可选项参数，若没有可传递`0` |
| `to` | 存有目标地址信息的`sockaddr`结构体变量的地址 |
| `addrlen` | 传递给参数`to`的地址值结构体变量的长度 |
* 由于`UDP`数据的发送端不定，所以采用接收函数`recvfrom()`，原型：

`int recvfrom(SOCKET sock, char *buf, int nbytes, int flags, const struct sockaddr *from, int *addrlen);  //Windows`

| 参数 | 解释 |
| :-: | :-: |
| `sock` | 用于接收`UDP`数据的套接字 |
| `buf` | 保存接收数据的缓冲区地址 |
| `nbytes` | 可接收的最大字节数（不能超过`buf`缓冲区的大小） |
| `flags` | 可选项参数，若没有可传递`0` |
| `from` | 存有发送端地址信息的`sockaddr`结构体变量的地址 |
| `addrlen` | 保存参数`from`的结构体变量长度的变量地址值 |
*
```javascript
服务器端 server.cpp：

#include <stdio.h>
#include <winsock2.h>
#pragma comment (lib, "ws2_32.lib")  //加载 ws2_32.dll

#define BUF_SIZE 100

int main(){
    WSADATA wsaData;
    WSAStartup( MAKEWORD(2, 2), &wsaData);

    //创建套接字
    SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);

    //绑定套接字
    sockaddr_in servAddr;
    memset(&servAddr, 0, sizeof(servAddr));  //每个字节都用0填充
    servAddr.sin_family = PF_INET;  //使用IPv4地址
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY); //使用htonl(INADDR_ANY)自动获取IP地址
    servAddr.sin_port = htons(1234);  //端口
    bind(sock, (SOCKADDR*)&servAddr, sizeof(SOCKADDR));

    //接收客户端请求
    SOCKADDR clntAddr;  //客户端地址信息
    int nSize = sizeof(SOCKADDR);
    char buffer[BUF_SIZE];  //缓冲区
    while(1){
        int strLen = recvfrom(sock, buffer, BUF_SIZE, 0, &clntAddr, &nSize);
        sendto(sock, buffer, strLen, 0, &clntAddr, nSize);
    }

    closesocket(sock);
    WSACleanup();
    return 0;
}

客户端 client.cpp：

#include <stdio.h>
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")  //加载 ws2_32.dll

#define BUF_SIZE 100

int main(){
    //初始化DLL
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    //创建套接字
    SOCKET sock = socket(PF_INET, SOCK_DGRAM, 0);

    //服务器地址信息
    sockaddr_in servAddr;
    memset(&servAddr, 0, sizeof(servAddr));  //每个字节都用0填充
    servAddr.sin_family = PF_INET;
    servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servAddr.sin_port = htons(1234);

    //不断获取用户输入并发送给服务器，然后接受服务器数据
    sockaddr fromAddr;
    int addrLen = sizeof(fromAddr);
    while(1){
        char buffer[BUF_SIZE] = {0};
        printf("Input a string: ");
        gets(buffer);
        sendto(sock, buffer, strlen(buffer), 0, (struct sockaddr*)&servAddr, sizeof(servAddr));
        int strLen = recvfrom(sock, buffer, BUF_SIZE, 0, &fromAddr, &addrLen);
        buffer[strLen] = 0;
        printf("Message form server: %s\n", buffer);
    }

    closesocket(sock);
    WSACleanup();
    return 0;
}

//先运行 server，再运行 client，client 输出结果为：
//Input a string: C语言中文网
//Message form server: C语言中文网
//Input a string: c.biancheng.net Founded in 2012
//Message form server: c.biancheng.net Founded in 2012
//Input a string:
```

#### 附录

#### 1.动态链接库DLL教程
*
| 属性 | 动态链接库基本概念 | 静态链接库基本概念 |
| :-: | :-: | :-: |
| `EXE`文件产生方式 | （1）在使用动态库的时候，往往提供两个文件：一个引入库`(.lib)`文件和一个`DLL (.dll)`文件。（2）虽然引入库的后缀名也是“lib”，但是，动态库的引入库文件和静态库文件有着本质上的区别，对一个`DLL`来说，其引入库文件`(.lib)`包含该`DLL`导出的函数和变量的符号名，而`.dll`文件包含该`DLL`实际的函数和数据。（3）在使用动态库的情况下，在编译链接可执行文件时，只需要链接该`DLL`的引入库文件`(.lib)`，该`DLL`中的函数代码和数据并不复制到可执行文件中。直到可执行程序运行时，才去加载所需的`DLL`，将该`DLL`映射到进程的地址空间中，然后访问`DLL`中导出的函数。（4）在发布产品时，除了发布可执行文件以外，同时还要发布该程序将要调用的动态链接库。 | （1）函数和数据被编译进一个二进制文件（通常扩展名为`.LIB`）。（2）在使用静态库的情况下， 在编译链接可执行文件时，链接器从库中复制这些函数和数据，并把它们和应用程序的其他模块组合起来创建最终的可执行文件（`.EXE`文件）。（3）当发布产品时，只需要发布这个可执行文件，并不需要发布被使用的静态库。 |
| `EXE`文件运行方式 | （1）动态链接库有两种加载方式：隐式加载和显示加载。（2）隐式加载又叫载入时加载，指在主程序载入内存时搜索`DLL`，并将`DLL`载入内存。隐式加载也会有静态链接库的问题，如果程序稍大，加载时间就会过长，用户不能接受。（3）显式加载又叫运行时加载，指主程序在运行过程中需要`DLL`中的函数时再加载。显式加载是将较大的程序分开加载的，程序运行时只需要将主程序载入内存，软件打开速度快，用户体验好。 | （1）静态链接库在链接时，编译器会将`.obj`文件和`.LIB`文件组织成一个`.exe`文件，程序运行时，将全部数据加载到内存。（2）如果程序体积较大，功能较为复杂，那么加载到内存中的时间就会比较长，最直接的一个例子就是双击打开一个软件，要很久才能看到界面。这是静态链接库的一个弊端。 |

* `DLL`是`Dynamic Link Library`的缩写，译为“动态链接库”。`DLL`也是一个被编译过的二进制程序，可以被其他程序调用，但与`exe`不同，`DLL`不能独立运行，必须由其他程序调用载入内存。

* `DLL`中封装了很多函数，只要知道函数的入口地址，就可以被其他程序调用。

*
| `Windows API`中所有的函数都包含在`DLL`中，其中有3个最重要的`DLL`： |
| :-: |
| `Kemel32.dll`：它包含那些用于管理内存、进程和线程的函数，例如`CreateThread`函数； |
| `User32.dll`：它包含那些用于执行用户界面任务（如窗口的创建和消息的传送）的函数，例如`CreateWindow`函数； |
| `GDI32.dll`：它包含那些用于画图和显示文本的函数。 |

*
```javascript
DLL 程序的入口函数是 DllMain()，就像 DOS 程序的入口函数是 main()、Win32 程序的入口函数是 WinMain() 一样：

BOOL APIENTRY DllMain(
    HANDLE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
);

hModule 表示本DLL程序的句柄。
ul_reason_for_call 表示DLL当前所处的状态，例如DLL_PROCESS_ATTACH表示DLL刚刚被加载到一个进程中，DLL_PROCESS_DETACH表示DLL刚刚从一个进程中卸载。
lpReserved 表示一个保留参数，目前已经很少使用。
```

*
```javascript
dllDemo.dll代码：

#include <objbase.h>  // 也可以 #include <windows.h>
#include <stdio.h>

_declspec(dllexport) int add(int a, int b){ 
//应用程序如果想要访问某个DLL中的函数，那么该函数必须是已经被导出的函数。为了导出一些函数，需要在函数前面添加标识符 _declspec(dllexport)。
    return a+b;
}
_declspec(dllexport)int sub(int a, int b){
    return a-b;
}

BOOL APIENTRY DllMain(HANDLE hModule , DWORD ul_reason_for_call , LPVOID lpReserved){
//DllMain() 函数在DLL程序载入和卸载时执行，可以用来做一些初始化和清理的工作，如果仅仅是向外暴露函数，就可以省略 DllMain() 函数。但是如果有 DllMain() 函数，就一定要 #include <objbase.h> 或 #include <windows.h>。
    if(ul_reason_for_call == DLL_PROCESS_ATTACH){
        printf("Congratulations! DLL is loaded!");
    }
}
```

*
```javascript
//动态链接库隐式加载
//以下代码仅在dllDemo.dll、cDemo.h、cDemo.cpp在同一project下成立，即必须可搜寻
cDemo.h代码：

#ifndef _DLLDEMO_H
#define _DLLDEMO_H
#pragma comment(lib, "dllDemo.lib") //在源码中引入 .lib 文件。
_declspec(dllexport) int add(int, int); //用 _declspec(dllimport) 标识符声明函数来自动态链接库。
_declspec(dllexport) int sub(int, int);
#endif

cDemo.cpp代码：

#include<stdio.h>
#include "cDemo.h"

int main(){
    int a=10, b=5;
    printf("a+b=%d\n", add(a, b));
    printf("a-b=%d\n", sub(a, b));
    return 0;
}
```

*
```javascript
//动态链接库显式加载
//以下代码仅在dllDemo.dll、cDemo.cpp在同一project下成立，但不需要 dllDemo.lib
//（1）LoadLibrary() 函数
//LoadLibrary() 函数的原型声明如下所示：HMODULE  LoadLibrary(LPCTSTR 1pFileName);LoadLibrary() 函数既可以接收.dll模块为参数，也可以接收.exe模块为参数。
//LoadLibrary() 加载可执行模块时，主要是为了访问该模块内的一些资源，例如位图资源或图标资源等。
//调用成功， LoadLibrary() 函数将返回所加载的那个模块的句柄。
//该函数的返回类型是HMODULE。HMODULE类型和HINSTANCE类型可以通用。
//（2）GetProcAddress() 函数
//当获取到动态链接库模块的句柄后，接下来就要想办法获取该动态链接库中导出函数的地址，这可以通过调用 GetProcAddress() 函数来实现。
//原型声明如下所示：FARPROC  GetProcAddress(HMODULE hModule, LPCSTR 1pProcName);
//参数：hModule：指定动态链接库模块的句柄，即 LoadLibrary() 函数的返回值; 1pProcName：字符串指针，表示DLL中函数的名字。
cDemo.cpp代码：

#include<stdio.h>
#include<stdlib.h>
#include<windows.h> // 必须包含 windows.h

typedef int (*FUNADDR)(); // 指向函数的指针

int main(){
    int a=10, b=5;
   
    HINSTANCE dllDemo = LoadLibrary("dllDemo.dll");
    FUNADDR add, sub;
    if(dllDemo){
        add = (FUNADDR)GetProcAddress(dllDemo, "add");
        sub = (FUNADDR)GetProcAddress(dllDemo, "sub");
    }else{
        printf("Fail to load DLL!\n");
        system("pause");
        exit(1);
    }

    printf("a+b=%d\n", add(a, b));
    printf("a-b=%d\n", sub(a, b));

    system("pause");
    return 0;
}
```

* `HMODULE`类型、`HINSTANCE`类型在`windows.h`中定义；`LoadLibrary()`函数、`GetProcAddress()`函数是`Win32 API`，也在`windows.h`中定义。

*
[动态链接库DLL教程1](http://c.biancheng.net/cpp/html/2750.html)

[动态链接库DLL教程2](http://c.biancheng.net/cpp/html/2753.html)

[动态链接库DLL教程3](http://c.biancheng.net/cpp/html/2754.html)

#### 2.socket编程入门

* [socket编程入门](http://c.biancheng.net/socket/)