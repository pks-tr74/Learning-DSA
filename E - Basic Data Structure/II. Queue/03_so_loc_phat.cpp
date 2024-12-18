/*

Input:
2
2
3
Output:
88 86 68 66 8 6
888 886 868 866 688 686 668 666 88 86 68 66 8 6

*/
#include<bits/stdc++.h>
using namespace std;
const int condition = 15;
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    freopen("QUEUE.INP", "r", stdin);
    freopen("QUEUE.OUT", "w", stdout);
    int n; cin >> n;
    queue<string> temp;
    vector<string> arr;
    temp.push("6");
    temp.push("8");
    arr.push_back("6");
    arr.push_back("8");
    while(true) {
        string top = temp.front();
        temp.pop();
        if(top.length() >= condition) break;
        temp.push(top + "6");
        temp.push(top + "8");
        arr.push_back(top + "6");
        arr.push_back(top + "8");
    }
    vector<string> output;
    for(string x: arr) {
        if(x.length() > n) break;
        output.push_back(x);
    }
    reverse(output.begin(), output.end());
    for(string x: output) cout << x << " ";
    return 0;
}