#include <bits/stdc++.h> 
using namespace std; 
stack<char> opStack; 
stack<string> valStack; 
int tempCount = 1; 

int precedence(char c) { 
    if (c == '+' || c == '-') return 1; 
    if (c == '*' || c == '/') return 2; 
    return 0; 
} 

string newTemp() { 
    return "t" + to_string(tempCount++); 
} 

void generateTAC(string op) { 
    string val2 = valStack.top(); valStack.pop(); 
    string val1 = valStack.top(); valStack.pop(); 
    string temp = newTemp(); 
    cout << temp << " = " << val1 << " " << op << " " << val2 << endl; 
    valStack.push(temp); 
} 

int main() { 
    string expr; 
    cout << "Enter expression (e.g., a=b+c*d): "; 
    cin >> expr; 

    string lhs = ""; 
    int pos = expr.find('='); 
    if (pos != string::npos) { 
        lhs = expr.substr(0, pos); 
        expr = expr.substr(pos + 1); 
    } 

    for (int i = 0; i < expr.length(); i++) { 
        char c = expr[i]; 

        if (isalnum(c)) { 
            string s(1, c); 
            valStack.push(s); 
        } 
        else if (c == '(') { 
            opStack.push(c); 
        } 
        else if (c == ')') { 
            while (!opStack.empty() && opStack.top() != '(') { 
                string op(1, opStack.top()); 
                opStack.pop(); 
                generateTAC(op); 
            } 
            opStack.pop(); 
        } 
        else if (c == '+' || c == '-' || c == '*' || c == '/') { 
            while (!opStack.empty() && precedence(opStack.top()) >= precedence(c)) { 
                string op(1, opStack.top()); 
                opStack.pop(); 
                generateTAC(op); 
            } 
            opStack.push(c); 
        } 
    } 

    while (!opStack.empty()) { 
        string op(1, opStack.top()); 
        opStack.pop(); 
        generateTAC(op); 
    } 

    if (!lhs.empty() && !valStack.empty()) { 
        cout << lhs << " = " << valStack.top() << endl; 
    } 

    return 0;  
}
