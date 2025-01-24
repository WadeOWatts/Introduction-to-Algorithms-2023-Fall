#include <iostream>
#include <vector>

using namespace std;

int C(int m, int n){
    return f(m) / (f(n) * f(m-n));
}
int f(int n){
    if(n == 0)
        return 1;
    else
        return n * f(n-1);
}

int main() {
    int n;
    cin >> n;
    vector<vector<int>> dp(n, vector<int>(6));
    
    for (int i = 0; i < n; i++){
        for(int j = 0; j < 6; j++){
            if(j == 0){
                dp[i][j] = 1;
            } else if(j == 3){
                dp[i][j] = dp[i][j-1];
            } else if(j == 5){
                dp[i][j] = dp[i][j-1];
            } else if(j == 1){
                if(i == 0)
                    dp[i][j] = 0;
                else{
                    for(int k = i; k >= (i+1)/2; k--){
                        dp[i][j] += C(k,i+1-k);
                    }
                }

            }

        }
    }
    


    



    return 0;
}