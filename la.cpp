#include <bits/stdc++.h>
using namespace std;

const int MAX_ID_LEN = 32;

bool isKeyword(const string &s) {
    return s == "int" || s == "float" || s == "if" || s == "else" ||
           s == "while" || s == "for" || s == "do" || s == "return";
}

int main() {
    cout << "Enter your code (type END to finish):\n";

    string codeLine, code = "";
    while (true) {
        getline(cin, codeLine);
        if (codeLine == "END")
            break;
        code += codeLine + "\n";
    }

    int i = 0;
    while (i < code.size()) {

        // Skip whitespaces
        if (isspace(code[i])) {
            i++;
            continue;
        }

        // Skip single-line comments
        if (code[i] == '/' && i + 1 < code.size() && code[i + 1] == '/') {
            i += 2;
            while (i < code.size() && code[i] != '\n') i++;
            continue;
        }

        // Identifiers and Keywords
        if (isalpha(code[i]) || code[i] == '_') {
            string word = "";
            while (i < code.size() && (isalnum(code[i]) || code[i] == '_')) {
                word += code[i];
                i++;
            }

            if (isKeyword(word))
                cout << word << " : Keyword\n";
            else
                cout << word << " : Identifier\n";

            continue;
        }

        // Numbers
        if (isdigit(code[i])) {
            string num = "";
            while (i < code.size() && isdigit(code[i])) {
                num += code[i];
                i++;
            }
            cout << num << " : Number\n";
            continue;
        }

        // Operators and Punctuations
        string ops = "+-*/%=<>(){};,";
        size_t pos = ops.find(code[i]);
        if (pos != string::npos) {
            cout << code[i] << " : Operator/Punctuation\n";
            i++;
            continue;
        }

        // Unknown characters
        cout << code[i] << " : Unknown\n";
        i++;
    }

    cout << "\nEnd of tokens.\n";
    return 0;
}
