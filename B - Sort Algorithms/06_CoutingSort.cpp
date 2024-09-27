#include<bits/stdc++.h>
using namespace std;
void CoutingSort(int a[], int n) {
    int max = *max_element(a, a + n);
    vector<int> b(max, 0);
    for(int i = 0; i < n; i++) b[a[i]]++;
    for(int i = 1; i < max; i++) b[i] += b[i - 1];
    vector<int> res(n);
    for(int i = 0; i < n; i++) {
        res[b[a[i] - 1]] = a[i];
        b[a[i]]--;
    }
    for(int i = 0; i < n; i++) a[i] = res[i];
}
int main() {
    int n; cin >> n; int a[n];
    for(int i = 0; i < n; i++) cin >> a[i];
    CoutingSort(a, n);
    for(int x : a) cout << x << " ";
    return 0;
}