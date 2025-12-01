#include <bits/stdc++.h>
using namespace std;

map<char, vector<string>> G;
map<char, set<char>> first, follow;
map<char, map<char, string>> table;
set<char> nt, terminals;
char start;

bool isNt(char ch) { return ch >= 'A' && ch <= 'Z'; }

set<char> getFirst(char ch) {
    if (!isNt(ch)) return {ch};
    if (!first[ch].empty()) return first[ch];
    for (auto &prod : G[ch]) {
        if (prod == "e") { first[ch].insert('e'); continue; }
        bool allE = true;
        for (int i = 0; i < (int)prod.size(); ++i) {
            char sym = prod[i];
            set<char> f = getFirst(sym);
            for (char c : f) if (c != 'e') first[ch].insert(c);
            if (f.count('e')) continue;
            allE = false;
            break;
        }
        if (allE) first[ch].insert('e');
    }
    return first[ch];
}

// Assumes grammar is non-recursive (as you said), but correctly handles whole suffix
void getFollow(char ch) {
    for (auto &rule : G) {
        char lhs = rule.first;
        for (auto &prod : rule.second) {
            for (int i = 0; i < (int)prod.size(); ++i) {
                if (prod[i] != ch) continue;
                // compute FIRST of suffix prod[i+1 .. end]
                bool allE = true;
                for (int j = i + 1; j < (int)prod.size(); ++j) {
                    char sym = prod[j];
                    set<char> f = getFirst(sym);
                    for (char c : f) if (c != 'e') follow[ch].insert(c);
                    if (f.count('e')) continue;
                    allE = false;
                    break;
                }
                if (allE && lhs != ch) {
                    // add FOLLOW(lhs) to FOLLOW(ch)
                    if (follow[lhs].empty()) getFollow(lhs);
                    for (char c : follow[lhs]) follow[ch].insert(c);
                }
            }
        }
    }
}

void constructParsingTable() {
    // Build table and detect conflicts
    for (auto &entry : G) {
        char A = entry.first;
        for (auto &prod : entry.second) {
            set<char> f;
            if (prod == "e") {
                f = follow[A];
            } else {
                bool allE = true;
                for (int i = 0; i < (int)prod.size(); ++i) {
                    set<char> tmp = getFirst(prod[i]);
                    for (char c : tmp) if (c != 'e') f.insert(c);
                    if (tmp.count('e')) continue;
                    allE = false;
                    break;
                }
                if (allE) { // entire RHS can be epsilon
                    for (char c : follow[A]) f.insert(c);
                }
            }
            for (char a : f) {
                if (a == 'e') continue;
                if (table[A].count(a) && table[A][a] != prod) {
                    cerr << "LL(1) conflict: M[" << A << "," << a << "] already has rule "
                         << A << "->" << table[A][a] << " , trying to add " << A << "->" << prod << "\n";
                    exit(1);
                }
                table[A][a] = prod;
            }
        }
    }
}

void parseString(string input) {
    input.push_back('$');
    stack<char> st;
    st.push('$'); st.push(start);
    int i = 0;
    cout << "\nParsing steps:\n";
    while (!st.empty()) {
        char top = st.top();
        char curr = input[i];
        if (top == curr) {
            cout << "Matched: " << curr << "\n";
            st.pop(); ++i;
        } else if (isNt(top)) {
            if (table[top].count(curr)) {
                string prod = table[top][curr];
                cout << top << " -> " << prod << "\n";
                st.pop();
                if (prod != "e") {
                    for (int k = (int)prod.size() - 1; k >= 0; --k) st.push(prod[k]);
                }
            } else {
                cout << "Error: No rule for [" << top << ", " << curr << "]\n";
                return;
            }
        } else {
            cout << "Error at " << curr << "\n";
            return;
        }
    }
    // safe acceptance check
    if (i == (int)input.size() - 1 && input[i] == '$') cout << "\nString accepted\n";
    else cout << "\nString rejected\n";
}

int main() {
    int n;
    cout << "Enter number of productions: ";
    if (!(cin >> n)) return 0;
    cout << "Enter productions (e for epsilon, e.g., E->TR):\n";
    for (int i = 0; i < n; ++i) {
        string s; cin >> s;
        if (s.size() < 3 || s[1] != '-' || s[2] != '>') { cerr << "Bad format\n"; return 0; }
        char lhs = s[0];
        string rhs = s.substr(3);
        G[lhs].push_back(rhs);
        nt.insert(lhs);
        for (char c : rhs) if (!isNt(c) && c != 'e') terminals.insert(c);
        if (i == 0) start = lhs;
    }
    cout << "\nUpdated Grammar:\n";
    for (auto &p : G) {
        cout << p.first << " -> ";
        for (int i = 0; i < (int)p.second.size(); ++i) {
            cout << p.second[i];
            if (i + 1 < (int)p.second.size()) cout << " | ";
        }
        cout << "\n";
    }

    // FIRST and FOLLOW
    for (char ch : nt) getFirst(ch);
    follow[start].insert('$');
    for (char ch : nt) getFollow(ch);
    cout << "\nFIRST sets:\n";
    for (auto &p : first) {
        cout << "FIRST(" << p.first << ") = { ";
        for (char c : p.second) cout << c << ' ';
        cout << "}\n";
    }
    cout << "\nFOLLOW sets:\n";
    for (auto &p : follow) {
        cout << "FOLLOW(" << p.first << ") = { ";
        for (char c : p.second) cout << c << ' ';
        cout << "}\n";
    }
    cout << "Step 3: Constructing Parsing Table...\n";
    constructParsingTable();
    for (auto &row : table) {
        for (auto &col : row.second)
            cout << "M[" << row.first << "," << col.first << "] = "
                 << row.first << "->" << col.second << "\n";
    }
    cout << "\nEnter input string to parse: ";
    string input; cin >> input;
    parseString(input);
    return 0;
}
/*
Enter number of productions: 8
Enter productions (e for epsilon, e.g., E->TR):
E->TP
P->+TP
P->e
T->FQ
Q->*FQ
Q->e
F->(E)
F->i

Updated Grammar:
E -> TP
F -> (E) | i
P -> +TP | e
Q -> *FQ | e
T -> FQ

FIRST sets:
FIRST(E) = { ( i }
FIRST(F) = { ( i }
FIRST(P) = { + e }
FIRST(Q) = { * e }
FIRST(T) = { ( i }

FOLLOW sets:
FOLLOW(E) = { $ ) }
FOLLOW(F) = { $ ) * + }
FOLLOW(P) = { $ ) }
FOLLOW(Q) = { $ ) + }
FOLLOW(T) = { $ ) + }
Step 3: Constructing Parsing Table...
M[E,(] = E->TP
M[E,i] = E->TP
M[F,(] = F->(E)
M[F,i] = F->i
M[P,$] = P->e
M[P,)] = P->e
M[P,+] = P->+TP
M[Q,$] = Q->e
M[Q,)] = Q->e
M[Q,*] = Q->*FQ
M[Q,+] = Q->e
M[T,(] = T->FQ
M[T,i] = T->FQ

Enter input string to parse: i+i$

Parsing steps:
E -> TP
T -> FQ
F -> i
Matched: i
Q -> e
P -> +TP
Matched: +
T -> FQ
F -> i
Matched: i
Q -> e
P -> e
Matched: $

String accepted
*/
