/*
条款43：学习处理模板化基类内的名称 
*/

/*
代码1：

//旨在设计一个类，用以将信息传送到若干不同的公司。信息要么以未加工（cleartext）的形式传送，
//要么以加密（encrypted）的形式传送。
//并设计一个日志类，在发送前和发送后志记。

class CompanyA{
	public :
		void sendCleartext(const string & msg);
		void sendEncrypted(const string & msg);
}; 
class CompanyB{
	public :
		void sendCleartext(const string & msg);
		void sendEncrypted(const string & msg);
};
//其他公司
class CompanyZ{ //只能使用加密方式为CompanyZ发送信息 
	public :
		void sendEncrypted(const string & msg);
};
 
class MsgInfo{ //该类用于存储信息 
	//do something
};

template<typename Company> //发送类 
class MsgSender{
	public :
		void sendClear(const MsgInfo & info){
			string msg;
			//根据info为msg赋值
			Company c;
			c.sendCleartext(msg); 
		}
		void sendSecret(const MsgInfo & info){
			string msg;
			//根据info为msg赋值
			Company c;
			c.sendEncrypted(msg); 
		}
};
template<> //针对CompanyZ的MsgSender特化版本，因为CompanyZ只有一种传送方式，不能使用一般的MsgSender 
class MsgSender<CompanyZ>{
	public :
		void sendSecret(const MsgInfo & info){
			string msg;
			//根据info为msg赋值
			Company c;
			c.sendEncrypted(msg); 
		}
}; 

template<typename Company> //日志+发送类 
class LoggingMsgSender : public MsgSender<Company>{
	public :
		void sendClearMsg(const MsgInfo & info){
			//志记传送前的信息
			sendClear(info); //该语句无法通过编译 
			//志记传送后的信息
		}
		void sendSecretMsg(const MsgInfo & info){
			//如上 
		}
};
//日志+发送类中的sendClear语句错误的原因在于，编译器在LoggingMsgSender具现化之前，是无法
//确定其继承自哪个版本的MsgSender<Company>的，既可能是一般性的，也可能是特化的。
//所以如果编译器指定选用了特化版本，则这里报错就会是必然的。若选用一般性的，对
//CompanyZ又不妥当。所以编译器直接选择不在模板基类中寻找继承而来的名称。 
//编译器可以确定的是MsgSender<Company>是一个模板类，是可以继承的，所以它不会在继承时报错。但是，
//编译器拒绝在模板基类中寻找继承而来的名称。
//这里写了一个特化版本：MsgSender<CompanyZ>，即提供了两个模板基类来例举。但不论写不写这个特化版本，
//即使代码中只有一个一般性的模板类，编译器也拒绝做出选择，也因此对模板类成员的直接调用将导致错误。
*/

/*
代码2：（旨在给出修正的LoggingMsgSender，以通过编译时的解析阶段） 

//（1）通过using 
template<typename Company>
class LoggingMsgSender : public MsgSender<Company>{
	public :
		using MsgSender<Company>::sendClear; //告诉编译器，让它假设sendClear在模板基类中存在，以期通过编译时的解析 
		void sendClearMsg(const MsgInfo & info){
			//志记传送前的信息
			sendClear(info); //该语句可以通过编译 
			//志记传送后的信息
		}
		void sendSecretMsg(const MsgInfo & info){
			//如上 
		}
};

//（2）通过this 
template<typename Company>
class LoggingMsgSender : public MsgSender<Company>{
	public :
		void sendClearMsg(const MsgInfo & info){
			//志记传送前的信息
			this->sendClear(info); //告诉编译器，让它假设sendClear是继承而来的函数  
			//志记传送后的信息
		}
		void sendSecretMsg(const MsgInfo & info){
			//如上 
		}
};

//这里谨慎地说是以期通过解析阶段，而不是通过编译阶段，是因为当编译器编译到具现化
//template class的阶段时，如果为LoggingMsgSender<Company>传入的是CompanyZ，那么
//解析之后的编译仍将报错，因为CompanyZ的特化版本的MsgSender<Company>不提供sendClear。 
*/ 
