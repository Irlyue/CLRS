#include "gap_automaton.h"
#include <iostream>
using namespace std;

void testFunc(IGapAutomaton *pga, const string &text){
    auto p = pga->process(text);
    string exout(text.size(), ' ');
    for (auto &x : p) exout[x] = '*';
    cout << text << endl;
    cout << exout << endl;
}

int main() {
    string text, pat;
    while(cin >> text >> pat){
        testFunc(new GapAutomatonOne(pat), text);
        testFunc(new GapAutomatonTwo(pat), text);
    }
    system("pause");
    return 0;
}
