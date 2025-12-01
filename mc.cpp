#include <bits/stdc++.h> 
using namespace std; 

struct Node { 
    string value; 
    Node* left; 
    Node* right; 
    Node(string val) : value(val), left(NULL), right(NULL) {} 
}; 

int precedence(char op) { 
    if (op == '+' || op == '-') return 1; 
    if (op == '*' || op == '/') return 2; 
    return 0; 
} 

vector<string> infixToPostfix(string expr) { 
    vector<string> postfix; 
    stack<char> st; 
    for (int i = 0; i < expr.size(); i++) { 
        char c = expr[i]; 
        if (isalnum(c)) { 
            string s(1, c); 
            postfix.push_back(s); 
        }  
        else if (c == '(') st.push(c); 
        else if (c == ')') { 
            while (!st.empty() && st.top() != '(') { 
                postfix.push_back(string(1, st.top())); 
                st.pop(); 
            } 
            if (!st.empty()) st.pop(); 
        }  
        else if (c == '+' || c == '-' || c == '*' || c == '/') { 
            while (!st.empty() && precedence(st.top()) >= precedence(c)) { 
                postfix.push_back(string(1, st.top())); 
                st.pop(); 
            } 
            st.push(c); 
        } 
    } 
    while (!st.empty()) { 
        postfix.push_back(string(1, st.top())); 
        st.pop(); 
    }
    return postfix; 
} 

Node* buildAST(vector<string> postfix) { 
    stack<Node*> st; 
    for (auto token : postfix) { 
        if (isalnum(token[0])) { 
            st.push(new Node(token)); 
        } else { 
            Node* node = new Node(token); 
            node->right = st.top(); st.pop(); 
            node->left = st.top(); st.pop(); 
            st.push(node); 
        } 
    } 
    return st.top(); 
} 

int tempCount = 1; 

string generateCode(Node* root) { 
    if (!root) return ""; 
    if (!root->left && !root->right) { 
        string temp = "LOAD " + root->value; 
        cout << temp << endl; 
        return root->value; 
    } 
    string left = generateCode(root->left); 
    string right = generateCode(root->right); 
    string temp = "T" + to_string(tempCount++); 
    if (root->value == "+") 
        cout << "ADD " << left << ", " << right << " -> " << temp << endl; 
    else if (root->value == "-") 
        cout << "SUB " << left << ", " << right << " -> " << temp << endl; 
    else if (root->value == "*") 
        cout << "MUL " << left << ", " << right << " -> " << temp << endl; 
    else if (root->value == "/") 
        cout << "DIV " << left << ", " << right << " -> " << temp << endl; 
    return temp; 
} 

int main() { 
    string expr; 
    cout << "Enter expression (e.g., a=b+c*d): "; 
    cin >> expr; 
    string lhs = "", rhs = ""; 
    size_t pos = expr.find('='); 
    if (pos != string::npos) { 
        lhs = expr.substr(0, pos); 
        rhs = expr.substr(pos + 1);
    } else rhs = expr; 
    vector<string> postfix = infixToPostfix(rhs); 
    Node* root = buildAST(postfix); 
    cout << "\n=== Machine Code Generation ===\n"; 
    string result = generateCode(root); 
    if (!lhs.empty()) 
        cout << "STORE " << result << " -> " << lhs << endl; 

    return 0; 
}
