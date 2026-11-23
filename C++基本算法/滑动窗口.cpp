/*
题目：给定一个数组和滑动窗口的大小，请找出所有滑动窗口里的最大值。例如，
如果输入数组{2, 3, 4, 2, 6, 2, 5, 1}及滑动窗口的大小3，那么一共存在6个
滑动窗口，它们的最大值分别为{4, 4, 6, 6, 6, 5}。
分析：
（1）可以蛮力，时间复杂度为o(kn)。
（2）借鉴选择排序，设置窗口初始包含第一个元素，设置max_ptr指向第一个元素。
滑动窗口一步，若max_ptr所指仍在窗口中，且max_ptr所指大于新进入窗口的元素，则max_ptr指向不变；
若max_ptr所指不窗口中，++max_ptr使max_ptr指向窗口内新的max。这样可以提高效率。
（3）设置vector构造双端队列。以{2, 3, 4, 2, 6, 2, 5, 1}为例：
2进入窗口->把2放到队列中。
3进入窗口->把3放到队列中，因2<3，故2不可能是max，删除队首2。
4进入窗口->把4放到队列中，因4>3，删除队首3。此时第一个3轮满，输出队首元素max。

2进入窗口->此时窗口{3,4,2}，队中元素是4。2入队。队中元素是4 2。输出队首元素max。
6进入窗口->此时窗口{4,2,6}，因为4<6且2<6，则先弹出4,2，再将6入队。输出队首元素max。
这里有个技巧是先比较6和队尾2，弹出2；再比6和新队尾4，弹出4，队空，压入6。
2进入窗口->此时窗口{2,6,2}，队中元素是6。2入队。队中元素是6 2。输出队首元素max。
5进入窗口->此时窗口{6,2,5}，因6>5且2<5，故先从队尾弹出2，再压入5。对中元素为6,5。输出队首元素max。
同样，比5和2，弹出队尾2,；再比5和新队尾6，因5<6，压入5。这样的小技术就可以使增删只发生在队尾。
以此类推。
总结一下，就是后进窗口的元素和先进窗口的两个比，若后进元素小于前两元素，则将后进元素压入队列；
若后进元素大于前面的一个元素，则将前面的那个元素从队尾弹出，压入新元素；
若后进元素大于前面的两个元素，则将前面的两个元素从队列弹出，压入新元素；
这样就保证了队首始终为max。
下面实现第三种方法。
*/
#include <iostream>
#include <vector>
using namespace std;
vector<int> max_elem(int arr[] , int length , int k) {
	vector<int> max_arr; //用于存放每个窗口中的最大值
	if(k > length || k <= 0) return max_arr;
	
	vector<int> que;
	for(int i = 0 , index = 0 ; i < length ; ++i) {
		if(i - index == k) que.erase(que.begin() , que.begin() + 1); 
		while(!que.empty() && arr[i] >= que.back()) que.pop_back();
		que.push_back(arr[i]);
		if(que.front() == arr[i]) index = i;
		if(i >= k-1) max_arr.push_back(que.front());
	}

	return max_arr;
}
int main() {
	int arr[] = {2, 3, 4, 2, 6, 2, 5, 1};
	vector<int> max_arr = max_elem(arr , 8 , 1);
	for(vector<int>::iterator it = max_arr.begin() ; it != max_arr.end() ; ++it) cout << * it;
	return 0;
}
