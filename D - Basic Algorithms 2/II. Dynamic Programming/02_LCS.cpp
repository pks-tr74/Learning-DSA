#include<bits/stdc++.h>
using namespace std;
int LCS(string a, string b) {
    int L[a.size() + 1][b.size() + 1];
    for(int i = 0; i <= a.size(); i++) {
        for(int j = 0; j <= b.size(); j++) {
            if(i == 0 || j == 0) {
                L[i][j] = 0;
            } else {
                if(a[i - 1] == b[j - 1]) {
                    L[i][j] = L[i - 1][j - 1] + 1;
                } else {
                    L[i][j] = max(L[i][j - 1], L[i - 1][j]);
                }
            }
        }
    }
    return L[a.size()][b.size()];
}
int main() {
    string a, b; cin >> a >> b;
    cout << LCS(a, b);
    return 0;
}