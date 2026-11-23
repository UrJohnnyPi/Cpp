/*
使用牛顿迭代法实现sqrt：
建立坐标轴，若求val的平方根，则设函数f(x)=x^2 - val。
该函数图像过x轴的点的横坐标就是所求。
f'(x) = 2x
于图像上随便取一点为(x`,y`)，有x`^2 = y`。
过该点做切线，有tan = y` / x` = (x`^2 - val) / x`，又tan = 2x`，
因此，x`` = x` -  (x`^2 - val) / 2x` = (x` + val / x`) / 2
儿x``比x`更接近交点。 
*/ 
#include <iostream>
#include <math.h>
using namespace std;
double sqrt(double val , double x , double epos) {
	double next = x;
	double present;
	do {
		present = next;
		next = (val / next + next) / 2;
		cout << fabs(next-present) << endl;
	} while (fabs(next-present) > epos); //注意是fabs()而非abs()；前者返回值是double，后者返回值是int。 
	return next;
}
int main() {
	cout << sqrt(29.0 , 5.0 , 0.00000000001) << endl << sqrt(29);
	return 0;
}
