#include<bits/stdc++.h>
using namespace std;
void bubbleSort(int n, int* a) {
    for(int i = 0; i < n - 1; i++){
        bool isSwapped = false;
        for(int j = 0; j < n - 1 - i; j++) {
            if(a[j] > a[j + 1]) {
                swap(a[j], a[j + 1]);
                isSwapped = true;
            }
        }
        if(!isSwapped) break;
    }
}
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    freopen("SORT.INP", "r", stdin);
    freopen("SORT.OUT", "w", stdout);
    int n; cin >> n; int a[n];
    for(int &x : a) cin >> x;
    bubbleSort(n, a);
    for(int &x : a) cout << x << " ";
    return 0;
}