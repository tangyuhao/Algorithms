#include <iostream>
#include <vector>
#include <stack>
#include <cmath>
#include <unordered_map>

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
            dp[i][j] = (dp[i - 1][j] && str1[i - 1] == str3[i + j - 1]) ||
                       (dp[i][j - 1] && str2[j - 1] == str3[i + j - 1]);
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
    std::cout << jumpProblem({2, 3, 1, 1, 0, 0, 1, 1, 6, 1, 7}) << std::endl;
    std::cout << jumpProblem({2}) << std::endl;
    std::cout << jumpProblem({2, 3, 1, 1, 2}) << std::endl;
}

inline int isPerfectSquare(int i) {
    int k = (int) std::sqrt(i);
    if (i == k * k)
        return k;
    else
        return 0;
}

int squareCut(int n, std::vector<std::vector<int>> &cuts) {
    std::vector<int> parts;
    if (n < 1)
        return 0;
    if (int k = isPerfectSquare(n)) {
        parts.push_back(k);
        cuts[n] = parts;
        return 1;
    }
    cuts[1] = std::vector<int>(1, 1);
    for (int nn = 2; nn <= n; nn++) {
        parts.clear();
        int k = (int) std::floor(std::sqrt(nn));
        int min = INT_MAX;
        int minLeftI = -1;
        for (int i = 1; i <= k; i++) {
            int left = i * i;
            int right = nn - left;
            int rightCuts = cuts[right].size();
            if (min > rightCuts + 1) {
                min = rightCuts + 1;
                parts = cuts[right];
                minLeftI = i;
            }
        }
        parts.push_back(minLeftI);
        cuts[nn] = parts;
    }
    return cuts[n].size();
}

void testSquareCut() {
    int n = 201314;
    std::vector<std::vector<int>> cuts(n + 1, std::vector<int>());
    squareCut(n, cuts);
    std::cout << n << " = ";
    std::for_each(cuts[n].begin(), cuts[n].end() - 1, [](int k) { std::cout << k << "^2 + "; });
    std::cout << *(cuts[n].end() - 1) << "^2" << std::endl;
}

/*
 * Average Time complexity: O(nlogn)
 */
int maxRectangleHistogram(std::vector<int> a, int left, int right) {
    int size = (int) a.size();
    if (left == right)
        return a[left];
    if (left > right)
        return 0;
    auto minIt = std::min_element(a.begin() + left, a.begin() + right + 1);
    int minIndex = std::distance(a.begin(), minIt);
    int maxRect = *minIt * (right - left + 1);
    maxRect = std::max(maxRect, maxRectangleHistogram(a, left, minIndex - 1));
    maxRect = std::max(maxRect, maxRectangleHistogram(a, minIndex + 1, right));
    return maxRect;
}

void testMaxRectangleHistogram() {
    std::vector<int> a({2, 7, 9, 4, 1});
    std::cout << maxRectangleHistogram(a, 0, a.size() - 1);
}

/*
 * Time Complexity: O(n)
 */
int maxRectangleHistogram2(const std::vector<int> &a) {
    std::stack<int> s;
    int size = (int) a.size();
    int maxArea = INT_MIN;
    for (int i = 0; i < size;) {
        if (s.empty() || a[i] > a[s.top()]) {
            s.push(i);
            i++;
        } else {
            int temp = s.top();
            s.pop();
            maxArea = std::max(maxArea, a[temp] * (s.empty() ? i : i - s.top() - 1));
        }
    }
    return maxArea;
}

void testMaxRectangleHistogram2() {
    std::vector<int> a({2, 7, 9, 4, 1});
    std::cout << maxRectangleHistogram2(a);
}

int max2DRectangle(const std::vector<std::vector<int>> &a) {
    if (a.size() == 0)
        return -1;
    if (a[0].size() == 0)
        return -1;
    int rows = (int) a.size();
    int cols = (int) a[0].size();
    std::vector<int> hist(cols);
    int maxArea = INT_MIN;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            int ii = i;
            int height = 0;
            while (ii >= 0 && a[ii][j] != 0) {
                ii--;
                height++;
            }
            hist[j] = height;
        }
        maxArea = std::max(maxArea, maxRectangleHistogram2(hist));
    }
    return maxArea;
}

