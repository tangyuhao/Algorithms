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

/*
 * Given a array of stock prices of different days, we need to figure out the max profit we can get within a max limit of
 * k deals
 *
 * Example: [3,6,2,3,6,7] and k = 2, then the max profit is 6 - 3 + 7 - 2 = 8
 */
int maxProfitWithKDeals(std::vector<int> a, int k) {
    std::vector<std::vector<int>> dp(k + 1, std::vector<int>(a.size(), 0));
    std::vector<std::vector<int>> pre(k + 1, std::vector<int>(a.size(), -1));
    int size = (int) a.size();
    for (int i = 1; i <= k; i++) {
        for (int j = 1; j < size; j++) {
            int mx = INT_MIN;
            int buyDay = -1;
            for (int m = 0; m < j; m++) {
                if (mx < dp[i - 1][m] + a[j] - a[m]) {
                    mx = dp[i - 1][m] + a[j] - a[m];
                    buyDay = m;
                }
            }
            if (dp[i][j - 1] > mx) {
                dp[i][j] = dp[i][j - 1];
            } else {
                dp[i][j] = mx;
                pre[i][j] = buyDay;
            }
        }
    }
    int kk = k, ii = size - 1;
    std::vector<int> buySell;
    while (dp[kk][ii] > 0) {
        if (pre[kk][ii] == -1) {
            ii--;
            continue;
        }
        buySell.push_back(ii);
        buySell.push_back(pre[kk][ii]);
        kk--;
        ii = pre[kk][ii] - 1;
    }
    std::reverse(buySell.begin(), buySell.end());
    std::for_each(buySell.begin(), buySell.end(), [](int k) { std::cout << k << " "; });
    std::cout << std::endl;
    return dp[k][size - 1];
}

void testMaxProfitWithKDeals() {
    std::vector<int> a = {3, 6, 2, 3, 6, 8};
    std::cout << maxProfitWithKDeals(a, 2);
}

/*
 * given a number n, we need to caculate the number of changes for 1 to become n,
 * each change allow us to plus 1 or time 2
 *
 * Note: this version is not the best, the best one is really easy: if the number is odd then number = number - 1;
 * else: number = number / 2;
 */
int minChanges(int n, std::vector<int> &times) {
    if (n == 1)
        return 0;
    if (times[n] != -1)
        return times[n];
    if (n % 2 == 0) {
        times[n] = std::min(minChanges(n / 2, times), minChanges(n - 1, times)) + 1;
    } else
        times[n] = minChanges(n - 1, times) + 1;
    return times[n];
}

void testMinChanges() {
    std::vector<int> times(2016, -1);
    std::cout << minChanges(2015, times);
}

inline bool testValid(int step, int row1, int row2, int rows, int cols) {
    int col1 = step - row1;
    int col2 = step - row2;
    return row1 >= 0 && row1 < rows && row2 >= 0 && row2 < rows && col1 >= 0 && col1 < cols && col2 >= 0 && col2 < cols;
}

inline int getValue(const std::vector<std::vector<std::vector<int>>> &dp,
                    int step, int row1, int row2, int rows, int cols) {
    return testValid(step, row1, row2, rows, cols) ? dp[step][row1][row2] : INT_MIN;

}

/*
 * The number in array a is weight, we need to find a path from the left-top corner to right-bottom corner and then go
 * back which is has the max sum of weights
 *
 * Note: if a placed is passed twice, only one time the weight should be calculated
 *
 * Definition: define the status of a position (i, j) as i + j so the status of a 3 by 3 array should be:
 *     0 1 2
 *     1 2 3
 *     2 3 4
 */
int chessTwiceTraverse(std::vector<std::vector<int>> a) {
    int rows = (int) a.size();
    int cols = (int) a[0].size();
    int steps = rows + cols - 1;
    std::vector<std::vector<std::vector<int>>> dp(steps,
                                                  std::vector<std::vector<int>>(rows, std::vector<int>(cols, INT_MIN)));
    dp[0][0][0] = a[0][0];

    for (int step = 1; step < steps; step++) {
        for (int i = 0; i < rows; i++) {
            for (int j = i; j < cols; j++) {
                if (!testValid(step, i, j, rows, cols))
                    continue;
                int tmp = dp[step][i][j];
                if (i != j) {
                    tmp = std::max(tmp, getValue(dp, step - 1, i - 1, j - 1, rows, cols));
                    tmp = std::max(tmp, getValue(dp, step - 1, i - 1, j, rows, cols));
                    tmp = std::max(tmp, getValue(dp, step - 1, i, j - 1, rows, cols));
                    tmp = std::max(tmp, getValue(dp, step - 1, i, j, rows, cols));
                    tmp += a[i][step - i] + a[j][step - j];
                    dp[step][i][j] = tmp;
                } else {
                    tmp = std::max(tmp, getValue(dp, step - 1, i - 1, j - 1, rows, cols));
                    tmp = std::max(tmp, getValue(dp, step - 1, i - 1, j, rows, cols));
                    tmp = std::max(tmp, getValue(dp, step - 1, i, j, rows, cols));
                    tmp += a[i][step - i];
                }
                dp[step][i][j] = tmp;
            }
        }
    }

    return dp[steps - 1][rows - 1][cols - 1];
}

void testChessTwiceTraverse() {
    std::vector<std::vector<int>> a = {{1, 2, 3, 4},
                                       {1, 2, 3, 4},
                                       {1, 2, 3, 4},
                                       {1, 2, 3, 4}};
    std::cout << chessTwiceTraverse(a);
}

int main() {
    testChessTwiceTraverse();
    return 0;
}