#include <iostream>
using namespace std;
int bag_problem(int c , int n , int w [] , int v []) { //c->容量；n->n个物品；w[]->各物品占据空间；v[]->价值 
	int * * arr = new int * [n + 1];
	for(int i = 0 ; i < n + 1 ; ++i) arr[i] = new int [c + 1];
	for(int i = 0 ; i < c + 1 ; ++i) arr[0][i] = 0;
	for(int i = 0 ; i < n + 1 ; ++i) arr[i][0] = 0;

	for(int i = 1 ; i < n + 1 ; ++i)
		for(int j = 1 ; j < c + 1 ; ++j)
			if(j < w[i - 1]) arr[i][j] = arr[i - 1][j];
			else arr[i][j] = (v[i - 1] + arr[i - 1][j - w[i - 1]] > arr[i - 1][j] ? v[i - 1] + arr[i - 1][j - w[i - 1]] : arr[i - 1][j]);
			
	return arr[n][c];
}
int main() {
	int c = 100;
	int n = 5;
	int w [] = {77,22,29,50,99};
	int v [] = {92,22,87,46,90};
	cout << bag_problem(c , n , w , v);
	return 0;
}
