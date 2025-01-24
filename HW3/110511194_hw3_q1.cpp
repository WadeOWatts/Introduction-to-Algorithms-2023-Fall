#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main() {
    int r, c;
    cin >> r >> c;
    string line;
    getline(cin, line);

    vector<vector<int>> maze(r, vector<int>(c));
    for(int i = 0; i < r; i++) {
        for(int j = 0; j < c; j++)
            cin >> maze[i][j];
    }

    vector<vector<int>> dp(r, vector<int>(c));
    int k = (r - 1) + (c - 1);
    for(int i = 0; i <= k; i++){
        for(int ri = i, ci = i - ri; ri + ci <= i; ri--, ci++){
                if(ri < 0 || ci >= c)
                    break;
                if(ri >= r){
                    ri = r - 1;
                    ci = i - ri;
                }
                if(ri == 0)
                    dp[ri][ci] = dp[ri][ci-1] + maze[ri][ci];
                else if(ci == 0)
                    dp[ri][ci] = dp[ri-1][ci] + maze[ri][ci];
                else
                    dp[ri][ci] = min(dp[ri-1][ci], dp[ri][ci-1]) + maze[ri][ci];
        }
    }

    cout << dp[r-1][c-1] << endl;

    return 0;
}