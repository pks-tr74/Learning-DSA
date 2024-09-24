#include<bits/stdc++.h>
using namespace std;
void InsertionSort(int a[], int n) {
    for(int i = 1; i < n; i++) {
        int selected = a[i];
        int j = i - 1;
        while(a[j] > selected && j >= 0){
            a[j + 1] = a[j];
            j--;
        }
        a[j + 1] = selected;
    }
}
int main() {
    int n; cin >> n; int a[n];
    for(int i = 0; i < n; i++) cin >> a[i];
    InsertionSort(a, n);
    for(int x : a) cout << x << " ";
    return 0;
}