void testMax2DRectangle() {
    std::vector<std::vector<int>> a = {{0, 1, 0, 1, 1, 1, 1, 0, 0},
                                       {0, 1, 0, 1, 0, 0, 0, 0, 0},
                                       {0, 1, 1, 1, 1, 1, 1, 0, 0},
                                       {0, 1, 1, 1, 1, 1, 0, 0, 0},
                                       {0, 1, 0, 1, 1, 1, 1, 0, 0},
                                       {0, 1, 0, 1, 0, 1, 1, 0, 0},
                                       {0, 1, 1, 0, 1, 0, 0, 0, 0}};
    std::cout << "max rectangle area: " << max2DRectangle(a);
}

long chargeProblem(std::vector<int> money, int amount) {
    int k = (int) money.size();
    std::vector<std::vector<long>> dp(k, std::vector<long>(amount + 1));
    for (int j = 0; j <= amount; j++) {
        dp[0][j] = 1;
    }
    // we assume the money 1 exists
    // or here we should update the initial state
    for (int i = 0; i < k; i++) {
        dp[i][0] = 1;
    }
    for (int i = 1; i < k; i++) {
        for (int j = 1; j <= amount; j++) {
            if (j < money[i])
                dp[i][j] = dp[i - 1][j];
            else
                dp[i][j] = dp[i][j - money[i]] + dp[i - 1][j];
        }
    }
    return dp[k - 1][amount];
}

void testChargeProblem() {
    std::vector<int> money = {1, 2, 5, 10, 20, 50, 100};
    int amount = 1000;
    std::cout << chargeProblem(money, amount);
}

bool scrambleString(std::string str1, std::string str2) {
    int size = (int) str1.size();
    if (size != str2.size())
        return false;
    if (size == 1)
        return str1 == str2;
    if (size == 2)
        return (str1 == str2 || (str1[0] == str2[1] && str1[1] == str2[0]));
    for (int k = 1; k < size; k++) {
        if (scrambleString(str1.substr(0, k), str2.substr(0, k)) &&
            scrambleString(str1.substr(k, size - k), str2.substr(k, size - k)))
            return true;
        if (scrambleString(str1.substr(0, k), str2.substr(size - k, k)) &&
            scrambleString(str1.substr(k, size - k), str2.substr(0, size - k)))
            return true;
    }
    return false;
}

void testScrambleString() {
    std::string str1 = "great";
    std::string str2 = "rgtae";
    std::cout << scrambleString(str1, str2);
}

void calculatePalindromeSubstring(const std::string &s, std::vector<std::vector<bool>> &isPalin) {
    int size = (int) s.size();
    // initialize
    for (int i = 0; i < size; i++)
        isPalin[i][i] = true;
    for (int i = 0; i < size - 1; i++)
        isPalin[i][i + 1] = (s[i] == s[i + 1]);
    for (int len = 3; len <= size; len++) {
        for (int i = 0; i <= size - len; i++) {
            // substring i to i + len - 1
            isPalin[i][i + len - 1] = isPalin[i + 1][i + len - 2] && s[i] == s[i + len - 1];
        }
    }
}

int minPalindromePartitioning(std::string s) {
    int size = (int) s.size();
    std::vector<std::vector<bool>> isPalin(size, std::vector<bool>(size));
    calculatePalindromeSubstring(s, isPalin);
    std::vector<int> dp(size, 0);
    std::vector<int> pre(size);
    pre[0] = -1;
    dp[0] = 1;
    for (int i = 1; i < size; i++) {
        if (isPalin[0][i]) {
            dp[i] = 1;
            pre[i] = -1;
            continue;
        }
        int minSplit = size;
        int preSplit = i - 1;
        for (int k = 0; k < i; k++) {
            if (isPalin[k + 1][i] && dp[k] + 1 < minSplit) {
                minSplit = dp[k] + 1;
                preSplit = k;
            }
        }
        dp[i] = minSplit;
        pre[i] = preSplit;
    }
    int k = size - 1;
    std::vector<std::string> result;
    while (k != -1) {
        result.push_back(s.substr(pre[k] + 1, k - pre[k]));
        k = pre[k];
    }
    std::for_each(result.rbegin(), result.rend(), [](std::string ss) { std::cout << ss << " "; });
    std::cout << std::endl;
    return dp[size - 1];
}

void testMinPalindromePartitioning() {
    std::string s = "abacdccdaa";
    std::cout << minPalindromePartitioning(s);
}

int main() {
    testMinPalindromePartitioning();
    return 0;
}