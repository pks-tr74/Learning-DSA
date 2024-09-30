/*
8 92
69 16 82 170 31 24 45 112
true
*/
#include<bits/stdc++.h>
using namespace std;
bool subsetSum(int n, int a[], int s) {
    bool dp[n + 1][s + 1];
    for(int i = 0; i <= n; i++) dp[i][0] = true;
    for(int j = 1; j <= s; j++) dp[0][j] = false;
    for(int i = 1; i <= n; i++){
        for(int j = 1; i <= s; i++) {
            dp[i][j] = a[i - 1] > j ? dp[i - 1][j] : dp[i - 1][j] || dp[i - 1][j - a[i - 1]];
        }
    }
    return dp[n][s];
}
int main() {
    int n, s; cin >> n >> s; int a[n];
    for(int i = 1; i <= n; i++) cin >> a[i];
    cout << boolalpha << subsetSum(n, a, s);
    return 0;
}

