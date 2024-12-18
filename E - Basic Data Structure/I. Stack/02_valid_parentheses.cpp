#include<bits/stdc++.h>
using namespace std;
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    freopen("STACK.INP", "r", stdin);
    freopen("STACK.OUT", "w", stdout);
    string s; cin >> s;
    stack<int> bracket;
    for(char x : s) {
        if(x == '(') bracket.push(x);
        else {
            if(bracket.empty()) cout << "Invalid";
            else bracket.pop();
        }
    }
    if(bracket.empty()) cout << "Valid";
    else cout << "Invalid";
    return 0;
}