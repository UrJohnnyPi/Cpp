/*
memset函数原型：
void * memset(void * dest , int ch , std::size_t count);
解释：
转换值ch为unsigned int，并复制它到dest所指向对象的前count个字节。
意即，你输入的ch将被取后1字节（8bits）为dest所指的前count个字节初始化。

要注意的点是：
（1）你ch可输入的是4B的整数，但实际上只有后1B有作用。意思是，不管你输入多少，最后起都能转化为0~255的unsigned int。
（2）不能用该函数将int数组初始化为0和-1之外的其他值。如下例：
int a[20];
std::memset(a, 2, sizeof a);
a的每一元素都是4B，但对于memset而言，类型是无意义的，它只按字节初始化。sizeof a = 80（因为4 ×20），所以这段代码
的含义是：以a所指的地址起始，为之后的80个字节赋值“0000 0010”。此时若有： 
for (int ai : a) std::cout << ai << std::endl;
就不能指望它输出20个2了。而是将每4个字节打包成一个int作为输出。输出20个33686018。
故，该注意点指出，只有该int值的4个字节均相等才能获得期望的输出，只有0和-1。 
*/ 
#include <iostream>
#include <cstring>
 
int main()
{
    int a[20];
    std::memset(a, 2, sizeof a);
    for (int ai : a) std::cout << ai << std::endl;
    //std::cout << sizeof a;
    return 0; 
} 
