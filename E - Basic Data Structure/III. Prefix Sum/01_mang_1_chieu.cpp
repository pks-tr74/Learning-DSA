#include<bits/stdc++.h>
using namespace std;
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    freopen("SUM.INP", "r", stdin);
    freopen("SUM.OUT", "w", stdout);
    int n; cin >> n; int a[n];
    for(int &x: a) cin >> x;
    int l, r; cin >> l >> r;
    int prefix[n + 1] = {0};
    for(int i = 1; i < n + 1; i++){
        prefix[i] = prefix[i - 1] + a[i];
    }
    cout << prefix[r] - prefix[l - 1];
    return 0;
}