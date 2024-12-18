/*
Input:
2
5
7

Output:
90
9009
*/
#include<bits/stdc++.h>
using namespace std;
using ll = long long;
const int maxSize = 10; // Độ dài tối đa của string
const int condition = 100; // Đk: 1 <= N <= 100
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    freopen("QUEUE.INP", "r", stdin);
    freopen("QUEUE.OUT", "w", stdout);
    int n; cin >> n;
    queue<string> temp;
    vector<ll> arr;
    temp.push("9");
    arr.push_back(9);
    while(true) {
        string top = temp.front();
        temp.pop();
        if(top.length() >= maxSize) break;
        temp.push(top + "0");
        temp.push(top + "9");
        arr.push_back(stoll(top + "0"));
        arr.push_back(stoll(top + "9"));
    }
    for(int i = 0; i < n; i++) {
        cout << arr[i] << " ";
    }
    return 0;
}