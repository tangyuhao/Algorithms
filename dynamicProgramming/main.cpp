#include <iostream>
#include <vector>
#include <stack>

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

bool stringCombination(std::string str1, std::string str2, std::string str3) {
    int len1 = str1.length();
    int len2 = str2.length();
    int len3 = str3.length();
    if (len1 + len2 != len3)
        return false;
    std::vector<std::vector<bool>> dp(len1 + 1, std::vector<bool>(len2 + 1, false));
    dp[0][0] = true;
    for (int i = 1; i <= len1; i++) {
        dp[i][0] = dp[i - 1][0] && str1[i - 1] == str3[i - 1];
    }
    for (int j = 1; j <= len2; j++) {
        dp[0][j] = dp[0][j - 1] && str2[j - 1] == str3[j - 1];
    }
    for (int i = 1; i <= len1; i++) {
        for (int j = 1; j <= len2; j++) {
            dp[i][j] = dp[i - 1][j] && str1[i - 1] == str3[i + j - 1] || dp[i][j - 1] && str2[j - 1] == str3[i + j - 1];
        }
    }
    return dp[len1][len2];
}

void testStringCombination() {
    std::cout << stringCombination("aabcc", "dbbca", "accabdbbca");
}

void getAllBreaks(const std::vector<std::vector<bool>> &pre, const std::string &str, int cur, std::vector<int> oneBreak,
                  std::vector<std::string> &answer) {
    // leaf
    if (cur == 0) {
        std::reverse(oneBreak.begin(), oneBreak.end());
        std::string tmp;
        int start = 0;
        for (auto it = oneBreak.begin(); it != oneBreak.end(); it++) {
            tmp += str.substr(start, *it - start) + " ";
            start = *it;
        }
        tmp.pop_back();
        answer.push_back(tmp);
        return;
    }

    for (int i = cur - 1; i >= 0; i--) {
        if (pre[cur][i]) {
            oneBreak.push_back(i);
            getAllBreaks(pre, str, i, oneBreak, answer);
            oneBreak.pop_back();
        }
    }
}

int wordBreak(std::string str, std::vector<std::string> dict) {
    int len = (int) str.length();
    std::vector<int> dp(len + 1, false);
    std::vector<std::vector<bool>> pre(len + 1, std::vector<bool>(len, false));
    std::vector<std::string> answers;
    dp[0] = true;
    for (int i = 1; i <= len; i++) {
        for (int j = i - 1; j >= 0; j--) {
            if (dp[j] == true && std::find(dict.begin(), dict.end(), str.substr(j, i - j)) != dict.end()) {
                dp[i] = true;
                pre[i][j] = true;
            }
        }
    }

    if (dp[len]) {
        std::vector<int> oneBreak;
        oneBreak.push_back(len);
        getAllBreaks(pre, str, len, oneBreak, answers);
        std::for_each(answers.begin(), answers.end(), [](std::string s) { std::cout << s << std::endl; });
    }
    return dp[len];
}

void testWordBreak() {
    std::vector<std::string> dict = {"cat", "cats", "and", "sand", "dog"};
    wordBreak("catsanddog", dict);
}

int matrixMultiply(std::vector<int> dim) {
    int size = (int) dim.size();
    if (size < 3)
        return -1;
    // the matrix start from 1 to size - 1, the matrix 0 is not valid
    std::vector<std::vector<int>> results(size, std::vector<int>(size, -1)); // -1 means invalid
    std::vector<std::vector<int>> cuts(size, std::vector<int>(size, -1)); // record the cutting place
    for (int i = 1; i < size; i++)
        results[i][i] = 0;
    for (int m = 1; m < size - 1; m++) {
        for (int i = 1; i < size - m; i++) { // i + m < size
            int j = i + m;
            int min = INT_MAX;
            int cut = -1;
            for (int k = i; k < j; k++) {
                if (min > results[i][k] + results[k + 1][j] + dim[i - 1] * dim[k] * dim[j]) {
                    min = results[i][k] + results[k + 1][j] + dim[i - 1] * dim[k] * dim[j];
                    cut = k;
                }
            }
            results[i][j] = min;
            cuts[i][j] = cut;
        }
    }
    return results[1][size - 1];
}

void testMatrixMultiply() {
    std::vector<int> dims = {3, 5, 4};
    std::cout << matrixMultiply(dims);
}

int subSequenceAppearTimes(std::string pattern, std::string text) {
    if (pattern.length() > text.length())
        return 0;
    int pSize = (int) pattern.length();
    int tSize = (int) text.length();
    std::vector<std::vector<int>> times(pSize + 1, std::vector<int>(tSize + 1, 0));
    for (int k = 0; k <= tSize; k++)
        times[0][k] = 1;
    for (int i = 1; i <= pSize; i++) {
        for (int j = 1; j <= tSize; j++) {
            if (pattern[i - 1] != text[j - 1])
                times[i][j] = times[i][j - 1];
            else
                times[i][j] = times[i - 1][j - 1] + times[i][j - 1];
        }
    }
    return times[pSize][tSize];
}

int subSequenceAppearTimes2(std::string pattern, std::string text) {
    if (pattern.length() > text.length())
        return 0;
    int pSize = (int) pattern.length();
    int tSize = (int) text.length();
    std::vector<int> times(pSize + 1, 0);
    times[0] = 1;
    for (int j = 1; j <= tSize; j++) {
        for (int i = pSize; i > 0; i--) {
            if (pattern[i - 1] != text[j - 1])
                times[i] = times[i];
            else
                times[i] = times[i - 1] + times[i];
        }
    }
    return times[pSize];
}

void testSubSequenceAppearTimes() {
    std::cout << subSequenceAppearTimes("rabbit", "rabbbbbbit") << std::endl;
    std::cout << subSequenceAppearTimes2("rabbit", "rabbbbbbit");
}

/*
 * assume that the characters only range from 'a' to 'z'
 */
std::string longestSubstringWithoutRepeating(std::string str) {
    std::vector<int> last(26, -1);
    int size = (int) str.size();
    int start = 0;
    int mx = -1;
    std::string substr;
    for (int i = 0; i < size; i++) {
        if (last[str[i] - 'a'] >= start) {
            if (mx < i - start) {
                mx = i - start;
                substr = str.substr(start, mx);
            }
            start = last[str[i] - 'a'] + 1;
        }
        last[str[i] - 'a'] = i;
    }
    if (mx < size - start) {
        substr = str.substr(start, size - start);
    }
    return substr;
}


void testLongestSubstringWithoutRepeating() {
    std::cout << longestSubstringWithoutRepeating("aaa");
}

int jumpProblem(std::vector<int> m) {
    int size = (int) m.size();
    int cur = 0;
    int start = 0;
    int cnt = 0;
    while (cur < size - 1) {
        if (m[cur] <= 0)
            return -1;
        int mx = start;
        for (int i = start; i <= cur; i++) {
            mx = std::max(mx, m[i] + i);
        }
        start = cur + 1;
        cur = mx;
        cnt++;
    }
    return cnt;
}

void testJumpProblem() {
    std::cout << jumpProblem({2, 3, 1, 1, 2, 4, 1, 1, 6, 1, 7}) << std::endl;
    std::cout << jumpProblem({2, 3, 1, 1, 2}) << std::endl;
}

int main() {
    testJumpProblem();
    return 0;
}