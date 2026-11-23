/*
条款03：尽可能使用const
（1）
char p [] = "love";
const char * ptr = "love"; //non-const ptr , const data
char * const ptr = p; //const ptr , non-const data
const char * const ptr = "love"; //const ptr , const data

（2）STL相关的const迭代器
const vector<int>::iterator iter = v.begin(); //const iter , non-const data
* iter = 5; //正确
++ iter = 5; //错误

vector<int>::const_iterator iter = v.begin(); //non-const iter , const data
* iter = 5; //错误
++ iter = 5; //正确
*/

/* 
代码1： 

#include <iostream>
#include <string>
using namespace std;
class TextBlock {
	public :
		TextBlock(string text):text(text){}
		const char & operator [] (int pos) const;
		char & operator [] (int pos);
	private :
		string text;
};
const char & TextBlock::operator [] (int pos) const {	
	//前一个const表示返回值是常量，后一个const表示函数是常成员函数。
	//常成员函数不能调用普通成员函数，不能修改成员变量。常对象只能
	//调用常成员函数。  
	return text[pos];
}
char & TextBlock::operator [] (int pos) {
	return text[pos];
}
int main() {
	const TextBlock ctb("peace");
	TextBlock tb("love");
	
	cout << ctb[0] << endl;
	cout << tb[0] << endl;
	
	//ctb[0] = 'p'; //错误，ctb是const对象，只能调用const重载函数，const函数的
	//意思是返回值是const，因此ctb[0]为const，不可变。 
	tb[0] = 'p';
	return 0;
}
*/

/*
代码2： 

#include <iostream>
#include <string>
using namespace std;
class TextBlock {
	public :
		TextBlock(char * text):text(text){}
		char & operator [] (int pos) const;
	private :
		char * text; //注意
};
char & TextBlock::operator [] (int pos) const{
	return text[pos];
}
int main(){
	char ch[] = "love";
	TextBlock tb(ch);
	tb[0] = 'x'; 
	//不妥之处就在这里，const函数本来要保证成员变量不被修改，但这里却失效了。
	//因此常成员函数常需要把返回值也置为const。 
	return 0;
} 
*/

/*
代码3：

#include <iostream>
#include <string>
using namespace std;
class TextBlock {
	public :
		TextBlock(int text):text(text){}
		void change() const;
	private :
		mutable int text;
		//代码2指出的问题是，如果确实需要在const函数中修改成员变量该怎么办？
		//如这里所写，声明变量为mutable，则变量总可以被修改，const成员函数不
		//能对其修改的规则失效。 
};
void TextBlock::change() const{
	text = 2;
}
int main(){
	TextBlock tb(1);
	tb.change();
	return 0;
}
*/

/*
代码4：

#include <iostream>
#include <string>
using namespace std;
class TextBlock {
	public :
		TextBlock(char * text):text(text){}
		const char & operator [] (int pos) const;
		char & operator [] (int pos);
	private :
		char * text;
};
const char & TextBlock::operator [] (int pos) const{
	//do more things
	return text[pos];
}
char & TextBlock::operator [] (int pos){
	return const_cast<char &>(static_cast<const TextBlock &>(* this) [pos]);
	//分析：
	//1.两个对[]的重载形式，区别仅在于const。两类重载内部必然有大量的相同代码。
	//即可能const重载函数仅是为应对const对象而设计的，内部实现和non-const函数
	//没有任何不同，因此为避免重复代码，有两种方式：一是设计一个private下的共享
	//函数供const和non-const调用；二是让non-const调用const，此种方式适用于二者内部
	//实现基本相同的情形。
	//2.this指向当前对象，因当前对象必为non-const，故要先转换为const对象才能使用
	//const重载函数。*this为当前对象。使用const TextBlock &而非const TextBlock是
	//为了直接使用该对象而不是创建临时对象。之后的[pos]属于调用const重载以完成共
	//享代码段的任务。因const重载返回一个const char &值，故使用const_cast将其转换为 
	//char &并return。 
}
int main(){
	char t [] = "love";
	TextBlock tb(t);
	cout << tb[0];
	return 0;
} 
*/

