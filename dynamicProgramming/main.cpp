#include <iostream>
#include <vector>

/*
 * Version 1: time complexity: O(n^2)
 */
int longestIncreasingSubsequence(std::vector<int> a) {
    int size = (int) a.size();
    std::vector<int> dp(size, 1);
    std::vector<int> pre(size, -1);

    for (int i = 1; i < size; i++) {
        for (int j = 0; j < i; j++) {
            if (a[j] < a[i] && dp[i] < dp[j] + 1) {
                dp[i] = dp[j] + 1;
                pre[i] = j;
            }
        }
    }
    std::vector<int> result;
    int idx = std::distance(dp.begin(), std::max_element(dp.begin(), dp.end()));
    int i = idx;
    while (i >= 0) {
        result.push_back(a[i]);
        i = pre[i];
    }
    std::reverse(result.begin(), result.end());
    std::for_each(result.begin(), result.end(), [](int k) { std::cout << k << " "; });
    std::cout << std::endl;
    return dp[idx];
}

void testLongestIncreasingSubsequence() {
    std::vector<int> a({2, 4, 2, 3, 6, 3, 4, 5, 7, 9});
    std::cout << longestIncreasingSubsequence(a);
}

int main() {
    testLongestIncreasingSubsequence();
    return 0;
}