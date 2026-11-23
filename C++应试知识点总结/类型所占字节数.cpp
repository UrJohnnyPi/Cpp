#include <iostream>
#include <string>
using namespace std;
struct A{
	virtual void display() = 0;
};
struct B{
	bool c;
};
struct C{
	bool c;
	char d;
};
struct D{
	bool c;
	short d;
};
struct E{
	bool c;
	int d;
};
struct F{ //注意这种边界对齐方式
	virtual void display() = 0;
	bool c;
};
struct G{
};
int main() {
	cout << sizeof(char) << ends; //1B
	cout << sizeof(short) << ends; //2B
	cout << sizeof(int) << ends; //4B
	cout << sizeof(long) << ends; //4B
	cout << sizeof(long int) << ends; //4B
	cout << sizeof(long long) << ends; //8B
	cout << sizeof(float) << ends; //4B
	cout << sizeof(double) << ends; //8B
	cout << sizeof(int *) << ends; //8B
	cout << sizeof(char *) << ends; //8B
	cout << sizeof(double *) << ends; //8B
	cout << sizeof(string) << ends; //8B
	cout << sizeof(bool) << ends; //1B
	
	cout << sizeof(A) << ends; //8B，struct有一个虚表指针
	cout << sizeof(B) << ends; //1B
	cout << sizeof(C) << ends; //2B
	cout << sizeof(D) << ends; //4B，边界对齐
	cout << sizeof(E) << ends; //8B，边界对齐
	cout << sizeof(F) << ends; //16B
	
	cout << sizeof(G) << ends; //1B，这个字节大小是标准制定的，如果为0，那么若该空类实例化多个对象，这些对象因不占内存而无法区分。只有实例对象占有空间，不同对象在不同的地址空间中，才是可区分的。
	return 0;
}

//上例中的struct换做class也可以
//字节对齐数：1B、2B、4B、8B、16B（不存在10B、12B等）