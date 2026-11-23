/*
条款08：别让异常逃离析构函数（别让析构函数抛出异常）

如果析构函数必须调用某一函数，而该函数可能抛出异常，析构函数就要有能力把这个异常
吞下。因为如果异常逃离了析构函数，比如：
class Widget{
	public :
		~Widget(){
			//do something
		}
};
void fun(){
	vector<Widget> v;
	//do something
}
当fun运行结束时，会调用n次~Widget来析构v[0~n-1]，释放资源。若v[i]析构失败，
那会终止程序，致使v[i~n-1]的资源未被释放，造成泄漏。
同理，即使只是一个对象，其析构失败，异常未被析构函数处理而是逃离，那么其所占
资源也会泄漏。 
*/
class DBConnection {
	public :
		DBConnection(); //创建数据库连接
		static DBConnection creat(); //返回DBConnection对象，之所以用static，是因为一个数据库提供的对象是稳定的，内容不变的。
		void close(); //关闭当前对象和数据库的连接，之所以不是static，是因为每个对象的关闭时机是不同的。
		~DBConnection(); //析构数据库连接，其内部不应该调用close()，因为该析构函数是整个数据库资源连接的释放，与个体无关。
		//该析构函数应存在这样的机制：在被调用前，所有的连接都先被释放。
};
class MyDBConnection {
	public :
		MyDBConnection();
		void close() { //提供给客户自行关闭的权利，客户也因此获得了处理异常的权利。
			db.close(); //客户可以调用这个函数，也可以忽略。若忽略，也有析构函数处理异常。 
			closed = true;
		}
		~MyDBConnection() {
			if(!closed) {
				try {
					db.close();
				} catch(error) {
					//作用1：防止异常逃离析构函数。
					//作用2：记录异常。
				}
			}
		}
	private :
		DBConnection db;
		bool closed;
};
