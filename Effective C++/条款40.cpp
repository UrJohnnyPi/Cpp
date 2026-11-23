/*
条款40：明智而审慎地使用多重继承
*/

/*
代码1：

class BorrowableItem{ //图书馆允许你借某些东西 
	public :
		void checkOut(); //离开时检查 
};
class ElectronicGadget{ //自带的一些电子器件 
	private :
		bool checkOut() const; //离开时自行检查 
};
class MP3Player : public BorrowableItem , public ElectronicGadget{
	//do something
};  
MP3Player myMp3;

myMp3.checkOut(); //错误
myMp3.BorrowableItem::checkOut(); //正确
myMp3.ElectronicGadget::checkOut(); //错误，试图调用private 

//这样的多重继承的问题在于，即使只有一个checkOut是可取用的（即public下的那个），但编译器
//仍会出现模棱两可的局面，因为在判断可取用之前，编译器会先判断最佳匹配。在这里，两个checkOut
//具有相同的匹配度，因此编译器到了这步便要报错（因为出现了不确定性），也就没有判断下一步“可取用”
//的机会。
//这就是为什么上例中第一个实例错误，第二个实例正确（消除匹配度分歧，指出确定路径）的原因。 
*/

/*
代码2：

//菱形继承时若只想维护一个间接基类副本，就可以考虑虚继承。如下： 
class File{};
class InputFile : virtual public File{}; 
class OutputFile : virtual public File{}; 
class IOFile : public InputFile , public OutputFile{}; 
//但代价是空间开销和时间开销变大。 
*/

/*
代码3：

class IPerson{
	public :
		virtual ~IPerson();
		virtual string name() const = 0;
		virtual string birthDate() const = 0;
};
class DatabaseID{};
class PersonInfo{
	public :
		explicit PersonInfo(DatabaseID pid);
		virtual ~PersonInfo();
		virtual const char * theName() const;
		virtual const char * theBirthDate() const;
		virtual const char * valueDelimOpen() const;
		virtual const char * valueDelimClose() const;
};
class CPerson : public IPerson , private PersonInfo{
	public :
		explicit CPerson(DatabaseID pid) : PersonInfo(pid){} //根据pid定位到个人 
		virtual string name() const{ //实现IPerson成员函数 
			return PersonInfo::theName(); //PersonInfo中的函数利用pid返回数据，以实现IPerson中的函数 
		}
		virtual string birthDate() const{ //实现IPerson成员函数 
			return PersonInfo::theBirthDate();
		}
	private :
		virtual const char * valueDelimOpen() const; //重写PersonInfo中的成员函数 
		virtual const char * valueDelimClose() const; 
};
//该代码说明，当public继承以期实现“A is-a B”关系，但又需要另一个private继承以期“根据C实现出A ”时，
//多重继承的使用就有其正当性。特别是如上例一般，需要对C类中的某些virtual函数重写时，更确定了要是有多重继承
//而非public继承+复合。
*/
