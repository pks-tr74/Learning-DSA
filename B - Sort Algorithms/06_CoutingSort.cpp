#include<bits/stdc++.h>
using namespace std;
void CoutingSort(int a[], int n) {

}
int main() {
    int n; cin >> n; int a[n];
    for(int i = 0; i < n; i++) cin >> a[i];
    CoutingSort(a, n);
    for(int x : a) cout << x << " ";
    return 0;
}