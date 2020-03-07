#include <cstdio>
#include "bstree.h"
using namespace std;

void traverseAscending(BSTree &tree){
	auto start = tree.min(tree.getRoot());
	while(start){
		printf("%d ", start->val);
		start = tree.successor(start);
	}
	printf("\n");
}

void traverseDescending(BSTree &tree){
	auto start = tree.max(tree.getRoot());
	while(start){
		printf("%d ", start->val);
		start = tree.predecessor(start);
	}
	printf("\n");
}


int main(){
	BSTree tree;
	tree.insert(new BSTNode(2));
	tree.insert(new BSTNode(1));
	tree.insert(new BSTNode(3));
	tree.insert(new BSTNode(4));
	tree.insert(new BSTNode(9));
	tree.insert(new BSTNode(5));
	tree.insert(new BSTNode(-5));
	tree.insert(new BSTNode(-2));
	tree.insert(new BSTNode(-3));
	auto pMin = tree.min(tree.getRoot()), pMax = tree.max(tree.getRoot());
	printf("min = %d, max = %d\n", pMin->val, pMax->val);
	tree.print();
	traverseAscending(tree);
	traverseDescending(tree);

	while(!tree.isEmpty()){
		tree.remove(tree.getRoot());
		printf("**********************************************\n");
		tree.print();
	}

	return 0;
}
