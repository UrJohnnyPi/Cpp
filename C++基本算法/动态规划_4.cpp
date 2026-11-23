#include <iostream>
#include <cstring>
using namespace std;
int LCS(char * str_1 , char * str_2 , int len_1 , int len_2) {
	int * * arr = new int * [len_1 + 1];
	for(int i = 0 ; i < len_1 + 1 ; ++i) arr[i] = new int [len_2 + 1];
	for(int i = 0 ; i < len_1 + 1; ++i) arr[i][0] = 0;
	for(int i = 0 ; i < len_2 + 1; ++i) arr[0][i] = 0;
	
	for(int i = 1 ; i < len_1 + 1; ++i)
	for(int j = 1 ; j < len_2 + 1; ++j)
		if(str_1[i] != str_2[j]) arr[i][j] = 0;
		else arr[i][j] = arr[i - 1][j - 1] + 1;
	
	int maxLen = 0;
	for(int i = 1 ; i < len_1 + 1; ++i)
	for(int j = 1 ; j < len_2 + 1; ++j){
		if(arr[i][j]){
			int row = i , col = j;
			for( ; arr[row][col] && row < len_1 + 1 && col < len_2 + 1 ; ++row , ++col);
			if(arr[row - 1][col - 1] > maxLen) maxLen = arr[row - 1][col - 1];
		}
	}
	
	return maxLen;
}
int main() {
	char * str_1 = "abadcztfdgs";
	char * str_2 = "barrtztfdgsds";
	cout << LCS(str_1 , str_2 , strlen(str_1) , strlen(str_2));
	return 0;
}