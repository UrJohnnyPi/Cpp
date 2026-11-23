/*
条款36：绝不重新定义继承而来的non-virtual函数

class B{
	public :
		void mf();
}; 
class D : public B{
	public :
		void mf();
};

D x;
B * pB = & x;
D * pD = & x;
pB->mf(); //B::mf()
pD->mf(); //D::mf()

如上代码所示，矛盾于此：
（1）如果D的确需要实现与B中同名的函数mf()，且B的对象只可以访问B中实现的mf()。那么因为后一个原因，mf()不能是
virtual；又因为要在类外要用指针访问mf()，所以mf()不应该是是private和protected；结合第一个原因，D应该private
或protected继承，总之不应该是public继承，因为如条款32所言，public继承是“is-a”的关系，即Derived就是一种特化
的Base， 是Base的就是Derived的，所以为已有non-virtual mf()的D类再写一个non-virtual mf()，很奇怪。 
（2）如果D的确需要public继承B，且D确实需要重写mf()，那么D中的mf()应当写为virtual。

以上矛盾均出于在派生类中重定义了继承而来的non-virtual函数。 
*/
