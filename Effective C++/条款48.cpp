/*
条款48：认识template元编程（TMP）

条款47中的代码2就是TMP的实例。条款47中用代码2替换代码1的原因再具体一点就是：像那些C/Object-Oriented C++
的部分比如if()、while()等许多符号只有在运行期才会发生（当然也有许多是编译期可以发生的）。而TMP中的所有构
件在编译期就发生。这就使得TMP注定比C/Object-Oriented C++部分在时间上更加高效。
此外运行期的代码会被连接于可执行文件，传统C/C++部分造成的可执行文件的膨胀程度更高。 
*/

/*
//使用TMP实现阶乘 
template<unsigned n>
struct Factorial{
	enum{
		value = n * Factorial<n - 1>::value
	};
};

template<> 
struct Factorial<0>{
	enum{
		value = 1
	};
};

int main(){
	std::cout << Factorial<5>::value;
	std::cout << Factorial<10>::value;
	return 0;
}
//TMP没有循环构件，循环效果通过递归实现。 
*/
