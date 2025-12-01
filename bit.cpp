#include <iostream> 
using namespace std; 

void stuff(string &s) { 
    int count = 0; 
    for (int i = 0; i < s.length(); i++) { 
        if (s[i] == '1') { 
            count++;
            if (count == 5) { 
                s.insert(i + 1, "0");   
                count = 0; 
                i++;  
            } 
        } else { 
            count = 0; 
        } 
    } 
    cout << "Stuffed sequence: " << s << endl; 
} 
  
void destuff(string &s) { 
    int count = 0; 
    for (int i = 0; i < s.length(); i++) { 
        if (s[i] == '1') { 
            count++; 
            if (count == 5) { 
                if (i + 1 < s.length() && s[i + 1] == '0') { 
                    s.erase(i + 1, 1);   
                } 
                count = 0; 
            } 
        } else { 
            count = 0; 
        } 
    } 
    cout << "De-stuffed sequence: " << s << endl; 
} 
  
int main() { 
    string s, str; 
    cout << "Enter bit sequence: ";
    cin >> s; 
    stuff(s); 
    cout << "Enter stuffed sequence: "; 
    cin >> str; 
    destuff(str); 
    return 0; 
}
