/*
条款44：将与参数无关的代码抽离templates
*/

//首先要理解template具现化的原理。比如我们这里有20个相似的类型，每个类型有15个成员函数。 
//为了避免代码重复，我们写一个class template来接收类型，以便编译器去具现化20个class和300
//个成员函数。但这20个class在被以参数形式传入前是不会被具现化的，这300个成员函数在被调用前
//也是不会被具现化的。对于成员函数，也就是说，被使用才被具现化。
//对仅以类型为参数的class/function template的具现化不会导致多份副本的产生，即20个类型/300个成员函数共享一个class template副本；
//但对以非类型为参数的class/function template的具现化就将导致一次调用便生成一份副本。 

//template导致的代码膨胀案例：旨在编写一个正方形矩阵，支持求逆 
template<typename T , size_t n> //template<>可接收一个类型参数T，也可接收一个非类型n 
class SquareMatrix{
	public :
		void invert(); //求逆矩阵 
};
SquareMatrix<double , 5> sm1; //被定义才被具现化 
sm1.invert(); //被使用才被具现化 
SquareMatrix<double , 10> sm2;
sm2.invert();
//上述案例的问题在于template接收非类型参数导致了代码的膨胀。

//如果非得在template中写入非类型参数，则可修改为：
template<typename T>
class SquareMatrixBase{
	protected :
		void invert(size_t matrixSize);
}; 
template<typename T , size_t n>
class SquareMatrix : private SquareMatrixBase<T>{
	public :
		void invert(){
			this->invert(n);
		}
	private :
		using SquareMatrixBase<T>::invert;
};
//通过将代码抽离出template简化了SquareMatrix，也就进而使得代码膨胀不再那么严重了。

//实际上，因非类型参数造成的代码膨胀，通过利用函数参数 / class数据成员 替换template非类型参数便可消除。
//也就是如第一份代码中，将n写为 SquareMatrix的一个数据成员，通过构造函数获值就可。 
