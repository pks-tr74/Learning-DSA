#include<bits/stdc++.h>
using namespace std;
int LomutoPartition(int a[], int l, int r) {
    int pivot = a[r];
    int j = l - 1;
    for(int i = 0; i < r; i++) {
        if(pivot >= a[i]) swap(a[++j], a[i]);
    }
    swap(a[++j], a[r]);
    return j;
}
int HoarePartition(int a[], int l, int r) {
    int pivot = a[l];
    int i = l - 1, j = r + 1;
    while(true) {
        do { ++i; } while(pivot > a[i]);
        do { --j; } while(pivot < a[j]);
        if(i < j) swap(a[i], a[j]);
        else return j;
    }
}
void QuickSort(int a[], int l, int r){
    if(l >= r) return;
    int p = LomutoPartition(a, l, r);
    QuickSort(a, l, p);
    QuickSort(a, p + 1, r);
}
int main() {
    int n; cin >> n; int a[n];
    for(int i = 0; i < n; i++) cin >> a[i];
    QuickSort(a, 0, n - 1);
    for(int x : a) cout << x << " ";
    return 0;
}