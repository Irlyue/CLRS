#include<iostream>
#include<vector>
#include<string>
using namespace std;

void printVector(const vector<int> &a, string sep=" ", string end="\n"){
    int n = a.size();
    for(int i = 0; i < n; i++){
        cout << a[i] << (i+1 == n ? end : sep);
    }
}