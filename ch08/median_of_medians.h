/* Some coding styles:
 *    1) all array range resembles that of the STL iterator range [l, r), namely not
 * including the end element.
 *
 *
 */
#ifndef MEDIAN_OF_MEDIAN_H
#define MEDIAN_OF_MEDIAN_H

#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;

static constexpr int BASE_SIZE = 32;

int select(vector<int> &A, int l, int r, int n);
int selectMM(vector<int> &A, int l, int r, int n);
pair<int, int> partition(vector<int> &A, int l, int r, int pivotIndex);
static int selectPivot(vector<int> &A, int l, int r);
static void insertionSort(vector<int> &A, int l, int r);
static int median3(vector<int> &A, int l, int r);
static void mySwap(int &lhs, int &rhs);

// First we start off with some "money" using the simple median-of-3 pivot strategy.
// When we spend all the "money" we have, we may have encountered a killer sequence
// and switch to the median-of-medians selection algorithm.
// 
// Arguments
// ---------
//   A:
//   l, r: denoting the target range [l, r), excluding the end element.
//   n: the n-th smallest element we want to select.
// 
// Return: just return the input argument `n`.
int select(vector<int> &A, int l, int r, int n){
    int money = 4 * (r - l) + 10;
    while(r - l > BASE_SIZE && money > 0){
        // printf("%d %d %d\n", l, r, money);
        money -= (r - l);
        int pivotIndex = median3(A, l, r);
        auto jk = partition(A, l, r, pivotIndex);
        if(n < jk.first) r = jk.first;
        else if(n >= jk.second) l = jk.second;
        else return n;
    }
    printf("Fallback l=%d r=%d len=%d\n", l, r, r-l);
    return selectMM(A, l, r, n);
}

// Reorder the elements {A[l], A[(l+r)/2], A[r-1]} so
// A[(l+r)/2] becomes the median of these three.
// 
// Return: (l+r)/2
static inline int median3(vector<int> &A, int l, int r){
    int mid = (l + r) / 2;
	if(A[l] > A[mid]){
		mySwap(A[l], A[mid]);
	}

	if(A[mid] > A[r-1]){
		mySwap(A[mid], A[r-1]);
		if(A[l] > A[mid]) mySwap(A[l], A[mid]);
	}
	return mid;
}


// The actuall median-of-medians selection algorithm, which has worst-case
// O(n) time complexity. The idea is to choose an approximate median as
// the partition pivot, so the left and right subarrays have roughly the same
// number of elements. But this algorithm has a relatively large constant
// factor so we will only use it as a fallback to handle "killer sequence".
int selectMM(vector<int> &A, int l, int r, int n){
    // coarsening the recursive call.
    while(r - l > BASE_SIZE){
        int pivotIndex = selectPivot(A, l, r);
        auto jk = partition(A, l, r, pivotIndex);
        if(n < jk.first) r = jk.first;
        else if(n >= jk.second) l = jk.second;
        else return n;  // j <= n < k
    }
    // When the input range [l, r) is small enough, just use insertion sorting
    // to sort the elements
    insertionSort(A, l, r);
    return n;
}

// 3-way partition function.
// This function returns a pair of integers marking the range whose elements
// are equal to the pivot.
// 
// Return: (j, k) which marks the following range:
//             [l, j): < pivot;
//             [j, k): = pivot;
//             [k, r): > pivot
//
//    *************ooooooo***************
//    l            j      k              r
pair<int, int> partition(vector<int> &A, int l, int r, int pivotIndex){
    int v = A[pivotIndex];
    //mySwap(A[l], A[pivotIndex]);
    int i = l, j = l;
    for(; i < r; i++){
        if(A[i] < v){
            mySwap(A[i], A[j++]);
        }
    }

    int k;
    for(i = j, k = j; i < r; i++){
        if(A[i] == v){
            mySwap(A[i], A[k++]);
        }
    }
    return {j, k};
}

// This function selects the median of medians as pivot for the partition
// function.
//   1) it first divides every consecutive 5 elements into one group;
//   2) use insertion sort to select the median of the 5 elements in one group;
//   3) recursively call selectMM on the medians of all the groups to find the median;
// 
// In order to be able to recursively call selectMM, we need to swap those medians into
// the starting end of input vector. The following example illustrates the steps.
// For examples, we have 25 elements, with each "*" denoting one element.
// 1) divide into 5 groups, each with 5 elements.
//          ***** ***** ***** ***** *****
//          l                            r
// 2) find the median of each group, namely a, b, c, d, e.
//          **a** **b** **c** **d** **e**
//          l                            r
// 3) swap the medians to the starting end of input vector
//          abcde ***** ***** ***** *****
//          l    j
// 4) recursively call selectMM on range [l, j)
//
// Return: (l+j)/2, the pivot index (not the pivot)
static int selectPivot(vector<int> &A, int l, int r){
    if(r - l <= 5) {
        insertionSort(A, l, r);
        return (l + r) / 2;
    }
    int i, j, i5;
    for(i = l, j = l; i < r; i += 5, j++){
        i5 = min(i + 5, r);
        insertionSort(A, i, i5); 
        int medianIndex = (i + i5) / 2;
        mySwap(A[medianIndex], A[j]);
    }
    int mid = (l + j) / 2;
    return selectMM(A, l, j, mid);
}

// Just a simple insertion sorting algorithm.
static void insertionSort(vector<int> &A, int l, int r){
    for(int i = l + 1; i < r; i++){
        int j = i;
        while(j > l && A[j-1] > A[j]) {
            mySwap(A[j-1], A[j]);
            j--;
        }
    }
}

static inline void mySwap(int &lhs, int &rhs){
    int tmp = lhs;
    lhs = rhs;
    rhs = tmp;
}

#endif
