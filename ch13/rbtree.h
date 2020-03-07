#ifndef RB_TREE_H
#define RB_TREE_H

#include <string>
#include <cstdio>


enum Color { RED, BLACK };

struct RBNode {
    int val;
    Color color;
    RBNode *left = nullptr, *right = nullptr, *parent = nullptr;
    RBNode(int v, Color c = RED) : val(v), color(c) {}
};

class RBTree {
public:
    static RBNode *RB_NIL;
    ~RBTree() { clear(root); }
    RBNode *successor(RBNode *x);
    RBNode *predecessor(RBNode *x);
    RBNode *min(RBNode *x);
    RBNode *max(RBNode *x);
    RBNode *search(int k);
    void insert(RBNode *x);
    void remove(RBNode *x, bool del = true);
    void print();
    void clear(RBNode *x);
    bool isEmpty() const;

    RBNode *getRoot() { return root; }

private:
    RBNode *root = RB_NIL;
    
    void print(std::string prefix, RBNode *x, bool left);
    void transplant(RBNode *x, RBNode *y);
    void leftRotate(RBNode *x);
    void rightRotate(RBNode *x);
    void insertFixup(RBNode *x);
    void removeFixup(RBNode *x);
};

RBNode *RBTree::RB_NIL = new RBNode(-1, BLACK);

bool RBTree::isEmpty() const { return root == RB_NIL; }

