class Solution {
public:
    bool winnerSquareGame(int n) {
        vector<bool> dp(n + 1, false);

        for (int stones = 1; stones <= n; stones++) {

            for (int num = 1; num * num <= stones; num++) {
                int square = num * num;
                int left = stones - square;

                if (dp[left] == false) {
                    dp[stones] = true;
                    break;
                }
            }
        }

        return dp[n];
    }
};
