#include <string>
#include <vector>
#include <iostream>
#include <iterator>
using namespace std;

// Return
// ------
//     pi: pi[q] indicates the next starting position when characters up
// to position `q` (including `q`) are already matched but pi[q+1] doesn't
// match with the pattern.
//
// -1 -1 -1  0  1
//  a  b  c  a  b
//  0  1  2  3  4
// Say when all characters are mached by now, we should start at position
// pi[4]+1 = 2.
vector<int> calcPrefix(const string &pat){
	vector<int> pi(pat.size());
	pi[0] = -1;
	int q = -1, m = pat.size();
	for(int i = 1; i < m; i++){
		while(q != -1 && pat[i] != pat[q+1]) q = pi[q];

		if(pat[i] == pat[q+1]) q = q + 1;

		pi[i] = q;
	}
	return pi;
}

vector<int> kmpMatch(const string &text, const string &pat){
	auto pi = calcPrefix(pat);
	vector<int> indices;
	int n = text.size(), m = pat.size();

	int q = -1;
	for(int i = 0; i < n; i++){
		while(q != -1 && text[i] != pat[q+1]) q = pi[q];

		if(text[i] == pat[q+1]) q = q + 1;

		if(q == m - 1) {
			indices.push_back(i-m+1);
			q = pi[q];
		}
	}
	return indices;
}

