#include <string>
#include <vector>
#include <algorithm>
using namespace std;

class Automaton {
public:
    Automaton(const string &pat);
    vector<int> process(const string &text);
    int sigma(int q, int a);
    int getAcceptState() const;

private:
    void preprocess(const string &pat);
    bool match(const string &lhs, int i, const string &rhs, int j, int k);
    vector<vector<int>> mp;
    int m;
};

int Automaton::sigma(int q, int a){
    return mp[q][a];
}

int Automaton::getAcceptState() const {
    return m;
}

Automaton::Automaton(const string &pat) : m(pat.size()) {
    mp.resize(m + 1);
    for (auto &x : mp) x.resize(256);
    preprocess(pat);
}

// O(m^3*256)
// Build the string matching automaton
void Automaton::preprocess(const string &pat) {
    string text(pat.size() + 1, ' ');
    for (int q = 0; q <= m; q++) {
        for (int j = 0; j < 256; j++) {
            text[q] = j;
            int k = min(m, q + 1);
            while (!match(pat, 0, text, q+1-k, k)) k--;
            mp[q][j] = k;
        }
        if (q < m) text[q] = pat[q];
    }
}

// Test whehter lhs[i:i+k] == rhs[j:j+k]
bool Automaton::match(const string &lhs, int i, const string &rhs, int j, int k) {
    for (int cnt = 0; cnt < k; cnt++) {
        if (lhs[i++] != rhs[j++]) return false;
    }
    return true;
}

vector<int> Automaton::process(const string &text) {
    vector<int> indices;
    int n = text.size();
    int q = 0;
    for (int i = 0; i < n; i++) {
        q = mp[q][text[i]];
        if (q == m) {
            indices.push_back(i - m + 1);
        }
    }
    return indices;
}
