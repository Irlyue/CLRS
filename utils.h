#include<iostream>
#include<vector>
#include<string>
#include<cstdlib>
using namespace std;

void printVector(const vector<int> &a, string sep=" ", string end="\n"){
    int n = a.size();
    for(int i = 0; i < n; i++){
        cout << a[i] << (i+1 == n ? end : sep);
    }
}

void generateRandomNumbers(vector<int> &a, int n, int seed=2){
    srand(seed);
    a.resize(n);
    for(int i = 0; i < n; i++){
        a[i] = rand();
    }
}

/*
 * Check if the given array is in ascending order.
 */
bool isSorted(const vector<int> &a){
    size_t n = a.size();
    for(int i = 1; i < n; i++){
        if(a[i-1] > a[i])
            return false;
    }
    return true;
}

void generateRange(vector<int> &a, int n){
    a.resize(n);
    for(int i = 0; i < n; i++) a[i] = i;
}