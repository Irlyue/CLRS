#ifndef BSTREE_H
#define BSTREE_H

#include <string>

struct BSTNode{
	int val;
	BSTNode *left = nullptr, *right = nullptr, *parent = nullptr;
	BSTNode(int v): val(v){}
};

class BSTree{
public:
	~BSTree() {clear(root);}
	BSTNode *successor(BSTNode *x);
	BSTNode *predecessor(BSTNode *x);
	BSTNode *min(BSTNode *x);
	BSTNode *max(BSTNode *x);
	BSTNode *search(int k);
	void insert(BSTNode *x);
	void remove(BSTNode *x, bool del=true);
	void print();
	void clear(BSTNode *x);
	bool isEmpty() const;

	BSTNode *getRoot() {return root;}

private:
	BSTNode *root = nullptr;
	void print(std::string prefix, BSTNode *x, bool left);
	void transplant(BSTNode *x, BSTNode *y);
};

bool BSTree::isEmpty() const {return root == nullptr;}

// Arguments
// ---------
// x: the node to be removed from the BST tree.
// del: a boolean flag to denote whether to deallocate the
//     memory pointed by `x`.
void BSTree::remove(BSTNode *x, bool del){
	if(x->left == nullptr){
		transplant(x, x->right);
	}else if(x->right == nullptr){
		transplant(x, x->left);
	}else{
		// at last y will replace x
		BSTNode *y = min(x->right);

		// handle x's right child
		if(y->parent != x){
			transplant(y, y->right);
			y->right = x->right;
			y->right->parent = y;
		}

		// handle x's left child
		transplant(x, y);
		y->left = x->left;
		y->left->parent = y;
	}
	if(del) delete x;
	else x->left = x->right = x->parent = nullptr;
}

// This function only changes the parent of y and 
// the one child of x's parent.
void BSTree::transplant(BSTNode *x, BSTNode *y){
	if(x->parent == nullptr){
		root = y;
	}else if(x->parent->left == x){
		x->parent->left = y;
	}else{
		x->parent->right = y;
	}
	if(y) y->parent = x->parent;
}

void BSTree::clear(BSTNode *x) {
	if(x){
		clear(x->left);
		clear(x->right);
		delete x;
	}
}

BSTNode *BSTree::predecessor(BSTNode *x){
	if(x->left) return max(x->left);

	BSTNode *y = x;
	x = x->parent;
	while(x && x->left == y){
		y = x;
		x = x->parent;
	}
	return x;
}

BSTNode *BSTree::successor(BSTNode *x){
	if(x->right) return min(x->right);

	BSTNode *y = x;
	x = x->parent;
	while(x && x->right == y){
		y = x;
		x = x->parent;
	}
	return x;
}

BSTNode *BSTree::search(int k){
	BSTNode *cur = root;
	while(cur){
		if(cur->val == k) return cur;
		else if(cur->val < k) cur = cur->right;
		else cur = cur->left;
	}
	return nullptr;
}

BSTNode *BSTree::min(BSTNode *x){
	BSTNode *cur = x, *prev = x->parent;
	while(cur){
		prev = cur;
		cur = cur->left;
	}
	return prev;
}

BSTNode *BSTree::max(BSTNode *x){
	BSTNode *cur = x, *prev = x->parent;
	while(cur){
		prev = cur;
		cur = cur->right;
	}
	return prev;
}

void BSTree::insert(BSTNode *x){
	BSTNode *cur = root, *prev = root;
	while(cur){
		prev = cur;
		if(x->val <= cur->val) cur = cur->left;
		else cur = cur->right;
	}

	if(prev == nullptr) root = x;
	else{
		x->parent = prev;
		if(x->val <= prev->val) prev->left = x;
		else prev->right = x;
	}
}

void BSTree::print(){
	print("", root, true);
}

// Left subtree "+" and right subtree "-"
void BSTree::print(std::string prefix, BSTNode *x, bool left){
	if(x){
		print(prefix + "     ", x->right, false);
		printf("%s%s %d\n", prefix.c_str(), left ? "++++" : "----", x->val);
		print(prefix + "     ", x->left, true);
	}
}

#endif
