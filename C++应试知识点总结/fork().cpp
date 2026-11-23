#include <iostream>
#include <unistd.h> //引入fork() 
using namespace std;

int main(int argc , char *argv[]) {
	int count = 0;
	cout << "MMMM" << endl;
	pid_t pid = fork();
	cout << "GGGG" << endl;
	//pid_t是一个宏定义；fork()若成功调用，一次则返回两个值，子进程返回0，父进程返回子进程ID；否则，出错返回-1
	if (pid < 0) cout << "error";
	else if(pid == 0) {
		cout << "子进程" << ends << getpid() << ends << count; //getpid()=该进程ID 
		return 0;
	} else {
		cout << "父进程" << ends << getpid() << ends << ++count; 
		return 0;
	}
	return 0;
}
/*
打印结果：
MMMM
GGGG
父进程 3 1
GGGG
子进程 4 0

fork()用于创建子进程，一个进程执行到fork()，分裂为：原进程充当父进程，新建子进程。 
两个进程将维护各自的进程空间，返回两个值。
上述代码，若fork()执行成功，则子进程和父进程语句都将被执行。 
fork()函数运行在linux上，若该代码在windows上运行会报错。 

fork与资源问题：https://blog.csdn.net/weixin_49690127/article/details/110482172
*/
