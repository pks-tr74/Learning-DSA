#include<bits/stdc++.h>
using namespace std;
const int binSize = 100000;
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    freopen("QUEUE.INP", "r", stdin);
    freopen("QUEUE.OUT", "w", stdout);
    int n; cin >> n; int a[n];
    for(int &x : a) cin >> x;
    queue<string> temp;
    vector<string> binary;
    temp.push("1");
    binary.push_back("1");
    while(binary.size() < binSize) {
        string top = temp.front();
        temp.push(top + "0");
        temp.push(top + "1");
        binary.push_back(top + "0");
        binary.push_back(top + "1");
    }
    for(int i = 0; i < n; i++) {
        cout << binary[i] << " ";
    }
    return 0;
}