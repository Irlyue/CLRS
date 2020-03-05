#include "heap_sort.h"
#include "qsort.h"
#include <vector>
#include <chrono>
#include <cstdlib>
#include <algorithm>
using namespace std;

using FunctionPtr = void(*)(int*, int, int);

void fastQSort(int *p, int l, int r){
	sort(p, p + r);
}

double timedSolution(FunctionPtr f, int *p, int left, int right){
	auto start = chrono::high_resolution_clock::now();
	f(p, left, right);
	auto finish = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = finish - start;
	return elapsed.count();
}

bool isSorted(int *p, int n){
	for(int i = 1; i < n; i++){
		if(p[i-1] > p[i])
			return false;
	}
	return true;
}


void testSortMethods(int n){
	bool flag;
	int success = 0, total = 0;

	// [0, 1, ..., n-1]
	printf("==> Array [0, 1, ..., n-1]\n");
	vector<int> array1(n);
	for(int i = 0; i < n; i++) array1[i] = i;
	double elapsed = timedSolution(heapSort, &array1[0], 0, n);
	flag = isSorted(&array1[0], n);
	success += flag; total++;
	printf("Heap sort %.3fs, %s\n", elapsed, (flag ? "sorted" : "not sorted"));

	for(int i = 0; i < n; i++) array1[i] = i;
    elapsed = timedSolution(quickSort, &array1[0], 0, n);
	flag = isSorted(&array1[0], n);
	printf("Quick sort %.3fs, %s\n", elapsed, (flag ? "sorted" : "not sorted"));
	success += flag; total++;

	// pretty slow for quick sort.
	// [n-1, n-2, ..., 0]
	printf("==> Array [n-1, n-2, ..., 0]\n");
	for(int i = 0; i < n; i++) array1[i] = n - i - 1;
	elapsed = timedSolution(heapSort, &array1[0], 0, n);
	flag = isSorted(&array1[0], n);
	printf("Heap sort %.3fs, %s\n", elapsed, (flag ? "sorted" : "not sorted"));
	success += flag; total++;

	for(int i = 0; i < n; i++) array1[i] = n - i - 1;
	elapsed = timedSolution(quickSort, &array1[0], 0, n);
	flag = isSorted(&array1[0], n);
	printf("Quick sort %.3fs, %s\n", elapsed, (flag ? "sorted" : "not sorted"));
	success += flag; total++;

	// [k] * n
	printf("==> Array [k]*n\n");
	array1.assign(n, 1024);
	elapsed = timedSolution(heapSort, &array1[0], 0, n);
	flag = isSorted(&array1[0], n);
	success += flag; total++;
	printf("Heap sort %.3fs, %s\n", elapsed, (flag ? "sorted" : "not sorted"));

	array1.assign(n, 1024);
	elapsed = timedSolution(quickSort, &array1[0], 0, n);
	flag = isSorted(&array1[0], n);
	success += flag; total++;
	printf("Quick sort %.3fs, %s\n", elapsed, (flag ? "sorted" : "not sorted"));


	// [random numbers]
	printf("==> Array [random numbers]\n");
	for(int i= 0; i < n; i++) array1[i] = rand();
	elapsed = timedSolution(heapSort, &array1[0], 0, n);
	flag = isSorted(&array1[0], n);
	success += flag; total++;
	printf("Heap sort %.3fs, %s\n", elapsed, (flag ? "sorted" : "not sorted"));

	for(int i= 0; i < n; i++) array1[i] = rand();
	elapsed = timedSolution(quickSort, &array1[0], 0, n);
	flag = isSorted(&array1[0], n);
	success += flag; total++;
	printf("Quick sort %.3fs, %s\n", elapsed, (flag ? "sorted" : "not sorted"));

	for(int i= 0; i < n; i++) array1[i] = rand();
	elapsed = timedSolution(fastQSort, &array1[0], 0, n);
	flag = isSorted(&array1[0], n);
	success += flag; total++;
	printf("Quick sort %.3fs, %s\n", elapsed, (flag ? "sorted" : "not sorted"));

	printf("Test %s [%d/%d]!\n", (success == total ? "passed" : "failed"),
		success, total);
}


int main(int, char *argv[]){
	testSortMethods(atoi(argv[1]));
	return 0;
}
