#ifndef INSERTION_SORT_H
#define INSERTION_SORT_H
#include<vector>
using namespace std;

void insertionSort(vector<int> &a){
	int key = 0, j = 0;
	size_t n = a.size();
	for(int i = 1; i < n; i++){
		key = a[i];
		j = i - 1;
		// find the correct position
		while(j >= 0 && key < a[j]){
			a[j+1] = a[j];
			j--;
		}
		// drop in the value
		a[j+1] = key;
	}
}

#endif
