#include <vector>
#include <cmath>
#include <algorithm>
using namespace std;


vector<int> solve(vector<int> &array, int k){
	int n = array.size();

	// 1. find the median
	nth_element(array.begin(),
		array.begin() + n / 2,
		array.end());
	int pivot = *(array.begin() + n / 2);

	// 2. construct an extra array whose elements are
	// (|x - x[i]|, x[i]), where x is the median.
	vector<pair<int, int>> vec;
	vec.reserve(n);
	for(auto &x: array){
		vec.push_back({abs(x - pivot), x});
	}

	// 3. find the k+1 smallest element (the closet element
	// is itself).
	nth_element(vec.begin(),
		vec.begin() + k + 1,
		vec.end());

	// 4. construct the solution from range [1, k+1)
	vector<int> sol(k);
	transform(vec.begin() + 1,
		vec.begin() + k + 1,
		sol.begin(),
		[](pair<int, int> p){return p.second;});
	return sol;
}

int main(){
	vector<int> s = {0, 1, 2, 3, 14, 8};
	vector<int> sol = solve(s, 2);
	for(auto &x: sol)
		printf("%d ", x);
	printf("\n");
}
