#ifndef SELECTION_SORT_H
#define SELECTION_SORT_H

#include <vector>
using namespace std;

void selectionSort(vector<int> &a){
    size_t n = a.size();
    int min_j = -1, tmp = 0;
    for(int i = 0; i < n - 1; i++){
        min_j = i;
        // find the minimum index inside [i, n)
        for(int j = i; j < n; j++){
            if(a[min_j] > a[j])
                min_j = j;
        }
        // swap(i, min_j)
        tmp = a[i];
        a[i] = a[min_j];
        a[min_j] = tmp;
    }
}

#endif