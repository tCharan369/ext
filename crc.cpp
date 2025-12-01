#include <bits/stdc++.h>
using namespace std;

string findXor(string a, string b) {
    // return full-length XOR of a and b (they must be same length)
    int n = b.length();
    string res = "";
    for (int i = 0; i < n; i++) {
        if (a[i] == b[i])
            res += "0";
        else
            res += "1";
    }
    return res;
}

string mod2div(string dividend, string divisor) {
    int m = divisor.length();
    // take the first m bits
    string remainder = dividend.substr(0, m);
    int n = dividend.length();

    // perform division
    for (int i = m; i < n; ++i) {
        if (remainder[0] == '1') {
            // XOR with divisor and append next bit
            string x = findXor(divisor, remainder);
            remainder = x.substr(1) + dividend[i];
        } else {
            // XOR with 0s (effectively keep remainder) and append next bit
            string zeros(m, '0');
            string x = findXor(zeros, remainder);
            remainder = x.substr(1) + dividend[i];
        }
    }

    // final step (no next bit to append)
    if (remainder[0] == '1') {
        remainder = findXor(divisor, remainder).substr(1);
    } else {
        string zeros(m, '0');
        remainder = findXor(zeros, remainder).substr(1);
    }

    return remainder; // length = m-1
}

string encodeData(string data, string key) {
    int keylen = key.length();
    string appended_data = data + string(keylen - 1, '0');
    string remainder = mod2div(appended_data, key);
    string codeword = data + remainder;
    return codeword;
}

int receiver(string code, string key) {
    string rem = mod2div(code, key);
    return (rem.find('1') == string::npos) ? 1 : 0;
}

int main() {
    string data, key;
    cout << "Sender side:\n";
    cout << "Enter data & key: ";
    cin >> data >> key;
    string code = encodeData(data, key);
    cout << "Encoded data: " << code << endl;
    cout << "\nReceiver side:\n";
    if (receiver(code, key))
        cout << "Data is correct (No errors detected)" << endl;
    else
        cout << "Data is incorrect (Error detected)" << endl;
    return 0;
}
