#include "string_automaton.h"
#include "kmp.h"
#include <iostream>
using namespace std;

using FuncPtr = vector<int>(const string &, const string &);

vector<int> matchSlow(const string &text, const string &pat){
	vector<int> indices;
	int nt = text.size(), np = pat.size();
	if(nt < np) return {};

	for(int i = 0; i < nt - np + 1; i++){
		if(text.substr(i, np) == pat) indices.push_back(i);
	}
	return indices;
}

vector<int> matchAutomaton(const string &text, const string &pat){
	return Automaton(pat).process(text);
}

void testFunc(FuncPtr pf, string text, string pat, string desc){
	cout << "==> " << desc << endl;
	string out(text.size(), ' ');
	cout << text << endl;
	for(auto x: pf(text, pat)) out[x] = '*';
	cout << out << endl;
}

int main(){
	string text, pat;
	while(cin>>text>>pat){
		testFunc(matchSlow, text, pat, "Expected");
		testFunc(matchAutomaton, text, pat, "Automaton");
		testFunc(kmpMatch, text, pat, "KMP");
	}
	return 0;
}
