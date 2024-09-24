#include<bits/stdc++.h>
#include<vector>
#include<iostream>
using namespace std;
void merge(int *a, int l, int m, int r){
    vector<int> x(a + l, a + m + 1);
    vector<int> y(a + m + 1, a + r + 1);
    int i = 0, j = 0;
    while(i < x.size() && j < y.size()) {
        a[l++] = x[i] < y[j] ? x[i++] : y[j++];
    }
    while(i < x.size()) a[l++] = x[i++];
    while(j < y.size()) a[l++] = y[j++];
}
void MergeSort(int *a, int l, int r) {
    int m = (l + r)/2;
    if(l >= r) return;
    MergeSort(a, l, m);
    MergeSort(a, m + 1, r);
    merge(a, l, m, r);
}
int main() {
    int n; cin >> n;
    int a[n];
    for(int i = 0; i < n; i++) cin >> a[i];
    MergeSort(a, 0, n - 1);
    for(int x : a) cout << x << " ";
    return 0;
}