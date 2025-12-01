#include <bits/stdc++.h>
using namespace std;

map<char, vector<string>> G;
map<char, set<char>> FIRST, FOLLOW;
set<char> nonterms;
char start;

bool isNt(char c){ return c >= 'A' && c <= 'Z'; }

set<char> getFirst(char X){
    if (!isNt(X)) return {X};
    if (!FIRST[X].empty()) return FIRST[X];
    for (auto &prod : G[X]) {
        if (prod == "e") { FIRST[X].insert('e'); continue; }
        bool allE = true;
        for (char s : prod) {
            auto f = getFirst(s);
            for (char c : f) if (c != 'e') FIRST[X].insert(c);
            if (f.count('e')) continue;
            allE = false; break;
        }
        if (allE) FIRST[X].insert('e');
    }
    return FIRST[X];
}

void getFollow(char B){
    if (!FOLLOW[B].empty() && B != start) return; 
    for (auto &rule : G) {
        char A = rule.first;
        for (auto &prod : rule.second) {
            for (int i = 0; i < (int)prod.size(); ++i) if (prod[i] == B) {
                bool allE = true;
                for (int j = i+1; j < (int)prod.size(); ++j) {
                    auto f = getFirst(prod[j]);
                    for (char c : f) if (c != 'e') FOLLOW[B].insert(c);
                    if (f.count('e')) continue;
                    allE = false; break;
                }
                if (allE && A != B) {
                    if (FOLLOW[A].empty()) getFollow(A);
                    for (char c : FOLLOW[A]) FOLLOW[B].insert(c);
                }
            }
        }
    }
}

int main(){
    int n; cout << "Enter number of productions: "; if (!(cin >> n)) return 0;
    cout << "Enter productions (E->TR style):\n";
    for (int i=0;i<n;++i){
        string s; cin >> s;
        char L = s[0]; string R = s.substr(3);
        G[L].push_back(R); nonterms.insert(L);
        if (i==0) start = L;
    }
    for (char A : nonterms) getFirst(A);
    FOLLOW[start].insert('$');
    for (char A : nonterms) getFollow(A);
    cout << "\nFIRST sets:\n";
    for (auto &p : FIRST){
        cout << "FIRST("<<p.first<<") = { ";
        for (char c : p.second) cout << c << ' ';
        cout << "}\n";
    }
    cout << "\nFOLLOW sets:\n";
    for (auto &p : FOLLOW){
        cout << "FOLLOW("<<p.first<<") = { ";
        for (char c : p.second) cout << c << ' ';
        cout << "}\n";
    }
}

/*
Enter number of productions: 8
Enter productions (E->TR style):
E->TR
R->+TR
R->e
T->FY
Y->*FY
Y->e
F->(E)
F->i
*/
