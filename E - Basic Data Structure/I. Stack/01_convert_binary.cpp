#include<bits/stdc++.h>
using namespace std;
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    freopen("STACK.INP", "r", stdin);
    freopen("STACK.OUT", "w", stdout);
    int n; cin >> n;
    stack<int> binary;
    while(n > 0) {
        binary.push(n % 2);
        n /= 2;
    }
    while(!binary.empty()){
        cout << binary.top();
        binary.pop();
    }
    return 0;
}