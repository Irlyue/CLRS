#include <vector>
#include <algorithm>
#include <cassert>
#include <chrono>
using namespace std;

using FunctionPtr = int(*)(vector<int>&, vector<int>&);

pair<int, double> timedSolution(FunctionPtr f, vector<int> &A, vector<int> &B){
	auto start = chrono::high_resolution_clock::now();
	int sol = f(A, B);
	auto finish = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = finish - start;
	return {sol, elapsed.count()};
}

// Simplest solution is just to sort all the numbers 
// and obtain the median.
int solveSlow(vector<int> &A, vector<int> &B){
	vector<int> AB = A;
	AB.insert(AB.end(), B.begin(), B.end());

	sort(AB.begin(), AB.end());
	return AB[AB.size() / 2 - 1];
}

int _solve(vector<int> &A, int al, int ar,
		vector<int> &B, int bl, int br){
	if(ar - al <= 20 && br - bl <= 20){
		vector<int> nA(A.begin() + al, A.begin() + ar);
		vector<int> nB(B.begin() + bl, B.begin() + br);
		return solveSlow(nA, nB);
	}

	// If we choose ma = (al + d/k), then there're (d/k+1) numbers
	// in the first half, the second half will contain (d - d/k - 1)
	// numbers. You could experiment with the value of k. Remember to
	// also tweak the value of base case above if you want to use large
	// value of k.
	int d = ar - al, k = 2;
	int ma = al + d / k, mb = bl + (d - d / k - 1);
	if(A[ma] < B[mb])
		return _solve(A, ma, ar, B, bl, mb + 1);
	return _solve(A, al, ma + 1, B, mb, br);
}

int solve(vector<int> &A, vector<int> &B){
	//assert(A.size() == B.size());
	return _solve(A, 0, A.size(), B, 0, B.size());
}

void fill(vector<int> &v, int n, int seed){
	srand(seed);
	v.clear();
	v.resize(n);
	transform(v.begin(), v.end(), v.begin(), [](int){return rand() * rand();});
	sort(v.begin(), v.end());
}

void print(const vector<int> &v){
	for(auto &x: v)
		printf("%d ", x);
	printf("\n");
}

void testMethods(){
	int n = 1;
	srand(2);
	vector<int> A, B;
	bool flag = true;
	for(int i = 0; i < 22; i++){
		fill(A, n, i);
		fill(B, n + rand(), i);
		auto sol1 = timedSolution(solveSlow, A, B);
		auto sol2 = timedSolution(solve, A, B);
		if(sol1.first != sol2.first) flag = false;
		printf("%s slow sol = %d(%.3fs), fast sol = %d(%.3fs), n = %d\n",
			(sol1.first == sol2.first ? "Passed" : "Failed"), sol1.first, sol1.second,
			sol2.first, sol2.second, n);

		n = n * 2 + rand() % 3;
	}
	printf("%s\n", flag ? "All passed!" : "Error!");
}

int main(){
	testMethods();
}
