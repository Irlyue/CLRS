#include <vector>
#include <chrono>
#include <map>
#include <algorithm>
#include "qsort.h"
using namespace std;

using FunctionPtr = int(*)(int*, int, int);
static inline void mySwap(int &lhs, int &rhs){
	int tmp = lhs;
	lhs = rhs;
	rhs = tmp;
}

// Random shuffle an array n times.
void randomShuffle(int *p, int size, int n){
	int left, right;
	for(int i = 0; i < n; i++){
		left = (rand() / 2 * rand()) % size;
		right = (rand() / 2 * rand()) % size;
		mySwap(p[left], p[right]);
	}
}

int _randomizedSelect(int *p, int l, int r, int i){
	if(l == r - 1)
		return p[l];
	int mid = randomizedPartition(p, l, r);
	if(i == mid)
		return p[mid];
	else if(i < mid)
		return _randomizedSelect(p, l, mid, i);
	else
		return _randomizedSelect(p, mid + 1, r, i);
}

int randomizedSelect(int *p, int n, int i){
	// introduce some randomness to handle inputs like [n-1, n-2, ..., 0]
	randomShuffle(p, n, n / 10);
	return _randomizedSelect(p, 0, n, i);
}

pair<int, double> timedSolution(FunctionPtr f, int *p, int n, int i){
	auto start = chrono::high_resolution_clock::now();
	int sol = f(p, n, i);
	auto finish = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = finish - start;
	return {sol, elapsed.count()};
}

void testMethods(int n){
	vector<int> array;
	array.reserve(n);

	array.clear();
	for(int i = 0; i < n; i++) array.push_back(i);
	int sol = 1024;
	auto mysol = timedSolution(randomizedSelect, &array[0], n, sol);
	bool flag = sol == mysol.first;
	printf("%s!(%.3fs)\n", flag ? "Passed" : "Failed", mysol.second);

	array.clear();
	for(int i = 0; i < n; i++) array.push_back(n - i - 1);
	sol = 1024;
	mysol = timedSolution(randomizedSelect, &array[0], n, sol);
	flag = sol == mysol.first;
	printf("%s!(%.3fs)\n", flag ? "Passed" : "Failed", mysol.second);

	array.clear();
	for(int i = 0; i < n; i++) array.push_back(1234);
	sol = 1234;
	mysol = timedSolution(randomizedSelect, &array[0], n, 11111);
	flag = sol == mysol.first;
	printf("%s!(%.3fs)\n", flag ? "Passed" : "Failed", mysol.second);

	array.clear();
	for(int i = 0; i < n; i++) array.push_back(rand());
	vector<int> copyed(array);
	sort(copyed.begin(), copyed.end());
	sol = copyed[1024];
	mysol = timedSolution(randomizedSelect, &array[0], n, 1024);
	flag = sol == mysol.first;
	printf("%s!(%.3fs)\n", flag ? "Passed" : "Failed", mysol.second);
}

int main(int, char *argv[]){
	testMethods(atoi(argv[1]));
}
