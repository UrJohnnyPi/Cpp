/*
题目：输入两个树结点，求它们的最低公共祖先。
分析：
（1）若树是二叉排序树，则判断根节点和两个节点的大小关系。若根节点大于两个节点，则这两个
节点位于根节点的左子树；若根节点小于两个节点，则这两个节点位于根节点的右子树；若这两个
节点，一个大于根节点，一个小于根节点，则根节点就是二者的最低公共祖先。
记住：二叉排序（搜索）树左小右大的特点。
（2）若树的每一节点都包含指向父节点的指针。则此问题演化为两个交叉链表找交叉点的问题。
（3）若树就是一颗普通二叉树。则：
递归思路：在节点函数中设置标识，先序遍历，若遍历到节点a，则将标识修改返回；同理，
若遍历到节点b，则将标识修改返回。这个标识不能是&，而是因不同节点而异的。出现的第
一个既被a修改又被b修改的标识所在的节点就是它们的最低公共祖先。

链表思路：用链表存储从根节点到a的路径，从根到a就是插入删除节点的过程。最终到a以后，不再删除
链表元素。开始回溯，然后寻找b节点，同样是插入删除节点的过程。到达b后，结束遍历。
这就构造了两个交叉链表，成为了情形（2）。 
下面的代码实现（3）递归思路。
*/
#include <iostream>
using namespace std;

typedef struct BiTNode {
	int data;
	BiTNode *lchild , *rchild;
} BiTNode , *BiTNodePtr;

void create_tree(BiTNodePtr ptr , int data , BiTNode * lchild , BiTNode * rchild) {
	ptr->data = data;
	ptr->lchild = lchild;
	ptr->rchild = rchild;
}

//求两节点的最低公共祖先。
int PreOrder(BiTNodePtr T , BiTNodePtr node_1 , BiTNodePtr node_2 , BiTNodePtr & f_node) {
	if(T!=nullptr) {
		if(T == node_1) return 1;
		if(T == node_2) return 2;
		
		int l = PreOrder(T->lchild , node_1 , node_2 , f_node);
		if(l == 3) return 3;
		
		int r = PreOrder(T->rchild , node_1 , node_2 , f_node);
		if(r == 3) return 3;
		
		if(l == 1)
			if(r == 2) {
				f_node = T;
				return 3;
			} else return 1;

		if(l == 2)
			if(r == 1) {
				f_node = T;
				return 3;
			} else return 2;
	}
	return 0;
}

int main() {
	BiTNodePtr T = new BiTNode[9]() , f_node = nullptr;

	create_tree(&T[0] , 0 , &T[1] , &T[2]);
	create_tree(&T[1] , 1 , &T[3] , &T[4]);
	create_tree(&T[2] , 2 , nullptr , nullptr);
	create_tree(&T[3] , 3 , &T[5] , &T[6]);
	create_tree(&T[4] , 4 , &T[7] , &T[8]);
	create_tree(&T[5] , 5 , nullptr , nullptr);
	create_tree(&T[6] , 6 , nullptr , nullptr);
	create_tree(&T[7] , 7 , nullptr , nullptr);
	create_tree(&T[8] , 8 , nullptr , nullptr);

	PreOrder(T , &T[5] , &T[6] , f_node);
	cout << f_node->data;
	return 0;
}

/*
还有一个思路：

借鉴tree_09中的v.push_back() 和 v.pop_back()的思路，如果当前的ptr指向了node_1，则将v中的值拷贝到node_1_ptr数组中；
同理，如果当前的ptr指向了node_2，则将v中的值拷贝到node_2_ptr数组中。有：
int i;
for(i = 0; node_1_ptr[i] == node_2_ptr[i]; ++i);
cout << node_1_ptr[--i]->data << ends;
*/