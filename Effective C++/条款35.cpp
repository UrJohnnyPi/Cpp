/*
条款35：考虑virtual函数以外的其他选择
*/
//NVI（non-virtual interface）手法实例：
//virtual interface： 
class Shape{
	public :
		enum ShapeColor{
			Red , Green , Blue
		};
		virtual void draw(ShapeColor color = Red) const = 0;
};
class Rectangle : public Shape{
	public :
		virtual void draw(ShapeColor color) const;
};

//等效的non-virtual interface：
class Shape{
	public :
		enum ShapeColor{
			Red , Green , Blue
		};
		void draw(ShapeColor color = Red) const {
			doDraw(color); 
		} 
	private :
		virtual void doDraw(ShapeColor color) const = 0;
};
class Rectangle : public Shape{
	private :
		virtual void doDraw(ShapeColor color) const ;
};
