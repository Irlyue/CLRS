#ifndef TWO_SUM_H
#define TWO_SUM_H

#include <vector>
#include "merge_sort.h"
using namespace std;

/*
 * Exercise 2.3-7
 *
 * Since we sort the array before looping through it, the time complexity is
 * at least n*log(n). Then each iteration in the while loop moves either end
 * towards the other by 1, so it will terminate after at most n iterations.
 * Thus the overall complexity is still:
 *               n*log(n)
 *
 */
pair<int, int> twoSum(vector<int> &a, int sum, bool &found){
    mergeSort(a);
    int left = 0, right = a.size() - 1, t = 0;
    while(left < right){
        t = a[left] + a[right];
        if(t == sum){
            found = true;
            return {a[left], a[right]};
        // largest + smallest > sum means largest + others > sum
        // so we could eliminate the largest number
        }else if(t > sum){
            right--;
        // largest + smallest < sum means smallest + others < sum
        // so we could eliminate the smallest number
        }else{
            left++;
        }
    }
    // not found
    found = false;
    return {-1, -1};
}

#endif