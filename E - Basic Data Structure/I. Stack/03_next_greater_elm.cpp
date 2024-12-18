#include<bits/stdc++.h>
using namespace std;
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    freopen("STACK.INP", "r", stdin);
    freopen("STACK.OUT", "w", stdout);
    int n; cin >> n; int a[n];
    for(int &x : a) cin >> x;
    stack<int> temp;
    vector<int> res(n, -1);
    for(int i = 0; i < n; i++) {
        while(!temp.empty() && a[temp.top()] < a[i]) {
            res[temp.top()] = a[i];
            temp.pop();
        }
        temp.push(i);
    }
    for(int x : res) cout << x << " ";
    return 0;
}