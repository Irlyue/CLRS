#ifndef MAX_HEAP_H
#define MAX_HEAP_H

#include <vector>
#include <cassert>
using namespace std;

static inline int parent(int i);
static inline int left(int i);
static inline int right(int i);
static inline bool isLeaf(int i, int n);
static inline void mySwap(int &lhs, int &rhs);

void maxHeapify(int *p, int i, int n);
void buildMaxHeap(int *p, int n);
void heapSort(int *p, int l, int r);


void heapSort(int *p, int l, int r){
	assert(l == 0);
	int n = r - l;
	buildMaxHeap(p, n);
	for(int i = 0; i < n; i++){
		mySwap(p[0], p[n-i-1]);
		maxHeapify(p, 0, n - i - 1);
	}
}

// n/2 is indeed the starting leaf node.
// so we loop from n/2-1 down to 0.
void buildMaxHeap(int *p, int n){
	for(int i = n/2 - 1; i >=0; i--) maxHeapify(p, i, n);
}

// Arguments
// ---------
// n: size of the heap
void maxHeapify(int *p, int i, int n){
	int lc = -1, rc = -1, largest = -1;

	// loop until i is leaf
	while(!isLeaf(i, n)){
		largest = lc = left(i);
		rc = right(i);
		// the index of the largest children of i
		// note that i may not have a right child
		// so we first test whether (rc < n)
		if(rc < n && p[lc] < p[rc])
			largest = rc;  

		if(p[i] >= p[largest]){
			break;
		}else{
			mySwap(p[i], p[largest]);
		}

		i = largest;
	}
}

static inline int parent(int i){
	return (i - 1) / 2;
}

static inline int left(int i){
	return 2 * i + 1;
}

static inline int right(int i){
	return 2 * i + 2;
}

// Denote the parent of the last node (n-1) as k.
// Than leaf nodes starts at k + 1. It's easy to
// figure out that k = parent(n-1) + 1 = floor(n/2).
static inline bool isLeaf(int i, int n){
	return i >= n / 2;
}

static inline void mySwap(int &lhs, int &rhs){
	int tmp = lhs;
	lhs = rhs;
	rhs = tmp;
}


#endif
