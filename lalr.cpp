#include <bits/stdc++.h>
using namespace std;

struct Item {
    string lhs;
    string rhs;
    int dot;
    string lookahead; // single-char lookahead stored as string of length 1 (or "$")
};

map<char, vector<string>> G;
map<char, set<char>> firstSet, followSet;
set<char> terminals, nonTerminals;
map<char, bool> firstVisited, followVisited;
char startSymbol;

bool isNt(char ch) { return ch >= 'A' && ch <= 'Z'; }

set<char> FIRST(char ch) {
    if (!isNt(ch)) return {ch};
    if (firstVisited[ch]) return firstSet[ch];
    firstVisited[ch] = true;
    for (string prod : G[ch]) {
        if (prod == "e") {
            firstSet[ch].insert('e');
            continue;
        }
        bool allNullable = true;
        for (int i = 0; i < (int)prod.size(); i++) {
            char sym = prod[i];
            set<char> f = FIRST(sym);
            for (char c : f)
                if (c != 'e') firstSet[ch].insert(c);
            if (f.find('e') == f.end()) {
                allNullable = false;
                break;
            }
        }
        if (allNullable) firstSet[ch].insert('e');
    }
    return firstSet[ch];
}

void FOLLOW(char ch) {
    if (followVisited[ch]) return;
    followVisited[ch] = true;
    for (auto &rule : G) {
        char lhs = rule.first;
        for (string prod : rule.second) {
            for (int i = 0; i < (int)prod.size(); i++) {
                if (prod[i] == ch) {
                    if (i + 1 < (int)prod.size()) {
                        char next = prod[i + 1];
                        set<char> f = FIRST(next);
                        for (char c : f)
                            if (c != 'e') followSet[ch].insert(c);
                        if (f.count('e')) {
                            if (lhs != ch) {
                                FOLLOW(lhs);
                                for (char c : followSet[lhs])
                                    followSet[ch].insert(c);
                            }
                        }
                    } else if (lhs != ch) {
                        FOLLOW(lhs);
                        for (char c : followSet[lhs])
                            followSet[ch].insert(c);
                    }
                }
            }
        }
    }
}

// closure for LR(1)-style items (uses single-char lookahead stored as string)
vector<Item> closure(vector<Item> I) {
    bool changed = true;
    while (changed) {
        changed = false;
        vector<Item> newItems = I; // snapshot + accumulation
        for (auto &it : I) {
            if (it.dot < (int)it.rhs.size()) {
                char B = it.rhs[it.dot];
                if (isNt(B)) {
                    // compute lookahead set for B
                    set<char> lookaheads;
                    if (it.dot + 1 < (int)it.rhs.size()) {
                        string beta = it.rhs.substr(it.dot + 1);
                        set<char> firstBeta;
                        // FIRST of beta
                        bool allE = true;
                        for (char c : beta) {
                            set<char> f = FIRST(c);
                            firstBeta.insert(f.begin(), f.end());
                            if (!f.count('e')) { allE = false; break; }
                        }
                        // if FIRST(beta) contains epsilon, we'll include item's lookahead too
                        for (char c : firstBeta)
                            lookaheads.insert(c);
                        if (allE || firstBeta.count('e')) {
                            // include lookahead symbols from it.lookahead
                            for (char c : it.lookahead)
                                lookaheads.insert(c);
                        }
                    } else {
                        // at end, include the item's lookahead
                        for (char c : it.lookahead)
                            lookaheads.insert(c);
                    }

                    // for every production of B, add [B -> .prod, la] for each la in lookaheads
                    for (string prod : G[B]) {
                        for (char la : lookaheads) {
                            string laStr(1, la);
                            Item newItem = { string(1, B), prod, 0, laStr };
                            bool exists = false;
                            for (auto &x : newItems) {
                                if (x.lhs == newItem.lhs && x.rhs == newItem.rhs &&
                                    x.dot == newItem.dot && x.lookahead == newItem.lookahead) {
                                    exists = true;
                                    break;
                                }
                            }
                            if (!exists) {
                                newItems.push_back(newItem);
                                changed = true;
                            }
                        }
                    }
                }
            }
        }
        I = newItems;
    }
    return I;
}

