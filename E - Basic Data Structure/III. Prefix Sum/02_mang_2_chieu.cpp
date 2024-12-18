/*
4 4
1 2 3 4
5 6 7 8
2 2 1 3
4 2 6 8
1
1 3 1 4
44 (Output)
*/
#include<bits/stdc++.h>
using namespace std;
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    freopen("SUM.INP", "r", stdin);
    freopen("SUM.OUT", "w", stdout);
    int n, m; cin >> n >> m; int a[n][m];
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; i++) cin >> a[n][m];
    }
    int h1, h2, c1, c2; cin >> h1 >> h2 >> c1 >> c2;
    int prefix[n + 1][m + 1] = {0};
    for(int i = 1; i < n + 1; i++) {
        for(int j = 1; j < m + 1; j++) {
            prefix[i][j] = prefix[i - 1][j] + prefix[i][j - 1] - prefix[i - 1][j - 1];
        }
    }
    cout << prefix[h2][c2] - (prefix[h1 - 1][c2] + prefix[h2][c1 - 1] - prefix[h1][c1]);
    return 0;
}