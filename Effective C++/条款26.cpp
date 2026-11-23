/*
条款26：尽可能延后变量定义式的出现时间
*/

/*
代码1：

void encrypt(string & s); //加密函数 
string encryptPassword(const string & password) { //验证加密 
	string encrypted;
	if(password.length() < MinimumPasswordLength)
		throw logic_error("Password is too short");
	encrypted = password;
	encrypt(encrypted);
	return encrypted;
}

这个代码有两个问题：
1.encrypted可能未被使用，但不论怎样都有构造开销。
2.encrypted调用的是default构造函数，然后使用copy assignment完成赋值——效率低于直接初始化。因为前者有构造函数+copyassignment+析构函数的开销，后者仅是构造函数+析构函数的开销。 
*/

/*
代码2：

void encrypt(string & s); //加密函数 
string encryptPassword(const string & password) { //验证加密 	
	if(password.length() < MinimumPasswordLength) //if安排在encrypted定义前，确保能用且用时再定义，解决问题1。 
		throw logic_error("Password is too short");
	string encrypted(password); //尽量不用定义+赋值的安排，而是用初始化，解决了问题2。 
	encrypt(encrypted);
	return encrypted;
}
*/

/*
代码3：

//code 1
Widget w;
for(int i = 0 ; i < n ; ++ i){
	w = depends on i;
} 

//code 2 
for(int i = 0 ; i < n ; ++ i){
	Widget w(depends on i);
}
//上述两则代码讨论了循环场景下的定义问题。
//code 1的开销：1次构造+n次copy assignment+1次析构。
//code 2的开销：n次构造+n次析构。
//code 1选用场景：效率敏感。w暴露在外部的作用域下不会造成危害，或外部需要w。 
//code 2选用场景：不用1就用2。  
*/
