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

/*
 * version 2: with time complexity O(nlogn)
 */
int longestIncreasingSubsequence2(std::vector<int> a) {
    int size = (int) a.size();
    std::vector<int> dp;
    std::vector<int> pre(size, -1);
    std::vector<int> idx;
    dp.push_back(a[0]);
    idx.push_back(0);
    int cnt = 1;
    for (int i = 1; i < size; i++) {
        // binary search to find the last number who is less than a[i]
        int start = 0, end = cnt - 1; //
        while (start <= end) {
            int mid = (start + end) / 2;
            if (dp[mid] >= a[i])
                end = mid - 1;
            else
                start = mid + 1;
        }
        pre[i] = end < 0 ? -1 : idx[end];
        if (end + 1 < cnt) {
            dp[end + 1] = a[i];
            idx[end + 1] = i;
        } else {
            dp.push_back(a[i]);
            idx.push_back(i);
            cnt++;
        }
    }
    std::vector<int> result;
    int i = idx[cnt - 1];
    while (i >= 0) {
        result.push_back(a[i]);
        i = pre[i];
    }
    std::reverse(result.begin(), result.end());
    std::for_each(result.begin(), result.end(), [](int k) { std::cout << k << " "; });
    std::cout << std::endl;
    return cnt;
}

void testLongestIncreasingSubsequence2() {
    std::vector<int> a({2, 4, 2, 3, 6, 45, 23, 0, 3, 3, 4, 5, 7, 9, 9, 9, 9, 9});
    std::cout << longestIncreasingSubsequence2(a);

}

int main() {
    testLongestIncreasingSubsequence2();
    return 0;
}