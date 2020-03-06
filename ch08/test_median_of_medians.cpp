#include "median_of_medians.h"
#include <iostream>
#include <iterator>
#include <cstdlib>
#include <chrono>
using namespace std;

using FuncPtr = int(vector<int>&, int, int, int);

int selectUsingSort(vector<int> &A, int l, int r, int n){
    sort(A.begin() + l, A.begin() + r);
    return A[n];
}

int selectUsingNthElement(vector<int> &A, int l, int r, int n) {
    nth_element(A.begin() + l, A.begin() + n, A.begin() + r);
    return A[n];
}

void testFunc(FuncPtr pf, const vector<int> &A, int n, string desc){
    //cout << "A: ";
    //copy(A.begin(), A.end(), ostream_iterator<int>(cout, " "));
    //cout << endl;
    vector<int> B(A);
    auto start = chrono::high_resolution_clock::now();
    pf(B, 0, B.size(), n);
    auto finish = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = finish - start;
    //cout << "B: ";
    //copy(B.begin(), B.end(), ostream_iterator<int>(cout, " "));
    //cout << endl;
    printf("%s: %d(%fms)\n", desc.c_str(), B[n], elapsed.count()*1000);
}

int main(int argc, char* argv[]){
	if("help" == string(argv[1])){
		printf("main.exe {data type} {#elements} {k-th element} \n");
		return 0;
	}
    int total = atoi(argv[2]), n = atoi(argv[3]) % total;
    vector<int> A(total);

    srand(4);
	if("descending" == string(argv[1])){
		for(int i = 0; i < total; i++) A[i] = total - i - 1;
	}else if("ascending" == string(argv[1])){
		for(int i = 0; i < total; i++) A[i] = i;
	}else if("rand1" == string(argv[1])){
		cout << RAND_MAX << endl;
		for(int i = 0; i < total; i++) A[i] = 1L * rand() * rand() % RAND_MAX;
	}else if("rand2" == string(argv[1])){
		for(int i = 0; i < total; i++) A[i] = rand();
	}else if("rand3" == string(argv[1])){
		for(int i = 0; i < total; i++) A[i] = rand() % 100;
	}else if("rand4" == string(argv[1])){
		for(int i = 0; i < total; i++) A[i] = rand() % 10;
	}else{
		printf("Argument error!\n");
		return 1;
	}

    printf("total = %d, k = %d\n", total, n);

    testFunc(select, A, n, "select  ");
    testFunc(selectMM, A, n, "selectMM");
    testFunc(selectUsingSort, A, n, "Expected");
    testFunc(selectUsingNthElement, A, n, "Nth     ");

    // system("pause");
    return 0;    
}
