#ifndef MERGE_SORT_H
#define MERGE_SORT_H

#include <vector>
using namespace std;

static void merge(vector<int> &a, vector<int> &t, int l, int m, int r){
    // 1) copy to working memory
    for(int i = l; i < r; i++) t[i] = a[i];

    // 2) compare two sub-arrays
    int i = l, j = m, p = i;
    while(i < m && j < r){
        if(t[i] < t[j])
            a[p++] = t[i++];
        else
            a[p++] = t[j++];
    }

    // 3) check if extra copying is needed
    // if j reaches the end, there are elements remaining in the left side.
    if(j == r){
        while(p < r) a[p++] = t[i++];
    }

    // if i reaches the end, no further job is needed since these numbers are
    // already in position.
}

static void mergeSort(vector<int> &a, vector<int> &t, int l, int r){
    // if there are more than 1 elements left, split and conquer.
    if(r - l > 1){
        int m = (l + r) / 2;
        mergeSort(a, t, l, m);
        mergeSort(a, t, m, r);
        merge(a, t, l, m, r);
    }
}

void mergeSort(vector<int> &a){
    // Pre-allocate working memory
    vector<int> t(a.size());
    mergeSort(a, t, 0, a.size());
}


#endif