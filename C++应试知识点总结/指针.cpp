int const * * const * const ptr = & x;
/*
请解释ptr：
1.去掉修饰符看：int * * * ptr; ptr是一个指针。
2.加修饰符看，谨记：修饰符在*的左侧，修饰的是指针所指对象，修饰符在*的右侧，修饰的是指针本身。
3.解释为：一个const指针（const ptr），指向一个常指针（const *告诉我们ptr指向的对象也是一个const），该常指针指向一个普通指针（int const * *中，第二个是const，第一个是普通指针），该普通指针指向一个const常量（int const等效于const int）。
*/