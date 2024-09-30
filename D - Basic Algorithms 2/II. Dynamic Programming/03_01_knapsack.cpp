#include<bits/stdc++.h>
using namespace std;
int knapsack(int n, int s, int w[], int v[]) {
    int dp[n + 1][s + 1];
    for(int i = 0; i <= n; i++) {
        for(int j = 0; j <= s; j++) {
            if(i == 0 || j == 0) {
                dp[i][j] = 0;
            } else {
                dp[i][j] = dp[i - 1][j];
                if(j >= w[i - 1]) dp[i][j] = max(dp[i][j], dp[i - 1][j - w[i]] + v[i]);
            }
        }
    }
    return dp[n][s];
}
int main() {
    int n, s; cin >> n >> s; int w[n + 1], v[n + 1];
    for(int i = 1; i <= n; i++) cin >> w[i];
    for(int i = 1; i <= n; i++) cin >> v[i];
    cout << knapsack(n, s, w, v);
    return 0;
}

