#include <iostream>
#include <vector>

using namespace std;

int main() {
    string source, target;
    cin >> source >> target;

    int source_length = source.length();
    int target_length = target.length();

    vector<vector<int>> dp(target_length + 1, vector<int>(source_length + 1, 0));

    for(int i = 0; i <= target_length; i++) {
        for(int j = 0; j <= source_length; j++) {
            if(i == 0) {
                dp[i][j] = j;
            } else if(j == 0) {
                dp[i][j] = i;
            } else if(target[i - 1] == source[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1];
            }
            else{
                dp[i][j] = min(dp[i - 1][j - 1], min(dp[i][j - 1], dp[i-1][j])) + 1;
            }
        }
    }

    cout << dp[target_length][source_length] << endl;

    return 0;
}