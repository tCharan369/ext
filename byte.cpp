#include <iostream> 
using namespace std; 

void stuff(string &data){ 
    string stuffed = "ESC";  
    int pos = 0; 
    while (pos < data.length()){ 
        if (data.substr(pos, 3) == "ESC"){ 
            stuffed += "ESCESC";  
            pos += 3; 
        } else { 
            stuffed += data[pos]; 
            pos++; 
        } 
    } 
    stuffed += "ESC";  
    cout << "Stuffed frame: " << stuffed << endl; 
} 

void destuff(string &stuffed){ 
    string ans; 
    int n = stuffed.length(); 
    if (n < 6 || stuffed.substr(0, 3) != "ESC" || stuffed.substr(n - 3) != "ESC") { 
        cout << "Invalid Frame!" << endl; 
        return; 
    } 
    for (int i = 3; i < n - 3;) { 
        if (stuffed.substr(i, 6) == "ESCESC") { 
            ans += "ESC"; 
            i += 6;  
        }
        else { 
        } 
        ans += stuffed[i]; 
        i++; 
    } 
    cout << "Destuffed data: " << ans << endl; 
} 

int main() { 
    string data, str; 
    cout << "Enter data to stuff: "; 
    cin >> data; 
    stuff(data); 
    cout << "Enter stuffed data: "; 
    cin >> str; 
    destuff(str); 
    return 0; 
}
