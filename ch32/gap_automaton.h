#include <vector>
#include <string>
#include <algorithm>
#include "string_automaton.h"
using namespace std;

class IGapAutomaton{
public:
    virtual vector<int> process(const string &text) = 0;
};

// Build one single automaton for patterns with gap character.
class GapAutomatonOne: public IGapAutomaton {
public:
    GapAutomatonOne(const string &pat);
    vector<int> process(const string &text);

private:
    void preprocess(const string &pat);
    bool match(const string &lhs, int i, const string &rhs, int j, int k);

    vector<vector<int>> mp;
    // used for reconstructing the result.
    vector<int> states;

    int finalState;
    int gapCount;
};

// Use gap character to split the input pattern into serveral sub-patterns
// and then build normal string automatons for each sub-pattern.
// 
// During matching, we start with the first automaton and switch to the
// next one whenever the current automaton encounters an accept state.
class GapAutomatonTwo: public IGapAutomaton{
public:
    GapAutomatonTwo(const string &pat);
    vector<int> process(const string &text);

private:
    void preprocess(const string &pat);
    vector<Automaton> matchers;
};

GapAutomatonTwo::GapAutomatonTwo(const string &pat){
    preprocess(pat);
}

void GapAutomatonTwo::preprocess(const string &pat){
    int last = -1;
    for(int i = 0; i < pat.size(); i++){
        if(pat[i] == '*'){
            matchers.emplace_back(pat.substr(last + 1, i - last - 1));
            last = i;
        }
    }
    // don't forget to build the last automaton
    if(last != pat.size() - 1) matchers.emplace_back(pat.substr(last+1, pat.size()));
}

vector<int> GapAutomatonTwo::process(const string &text){
    int matcherIndex = 0, q = 0;
    vector<int> indices;
    for(int i = 0; i < text.size(); i++){
        q = matchers[matcherIndex].sigma(q, text[i]);
        if(q == matchers[matcherIndex].getAcceptState()){
            for(int k = i - matchers[matcherIndex].getAcceptState() + 1; k <= i; k++) indices.push_back(k);
            q = 0;
            matcherIndex++;
            if(matcherIndex == matchers.size()) return indices;
        }
    }
    return {};
}

GapAutomatonOne::GapAutomatonOne(const string &pat) {
    gapCount = count(pat.cbegin(), pat.cend(), '*');
    finalState = pat.size() - gapCount;
    mp.resize(finalState+1);   // "+1": in case the pattern is just "*"
    states.push_back(0);
    for (auto &x : mp) x.resize(256);
    preprocess(pat);
}

void GapAutomatonOne::preprocess(const string &pat) {
    int n = pat.size();
    // `lastGap` records the last index of the gap character
    // and `lastState` records the corresponding state of last gap character.
    int q = 0, lastGap = -1, lastState = 0;
    string text(n + 1, ' ');
    for (int i = 0; i < n; i++) {
        if (pat[i] == '*') {
            states.push_back(q);
            lastGap = i;
            lastState = q;
            continue;
        }
        for (int j = 0; j < 256; j++) {
            text[i - lastGap - 1] = j;
            int k = i - lastGap;
            while (!match(pat, lastGap + 1, text, i - lastGap - k, k)) k--;
            mp[q][j] = k + lastState;
        }
        text[i - lastGap - 1] = pat[i];
        q += (pat[i] != '*');
    }
    states.push_back(finalState);
}

bool GapAutomatonOne::match(const string &lhs, int i, const string &rhs, int j, int k) {
    for (int cnt = 0; cnt < k; cnt++) {
        if (lhs[i++] != rhs[j++]) return false;
    }
    return true;
}

// Pretty much like the `process` in the normal automaton
vector<int> GapAutomatonOne::process(const string &text) {
    int n = text.size();
    int q = 0, cnt = 1;
    vector<int> indices;
    for (int i = 0; i < n; i++) {
        q = mp[q][text[i]];
        if(states[cnt] == q) {
			int len = states[cnt] - states[cnt-1];
            for(int k = i-len+1; k <= i; k++) indices.push_back(k);

            cnt++;
			if(q == finalState) return indices;
        }
    }
    return {};
}
