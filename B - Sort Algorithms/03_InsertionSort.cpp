#include<bits/stdc++.h>
using namespace std;
void insertionSort(int n, int* a) {
    for(int i = 1; i < n; i++) {
        int selected = a[i];
        int j = i - 1;
        while(j >= 0 && a[j] > selected) {
            a[j + 1] = a[j];
            j--;
        }
        a[j + 1] = selected;
    }
}
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    freopen("SORT.INP", "r", stdin);
    freopen("SORT.OUT", "w", stdout);
    int n; cin >> n; int a[n];
    for(int &x : a) cin >> x;
    insertionSort(n, a);
    for(int &x : a) cout << x << " ";
    return 0;
}