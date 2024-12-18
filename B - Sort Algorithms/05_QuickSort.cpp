#include <bits/stdc++.h>
using namespace std;
int lomutoPartition(int* a, int l, int r) {
    int pivot = a[r];
    int i = l - 1;
    for(int j = l; j < r; j++) {
        if(a[j] <= pivot) swap(a[++i], a[j]);
    }
    swap(a[++i], a[r]);
    return i;
}
int hoarePartition(int* a, int l, int r) {
    int pivot = a[l];
    int i = l - 1, j = r + 1;
    while(true) {
        do { ++i; } while(a[i] < pivot);
        do { --j; } while(a[j] > pivot);
        if(i < j) swap(a[i], a[j]);
        else return j;
    }
}
void quickSort(int* a, int l, int r) {
    if(l > r) return;
    int p = hoarePartition(a, l, r);
    quickSort(a, l, p - 1);
    quickSort(a, p + 1, r);
}
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    freopen("SORT.INP", "r", stdin);
    freopen("SORT.OUT", "w", stdout);
    int n; cin >> n; int a[n];
    for(int &x : a) cin >> x;
    quickSort(a, 0, n - 1);
    for(int &x : a) cout << x << " ";
    return 0;
}