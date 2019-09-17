//
// Created by Wenfeng on 2019/9/18.
//

#ifndef CLRS_INVERSION_PAIR_H
#define CLRS_INVERSION_PAIR_H

#include <vector>
#include <chrono>
using namespace std;


/*
 * Here we merge the elements in descending order.
 */
static int merge_descending_order(vector<int> &a, vector<int> &t, int left, int mid, int right){
    for(int i = left; i < right; i++) t[i] = a[i];

    int i = left, p = left, q = mid, count = 0;
    while(p < mid && q < right){
        // if (left > right), then (left > all others in the right) since
        // the sub-lists are in descending order
        if(t[p] > t[q]){
            count += right - q;
            a[i++] = t[p++];
        }else{
            a[i++] = t[q++];
        }
    }
    if(q == right) {
        while (i < right) a[i++] = t[p++];
    }
    return count;

}

static int calc_inversion_pair(vector<int> &a, vector<int> &t, int left, int right){
    if(right - left > 1){
        int mid = (left + right) >> 1;
        // count left
        int k1 = calc_inversion_pair(a, t, left, mid);
        // count right
        int k2 = calc_inversion_pair(a, t, mid, right);
        // count mixing
        int k3 = merge_descending_order(a, t, left, mid, right);
        return k1 + k2 + k3;
    }
    // if only one or zero element left, simply return 0
    return 0;
}


/*
 * This function actually makes a copy of the input vector to avoid any
 * inplace changes in the original vector. Unlike mergeSort that sorts
 * the vector in ascending order, here we should sort the elements in
 * descending order.
 *
 * It's important to pre-allocate the working memory, otherwise we'll
 * end up with allocating and freeing memory all the time, which is
 * really time-consuming. When n is 1e6, this implementation actually
 * takes about 0.6 second on my computer but the official implementation
 * takes about 2.7 seconds to finish.
 */
int calc_inversion_pair(vector<int> a){
    // setup working space
    vector<int> t(a.size());
    return calc_inversion_pair(a, t, 0, a.size());
}


/*
 * Implementation from the official CLRS repo that doesn't use pre-allocated
 * memory.
 */
void MergeWithCountInversions(vector<int>& v, int p, int q, int r, int& cnt) {
    int n1 = q - p, n2 = r - q;
    vector<int> L, R;
    for (int i = 0; i < n1; ++i) L.push_back(v[p + i]);
    for (int i = 0; i < n2; ++i) R.push_back(v[q + i]);
    L.push_back(INT_MAX); R.push_back(INT_MAX);

    int i = 0, j = 0;
    for (int k = p; k < r; ++k) {
        //   i      n1    j    n2
        //   |      |     |    |
        // |--------)|---------）
        // 若此时发生L[i] > R[j]，则由于L[0..i) < R[j]且R[0..j) < R[j]
        // 故与R[j]构成逆序对的元素是L[i..n1)
        if (L[i] <= R[j]) { v[k] = L[i]; i += 1; }
        else { v[k] = R[j]; j += 1; cnt += n1 - i;
//            { for (int u = i; u < n1; ++u) cout << "(" << L[u] << "," << R[j - 1] << "), "; } // Print Inversions
        }
    }
}

void MergeSort(vector<int>& v, int p, int r, int& cnt) {
    if (p >= r - 1) return;
    int q = (p + r) >> 1;
    MergeSort(v, p, q, cnt);
    MergeSort(v, q, r, cnt);
    MergeWithCountInversions(v, p, q, r, cnt);
}

int test_different_implementation() {
    int n = int(1e6);
    vector<int> v;
    generateRandomNumbers(v, n);
    int cntInversions = 0;

    auto start = chrono::high_resolution_clock::now();
    printf("%d\n", calc_inversion_pair(v));
    auto finish = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = finish - start;
    printf("==> My implementation\n");
    printf("Time elapsed: %.3f s\n", elapsed.count());

    start = chrono::high_resolution_clock::now();
    MergeSort(v, 0, (int)v.size(), cntInversions);
    finish = chrono::high_resolution_clock::now();
    cout << "Whose amount is: " << cntInversions << "." << endl;
    elapsed = finish - start;
    printf("==> CLRS implementation\n");
    printf("Time elapsed: %.3f s\n", elapsed.count());

    return 0;
}

#endif //CLRS_INVERSION_PAIR_H
