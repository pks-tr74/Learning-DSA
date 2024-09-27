#include<bits/stdc++.h>
using namespace std;
void heapify(int a[], int n, int i) {
    int largest = i;
    int l = i * 2 + 1;
    int r = i * 2 + 2;
    if(r < n && a[r] > a[largest]) largest = r;
    if(l < n && a[l] > a[largest]) largest = l;
    if(largest != i) {
        swap(a[i], a[largest]);
        heapify(a, n, i);
    }
}
void HeapSort(int a[], int n) {
    for(int i = n/2 - 1; i >= 0; i--) heapify(a, n, i);
    for(int i = n - 1; i >= 0; i--) {
        swap(a[i], a[0]);
        heapify(a, i, 0);
    }
}
int main() {
    int n; cin >> n; int a[n];
    for(int i = 0; i < n; i++) cin >> a[i];
    HeapSort(a, n);
    for(int x : a) cout << x << " ";
    return 0;
}