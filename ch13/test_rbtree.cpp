#include "rbtree.h"
#include <iostream>
#include <iterator>
#include <cstdlib>
#include <chrono>
#include <vector>
#include <map>
using namespace std;

void inOrder(const RBNode *x){
    if(x != RBTree::RB_NIL){
        inOrder(x->left);
        cout << x->val << ' ';
        inOrder(x->right);
    }
}

int main(int argc, char* argv[]){
    map<int, int> mp;
    RBTree rbt;
    vector<int> nums1 = {41, 38, 31, 12, 19, 8};  // case 1, 2, 3
    vector<int> nums2 = {8, 19, 12, 31, 38, 41};    // case 4, 5, 6

    //int n = 100;
    //nums1.resize(n);
    //nums2.resize(n);
    //for(int i = 0; i < n; i++) nums1[i] = nums2[i] = i;
    for(auto &i: nums1){
        rbt.insert(new RBNode(i));
        rbt.print();
        inOrder(rbt.getRoot());
        cout << endl;
    }

    for(auto &i: nums2){
        rbt.remove(rbt.search(i));
        rbt.print();
        cout << endl;
    }
    system("pause");
    return 0;    
}
