#include<bits/stdc++.h>
using namespace std;
void CoutingSort(int a[], int n, int div) {
    vector<int> count(10, 0);
    for(int i = 0; i < n; i++) count[(a[i] / div) % 10]++;
    for(int i = 1; i < 10; i++) count[i] += count[i - 1];
    vector<int> res(n);
    for(int i = n - 1; i >= 0; i--) res[--count[(a[i] / div) % 10]] = a[i];
    for(int i = 0; i < n; i++) a[i] = res[i];
}
void RadixSort(int a[], int n) {
    int max = *max_element(a, a + n);
    for(int div = 1; max/div > 0; div *= 10) CoutingSort(a, n, div);
}
int main() {
    int n; cin >> n; int a[n];
    for(int i = 0; i < n; i++) cin >> a[i];
    RadixSort(a, n);
    for(int x : a) cout << x << " ";
    return 0;
}