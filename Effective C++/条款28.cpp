/*
条款28：避免返回handles指向对象内部成分

references、指针、迭代器统称为handles（号码牌、用来取得某个对象）。 
*/

/*
代码1：

//一个错误的示例：
class Point{
	public :
		Point(int x , int y);
		void setX(int newX);
		void setY(int newY);
};
struct RectData{ //一个矩形由左上点坐标和右下点坐标确定 
	Point ulhc;
	Point lrhc;
};
class Rectangle{
	public :
		Rectangle(Point coord1 , Point coord2) : pData(new RectData(coord1 , coord2)){
		}
		Point & upperLeft() const{ return pData->ulhc; }
		Point & lowerRight() const{ return pData->lrhc; }
	private :
		tr1::shared_ptr<RectData> pData;
};

Point coord1(0 , 0);
Point coord2(100 , 100);
const Rectangle rec(coord1 , coord2);
rec.upperLeft().setX(50);
//旨在将矩形的左上点横坐标调整为50。

//上述代码的错误之处：
//（1）Rectangle类中uperLeft()和lowerRight()函数返回类型中&的设计。我们可以通过公有函数成员获取私有数据成员的值 或 实现对私有函数成员的调用， 
//但我们不应当通过公有函数成员获得修改私有数据成员 或 直接访问私有函数成员的权限。因此：never return references/pointers/iterators of private members。
//（2）Rectangle类中uperLeft()和lowerRight()函数修饰符const的设计。我们欲通过用const修饰函数来使函数：可被const对象访问、该函数不改变数据成员、该函数
//也最好不提供对数据成员修改的可能（条款03）。但该函数很明显提供了对数据成员修改的可能。因此，有：一旦想声明该函数为const，也要声明其返回值为const。
//（3）const Rectangle rec(coord1 , coord2);对象的const设计。rec内容本不应该改变，但这里却通过一系列调用改变了。分析，rec的数据成员pData中的内容确实没变，
//但pData所指向的数据发生了改变。因此：要确保一个对象是logic const而不是bitwise const（像当前这样的情况）。即要保证const对象的指针和指针指向的内容都是const，
//要传递const属性到const对象的所有相关数据。 
*/ 

/*
代码2：

class GUIObject{
	//do something 
};
const Rectangle boundingBox(const GUIObject & obj){ //返回GUI的矩形外框的副本 
	//do something 
}
GUIObject go(para); 
GUIObject * pgo = &go;
const Point * pUpperLeft = & (boundingBox(* pgo).upperLeft()); //获取到go的矩形外框的副本的左上点对象，取其地址赋予pUpperLeft。
//当最后这步执行完后，pUpperLeft将空悬。因为在 取go的矩形外框的副本的左上点对象的地址赋值pUpperLeft后，这个左上点临时对象
//随即被析构。因此提出了本条款：任何将定义于域内的内容（private、protected、public）引入到域外的handles都是危险的。不是不允许
//将域内的内容引出到域外，而是不要用handles来实现这个过程。  
*/



