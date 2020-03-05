#ifndef Q_SORT_H
#define Q_SORT_H

#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <map>

void quickSort(int *p, int l, int r);
static inline void iSort(int *p, int l, int r);
int partition(int *p, int l, int r);
static inline int randomizedPartition(int *p, int l, int r);
static inline void mySwap(int &lhs, int &rhs);
std::pair<int, int> fastPartition(int *p, int l, int r);
static inline std::pair<int, int> fastRandomizedPartition(int *p, int l, int r);

void quickSort(int *p, int l, int r){
	if(r - l > 1){
		if(r - l < 32){
			iSort(p, l, r);
			return;
		}

		// one mistake made by me is
		// to partition into subarrays [l, mid - 1) and [mid + 1, r);
		// so element at mid is discarded. Jenus!
		//int mid = randomizedPartition(p, l, r);
		//quickSort(p, l, mid);
		//quickSort(p, mid + 1, r);
		
		auto mid = fastRandomizedPartition(p, l, r);
		quickSort(p, l, mid.first);
		quickSort(p, mid.second, r);
	}
}

void iSort(int *p, int l, int r){
	for(int i = l + 1; i < r; i++){
		int v = p[i], j = i - 1;
		while(j >= l && p[j] > v){
			p[j+1] = p[j];
			j--;
		}
		p[j+1] = v;
	}
}

// Use the value at position (r-1) as pivot.
static inline int partition(int *p, int l, int r){
	int v = p[r-1];
	int i = l;
	bool flag = true;
	for(int j = l; j < r - 1; j++){
		if(p[j] < v){
			mySwap(p[i], p[j]);
			i++;
		}

		// handle array that has the same values.
		if(flag && p[j] == v){
			mySwap(p[i], p[j]);
			i++;
		}
		flag = !flag;
	}
	mySwap(p[i], p[r-1]);
	return i;
}

// Really tricky.
// This function returns a pair [i, k) marking the range
// that has the same value as the pivot. So for the recursion,
// we only need to consider subarray [l, i) and [k, r).
std::pair<int, int> fastPartition(int *p, int l, int r){
	int v = p[r-1];
	// swap the pivot so the following `the else if(p[j] == v)` is excuted
	// in the first iteration.
	// Or alternatively, we could choose p[l] as pivot.
	mySwap(p[l], p[r-1]);
	int i = l, j = l, k = l;
	for(; j < r; j++){
		if(p[j] < v){
			mySwap(p[k], p[j]);
			mySwap(p[i], p[k]);
			i++;
			k++;
		}else if(p[j] == v){
			mySwap(p[k], p[j]);
			k++;
		}
	}
	return {i, k};
}

static inline std::pair<int, int> fastRandomizedPartition(int *p, int l, int r){
	int i = l + rand() % (r - l);
	mySwap(p[i], p[r-1]);
	return fastPartition(p, l, r);
}

static inline int randomizedPartition(int *p, int l, int r){
	int i = l + rand() % (r - l);
	mySwap(p[i], p[r-1]);
	return partition(p, l, r);
}

#endif
