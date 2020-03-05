#include "heap_sort.h"
#include <vector>
#include <algorithm>
using namespace std;


class PriorityQueue{
public:
	void insert(int x);
	int max() const;
	int extractMax();
	bool increaseKey(int i, int newKey);
	void clear();
	bool isEmpty() const;
	void remove(int i);
	int size() const;
private:
	vector<int> heap;
};

// 1. increase the heap size by 1;
// 2. place (-infinity) in the last slot;
// 3. increase the value in the lsat slot to target value x;
void PriorityQueue::insert(int x){
	heap.push_back(INT_MIN);
	increaseKey(size() - 1, x);
}

int PriorityQueue::max() const {
	return heap[0];
}

int PriorityQueue::size() const {
	return heap.size();
}

// Remove the maximum value and return it.
//
// 1. save the maximum value for return
// 2. swap the values in the first and last slots.
// 3. decrease the size of the heap by 1
// 4. call maxHeapify to maintain heap property.
int PriorityQueue::extractMax() {
	int ret = heap[0];
	mySwap(heap[0], heap[size() - 1]);
	heap.pop_back();

	if(size() > 0)
		maxHeapify(&heap[0], 0, size());
	return ret;
}

// Return
// ------
//     False if heap[i] > newKey else true. Namely return 
// true if value in slot `i` is succussfully raised to 
// value `newKey`.
bool PriorityQueue::increaseKey(int i, int newKey){
	if(heap[i] > newKey)
		return false;

	// inspired by the shifting operation from insertion sort
	// shift down all the smaller ancester nodes.
	int p = parent(i);
	while(i > 0 && newKey > heap[p]){
		heap[i] = heap[p];
		i = p;
		p = parent(i);
	}
	// remember to place back the target value.
	heap[i] = newKey;

	return true;
}

// Not tested.
// 1. swap the values in the i-th and last slots.
// 2. decrease the size of the heap by 1.
// 3. call `maxHeapify` at slot i.
void PriorityQueue::remove(int i){
	mySwap(heap[i], heap[size() - 1]);
	heap.pop_back();

	maxHeapify(&heap[0], i, size());
}

void PriorityQueue::clear(){
	heap.clear();
}

bool PriorityQueue::isEmpty() const {
	return heap.size() == 0;
}


void testPriorityQueue(){
	bool flag;
	int success = 0, total = 0;
	PriorityQueue q;

	// [0, 1, ..., 9]
	vector<int> array1 = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	vector<int> sol1 = {9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
	q.clear();
	for(auto &x: array1) q.insert(x);
	vector<int> mySol1;
	while(!q.isEmpty()) mySol1.push_back(q.extractMax());
	//for(auto &x: mySol1) printf("%d ", x);
	flag = (mySol1 == sol1);
	success += flag; total++;
	printf("%s\n", (flag ? "Passed" : "Failed"));

	// [9, 8, ..., 0]
	vector<int> array2 = {9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
	vector<int> sol2 = {9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
	q.clear();
	for(auto &x: array2) q.insert(x);
	vector<int> mySol2;
	while(!q.isEmpty()) mySol2.push_back(q.extractMax());
	flag = (array2 == sol2);
	success += flag; total++;
	printf("%s\n", (flag ? "Passed" : "Failed"));

	// [0, 1, ..., n-1]
	int n = 1e6;
	vector<int> array3(n);
	vector<int> sol3(n);
	for(int i = 0; i < n; i++) {
		array3[i] = i;
		sol3[i] = n - i - 1;
	}
	q.clear();
	for(auto &x: array3) q.insert(x);
	vector<int> mySol3;
	while(!q.isEmpty()) mySol3.push_back(q.extractMax());
	flag = (mySol3 == sol3);
	success += flag; total++;
	printf("%s\n", (flag ? "Passed" : "Failed"));

	// [Random numbers]
	vector<int> array4(n);
	vector<int> sol4(n);
	for(int i = 0; i < n; i++) {
		sol4[i] = array4[i] = rand();
	}
	sort(sol4.begin(), sol4.end(), greater<int>());
	q.clear();
	for(auto &x: array4) q.insert(x);
	vector<int> mySol4;
	while(!q.isEmpty()) mySol4.push_back(q.extractMax());
	flag = (mySol4 == sol4);
	success += flag; total++;
	printf("%s\n", (flag ? "Passed" : "Failed"));

	printf("Test %s [%d/%d]!\n", (success == total ? "passed" : "failed"),
		success, total);
}


int main(){
	testPriorityQueue();
}
