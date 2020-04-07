#include <iostream>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <chrono>

using namespace std;


// (left, right, sum)
struct Solution{
	int left, right, sum;
	bool operator==(const Solution &rhs) const {
		return left == rhs.left && right == rhs.right && sum == rhs.sum;
	}
};
using FunctionPtr = Solution (*)(const vector<int>&, int, int);

Solution findMaxCrossingSubarray(const vector<int> &vec, int left, int mid, int right){
	int leftSum = INT_MIN, leftIndex = -1;

	int sum = 0;
	for(int i = mid - 1; i >= left; i--){
		sum += vec[i];
		if(leftSum < sum){
			leftSum = sum;
			leftIndex = i;
		}
	}

	int rightSum = INT_MIN, rightIndex = -1;
	sum = 0;
	for(int i = mid; i < right; i++){
		sum += vec[i];
		if(rightSum < sum){
			rightSum = sum;
			rightIndex = i;
		}
	}
	return {leftIndex, rightIndex, leftSum + rightSum};
}

// Divide and conquer, O(nlogn)
// Range denoted as [left, right), namely the right index is excluded
Solution findMaximumSubarray(const vector<int> &vec, int left, int right){
	if(right - left == 1)
		return {left, right, vec[left]};

	int mid = (left + right) / 2;

	auto sol1 = findMaximumSubarray(vec, left, mid);
	auto sol2 = findMaximumSubarray(vec, mid, right);
	auto sol3 = findMaxCrossingSubarray(vec, left, mid, right);

	if(sol1.sum > sol2.sum && sol1.sum > sol3.sum)
		return sol1;
	else if(sol2.sum > sol1.sum && sol2.sum > sol3.sum)
		return sol2;
	else
		return sol3;
}

// Brute force O(n^2)
// Range denoted as [left, right), namely the right index is excluded
Solution findMaximumSubarrayBF(const vector<int> &vec, int left, int right){
	int sum;
	Solution sol = {-1, -1, INT_MIN};
	for(int i = left; i < right; i++){
		sum = 0;
		for(int j = i; j < right; j++){
			sum += vec[j];
			if(sum > sol.sum){
				sol = {i, j, sum};
			}
		}
	}
	return sol;
}

// Linear O(n)
// Range denoted as [left, right), namely the right index is excluded
Solution findMaximumSubarrayLinear(const vector<int> &vec, int left, int right){
	int sum = 0, leftIndex = -1, rightIndex = -1;
	Solution sol = {0, 0, INT_MIN};

	for(int i = left; i < right; i++){
		rightIndex = i;
		if(sum > 0){
			sum += vec[i];
		}else{
			leftIndex = i;
			sum = vec[i];
		}

		if(sum > sol.sum){
			sol = {leftIndex, rightIndex, sum};
		}
	}
	return sol;
}

pair<Solution, double> timedSolution(FunctionPtr f, const vector<int> &vec, int left, int right){
	auto start = chrono::high_resolution_clock::now();
	auto sol = f(vec, left, right);
	auto finish = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = finish - start;
	return {sol, elapsed.count()};
}

pair<int, int> testFindMaximumSubarray(){
	bool flag;
	int success = 0, total = 0;

	vector<int> array1 = {13, -3, -25, 20, -3, -16, -23, 18, 20, -7, 12, -5, -22, 15, -4, 7};
	Solution sol1 = {7, 10, 43};
	flag = findMaximumSubarray(array1, 0, array1.size()) == sol1;
	success += flag;
	total++;

	flag = findMaximumSubarrayBF(array1, 0, array1.size()) == sol1;
	success += flag;
	total++;

	flag = findMaximumSubarrayLinear(array1, 0, array1.size()) == sol1;
	success += flag;
	total++;

	int ns[] = {10, 100, 1000, 10000, 100000};
	srand(2);
	vector<int> array2;
	for(int &n: ns){
		array2.reserve(n);
		array2.clear();
		for(int i = 0; i < n; i++){
			array2.push_back(rand() % RAND_MAX - RAND_MAX / 2);
		}

		auto sol1 = timedSolution(findMaximumSubarrayBF, array2, 0, n);
		auto sol2 = timedSolution(findMaximumSubarray, array2, 0, n);
		auto sol3 = timedSolution(findMaximumSubarrayLinear, array2, 0, n);
		flag = (sol1.first == sol2.first) && (sol2.first == sol3.first);

		printf("n = %d, BF %.3f s, Divide-and-conquer %.3f s, Linear %.3f\n",
			n, sol1.second, sol2.second, sol3.second);

		success += flag;
		total++;
	}

	return {success, total};
}


int main(){
	vector<int> nums = {-1,-6,-9,4,-8,5,-4,2,-1,1,-8,0,1,3,1};
	auto sol = findMaximumSubarray(nums, 0, nums.size());
	printf("%d %d %d \n", sol.left, sol.right, sol.sum);
	auto info = testFindMaximumSubarray();
	if(info.first == info.second){
		printf("Test passed [%d/%d]!\n", info.first, info.second);
	}else{
		printf("Test failed [%d/%d]!\n", info.first, info.second);
	}
}