// Arguments
// ---------
// x: the node to be removed from the BST tree.
// del: a boolean flag to denote whether to deallocate the
//     memory pointed by `x`.
void RBTree::remove(RBNode *z, bool del) {
    RBNode *y = z, *x;
    Color yOldColor = y->color;
    if (z->left == RB_NIL) {
        x = z->right;
        transplant(z, z->right);
    }else if(z->right == RB_NIL) {
        x = z->left;
        transplant(z, z->left);
    }else{
        y = min(z->right);
        yOldColor = y->color;

        x = y->right;
        if(z == y->parent){
            x->parent = y;
        }else{
            transplant(y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }

        // handle x's left child
        transplant(z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }
    if(yOldColor == BLACK) removeFixup(x);

    if(del) delete z;
    else z->left = z->right = z->parent = nullptr;
}

/* Node marked with @ at its left is the new node in the next iteration
 * after transformation.
 *
 * Case 1: x's uncle w is RED
 *               p(B)                        w(B)
 *              /   \                       /   \
 *             /     \                     /     \
 *            /       \                   /       \
 *          x(B)      w(R)     -->      p(R)      v(B)
 *          / \       /  \              /  \      / \
 *         a   b   u(B)  v(B)        @x(B)  u(B)  e  f
 *                 / \    / \         / \    / \
 *                c   d   e  f       a   b  c   d
 * After a left rotate on x's parent, u becomes its new uncle.
 *
 * Case 2: x's uncle w is BLACK and w's two children are BLACK.
 *               p(?)                       @p(?)
 *              /   \                       /   \
 *             /     \                     /     \
 *            /       \                   /       \
 *          x(B)      w(B)     -->      x(B)      w(R)
 *          / \       /  \              / \      /  \
 *         a   b   u(B)  v(B)          a   b  u(B)  v(B)
 *                 / \    / \                 / \   / \
 *                c   d   e  f               c   d  e  f
 * Change w's color to RED and move one level up.
 * 
 * Case 3: x's uncle is BLACK and w's left child is RED and its
 * right child is BLACK.
 *               p(?)                        p(?)
 *              /   \                       /   \
 *             /     \                     /     \
 *            /       \                   /       \
 *          x(B)      w(B)     -->     @x(B)      u(B)
 *          / \       /  \              / \      /  \
 *         a   b   u(R)  v(B)          a   b    c   w(R)
 *                 / \    / \                       / \
 *                c   d   e  f                     d  v(B)
 *                                                    / \
 *                                                   e   f
 * After a right rotate w and we go into case 4
 *
 * Case 4: x's uncle is BLACK and w's right child is RED.
 *               p(*)                        w(*)
 *              /   \                       /   \
 *             /     \                     /     \
 *            /       \                   /       \             @root
 *          x(B)      w(B)     -->      p(B)      v(B)
 *          / \       /  \             /   \      / \
 *         a   b     u   v(R)         x(B)  u     e  f
 *                  / \   / \         /\   / \   
 *                 c   d e   f       a  b  c  d                                         
 * Swith p and w's color and change v's color to black;
 * then perform a left rotate on w;
 * Then we set x to root and will exit the loop in the next iteration.
 *
 * The transition between different cases is as follows:
 *        2-----------------------+           
 *        ^                       | 
 *        |-----------+           |
 *        |           |           |
 *        |           V           V
 *        1 --> 3 --> 4 --> loop termination
 *        ^     ^     ^
 *        |     |     |
 *        +---- 2 ----+
 *             | ^
 *             | |
 *              +
 * Whenever case 1, 3 or 4 is encountered, the loop will exit.
 * Case 1 could switch to either of the other 3 cases. But whenever it
 * switches to case 2, it'll exit. Here is the reason: when case 1 enters
 * into case 2, notice that x's parent is RED. So after some color
 * chaning in case 2, the loop condition no longer satisfies.
 *
 * So only when we in case 2, the loop may repeat. And since case 2 will
 * moves one level up, it could be repeated at most O(lg n) times. As a
 * result, the complexity of removeFixup is O(lg n).
 */
void RBTree::removeFixup(RBNode *x){
    while(x != root && x->color == BLACK){
        // x is a left child
        if(x == x->parent->left){
            RBNode *w = x->parent->right;
            if(w->color == RED){                                         // case 1: x's uncle w is RED.
                x->parent->color = RED;
                w->color = BLACK;
                leftRotate(x->parent);
                w = x->parent->right;   // remember to update x's uncle after rotation
            }

            // case 2,3,4: x's uncle w is BLACK
            if(w->left->color == BLACK && w->right->color == BLACK){    // case 2: w's 2 children are BLACK
                w->color = RED;
                x = x->parent;
            }else{
                if(w->left->color == RED && w->right->color == BLACK){  // case 3: w's left child is RED and its right child is BLACK
                    w->color = RED;
                    w->left->color = BLACK;
                    rightRotate(w);
                }
                w->color = x->parent->color;                            // case 4: w's right child is RED
                x->parent->color = BLACK;
                w->right->color = BLACK;
                leftRotate(x->parent);
                x = root;
            }
        }else{
            RBNode *w = x->parent->left;
            if(w->color == RED){
                x->parent->color = RED;
                w->color = BLACK;
                rightRotate(x->parent);
            }

            if(w->right->color == BLACK && w->left->color == BLACK){
                w->color = RED;
                x = x->parent;
            }else{
                if(w->right->color == RED && w->left->color == BLACK){
                    w->color = RED;
                    w->right->color = BLACK;
                    leftRotate(w);
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->left->color = BLACK;
                rightRotate(x->parent);
                x = root;
            }
        }
    }
    x->color = BLACK;
}

// This function only changes the parent of y and 
// the one child of x's parent.
//
// y could be RB_NIL but we don't care.
void RBTree::transplant(RBNode *x, RBNode *y) {
    if(x->parent == RB_NIL) {
        root = y;
    }else if (x->parent->left == x) {
        x->parent->left = y;
    }else {
        x->parent->right = y;
    }
    y->parent = x->parent;
}

// Deallocate all the tree nodes
void RBTree::clear(RBNode *x) {
    if (x != RB_NIL) {
        clear(x->left);
        clear(x->right);
        delete x;
    }
}

/*
 * 2 cases
 * Case 1: x has left child, just min(x->left);
 * Case 2: x has no left child, find the firt ancestor that has the folloing
 *         structure.
 *              p  <-----
 *               \
 *                *
 *               /
 *              x
 *             /
 *            y
 * Return: x's predecessor or RB_NIL if x is the first node in the inOrder
 * sequence.
 */
RBNode *RBTree::predecessor(RBNode *x) {
    if (x->left != RB_NIL) return max(x->left);

    RBNode *y = x;
    x = x->parent;
    while (x != RB_NIL && x->left == y) {
        y = x;
        x = x->parent;
    }
    return x;
}

/*
* 2 cases
* Case 1: x has right child, just max(x->right);
* Case 2: x has no left child, find the firt ancestor that has the folloing
*         structure.
*                  p  <-----
*                 /
*                *
*                 \
*                  x
*                   \
*                    y
* Return: x's successor or RB_NIL if x is the last node in the inOrder
* sequence.
*/
RBNode *RBTree::successor(RBNode *x) {
    if (x->right != RB_NIL) return min(x->right);

    RBNode *y = x;
    x = x->parent;
    while (x != RB_NIL && x->right == y) {
        y = x;
        x = x->parent;
    }
    return x;
}

// Return: the highest node with the value k or RB_NIL if no such node exists.
RBNode *RBTree::search(int k) {
    RBNode *cur = root;
    while (cur != RB_NIL) {
        if (cur->val == k) return cur;
        else if (cur->val < k) cur = cur->right;
        else cur = cur->left;
    }
    return RB_NIL;
}

RBNode *RBTree::min(RBNode *x) {
    RBNode *cur = x, *prev = x->parent;
    while (cur != RB_NIL) {
        prev = cur;
        cur = cur->left;
    }
    return prev;
}

RBNode *RBTree::max(RBNode *x) {
    RBNode *cur = x, *prev = x->parent;
    while (cur != RB_NIL) {
        prev = cur;
        cur = cur->right;
    }
    return prev;
}

void RBTree::insert(RBNode *z) {
    RBNode *cur = root, *prev = RB_NIL;
    while (cur != RB_NIL) {
        prev = cur;
        if (z->val <= cur->val) cur = cur->left;
        else cur = cur->right;
    }

    z->parent = z->left = z->right = RB_NIL;
    z->color = RED;
    if (prev == RB_NIL) root = z;
    else {
        z->parent = prev;
        if (z->val <= prev->val) prev->left = z;
        else prev->right = z;
    }
    insertFixup(z);
}

/*
 *      case 1 --> case 2 --> case 3 --> loop terminate
 *      |  ^
 *      |  |
 *      ---+
 * Case 1 moves 2 levels up and whenever case 2 or 3 is encountered,
 * we break out of the loop. So the runtime of fixup is O(h).
 */
void RBTree::insertFixup(RBNode *z){
    while(z->parent->color == RED){
        if(z->parent == z->parent->parent->left){
            RBNode *y = z->parent->parent->right;
            if(y->color == RED){                      // case 1: z's uncle y is red.
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            }else{
                if(z == z->parent->right){           // case 2: z's uncle y is black and z is a right child
                    z = z->parent;
                    leftRotate(z);
                }
                z->parent->color = BLACK;            // case 3: z's uncle y is black and z is a left child
                z->parent->parent->color = RED;
                rightRotate(z->parent->parent);
            }
        }else{
            RBNode *y = z->parent->parent->left;
            if (y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            }
            else {
                if (z == z->parent->left) {
                    z = z->parent;
                    rightRotate(z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                leftRotate(z->parent->parent);
            }
        }
    }

    root->color = BLACK;
}


/*             y                 x
 *            / \               / \
 *           x   c     <--     a   y
 *          / \                   / \
 *         a   b                 b   c
 * 
 * The key to code it right is to draw directed edges between nodes.
 * And you'll figure out 6 edges in total are needed to change. The
 * if statement is to handle one of the trickest edge.
 */
void RBTree::leftRotate(RBNode *x){
    RBNode *y = x->right;
    y->parent = x->parent;

    x->right = y->left;
    x->right->parent = x;    // no need to test if x->right == NIL

    y->left = x;
    if(x == root){
        root = y;
    }else if(x == x->parent->right){
        y->parent->right = y;
    }else{
        y->parent->left = y;
    }
    x->parent = y;
}

/*             y                 x
 *            / \               / \
 *           x   c     -->     a   y
 *          / \                   / \
 *         a   b                 b   c
 *
 * switch x and y, left and right from the leftRotate function
 */
void RBTree::rightRotate(RBNode *y){
    RBNode *x = y->left;
    x->parent = y->parent;
    
    y->left = x->right;
    y->left->parent = y;

    x->right = y;

    if(y == root){
        root = x;
    }else if(y == y->parent->left){
        x->parent->left = x;
    }else{
        x->parent->right = x;
    }
    y->parent = x;
}

void RBTree::print() {
    print("", root, true);
}

// Left subtree "+" and right subtree "-"
void RBTree::print(std::string prefix, RBNode *x, bool left) {
    if (x != RB_NIL) {
        print(prefix + "     ", x->right, false);
        printf("%s%s %d(%s)\n", prefix.c_str(), left ? "++++" : "----", x->val, x->color == RED ? "R" : "B");
        print(prefix + "     ", x->left, true);
    }
}

#endif
