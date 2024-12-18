#include<bits/stdc++.h>
using namespace std;
void selectionSort(int n, int *a) {
    for(int i = 0; i < n - 1; i++) {
        for(int j = i + 1; j < n; j++) {
            if(a[i] > a[j]) swap(a[i], a[j]);
        }
    }
}
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    freopen("SORT.INP", "r", stdin);
    freopen("SORT.OUT", "w", stdout);
    int n; cin >> n; int a[n];
    for(int &x : a) cin >> x;
    selectionSort(n, a);
    for(int &x : a) cout << x << " ";
    return 0;
}