#include<bits/stdc++.h>
using namespace std;
void heapify(int* a, int n, int i) {
    int largest = i;
    int l = 2 * i + 1;
    int r = 2 * i + 1;
    if(l < n && a[l] > a[largest]) largest = l;
    if(r < n && a[r] > a[largest]) largest = r;
    if(largest != i) {
        swap(a[largest], a[i]);
        heapify(a, n, largest);
    }
}
void heapSort(int* a, int n) {
    for(int i = n/2 - 1; i >= 0; i--) {
        heapify(a, n, i);
    }
    for(int i = n - 1; i >= 0; i--) {
        swap(a[0], a[i]);
        heapify(a, i, 0);
    }
}
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    freopen("SORT.INP", "r", stdin);
    freopen("SORT.OUT", "w", stdout);
    int n; cin >> n; int a[n];
    for(int i = 0; i < n; i++) cin >> a[i];
    heapSort(a, n);
    for(int x : a) cout << x << " ";
    return 0;
}