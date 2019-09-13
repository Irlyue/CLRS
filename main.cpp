#include "utils.h"
#include "insertion_sort.h"
#include "selection_sort.h"
#include "merge_sort.h"

int main(){
	vector<int> a{4,3,2,1,17};
	printVector(a, ",");
	mergeSort(a);
	printVector(a);
	return 0;
}