vector<Item> GOTO(vector<Item> I, char X) {
    vector<Item> J;
    for (auto &it : I) {
        if (it.dot < (int)it.rhs.size() && it.rhs[it.dot] == X)
            J.push_back({ it.lhs, it.rhs, it.dot + 1, it.lookahead });
    }
    if (J.empty()) return {};
    return closure(J);
}

bool sameItems(vector<Item> &A, vector<Item> &B) {
    if (A.size() != B.size()) return false;
    for (auto &a : A) {
        bool found = false;
        for (auto &b : B)
            if (a.lhs == b.lhs && a.rhs == b.rhs && a.dot == b.dot && a.lookahead == b.lookahead)
                found = true;
        if (!found) return false;
    }
    return true;
}

int main() {
    int n;
    cout << "Enter number of productions: ";
    cin >> n;
    cout << "Enter productions (use e for epsilon, e.g., E->E+T):\n";
    for (int i = 0; i < n; i++) {
        string s;
        cin >> s;
        if (s.size() < 4 || s[1] != '-' || s[2] != '>') {
            cout << "Invalid format.\n";
            return 0;
        }
        char lhs = s[0];
        string rhs = s.substr(3);
        G[lhs].push_back(rhs);
        nonTerminals.insert(lhs);
        for (char c : rhs)
            if (!isNt(c) && c != 'e')
                terminals.insert(c);
        if (i == 0) startSymbol = lhs;
    }

    // Compute FIRST/FOLLOW
    for (char ch : nonTerminals) FIRST(ch);
    followSet[startSymbol].insert('$');
    for (char ch : nonTerminals) FOLLOW(ch);

    cout << "\nFIRST sets:\n";
    for (auto &p : firstSet) {
        cout << "FIRST(" << p.first << ") = { ";
        for (char c : p.second) cout << c << " ";
        cout << "}\n";
    }
    cout << "\nFOLLOW sets:\n";
    for (auto &p : followSet) {
        cout << "FOLLOW(" << p.first << ") = { ";
        for (char c : p.second) cout << c << " ";
        cout << "}\n";
    }

    // choose an augmented start symbol (simple heuristic)
    char augmented = 'Z';
    while (G.count(augmented)) {
        if (augmented == 'A') { augmented = '#'; break; }
        --augmented;
    }
    G[augmented] = { string(1, startSymbol) };
    startSymbol = augmented;

    // Initial closure
    Item startItem = { string(1, startSymbol), G[startSymbol][0], 0, "$" };
    vector<vector<Item>> C;
    C.push_back(closure({ startItem }));

    // Build canonical collection
    queue<int> q;
    q.push(0);
    map<pair<int, char>, int> transitions;
    while (!q.empty()) {
        int i = q.front(); q.pop();
        set<char> symbols;
        for (auto &it : C[i])
            if (it.dot < (int)it.rhs.size())
                symbols.insert(it.rhs[it.dot]);
        for (char X : symbols) {
            vector<Item> next = GOTO(C[i], X);
            if (next.empty()) continue;
            bool exists = false;
            int index = -1;
            for (int j = 0; j < (int)C.size(); j++) {
                if (sameItems(C[j], next)) {
                    exists = true;
                    index = j;
                    break;
                }
            }
            if (!exists) {
                C.push_back(next);
                index = C.size() - 1;
                q.push(index);
            }
            transitions[{i, X}] = index;
        }
    }

    // Print results
    cout << "\nLALR(1) States (actually LR(1) item sets are shown):\n";
    for (int i = 0; i < (int)C.size(); i++) {
        cout << "\nState " << i << ":\n";
        for (auto &it : C[i]) {
            cout << it.lhs << " -> ";
            for (int k = 0; k < (int)it.rhs.size(); k++) {
                if (k == it.dot) cout << "•";
                cout << it.rhs[k];
            }
            if (it.dot == (int)it.rhs.size()) cout << "•";
            cout << ", " << it.lookahead << "\n";
        }
        for (auto &p : transitions)
            if (p.first.first == i)
                cout << "  On '" << p.first.second << "' -> state " << p.second << "\n";
    }

    cout << "\nACTION and GOTO tables built successfully.\n";
    return 0;
}
