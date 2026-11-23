/*
题目：请实现一个函数，用来判断一棵二叉树是不是对称的。如果一棵二叉树和
它的镜像一样，那么它是对称的。
（1）判断左根右遍历的data值 是否等于 右根左遍历的data值。
（2）判断两种遍历情况下，树根位置是否相同。为应对全树等值的情况。 
*/
/*
#include <iostream>
#include <vector>
using namespace std;

typedef struct BiTNode {
	int data;
	BiTNode *lchild , *rchild;
} BiTNode , *BiTNodePtr;

//构建函数
void create_tree(BiTNodePtr ptr , int data , BiTNode * lchild , BiTNode * rchild) {
	ptr->data = data;
	ptr->lchild = lchild;
	ptr->rchild = rchild;
}

//对称判定 
void InOrder_l_r(BiTNodePtr T , vector<BiTNodePtr> &arr) {
	if(T!=nullptr) {
		InOrder_l_r(T->lchild , arr);
		arr.push_back(T);
		InOrder_l_r(T->rchild , arr);
	}
}

void InOrder_r_l(BiTNodePtr T , vector<BiTNodePtr> &arr) {
	if(T!=nullptr) {
		InOrder_r_l(T->rchild , arr);
		arr.push_back(T);
		InOrder_r_l(T->lchild , arr);
	}
}

bool symmetry_tree(BiTNodePtr T) {
	vector<BiTNodePtr> arr_1; //arr_1中存放左根右指针。 
	vector<BiTNodePtr> arr_2; //arr_1中存放右根左指针。
	
	InOrder_l_r(T , arr_1);
	InOrder_r_l(T , arr_2);
	
	for(int i = 0 ; i < arr_1.size() ; ++i) {
		if(arr_1[i]->data != arr_2[i]->data) 
			return false; //左根右遍历不等于右根左遍历，return false。 
			
		if(arr_1[i] == T)
			if(arr_2[i] != T)
				return false; //左根右遍历中树根位置不等于右根左遍历树根位置，return false。 
	}
	
	return true;
}

int main() {
	BiTNodePtr T = new BiTNode[6]();
	create_tree(&T[0] , 7 , &T[1] , &T[2]);
	create_tree(&T[1] , 7 , &T[3] , &T[4]);
	create_tree(&T[2] , 7 , &T[5] , nullptr);
	create_tree(&T[3] , 7 , nullptr , nullptr);
	create_tree(&T[4] , 7 , nullptr , nullptr);
	create_tree(&T[5] , 7 , nullptr , nullptr);

	if(symmetry_tree(T)) cout << "this tree is symmetrical.";
	else cout << "this tree isn't symmetrical.";

	return 0;
}
*/

#include <iostream>
#include <queue>
using namespace std;

typedef struct BiTNode{
	int data;
	BiTNode *lchild , *rchild;
} BiTNode , *BiTNodePtr;

//层次遍历，用于测试 
void visit(BiTNodePtr T) {
	cout << T->data << ends;
}

void LevelOrder(BiTNodePtr T){
	queue<BiTNodePtr> BiTQueue;
	BiTNodePtr ptr;
	BiTQueue.push(T);
	while(!BiTQueue.empty()){
		ptr = BiTQueue.front();
		BiTQueue.pop();
		visit(ptr);
		if(ptr->lchild!=nullptr) BiTQueue.push(ptr->lchild);
		if(ptr->rchild!=nullptr) BiTQueue.push(ptr->rchild);
	}
}

//构建函数
void create_tree(BiTNodePtr ptr , int data , BiTNode * lchild , BiTNode * rchild) {
	ptr->data = data;
	ptr->lchild = lchild;
	ptr->rchild = rchild;
}

//求镜像 
void mirror_tree(BiTNodePtr T){
	if(T){
		BiTNodePtr ptr = T->lchild;
		T->lchild = T->rchild;
		T->rchild = ptr;
		if(T->lchild) mirror_tree(T->lchild);
		if(T->rchild) mirror_tree(T->rchild);
	}
}

//复制树
void copy_tree(BiTNodePtr & A, BiTNodePtr B){
	if(B){
		A = new BiTNode();
		A->data = B->data;
		if(B->lchild) copy_tree(A->lchild, B->lchild);
		else A->lchild = nullptr;
		if(B->rchild) copy_tree(A->rchild, B->rchild);
		else A->rchild = nullptr;
	}
}

/*
求镜像 和 复制树 可以合二为一：
void mirror_copy(BiTNodePtr & A, BiTNodePtr B) {
	if(B){
		A = new BiTNode();
		A->data = B->data;
		if(B->lchild) mirror_copy(A->rchild, B->lchild);
		else A->rchild = nullptr;
		if(B->rchild) mirror_copy(A->lchild, B->rchild);
		else A->lchild = nullptr;
	}
}
*/

bool match(BiTNodePtr A, BiTNodePtr B){
	if(!A && !B) return true;
	if(A && B) {
		if(A->data == B->data && match(A->lchild, B->lchild) && match(A->rchild, B->rchild)) return true;
	}
	return false;
}

int main(){
	BiTNodePtr T = new BiTNode[7](), M = nullptr;
	/*
	create_tree(&T[0] , 8 , &T[1] , &T[2]);
	create_tree(&T[1] , 6 , &T[3] , &T[4]);
	create_tree(&T[2] , 10 , &T[5] , &T[6]);
	create_tree(&T[3] , 5 , nullptr , nullptr);
	create_tree(&T[4] , 7 , nullptr , nullptr);
	create_tree(&T[5] , 9 , nullptr , nullptr);
	create_tree(&T[6] , 11 , nullptr , nullptr);
	*/
///*
	create_tree(&T[0] , 7 , &T[1] , &T[2]);
	create_tree(&T[1] , 7 , &T[3] , &T[4]);
	create_tree(&T[2] , 7 , &T[5] , &T[6]);
	create_tree(&T[3] , 7 , nullptr , nullptr);
	create_tree(&T[4] , 7 , nullptr , nullptr);
	create_tree(&T[5] , 7 , nullptr , nullptr);
	create_tree(&T[6] , 7 , nullptr , nullptr);
//*/
	cout << "T 1:" << endl;
    LevelOrder(T);
    cout << endl;

	mirror_tree(T);
	cout << "T 2:" << endl;
    LevelOrder(T);
    cout << endl;

	copy_tree(M, T);
	cout << "M:" << endl;
    LevelOrder(M);
    cout << endl;

	mirror_tree(T);
	cout << "T 3:" << endl;
    LevelOrder(T);
    cout << endl;

	if (match(M, T)) cout << "this tree is symmetrical.";
	else  cout << "this tree isn't symmetrical.";
	
	return 0;
}