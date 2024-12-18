#include <bits/stdc++.h>
using namespace std;
void coutingSort(int* a, int n) {
    int max_val = *max_element(a, a + n);
    vector<int> count(max_val + 1, 0);
    for(int i = 0; i < n; i++) count[a[i]]++;
    for(int i = 1; i < max_val + 1; i++) count[i] += count[i - 1];
    vector<int> res(n, 0);
    for(int i = n - 1; i >= 0; i--) res[--count[a[i]]] = a[i];
    for(int i = 0; i < n; i++) a[i] = res[i];
}
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    freopen("SORT.INP", "r", stdin);
    freopen("SORT.OUT", "w", stdout);
    int n; cin >> n; int a[n];
    for(int &x : a) cin >> x;
    coutingSort(a, n);
    for(int &x : a) cout << x << " ";
    return 0;
}